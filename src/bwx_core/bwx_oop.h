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

	/**
     * @brief Template class representing a property with advanced features.
     *
     * This class manages a single value with the following capabilities:
     * - Value validation with customizable validator.
     * - Change and rejection callbacks.
     * - Undo/redo functionality with configurable history limit.
     * - Read-only mode to prevent modifications.
     * - wxWidgets event notifications and callback handling.
     * - Timestamp tracking for the last modification.
     * - Property binding to synchronize values between properties.
     * - Arithmetic and comparison operators.
     * - Conversion of the value to C-style strings (for supported types).
     *
     * @tparam T Type of the value.
     */
    template <typename T>
    class bwxProperty {
    public:
        using Validator = std::function<bool(const T&)>;              ///< Validator function to validate new values.
        using ChangeCallback = std::function<void(const T&, const T&)>; ///< Callback invoked when the value changes.
        using RejectCallback = std::function<void(const T&)>;        ///< Callback invoked when a value is rejected.
        using Timestamp = std::chrono::system_clock::time_point;     ///< Timestamp of the last modification.

        /**
         * @brief Constructs a Property with optional parameters.
         * @param defaultValue Initial value of the property.
         * @param validator Validator function for new values.
         * @param onChange Callback invoked on value change.
         * @param onReject Callback invoked when value is rejected.
         * @param historyLimit Maximum number of undo/redo states (0 disables history).
         * @param undoTimeout Optional timeout for undo operations.
         * @param eventHandler Optional wxWidgets event handler for notifications.
         */
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

        /**
         * @brief Assigns a new value to the property.
         * @param newValue Value to assign.
         * @return Reference to the updated property.
         */
        bwxProperty<T>& operator=(const T& newValue) {
            set(newValue);
            return *this;
        }

        /**
         * @brief Sets a new value for the property.
         * @param newValue Value to set.
         */
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

        /**
         * @brief Retrieves the current value.
         * @return Constant reference to the value.
         */
        [[nodiscard]] const T& get() const noexcept {
            wxMutexLocker lock(mutex_);
            return value_;
        }

        /**
         * @brief Converts the value to a C-style string.
         * @return Pointer to the C-string or nullptr if unsupported.
         */
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

        /**
         * @brief Resets the value to its default.
         */
        void reset() {
            set(defaultValue_);
        }

        /**
         * @brief Sets a new validator function.
         * @param validator Validator function to set.
         */
        void setValidator(Validator validator) {
            wxMutexLocker lock(mutex_);
            validator_ = std::move(validator);
        }

        /**
         * @brief Assigns a callback for value changes.
         * @param onChange Callback to set.
         */
        void setOnChangeCallback(ChangeCallback onChange) {
            wxMutexLocker lock(mutex_);
            onChange_ = std::move(onChange);
        }

        /**
         * @brief Assigns a callback for value rejections.
         * @param onReject Callback to set.
         */
        void setOnRejectCallback(RejectCallback onReject) {
            wxMutexLocker lock(mutex_);
            onReject_ = std::move(onReject);
        }

        /**
         * @brief Enables or disables read-only mode.
         * @param readOnly True to enable, false to disable.
         */
        void setReadOnly(bool readOnly) {
            wxMutexLocker lock(mutex_);
            readOnly_ = readOnly;
        }

        /**
         * @brief Checks if the property is read-only.
         * @return True if read-only, false otherwise.
         */
        [[nodiscard]] bool isReadOnly() const noexcept {
            wxMutexLocker lock(mutex_);
            return readOnly_;
        }

        /**
         * @brief Undoes the last value change.
         * @return True if successful, false otherwise.
         */
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

        /**
         * @brief Redoes the previously undone value change.
         * @return True if successful, false otherwise.
         */
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

        /**
         * @brief Sets the history limit for undo/redo operations.
         * @param limit Number of history entries to retain.
         */
        void setHistoryLimit(size_t limit) {
            wxMutexLocker lock(mutex_);
            historyLimit_ = limit;
            trimHistory(undoHistory_);
            trimHistory(redoHistory_);
        }

        /**
         * @brief Retrieves the configured history limit.
         * @return History limit.
         */
        [[nodiscard]] size_t getHistoryLimit() const noexcept {
            wxMutexLocker lock(mutex_);
            return historyLimit_;
        }

        /**
         * @brief Clears the undo and redo histories.
         */
        void clearHistory() {
            wxMutexLocker lock(mutex_);
            undoHistory_.clear();
            redoHistory_.clear();
        }

        /**
         * @brief Sets the timeout for undo operations.
         * @param timeout Timeout duration.
         */
        void setUndoTimeout(std::chrono::seconds timeout) {
            wxMutexLocker lock(mutex_);
            undoTimeout_ = timeout;
        }

        /**
         * @brief Retrieves the undo timeout duration.
         * @return Optional undo timeout.
         */
        [[nodiscard]] std::optional<std::chrono::seconds> getUndoTimeout() const noexcept {
            wxMutexLocker lock(mutex_);
            return undoTimeout_;
        }

        /**
         * @brief Retrieves the timestamp of the last change.
         * @return Last change timestamp.
         */
        Timestamp getLastChangeTime() const {
            wxMutexLocker lock(mutex_);
            return lastChangeTime_;
        }

        /**
         * @brief Binds another property to synchronize value changes.
         * @param other Property to bind to.
         */
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
        /** @brief Records the current value into the undo history. */
        void recordHistory(const T& oldValue) {
            if (historyLimit_ == 0) return;
            undoHistory_.push_front(oldValue);
            trimHistory(undoHistory_);
        }

        /** @brief Clears the redo history. */
        void clearRedoHistory() {
            redoHistory_.clear();
        }

        /** @brief Trims the history to respect the configured limit. */
        void trimHistory(std::deque<T>& history) {
            while (history.size() > historyLimit_) {
                history.pop_back();
            }
        }

        /** @brief Notifies listeners through callbacks and wxWidgets events. */
        void notifyChange() {
            if (eventHandler_) {
                wxCommandEvent evt(EVT_BWXPROPERTY_CHANGED);
                eventHandler_->AddPendingEvent(evt);
            }
        }

        /** @brief Propagates value changes to bound properties. */
        void propagateBinding(const T& newValue) {
            for (auto* boundProp : boundProperties_) {
                if (boundProp) boundProp->set(newValue);
            }
        }

        T value_{};                                    ///< Current value of the property.
        T defaultValue_{};                              ///< Default value used for resets.
        Validator validator_ = nullptr;                 ///< Validator function for new values.
        ChangeCallback onChange_ = nullptr;             ///< Callback for value changes.
        RejectCallback onReject_ = nullptr;             ///< Callback for rejected values.
        size_t historyLimit_ = 0;                       ///< Undo/redo history limit.
        std::optional<std::chrono::seconds> undoTimeout_ = std::nullopt; ///< Undo operation timeout.
        bool readOnly_ = false;                         ///< Read-only mode flag.
        wxEvtHandler* eventHandler_ = nullptr;          ///< wxWidgets event handler.
        Timestamp lastChangeTime_;                      ///< Timestamp of the last modification.
        std::deque<T> undoHistory_;                     ///< Undo history.
        std::deque<T> redoHistory_;                     ///< Redo history.
        std::vector<Property<T>*> boundProperties_;     ///< Bound properties for synchronization.
        mutable wxMutex mutex_;                         ///< Mutex for thread safety.
    };
    
    //-----------------------------------------------------------------------------------

	/**
	 * @brief Template class representing a vector-based property with advanced features.
	 *
	 * This class manages a vector of values with the following functionalities:
	 * - Safe element insertion, removal, and retrieval.
	 * - Undo/redo support with configurable history limits.
	 * - Read-only mode to prevent modifications.
	 * - wxWidgets event notifications and callback handling on data changes.
	 * - Timestamp recording for the last data modification.
	 * - Iterators for standard container traversal.
	 * - Optional value retrieval for safe access.
	 * - Capacity management to limit the number of elements.
	 * - Data filtering and sorting functionalities.
	 * - Conversion of elements to C-style strings when applicable.
	 *
	 * @tparam T Type of the elements in the vector.
	 */
	template <typename T>
	class bwxPropertyVector {
	public:
		using ChangeCallback = std::function<void()>; ///< Callback invoked when the vector changes.
		using Timestamp = std::chrono::system_clock::time_point; ///< Timestamp of the last modification.

		/**
		 * @brief Constructs a PropertyVector with optional parameters.
		 * @param handler Optional wxWidgets event handler for change notifications.
		 * @param callback Optional callback invoked when the vector changes.
		 * @param historyLimit Maximum number of undo/redo states (0 disables history).
		 * @param capacityLimit Maximum number of elements allowed in the vector (0 means unlimited).
		 */
		explicit bwxPropertyVector(wxEvtHandler* handler = nullptr,
			ChangeCallback callback = nullptr,
			size_t historyLimit = 0,
			size_t capacityLimit = 0)
			: eventHandler_(handler),
			onChange_(std::move(callback)),
			historyLimit_(historyLimit),
			capacityLimit_(capacityLimit),
			lastChangeTime_(std::chrono::system_clock::now()) {}

		/**
		 * @brief Adds a value to the vector.
		 * @param value The value to be added.
		 * @throws std::overflow_error If capacity limit is reached.
		 */
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

		/**
		 * @brief Retrieves the value at a specified index safely.
		 * @param index Index of the element.
		 * @return Optional containing the value if valid, otherwise std::nullopt.
		 */
		std::optional<T> get(size_t index) const {
			wxMutexLocker lock(mutex_);
			return (index < data_.size()) ? std::optional<T>(data_[index]) : std::nullopt;
		}

		/**
		 * @brief Accesses the element at the specified index with bounds checking.
		 * @param index Index of the element.
		 * @return Reference to the element.
		 * @throws std::out_of_range If the index is invalid.
		 */
		T& at(size_t index) {
			wxMutexLocker lock(mutex_);
			if (index >= data_.size()) throw std::out_of_range("Index out of range");
			return data_[index];
		}

		/**
		 * @brief Clears all elements from the vector.
		 */
		void clear() {
			wxMutexLocker lock(mutex_);
			recordHistory();
			data_.clear();
			notifyChange();
		}

		/**
		 * @brief Retrieves the size of the vector.
		 * @return Number of elements in the vector.
		 */
		size_t size() const {
			wxMutexLocker lock(mutex_);
			return data_.size();
		}

		typename std::vector<T>::iterator begin() { return data_.begin(); }
		typename std::vector<T>::iterator end() { return data_.end(); }
		typename std::vector<T>::const_iterator cbegin() const { return data_.cbegin(); }
		typename std::vector<T>::const_iterator cend() const { return data_.cend(); }

		/**
		 * @brief Enables or disables read-only mode.
		 * @param readOnly True to enable, false to disable modifications.
		 */
		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(mutex_);
			readOnly_ = readOnly;
		}

		/**
		 * @brief Checks if the vector is in read-only mode.
		 * @return True if read-only, otherwise false.
		 */
		bool isReadOnly() const noexcept {
			wxMutexLocker lock(mutex_);
			return readOnly_;
		}

		/**
		 * @brief Reverts to the previous state from the undo history.
		 * @return True if successful, false otherwise.
		 */
		bool undo() {
			wxMutexLocker lock(mutex_);
			if (undoHistory_.empty()) return false;

			redoHistory_.push_front(data_);
			data_ = undoHistory_.front();
			undoHistory_.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Restores the last undone state from the redo history.
		 * @return True if successful, false otherwise.
		 */
		bool redo() {
			wxMutexLocker lock(mutex_);
			if (redoHistory_.empty()) return false;

			undoHistory_.push_front(data_);
			data_ = redoHistory_.front();
			redoHistory_.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Sets the history limit for undo/redo operations.
		 * @param limit Number of history entries to retain.
		 */
		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			historyLimit_ = limit;
			trimHistory(undoHistory_);
			trimHistory(redoHistory_);
		}

		/**
		 * @brief Retrieves the current undo/redo history limit.
		 * @return Configured history limit.
		 */
		size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return historyLimit_;
		}

		/**
		 * @brief Clears the undo and redo history.
		 */
		void clearHistory() {
			wxMutexLocker lock(mutex_);
			undoHistory_.clear();
			redoHistory_.clear();
		}

		/**
		 * @brief Sets the capacity limit of the vector.
		 * @param limit Maximum allowed number of elements.
		 */
		void setCapacityLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			capacityLimit_ = limit;
			trimToCapacity();
		}

		/**
		 * @brief Retrieves the vector's capacity limit.
		 * @return Configured capacity limit.
		 */
		size_t getCapacityLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return capacityLimit_;
		}

		/**
		 * @brief Retrieves the timestamp of the last modification.
		 * @return Timestamp of the last change.
		 */
		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(mutex_);
			return lastChangeTime_;
		}

		/**
		 * @brief Assigns a callback function for change notifications.
		 * @param callback Callback function to be invoked.
		 */
		void setOnChangeCallback(ChangeCallback callback) {
			wxMutexLocker lock(mutex_);
			onChange_ = std::move(callback);
		}

		/**
		 * @brief Assigns a wxWidgets event handler for notifications.
		 * @param handler Pointer to the event handler.
		 */
		void setEventHandler(wxEvtHandler* handler) {
			wxMutexLocker lock(mutex_);
			eventHandler_ = handler;
		}

		/**
		 * @brief Converts an element to a C-style string.
		 * @param index Index of the element.
		 * @return Pointer to the C-string or nullptr if invalid or unsupported.
		 */
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
		/** @brief Notifies registered listeners via callbacks and wxWidgets events. */
		void notifyChange() {
			if (onChange_) onChange_();
			if (eventHandler_) {
				wxCommandEvent evt(EVT_PROPERTY_VECTOR_CHANGED);
				eventHandler_->AddPendingEvent(evt);
			}
		}

		/** @brief Records the current vector state into the undo history. */
		void recordHistory() {
			if (historyLimit_ == 0) return;
			undoHistory_.push_front(data_);
			trimHistory(undoHistory_);
			redoHistory_.clear();
		}

		/** @brief Trims the history deque to comply with the history limit. */
		void trimHistory(std::deque<std::vector<T>>& history) {
			while (history.size() > historyLimit_) {
				history.pop_back();
			}
		}

		/** @brief Ensures the vector respects the capacity limit by removing oldest elements. */
		void trimToCapacity() {
			while (capacityLimit_ > 0 && data_.size() > capacityLimit_) {
				data_.erase(data_.begin());
			}
		}

		std::vector<T> data_{};                          ///< Internal vector storing elements.
		size_t historyLimit_ = 0;                        ///< Undo/redo history limit.
		size_t capacityLimit_ = 0;                       ///< Maximum allowed vector capacity.
		std::deque<std::vector<T>> undoHistory_;        ///< Undo history states.
		std::deque<std::vector<T>> redoHistory_;        ///< Redo history states.
		wxEvtHandler* eventHandler_ = nullptr;           ///< wxWidgets event handler.
		ChangeCallback onChange_ = nullptr;              ///< Callback for vector changes.
		Timestamp lastChangeTime_;                       ///< Last modification timestamp.
		bool readOnly_ = false;                          ///< Read-only mode flag.
		mutable wxMutex mutex_;                          ///< Mutex for thread-safe access.
	};

	//-----------------------------------------------------------------------------------

	/**
 * @brief Template class representing a map-based property with advanced features.
 *
 * This class manages a map of key-value pairs with the following functionalities:
 * - Insertion, retrieval, and removal of elements with thread safety.
 * - Undo/redo support with configurable history limits.
 * - Read-only mode to prevent modifications.
 * - wxWidgets event notifications and callback handling on data changes.
 * - Timestamp tracking of the last modification.
 * - Iterators for standard container traversal.
 * - Optional value retrieval for safe access.
 * - Capacity management to limit the number of elements.
 * - Data filtering and sorting capabilities.
 * - Conversion of values to C-style strings when applicable.
 *
 * @tparam K Type of the keys in the map.
 * @tparam V Type of the values in the map.
 */
	template <typename K, typename V>
	class bwxPropertyMap {
	public:
		using ChangeCallback = std::function<void()>; ///< Callback invoked when the map changes.
		using Timestamp = std::chrono::system_clock::time_point; ///< Timestamp of the last modification.

		/**
		 * @brief Constructs a PropertyMap with optional parameters.
		 * @param handler Optional wxWidgets event handler for change notifications.
		 * @param callback Optional callback invoked when the map changes.
		 * @param historyLimit Maximum number of undo/redo states (0 disables history).
		 * @param capacityLimit Maximum number of elements allowed in the map (0 means unlimited).
		 */
		explicit bwxPropertyMap(wxEvtHandler* handler = nullptr,
			ChangeCallback callback = nullptr,
			size_t historyLimit = 0,
			size_t capacityLimit = 0)
			: eventHandler_(handler),
			onChange_(std::move(callback)),
			historyLimit_(historyLimit),
			capacityLimit_(capacityLimit),
			lastChangeTime_(std::chrono::system_clock::now()) {}

		/**
		 * @brief Inserts or updates a value associated with a key.
		 * @param key Key to be inserted or updated.
		 * @param value Value to be associated with the key.
		 * @throws std::overflow_error If the capacity limit is reached.
		 */
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

		/**
		 * @brief Retrieves the value associated with a given key.
		 * @param key Key whose associated value is to be retrieved.
		 * @return Optional containing the value if found, otherwise std::nullopt.
		 */
		std::optional<V> get(const K& key) const {
			wxMutexLocker lock(mutex_);
			auto it = data_.find(key);
			return (it != data_.end()) ? std::optional<V>(it->second) : std::nullopt;
		}

		/**
		 * @brief Retrieves a reference to the value associated with a key.
		 * @param key Key whose value reference is to be retrieved.
		 * @return Reference to the associated value.
		 * @throws std::out_of_range If the key does not exist.
		 */
		const V& getRef(const K& key) const {
			wxMutexLocker lock(mutex_);
			auto it = data_.find(key);
			if (it == data_.end()) throw std::out_of_range("Key not found");
			return it->second;
		}

		/**
		 * @brief Clears all elements from the map.
		 */
		void clear() {
			wxMutexLocker lock(mutex_);
			recordHistory();
			data_.clear();
			notifyChange();
		}

		/**
		 * @brief Returns the number of elements in the map.
		 * @return Current size of the map.
		 */
		size_t size() const {
			wxMutexLocker lock(mutex_);
			return data_.size();
		}

		typename std::map<K, V>::iterator begin() { return data_.begin(); }
		typename std::map<K, V>::iterator end() { return data_.end(); }
		typename std::map<K, V>::const_iterator cbegin() const { return data_.cbegin(); }
		typename std::map<K, V>::const_iterator cend() const { return data_.cend(); }

		/**
		 * @brief Enables or disables read-only mode.
		 * @param readOnly True to enable read-only mode, false to disable it.
		 */
		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(mutex_);
			readOnly_ = readOnly;
		}

		/**
		 * @brief Checks if the map is currently in read-only mode.
		 * @return True if read-only mode is enabled, otherwise false.
		 */
		bool isReadOnly() const noexcept {
			wxMutexLocker lock(mutex_);
			return readOnly_;
		}

		/**
		 * @brief Reverts to the previous state using the undo history.
		 * @return True if undo was successful, false otherwise.
		 */
		bool undo() {
			wxMutexLocker lock(mutex_);
			if (undoHistory_.empty()) return false;

			redoHistory_.push_front(data_);
			data_ = undoHistory_.front();
			undoHistory_.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Restores the last undone state using the redo history.
		 * @return True if redo was successful, false otherwise.
		 */
		bool redo() {
			wxMutexLocker lock(mutex_);
			if (redoHistory_.empty()) return false;

			undoHistory_.push_front(data_);
			data_ = redoHistory_.front();
			redoHistory_.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Sets the maximum number of undo/redo history entries.
		 * @param limit Number of history entries to retain.
		 */
		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			historyLimit_ = limit;
			trimHistory(undoHistory_);
			trimHistory(redoHistory_);
		}

		/**
		 * @brief Retrieves the configured undo/redo history limit.
		 * @return History limit value.
		 */
		size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return historyLimit_;
		}

		/**
		 * @brief Clears the undo and redo histories.
		 */
		void clearHistory() {
			wxMutexLocker lock(mutex_);
			undoHistory_.clear();
			redoHistory_.clear();
		}

		/**
		 * @brief Sets the capacity limit for the map.
		 * @param limit Maximum number of elements allowed.
		 */
		void setCapacityLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			capacityLimit_ = limit;
			trimToCapacity();
		}

		/**
		 * @brief Retrieves the configured capacity limit.
		 * @return Capacity limit value.
		 */
		size_t getCapacityLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return capacityLimit_;
		}

		/**
		 * @brief Retrieves the timestamp of the last modification.
		 * @return Timestamp of the most recent change.
		 */
		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(mutex_);
			return lastChangeTime_;
		}

		/**
		 * @brief Assigns a callback function for change notifications.
		 * @param callback Callback function to be triggered.
		 */
		void setOnChangeCallback(ChangeCallback callback) {
			wxMutexLocker lock(mutex_);
			onChange_ = std::move(callback);
		}

		/**
		 * @brief Assigns a wxWidgets event handler for notifications.
		 * @param handler Pointer to the wxWidgets event handler.
		 */
		void setEventHandler(wxEvtHandler* handler) {
			wxMutexLocker lock(mutex_);
			eventHandler_ = handler;
		}

		/**
		 * @brief Converts the value of a key to a C-style string.
		 * @param key Key whose associated value is converted.
		 * @return Pointer to the C-string or nullptr if unsupported or not found.
		 */
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
		/** @brief Notifies listeners through callbacks and wxWidgets events. */
		void notifyChange() {
			if (onChange_) onChange_();
			if (eventHandler_) {
				wxCommandEvent evt(EVT_PROPERTY_MAP_CHANGED);
				eventHandler_->AddPendingEvent(evt);
			}
		}

		/** @brief Records the current state into the undo history. */
		void recordHistory() {
			if (historyLimit_ == 0) return;
			undoHistory_.push_front(data_);
			trimHistory(undoHistory_);
			redoHistory_.clear();
		}

		/** @brief Trims the history to respect the configured limit. */
		void trimHistory(std::deque<std::map<K, V>>& history) {
			while (history.size() > historyLimit_) {
				history.pop_back();
			}
		}

		/** @brief Ensures the map respects the capacity limit by removing oldest entries. */
		void trimToCapacity() {
			while (capacityLimit_ > 0 && data_.size() > capacityLimit_) {
				data_.erase(data_.begin());
			}
		}

		std::map<K, V> data_{};                          ///< Internal map storing key-value pairs.
		size_t historyLimit_ = 0;                        ///< Undo/redo history limit.
		size_t capacityLimit_ = 0;                       ///< Maximum allowed number of elements.
		std::deque<std::map<K, V>> undoHistory_;         ///< Undo history states.
		std::deque<std::map<K, V>> redoHistory_;         ///< Redo history states.
		wxEvtHandler* eventHandler_ = nullptr;           ///< wxWidgets event handler.
		ChangeCallback onChange_ = nullptr;              ///< Callback function for changes.
		Timestamp lastChangeTime_;                       ///< Timestamp of the last change.
		bool readOnly_ = false;                          ///< Read-only mode flag.
		mutable wxMutex mutex_;                          ///< Mutex for thread-safe access.
	};

}

#endif // _BWXOOP_H_
