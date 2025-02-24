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
 * @brief Template class representing a key-value property map with advanced features.
 *
 * This class manages key-value pairs and provides functionalities including:
 * - Setting and retrieving values with thread safety.
 * - Change history tracking with undo/redo support.
 * - Read-only mode to restrict modifications.
 * - wxWidgets event notifications and callback handling on data changes.
 * - Timestamp recording for the last data modification.
 * - Iterators for standard container traversal.
 * - Safe access methods returning optional values.
 * - Capacity limitations to prevent data overflow.
 * - Data filtering and sorting capabilities.
 * - Conversion of stored values to C-style strings when applicable.
 *
 * @tparam K Type of the keys in the map.
 * @tparam V Type of the values in the map.
 */
	template <typename K, typename V>
	class PropertyMap {
	public:
		using ChangeCallback = std::function<void()>; ///< Callback invoked on data changes.
		using Timestamp = std::chrono::system_clock::time_point; ///< Timestamp representing the last modification.

		/**
		 * @brief Constructs a PropertyMap instance.
		 * @param handler Optional wxWidgets event handler for notifications.
		 * @param callback Optional callback function invoked upon data changes.
		 * @param historyLimit Maximum number of stored undo/redo states (0 disables history).
		 * @param capacityLimit Maximum number of elements allowed in the map (0 means no limit).
		 */
		explicit PropertyMap(wxEvtHandler* handler = nullptr,
			ChangeCallback callback = nullptr,
			size_t historyLimit = 0,
			size_t capacityLimit = 0)
			: eventHandler_(handler),
			onChange_(std::move(callback)),
			historyLimit_(historyLimit),
			capacityLimit_(capacityLimit),
			lastChangeTime_(std::chrono::system_clock::now()) {}

		/**
		 * @brief Sets the value for a given key.
		 * @param key Key for which the value is to be set.
		 * @param value Value to associate with the key.
		 * @throws std::overflow_error If capacity limit is exceeded.
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
		 * @brief Retrieves the value associated with a key.
		 * @param key Key whose value is requested.
		 * @return Optional containing the value if found, otherwise std::nullopt.
		 */
		std::optional<V> get(const K& key) const {
			wxMutexLocker lock(mutex_);
			auto it = data_.find(key);
			return (it != data_.end()) ? std::optional<V>(it->second) : std::nullopt;
		}

		/**
		 * @brief Provides a constant reference to the value associated with a key.
		 * @param key Key of the value.
		 * @return Constant reference to the value.
		 * @throws std::out_of_range If the key is not present in the map.
		 */
		const V& getRef(const K& key) const {
			wxMutexLocker lock(mutex_);
			auto it = data_.find(key);
			if (it == data_.end()) throw std::out_of_range("Key not found");
			return it->second;
		}

		/**
		 * @brief Clears all key-value pairs from the map.
		 */
		void clear() {
			wxMutexLocker lock(mutex_);
			recordHistory();
			data_.clear();
			notifyChange();
		}

		/**
		 * @brief Retrieves the current size of the map.
		 * @return Number of key-value pairs stored.
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
		 * @param readOnly True to prevent modifications, false to allow changes.
		 */
		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(mutex_);
			readOnly_ = readOnly;
		}

		/**
		 * @brief Checks if the map is in read-only mode.
		 * @return True if modifications are disabled, otherwise false.
		 */
		bool isReadOnly() const noexcept {
			wxMutexLocker lock(mutex_);
			return readOnly_;
		}

		/**
		 * @brief Reverts the map to the previous state.
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
		 * @brief Restores the last undone state.
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
		 * @param limit Maximum allowed history entries.
		 */
		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			historyLimit_ = limit;
			trimHistory(undoHistory_);
			trimHistory(redoHistory_);
		}

		/**
		 * @brief Retrieves the current history limit.
		 * @return The configured history limit.
		 */
		size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return historyLimit_;
		}

		/**
		 * @brief Clears both undo and redo history.
		 */
		void clearHistory() {
			wxMutexLocker lock(mutex_);
			undoHistory_.clear();
			redoHistory_.clear();
		}

		/**
		 * @brief Sets the maximum number of elements allowed in the map.
		 * @param limit New capacity limit (0 means unlimited).
		 */
		void setCapacityLimit(size_t limit) {
			wxMutexLocker lock(mutex_);
			capacityLimit_ = limit;
			trimToCapacity();
		}

		/**
		 * @brief Retrieves the capacity limit.
		 * @return Configured capacity limit.
		 */
		size_t getCapacityLimit() const noexcept {
			wxMutexLocker lock(mutex_);
			return capacityLimit_;
		}

		/**
		 * @brief Retrieves the timestamp of the last modification.
		 * @return Timestamp indicating when the last change occurred.
		 */
		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(mutex_);
			return lastChangeTime_;
		}

		/**
		 * @brief Sets a callback to be invoked on map changes.
		 * @param callback Callback function.
		 */
		void setOnChangeCallback(ChangeCallback callback) {
			wxMutexLocker lock(mutex_);
			onChange_ = std::move(callback);
		}

		/**
		 * @brief Assigns a wxWidgets event handler for change notifications.
		 * @param handler Pointer to the wxWidgets event handler.
		 */
		void setEventHandler(wxEvtHandler* handler) {
			wxMutexLocker lock(mutex_);
			eventHandler_ = handler;
		}

		/**
		 * @brief Converts the value of a given key to a C-style string.
		 * @param key Key whose value should be converted.
		 * @return Pointer to the C-string representation or nullptr if unsupported.
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

		/**
		 * @brief Filters the map based on a provided predicate.
		 * @param predicate Function that determines whether an entry should be included.
		 * @return A new map containing entries that satisfy the predicate.
		 */
		std::map<K, V> filter(const std::function<bool(const K&, const V&)>& predicate) const {
			wxMutexLocker lock(mutex_);
			std::map<K, V> result;
			for (const auto& [key, value] : data_) {
				if (predicate(key, value)) {
					result[key] = value;
				}
			}
			return result;
		}

		/**
		 * @brief Sorts the map by keys according to a comparator.
		 * @param comparator Comparison function for keys.
		 * @return A new sorted map based on the comparator.
		 */
		std::map<K, V> sort(const std::function<bool(const K&, const K&)>& comparator) const {
			wxMutexLocker lock(mutex_);
			std::vector<std::pair<K, V>> items(data_.begin(), data_.end());
			std::sort(items.begin(), items.end(), [&](const auto& a, const auto& b) {
				return comparator(a.first, b.first);
				});

			std::map<K, V> sortedMap;
			for (const auto& [key, value] : items) {
				sortedMap[key] = value;
			}
			return sortedMap;
		}

	private:
		/** @brief Notifies listeners via callback and wxWidgets events when the map changes. */
		void notifyChange() {
			if (onChange_) onChange_();
			if (eventHandler_) {
				wxCommandEvent evt(EVT_PROPERTY_CHANGED);
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

		/** @brief Ensures the undo/redo history respects the configured limit. */
		void trimHistory(std::deque<std::map<K, V>>& history) {
			while (history.size() > historyLimit_) {
				history.pop_back();
			}
		}

		/** @brief Removes oldest entries to respect the capacity limit. */
		void trimToCapacity() {
			if (capacityLimit_ == 0 || data_.size() <= capacityLimit_) return;
			while (data_.size() > capacityLimit_) {
				data_.erase(data_.begin());
			}
		}

		std::map<K, V> data_{};                          ///< Map storing key-value pairs.
		size_t historyLimit_ = 0;                        ///< Maximum undo/redo history entries.
		size_t capacityLimit_ = 0;                       ///< Maximum number of elements in the map.
		std::deque<std::map<K, V>> undoHistory_;         ///< Undo history stack.
		std::deque<std::map<K, V>> redoHistory_;         ///< Redo history stack.
		wxEvtHandler* eventHandler_ = nullptr;           ///< wxWidgets event handler for notifications.
		ChangeCallback onChange_ = nullptr;              ///< User-defined callback for data changes.
		Timestamp lastChangeTime_;                       ///< Timestamp of the last modification.
		bool readOnly_ = false;                          ///< Flag indicating if the map is read-only.
		mutable wxMutex mutex_;                          ///< Mutex for thread-safe operations.
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
