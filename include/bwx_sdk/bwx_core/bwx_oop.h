/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_oop.h
// Purpose:     BWX_SDK Library; OOP stuff
// Author:      Bartosz Warzocha
// Created:     2025-02-24
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWXOOP_H_
#define _BWXOOP_H_

#include <algorithm>     // Sorting and algorithms
#include <chrono>        // Timestamp handling
#include <deque>         // History storage
#include <functional>    // Callbacks and predicates
#include <map>           // Map container
#include <optional>      // Optional value handling
#include <stdexcept>     // Exception handling
#include <type_traits>   // Type traits and static assertions
#include <vector>        // Temporary storage for sorting

#include <wx/event.h>    // wxWidgets events
#include <wx/thread.h>   // Thread safety with wxMutex

// wxWidgets events declaration for value change notifications
wxDECLARE_EVENT(EVT_PROPERTY_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_PROPERTY_VECTOR_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_PROPERTY_MAP_CHANGED, wxCommandEvent);

//-----------------------------------------------------------------------------------

namespace bwx_sdk {

    template <typename T>
    class bwxProperty {
    public:
        using Validator = std::function<bool(const T&)>;
        using ChangeCallback = std::function<void(const T&, const T&)>;
        using RejectCallback = std::function<void(const T&)>;
        using Timestamp = std::chrono::system_clock::time_point;

        explicit bwxProperty(const T& defaultValue = T(),
                          Validator validator = nullptr,
                          ChangeCallback onChange = nullptr,
                          RejectCallback onReject = nullptr,
                          size_t historyLimit = 0,
                          std::optional<std::chrono::seconds> undoTimeout = std::nullopt,
                          wxEvtHandler* eventHandler = nullptr)
            : value_(defaultValue),
              defaultValue_(defaultValue),
              validator_(std::move(validator)),
              onChange_(std::move(onChange)),
              onReject_(std::move(onReject)),
              historyLimit_(historyLimit),
              undoTimeout_(undoTimeout),
              eventHandler_(eventHandler),
              lastChangeTime_(std::chrono::system_clock::now()) {}

        bwxProperty<T>& operator=(const T& newValue) {
            set(newValue);
            return *this;
        }

        void set(const T& newValue) {
            wxMutexLocker lock(mutex_);
            if (readOnly_) return;

            if (validator_ && !validator_(newValue)) {
                if (onReject_) onReject_(newValue);
                return;
            }

            if (newValue != value_) {
                recordHistory(value_);
                clearRedoHistory();
                if (onChange_) onChange_(value_, newValue);

                value_ = newValue;
                lastChangeTime_ = std::chrono::system_clock::now();

                notifyChange();
                propagateBinding(newValue);
            }
        }

        [[nodiscard]] const T& get() const noexcept {
            wxMutexLocker lock(mutex_);
            return value_;
        }

        const char* AsCStr() const {
            if constexpr (std::is_same_v<T, std::string>) {
                return value_.c_str();
            } else if constexpr (std::is_same_v<T, wxString>) {
                return value_.mb_str().data();
            } else {
                static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, wxString>,
                              "AsCStr() is only supported for std::string or wxString.");
                return nullptr;
            }
        }

        void reset() {
            set(defaultValue_);
        }

        void setValidator(Validator validator) {
            wxMutexLocker lock(mutex_);
            validator_ = std::move(validator);
        }

        void setOnChangeCallback(ChangeCallback onChange) {
            wxMutexLocker lock(mutex_);
            onChange_ = std::move(onChange);
        }

        void setOnRejectCallback(RejectCallback onReject) {
            wxMutexLocker lock(mutex_);
            onReject_ = std::move(onReject);
        }

        void setReadOnly(bool readOnly) {
            wxMutexLocker lock(mutex_);
            readOnly_ = readOnly;
        }

        [[nodiscard]] bool isReadOnly() const noexcept {
            wxMutexLocker lock(mutex_);
            return readOnly_;
        }

        bool undo() {
            wxMutexLocker lock(mutex_);
            if (undoHistory_.empty()) return false;

            auto now = std::chrono::system_clock::now();
            if (undoTimeout_ && std::chrono::duration_cast<std::chrono::seconds>(now - lastChangeTime_) > *undoTimeout_) {
                return false;
            }

            redoHistory_.push_front(value_);
            value_ = undoHistory_.front();
            undoHistory_.pop_front();
            lastChangeTime_ = now;

            notifyChange();
            propagateBinding(value_);
            return true;
        }

        bool redo() {
            wxMutexLocker lock(mutex_);
            if (redoHistory_.empty()) return false;

            undoHistory_.push_front(value_);
            value_ = redoHistory_.front();
            redoHistory_.pop_front();
            lastChangeTime_ = std::chrono::system_clock::now();

            notifyChange();
            propagateBinding(value_);
            return true;
        }

        void setHistoryLimit(size_t limit) {
            wxMutexLocker lock(mutex_);
            historyLimit_ = limit;
            trimHistory(undoHistory_);
            trimHistory(redoHistory_);
        }

        [[nodiscard]] size_t getHistoryLimit() const noexcept {
            wxMutexLocker lock(mutex_);
            return historyLimit_;
        }

        void clearHistory() {
            wxMutexLocker lock(mutex_);
            undoHistory_.clear();
            redoHistory_.clear();
        }

        void setUndoTimeout(std::chrono::seconds timeout) {
            wxMutexLocker lock(mutex_);
            undoTimeout_ = timeout;
        }

        [[nodiscard]] std::optional<std::chrono::seconds> getUndoTimeout() const noexcept {
            wxMutexLocker lock(mutex_);
            return undoTimeout_;
        }

        Timestamp getLastChangeTime() const {
            wxMutexLocker lock(mutex_);
            return lastChangeTime_;
        }

        void bind(Property<T>& other) {
            wxMutexLocker lock(mutex_);
            boundProperties_.push_back(&other);
        }

        // Arithmetic operators
        Property<T>& operator+=(const T& rhs) { set(value_ + rhs); return *this; }
        Property<T>& operator-=(const T& rhs) { set(value_ - rhs); return *this; }
        Property<T>& operator*=(const T& rhs) { set(value_ * rhs); return *this; }
        Property<T>& operator/=(const T& rhs) { set(value_ / rhs); return *this; }

        // Comparison operators
        bool operator==(const T& rhs) const { return value_ == rhs; }
        bool operator!=(const T& rhs) const { return value_ != rhs; }
        bool operator<(const T& rhs) const { return value_ < rhs; }
        bool operator<=(const T& rhs) const { return value_ <= rhs; }
        bool operator>(const T& rhs) const { return value_ > rhs; }
        bool operator>=(const T& rhs) const { return value_ >= rhs; }

    private:
        void recordHistory(const T& oldValue) {
            if (historyLimit_ == 0) return;
            undoHistory_.push_front(oldValue);
            trimHistory(undoHistory_);
        }

        void clearRedoHistory() {
            redoHistory_.clear();
        }

        void trimHistory(std::deque<T>& history) {
            while (history.size() > historyLimit_) {
                history.pop_back();
            }
        }

        void notifyChange() {
            if (eventHandler_) {
                wxCommandEvent evt(EVT_BWXPROPERTY_CHANGED);
                eventHandler_->AddPendingEvent(evt);
            }
        }

        void propagateBinding(const T& newValue) {
            for (auto* boundProp : boundProperties_) {
                if (boundProp) boundProp->set(newValue);
            }
        }

        T value_{};
        T defaultValue_{};
        Validator validator_ = nullptr;
        ChangeCallback onChange_ = nullptr;
        RejectCallback onReject_ = nullptr;
        size_t historyLimit_ = 0;
        std::optional<std::chrono::seconds> undoTimeout_ = std::nullopt;
        bool readOnly_ = false;
        wxEvtHandler* eventHandler_ = nullptr;
        Timestamp lastChangeTime_;
        std::deque<T> undoHistory_;
        std::deque<T> redoHistory_;
        std::vector<Property<T>*> boundProperties_;
        mutable wxMutex mutex_;
    };
    
    //-----------------------------------------------------------------------------------

	template <typename T>
	class bwxPropertyVector {
	public:
		using ChangeCallback = std::function<void()>;
		using Timestamp = std::chrono::system_clock::time_point;

		explicit bwxPropertyVector(wxEvtHandler* handler = nullptr,
			ChangeCallback callback = nullptr,
			size_t historyLimit = 0,
			size_t capacityLimit = 0)
			: eventHandler_(handler),
			onChange_(std::move(callback)),
			historyLimit_(historyLimit),
			capacityLimit_(capacityLimit),
			lastChangeTime_(std::chrono::system_clock::now()) {}

		void add(const T& value) {
			wxMutexLocker lock(mutex_);
			if (readOnly_) return;
			if (capacityLimit_ > 0 && data_.size() >= capacityLimit_) {
				throw std::overflow_error("Capacity limit reached");
			}

			recordHistory();
			data_.push_back(value);
			lastChangeTime_ = std::chrono::system_clock::now();
			notifyChange();
		}

		std::optional<T> get(size_t index) const {
			wxMutexLocker lock(mutex_);
			return (index < data_.size()) ? std::optional<T>(data_[index]) : std::nullopt;
		}

		T& at(size_t index) {
			wxMutexLocker lock(mutex_);
			if (index >= data_.size()) throw std::out_of_range("Index out of range");
			return data_[index];
		}

		void clear() {
			wxMutexLocker lock(mutex_);
			recordHistory();
			data_.clear();
			notifyChange();
		}

		size_t size() const {
			wxMutexLocker lock(mutex_);
			return data_.size();
		}

		typename std::vector<T>::iterator begin() { return data_.begin(); }
		typename std::vector<T>::iterator end() { return data_.end(); }
		typename std::vector<T>::const_iterator cbegin() const { return data_.cbegin(); }
		typename std::vector<T>::const_iterator cend() const { return data_.cend(); }

		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(mutex_);
			readOnly_ = readOnly;
		}

		bool isReadOnly() const noexcept {
			wxMutexLocker lock(mutex_);
			return readOnly_;
		}

		bool undo() {
			wxMutexLocker lock(mutex_);
			if (undoHistory_.empty()) return false;

			redoHistory_.push_front(data_);
			data_ = undoHistory_.front();
			undoHistory_.pop_front();
			notifyChange();
			return true;
		}

		bool redo() {
			wxMutexLocker lock(mutex_);
			if (redoHistory_.empty()) return false;

			undoHistory_.push_front(data_);
			data_ = redoHistory_.front();
			redoHistory_.pop_front();
			notifyChange();
			return true;
		}

		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			historyLimit_ = limit;
			trimHistory(undoHistory_);
			trimHistory(redoHistory_);
		}

		size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return historyLimit_;
		}

		void clearHistory() {
			wxMutexLocker lock(mutex_);
			undoHistory_.clear();
			redoHistory_.clear();
		}

		void setCapacityLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			capacityLimit_ = limit;
			trimToCapacity();
		}

		size_t getCapacityLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return capacityLimit_;
		}

		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(mutex_);
			return lastChangeTime_;
		}

		void setOnChangeCallback(ChangeCallback callback) {
			wxMutexLocker lock(mutex_);
			onChange_ = std::move(callback);
		}

		void setEventHandler(wxEvtHandler* handler) {
			wxMutexLocker lock(mutex_);
			eventHandler_ = handler;
		}

		const char* AsCStr(size_t index = 0) const {
			wxMutexLocker lock(mutex_);
			if (index >= data_.size()) return nullptr;

			if constexpr (std::is_same_v<T, std::string>) {
				return data_[index].c_str();
			}
			else if constexpr (std::is_same_v<T, wxString>) {
				return data_[index].mb_str().data();
			}
			else {
				static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, wxString>,
					"AsCStr() is only supported for std::string or wxString.");
				return nullptr;
			}
		}

	private:
		void notifyChange() {
			if (onChange_) onChange_();
			if (eventHandler_) {
				wxCommandEvent evt(EVT_PROPERTY_VECTOR_CHANGED);
				eventHandler_->AddPendingEvent(evt);
			}
		}

		void recordHistory() {
			if (historyLimit_ == 0) return;
			undoHistory_.push_front(data_);
			trimHistory(undoHistory_);
			redoHistory_.clear();
		}

		void trimHistory(std::deque<std::vector<T>>& history) {
			while (history.size() > historyLimit_) {
				history.pop_back();
			}
		}

		void trimToCapacity() {
			while (capacityLimit_ > 0 && data_.size() > capacityLimit_) {
				data_.erase(data_.begin());
			}
		}

		std::vector<T> data_{};
		size_t historyLimit_ = 0;
		size_t capacityLimit_ = 0;
		std::deque<std::vector<T>> undoHistory_;
		std::deque<std::vector<T>> redoHistory_;
		wxEvtHandler* eventHandler_ = nullptr;
		ChangeCallback onChange_ = nullptr;
		Timestamp lastChangeTime_;
		bool readOnly_ = false;
		mutable wxMutex mutex_;
	};

	//-----------------------------------------------------------------------------------

	template <typename K, typename V>
	class bwxPropertyMap {
	public:
		using ChangeCallback = std::function<void()>;
		using Timestamp = std::chrono::system_clock::time_point;

		explicit bwxPropertyMap(wxEvtHandler* handler = nullptr,
			ChangeCallback callback = nullptr,
			size_t historyLimit = 0,
			size_t capacityLimit = 0)
			: eventHandler_(handler),
			onChange_(std::move(callback)),
			historyLimit_(historyLimit),
			capacityLimit_(capacityLimit),
			lastChangeTime_(std::chrono::system_clock::now()) {}

		void set(const K& key, const V& value) {
			wxMutexLocker lock(mutex_);
			if (readOnly_) return;

			if (capacityLimit_ > 0 && data_.size() >= capacityLimit_ && data_.find(key) == data_.end()) {
				throw std::overflow_error("Capacity limit reached");
			}

			recordHistory();
			data_[key] = value;
			lastChangeTime_ = std::chrono::system_clock::now();
			notifyChange();
		}

		std::optional<V> get(const K& key) const {
			wxMutexLocker lock(mutex_);
			auto it = data_.find(key);
			return (it != data_.end()) ? std::optional<V>(it->second) : std::nullopt;
		}

		const V& getRef(const K& key) const {
			wxMutexLocker lock(mutex_);
			auto it = data_.find(key);
			if (it == data_.end()) throw std::out_of_range("Key not found");
			return it->second;
		}

		void clear() {
			wxMutexLocker lock(mutex_);
			recordHistory();
			data_.clear();
			notifyChange();
		}

		size_t size() const {
			wxMutexLocker lock(mutex_);
			return data_.size();
		}

		typename std::map<K, V>::iterator begin() { return data_.begin(); }
		typename std::map<K, V>::iterator end() { return data_.end(); }
		typename std::map<K, V>::const_iterator cbegin() const { return data_.cbegin(); }
		typename std::map<K, V>::const_iterator cend() const { return data_.cend(); }

		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(mutex_);
			readOnly_ = readOnly;
		}

		bool isReadOnly() const noexcept {
			wxMutexLocker lock(mutex_);
			return readOnly_;
		}

		bool undo() {
			wxMutexLocker lock(mutex_);
			if (undoHistory_.empty()) return false;

			redoHistory_.push_front(data_);
			data_ = undoHistory_.front();
			undoHistory_.pop_front();
			notifyChange();
			return true;
		}

		bool redo() {
			wxMutexLocker lock(mutex_);
			if (redoHistory_.empty()) return false;

			undoHistory_.push_front(data_);
			data_ = redoHistory_.front();
			redoHistory_.pop_front();
			notifyChange();
			return true;
		}

		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			historyLimit_ = limit;
			trimHistory(undoHistory_);
			trimHistory(redoHistory_);
		}

		size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return historyLimit_;
		}

		void clearHistory() {
			wxMutexLocker lock(mutex_);
			undoHistory_.clear();
			redoHistory_.clear();
		}

		void setCapacityLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			capacityLimit_ = limit;
			trimToCapacity();
		}

		size_t getCapacityLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return capacityLimit_;
		}

		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(mutex_);
			return lastChangeTime_;
		}

		void setOnChangeCallback(ChangeCallback callback) {
			wxMutexLocker lock(mutex_);
			onChange_ = std::move(callback);
		}

		void setEventHandler(wxEvtHandler* handler) {
			wxMutexLocker lock(mutex_);
			eventHandler_ = handler;
		}

		const char* AsCStr(const K& key) const {
			wxMutexLocker lock(mutex_);
			auto it = data_.find(key);
			if (it == data_.end()) return nullptr;

			if constexpr (std::is_same_v<V, std::string>) {
				return it->second.c_str();
			}
			else if constexpr (std::is_same_v<V, wxString>) {
				return it->second.mb_str().data();
			}
			else {
				static_assert(std::is_same_v<V, std::string> || std::is_same_v<V, wxString>,
					"AsCStr() is only supported for std::string or wxString.");
				return nullptr;
			}
		}

	private:
		void notifyChange() {
			if (onChange_) onChange_();
			if (eventHandler_) {
				wxCommandEvent evt(EVT_PROPERTY_MAP_CHANGED);
				eventHandler_->AddPendingEvent(evt);
			}
		}

		void recordHistory() {
			if (historyLimit_ == 0) return;
			undoHistory_.push_front(data_);
			trimHistory(undoHistory_);
			redoHistory_.clear();
		}

		void trimHistory(std::deque<std::map<K, V>>& history) {
			while (history.size() > historyLimit_) {
				history.pop_back();
			}
		}

		void trimToCapacity() {
			while (capacityLimit_ > 0 && data_.size() > capacityLimit_) {
				data_.erase(data_.begin());
			}
		}

		std::map<K, V> data_{};
		size_t historyLimit_ = 0;
		size_t capacityLimit_ = 0;
		std::deque<std::map<K, V>> undoHistory_;
		std::deque<std::map<K, V>> redoHistory_;
		wxEvtHandler* eventHandler_ = nullptr;
		ChangeCallback onChange_ = nullptr;
		Timestamp lastChangeTime_;
		bool readOnly_ = false;
		mutable wxMutex mutex_;
	};

}

#endif // _BWXOOP_H_
