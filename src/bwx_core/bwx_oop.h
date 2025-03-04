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

#include <algorithm>
#include <chrono>
#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <wx/event.h>
#include <wx/thread.h>

/**
 * @file bwx_oop.h
 * @brief Provides a template class for advanced property management.
 */

 // wxWidgets events declaration for value change notifications
wxDECLARE_EVENT(EVT_BWXPROPERTY_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_BWXPROPERTY_VECTOR_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_BWXPROPERTY_MAP_CHANGED, wxCommandEvent);

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
				: m_value(defaultValue),
				m_defaultValue(defaultValue),
				m_validator(std::move(validator)),
				m_onChange(std::move(onChange)),
				m_onReject(std::move(onReject)),
				m_historyLimit(historyLimit),
				m_undoTimeout(undoTimeout),
				m_lastChangeTime(std::chrono::system_clock::now()),
				m_eventHandler(eventHandler) {} 

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
			wxMutexLocker lock(m_mutex);
			if (m_readOnly) return;

			if (m_validator && !m_validator(newValue)) {
				if (m_onReject) m_onReject(newValue);
				return;
			}

			if (newValue != m_value) {
				recordHistory(m_value);
				clearRedoHistory();
				if (m_onChange) m_onChange(m_value, newValue);

				m_value = newValue;
				m_lastChangeTime = std::chrono::system_clock::now();

				notifyChange();
				propagateBinding(newValue);
			}
		}

		/**
		 * @brief Retrieves the current value.
		 * @return Constant reference to the value.
		 */
		[[nodiscard]] const T& get() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_value;
		}

		/**
		 * @brief Converts the value to a C-style string.
		 * @return Pointer to the C-string or nullptr if unsupported.
		 */
		const char* AsCStr() const {
			if constexpr (std::is_same_v<T, std::string>) {
				return m_value.c_str();
			}
			else if constexpr (std::is_same_v<T, wxString>) {
				return m_value.mb_str().data();
			}
			else {
				static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, wxString>,
					"AsCStr() is only supported for std::string or wxString.");
				return nullptr;
			}
		}

		/**
		 * @brief Resets the value to its default.
		 */
		void reset() {
			set(m_defaultValue);
		}

		/**
		 * @brief Sets a new validator function.
		 * @param validator Validator function to set.
		 */
		void setValidator(Validator validator) {
			wxMutexLocker lock(m_mutex);
			m_validator = std::move(validator);
		}

		/**
		 * @brief Assigns a callback for value changes.
		 * @param onChange Callback to set.
		 */
		void setOnChangeCallback(ChangeCallback onChange) {
			wxMutexLocker lock(m_mutex);
			m_onChange = std::move(onChange);
		}

		/**
		 * @brief Assigns a callback for value rejections.
		 * @param onReject Callback to set.
		 */
		void setOnRejectCallback(RejectCallback onReject) {
			wxMutexLocker lock(m_mutex);
			m_onReject = std::move(onReject);
		}

		/**
		 * @brief Enables or disables read-only mode.
		 * @param readOnly True to enable, false to disable.
		 */
		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(m_mutex);
			m_readOnly = readOnly;
		}

		/**
		 * @brief Checks if the property is read-only.
		 * @return True if read-only, false otherwise.
		 */
		[[nodiscard]] bool isReadOnly() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_readOnly;
		}

		/**
		 * @brief Undoes the last value change.
		 * @return True if successful, false otherwise.
		 */
		bool undo() {
			wxMutexLocker lock(m_mutex);
			if (m_undoHistory.empty()) return false;

			auto now = std::chrono::system_clock::now();
			if (m_undoTimeout && std::chrono::duration_cast<std::chrono::seconds>(now - m_lastChangeTime) > *m_undoTimeout) {
				return false;
			}

			m_redoHistory.push_front(m_value);
			m_value = m_undoHistory.front();
			m_undoHistory.pop_front();
			m_lastChangeTime = now;

			notifyChange();
			propagateBinding(m_value);
			return true;
		}

		/**
		 * @brief Redoes the previously undone value change.
		 * @return True if successful, false otherwise.
		 */
		bool redo() {
			wxMutexLocker lock(m_mutex);
			if (m_redoHistory.empty()) return false;

			m_undoHistory.push_front(m_value);
			m_value = m_redoHistory.front();
			m_redoHistory.pop_front();
			m_lastChangeTime = std::chrono::system_clock::now();

			notifyChange();
			propagateBinding(m_value);
			return true;
		}

		/**
		 * @brief Sets the history limit for undo/redo operations.
		 * @param limit Number of history entries to retain.
		 */
		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(m_mutex);
			m_historyLimit = limit;
			trimHistory(m_undoHistory);
			trimHistory(m_redoHistory);
		}

		/**
		 * @brief Retrieves the configured history limit.
		 * @return History limit.
		 */
		[[nodiscard]] size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_historyLimit;
		}

		/**
		 * @brief Clears the undo and redo histories.
		 */
		void clearHistory() {
			wxMutexLocker lock(m_mutex);
			m_undoHistory.clear();
			m_redoHistory.clear();
		}

		/**
		 * @brief Sets the timeout for undo operations.
		 * @param timeout Timeout duration.
		 */
		void setUndoTimeout(std::chrono::seconds timeout) {
			wxMutexLocker lock(m_mutex);
			m_undoTimeout = timeout;
		}

		/**
		 * @brief Retrieves the undo timeout duration.
		 * @return Optional undo timeout.
		 */
		[[nodiscard]] std::optional<std::chrono::seconds> getUndoTimeout() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_undoTimeout;
		}

		/**
		 * @brief Retrieves the timestamp of the last change.
		 * @return Last change timestamp.
		 */
		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(m_mutex);
			return m_lastChangeTime;
		}

		/**
		 * @brief Binds another property to synchronize value changes.
		 * @param other Property to bind to.
		 */
		void bind(bwxProperty<T>& other) {
			wxMutexLocker lock(m_mutex);
			m_boundProperties.push_back(&other);
		}

		// Arithmetic operators
		bwxProperty<T>& operator+=(const T& rhs) { set(m_value + rhs); return *this; }
		bwxProperty<T>& operator-=(const T& rhs) { set(m_value - rhs); return *this; }
		bwxProperty<T>& operator*=(const T& rhs) { set(m_value * rhs); return *this; }
		bwxProperty<T>& operator/=(const T& rhs) { set(m_value / rhs); return *this; }

		// Comparison operators
		bool operator==(const T& rhs) const { return m_value == rhs; }
		bool operator!=(const T& rhs) const { return m_value != rhs; }
		bool operator<(const T& rhs) const { return m_value < rhs; }
		bool operator<=(const T& rhs) const { return m_value <= rhs; }
		bool operator>(const T& rhs) const { return m_value > rhs; }
		bool operator>=(const T& rhs) const { return m_value >= rhs; }

	private:
		/** @brief Records the current value into the undo history. */
		void recordHistory(const T& oldValue) {
			if (m_historyLimit == 0) return;
			m_undoHistory.push_front(oldValue);
			trimHistory(m_undoHistory);
		}

		/** @brief Clears the redo history. */
		void clearRedoHistory() {
			m_redoHistory.clear();
		}

		/** @brief Trims the history to respect the configured limit. */
		void trimHistory(std::deque<T>& history) {
			while (history.size() > m_historyLimit) {
				history.pop_back();
			}
		}

		/** @brief Notifies listeners through callbacks and wxWidgets events. */
		void notifyChange() {
			if (m_eventHandler) {
				wxCommandEvent evt(EVT_BWXPROPERTY_CHANGED);
				m_eventHandler->AddPendingEvent(evt);
			}
		}

		/** @brief Propagates value changes to bound properties. */
		void propagateBinding(const T& newValue) {
			for (auto* boundProp : m_boundProperties) {
				if (boundProp) boundProp->set(newValue);
			}
		}

		T m_value{};                                    ///< Current value of the property.
		T m_defaultValue{};                              ///< Default value used for resets.
		Validator m_validator = nullptr;                 ///< Validator function for new values.
		ChangeCallback m_onChange = nullptr;             ///< Callback for value changes.
		RejectCallback m_onReject = nullptr;             ///< Callback for rejected values.
		size_t m_historyLimit = 0;                       ///< Undo/redo history limit.
		std::optional<std::chrono::seconds> m_undoTimeout = std::nullopt; ///< Undo operation timeout.
		bool m_readOnly = false;                         ///< Read-only mode flag.
		wxEvtHandler* m_eventHandler = nullptr;          ///< wxWidgets event handler.
		Timestamp m_lastChangeTime;                      ///< Timestamp of the last modification.
		std::deque<T> m_undoHistory;                     ///< Undo history.
		std::deque<T> m_redoHistory;                     ///< Redo history.
		std::vector<bwxProperty<T>*> m_boundProperties;     ///< Bound properties for synchronization.
		mutable wxMutex m_mutex;                         ///< Mutex for thread safety.
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
				: m_eventHandler(handler),
				m_onChange(std::move(callback)),
				m_historyLimit(historyLimit),
				m_capacityLimit(capacityLimit),
				m_lastChangeTime(std::chrono::system_clock::now()) {
		}

		/**
		 * @brief Adds a value to the vector.
		 * @param value The value to be added.
		 * @throws std::overflow_error If capacity limit is reached.
		 */
		void add(const T& value) {
			wxMutexLocker lock(m_mutex);
			if (m_readOnly) return;
			if (m_capacityLimit > 0 && m_data.size() >= m_capacityLimit) {
				throw std::overflow_error("Capacity limit reached");
			}

			recordHistory();
			m_data.push_back(value);
			m_lastChangeTime = std::chrono::system_clock::now();
			notifyChange();
		}

		/**
		 * @brief Retrieves the value at a specified index safely.
		 * @param index Index of the element.
		 * @return Optional containing the value if valid, otherwise std::nullopt.
		 */
		std::optional<T> get(size_t index) const {
			wxMutexLocker lock(m_mutex);
			return (index < m_data.size()) ? std::optional<T>(m_data[index]) : std::nullopt;
		}

		/**
		 * @brief Accesses the element at the specified index with bounds checking.
		 * @param index Index of the element.
		 * @return Reference to the element.
		 * @throws std::out_of_range If the index is invalid.
		 */
		T& at(size_t index) {
			wxMutexLocker lock(m_mutex);
			if (index >= m_data.size()) throw std::out_of_range("Index out of range");
			return m_data[index];
		}

		/**
		 * @brief Clears all elements from the vector.
		 */
		void clear() {
			wxMutexLocker lock(m_mutex);
			recordHistory();
			m_data.clear();
			notifyChange();
		}

		/**
		 * @brief Retrieves the size of the vector.
		 * @return Number of elements in the vector.
		 */
		size_t size() const {
			wxMutexLocker lock(m_mutex);
			return m_data.size();
		}

		typename std::vector<T>::iterator begin() { return m_data.begin(); }
		typename std::vector<T>::iterator end() { return m_data.end(); }
		typename std::vector<T>::const_iterator cbegin() const { return m_data.cbegin(); }
		typename std::vector<T>::const_iterator cend() const { return m_data.cend(); }

		/**
		 * @brief Enables or disables read-only mode.
		 * @param readOnly True to enable, false to disable modifications.
		 */
		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(m_mutex);
			m_readOnly = readOnly;
		}

		/**
		 * @brief Checks if the vector is in read-only mode.
		 * @return True if read-only, otherwise false.
		 */
		bool isReadOnly() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_readOnly;
		}

		/**
		 * @brief Reverts to the previous state from the undo history.
		 * @return True if successful, false otherwise.
		 */
		bool undo() {
			wxMutexLocker lock(m_mutex);
			if (m_undoHistory.empty()) return false;

			m_redoHistory.push_front(m_data);
			m_data = m_undoHistory.front();
			m_undoHistory.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Restores the last undone state from the redo history.
		 * @return True if successful, false otherwise.
		 */
		bool redo() {
			wxMutexLocker lock(m_mutex);
			if (m_redoHistory.empty()) return false;

			m_undoHistory.push_front(m_data);
			m_data = m_redoHistory.front();
			m_redoHistory.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Sets the history limit for undo/redo operations.
		 * @param limit Number of history entries to retain.
		 */
		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(m_mutex);
			m_historyLimit = limit;
			trimHistory(m_undoHistory);
			trimHistory(m_redoHistory);
		}

		/**
		 * @brief Retrieves the current undo/redo history limit.
		 * @return Configured history limit.
		 */
		size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_historyLimit;
		}

		/**
		 * @brief Clears the undo and redo history.
		 */
		void clearHistory() {
			wxMutexLocker lock(m_mutex);
			m_undoHistory.clear();
			m_redoHistory.clear();
		}

		/**
		 * @brief Sets the capacity limit of the vector.
		 * @param limit Maximum allowed number of elements.
		 */
		void setCapacityLimit(size_t limit) {
			wxMutexLocker lock(m_mutex);
			m_capacityLimit = limit;
			trimToCapacity();
		}

		/**
		 * @brief Retrieves the vector's capacity limit.
		 * @return Configured capacity limit.
		 */
		size_t getCapacityLimit() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_capacityLimit;
		}

		/**
		 * @brief Retrieves the timestamp of the last modification.
		 * @return Timestamp of the last change.
		 */
		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(m_mutex);
			return m_lastChangeTime;
		}

		/**
		 * @brief Assigns a callback function for change notifications.
		 * @param callback Callback function to be invoked.
		 */
		void setOnChangeCallback(ChangeCallback callback) {
			wxMutexLocker lock(m_mutex);
			m_onChange = std::move(callback);
		}

		/**
		 * @brief Assigns a wxWidgets event handler for notifications.
		 * @param handler Pointer to the event handler.
		 */
		void setEventHandler(wxEvtHandler* handler) {
			wxMutexLocker lock(m_mutex);
			m_eventHandler = handler;
		}

		/**
		 * @brief Converts an element to a C-style string.
		 * @param index Index of the element.
		 * @return Pointer to the C-string or nullptr if invalid or unsupported.
		 */
		const char* AsCStr(size_t index = 0) const {
			wxMutexLocker lock(m_mutex);
			if (index >= m_data.size()) return nullptr;

			if constexpr (std::is_same_v<T, std::string>) {
				return m_data[index].c_str();
			}
			else if constexpr (std::is_same_v<T, wxString>) {
				return m_data[index].mb_str().data();
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
			if (m_onChange) m_onChange();
			if (m_eventHandler) {
				wxCommandEvent evt(EVT_BWXPROPERTY_VECTOR_CHANGED);
				m_eventHandler->AddPendingEvent(evt);
			}
		}

		/** @brief Records the current vector state into the undo history. */
		void recordHistory() {
			if (m_historyLimit == 0) return;
			m_undoHistory.push_front(m_data);
			trimHistory(m_undoHistory);
			m_redoHistory.clear();
		}

		/** @brief Trims the history deque to comply with the history limit. */
		void trimHistory(std::deque<std::vector<T>>& history) {
			while (history.size() > m_historyLimit) {
				history.pop_back();
			}
		}

		/** @brief Ensures the vector respects the capacity limit by removing oldest elements. */
		void trimToCapacity() {
			while (m_capacityLimit > 0 && m_data.size() > m_capacityLimit) {
				m_data.erase(m_data.begin());
			}
		}

		std::vector<T> m_data{};                          ///< Internal vector storing elements.
		size_t m_historyLimit = 0;                        ///< Undo/redo history limit.
		size_t m_capacityLimit = 0;                       ///< Maximum allowed vector capacity.
		std::deque<std::vector<T>> m_undoHistory;        ///< Undo history states.
		std::deque<std::vector<T>> m_redoHistory;        ///< Redo history states.
		wxEvtHandler* m_eventHandler = nullptr;           ///< wxWidgets event handler.
		ChangeCallback m_onChange = nullptr;              ///< Callback for vector changes.
		Timestamp m_lastChangeTime;                       ///< Last modification timestamp.
		bool m_readOnly = false;                          ///< Read-only mode flag.
		mutable wxMutex m_mutex;                          ///< Mutex for thread-safe access.
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
				: m_eventHandler(handler),
				m_onChange(std::move(callback)),
				m_historyLimit(historyLimit),
				m_capacityLimit(capacityLimit),
				m_lastChangeTime(std::chrono::system_clock::now()) {
		}

		/**
		 * @brief Inserts or updates a value associated with a key.
		 * @param key Key to be inserted or updated.
		 * @param value Value to be associated with the key.
		 * @throws std::overflow_error If the capacity limit is reached.
		 */
		void set(const K& key, const V& value) {
			wxMutexLocker lock(m_mutex);
			if (m_readOnly) return;

			if (m_capacityLimit > 0 && m_data.size() >= m_capacityLimit && m_data.find(key) == m_data.end()) {
				throw std::overflow_error("Capacity limit reached");
			}

			recordHistory();
			m_data[key] = value;
			m_lastChangeTime = std::chrono::system_clock::now();
			notifyChange();
		}

		/**
		 * @brief Retrieves the value associated with a given key.
		 * @param key Key whose associated value is to be retrieved.
		 * @return Optional containing the value if found, otherwise std::nullopt.
		 */
		std::optional<V> get(const K& key) const {
			wxMutexLocker lock(m_mutex);
			auto it = m_data.find(key);
			if (it != m_data.end()) return std::make_optional(it->second);
			return std::nullopt;
		}

		/**
		 * @brief Retrieves a reference to the value associated with a key.
		 * @param key Key whose value reference is to be retrieved.
		 * @return Reference to the associated value.
		 * @throws std::out_of_range If the key does not exist.
		 */
		const V& getRef(const K& key) const {
			wxMutexLocker lock(m_mutex);
			auto it = m_data.find(key);
			if (it == m_data.end()) throw std::out_of_range("Key not found");
			return it->second;
		}

		/**
		 * @brief Clears all elements from the map.
		 */
		void clear() {
			wxMutexLocker lock(m_mutex);
			recordHistory();
			m_data.clear();
			notifyChange();
		}

		/**
		 * @brief Returns the number of elements in the map.
		 * @return Current size of the map.
		 */
		size_t size() const {
			wxMutexLocker lock(m_mutex);
			return m_data.size();
		}

		typename std::map<K, V>::iterator begin() { return m_data.begin(); }
		typename std::map<K, V>::iterator end() { return m_data.end(); }
		typename std::map<K, V>::const_iterator cbegin() const { return m_data.cbegin(); }
		typename std::map<K, V>::const_iterator cend() const { return m_data.cend(); }

		/**
		 * @brief Enables or disables read-only mode.
		 * @param readOnly True to enable read-only mode, false to disable it.
		 */
		void setReadOnly(bool readOnly) {
			wxMutexLocker lock(m_mutex);
			m_readOnly = readOnly;
		}

		/**
		 * @brief Checks if the map is currently in read-only mode.
		 * @return True if read-only mode is enabled, otherwise false.
		 */
		bool isReadOnly() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_readOnly;
		}

		/**
		 * @brief Reverts to the previous state using the undo history.
		 * @return True if undo was successful, false otherwise.
		 */
		bool undo() {
			wxMutexLocker lock(m_mutex);
			if (m_undoHistory.empty()) return false;

			m_redoHistory.push_front(m_data);
			m_data = m_undoHistory.front();
			m_undoHistory.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Restores the last undone state using the redo history.
		 * @return True if redo was successful, false otherwise.
		 */
		bool redo() {
			wxMutexLocker lock(m_mutex);
			if (m_redoHistory.empty()) return false;

			m_undoHistory.push_front(m_data);
			m_data = m_redoHistory.front();
			m_redoHistory.pop_front();
			notifyChange();
			return true;
		}

		/**
		 * @brief Sets the maximum number of undo/redo history entries.
		 * @param limit Number of history entries to retain.
		 */
		void setHistoryLimit(size_t limit) {
			wxMutexLocker lock(m_mutex);
			m_historyLimit = limit;
			trimHistory(m_undoHistory);
			trimHistory(m_redoHistory);
		}

		/**
		 * @brief Retrieves the configured undo/redo history limit.
		 * @return History limit value.
		 */
		size_t getHistoryLimit() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_historyLimit;
		}

		/**
		 * @brief Clears the undo and redo histories.
		 */
		void clearHistory() {
			wxMutexLocker lock(m_mutex);
			m_undoHistory.clear();
			m_redoHistory.clear();
		}

		/**
		 * @brief Sets the capacity limit for the map.
		 * @param limit Maximum number of elements allowed.
		 */
		void setCapacityLimit(size_t limit) {
			wxMutexLocker lock(m_mutex);
			m_capacityLimit = limit;
			trimToCapacity();
		}

		/**
		 * @brief Retrieves the configured capacity limit.
		 * @return Capacity limit value.
		 */
		size_t getCapacityLimit() const noexcept {
			wxMutexLocker lock(m_mutex);
			return m_capacityLimit;
		}

		/**
		 * @brief Retrieves the timestamp of the last modification.
		 * @return Timestamp of the most recent change.
		 */
		Timestamp getLastChangeTime() const {
			wxMutexLocker lock(m_mutex);
			return m_lastChangeTime;
		}

		/**
		 * @brief Assigns a callback function for change notifications.
		 * @param callback Callback function to be triggered.
		 */
		void setOnChangeCallback(ChangeCallback callback) {
			wxMutexLocker lock(m_mutex);
			m_onChange = std::move(callback);
		}

		/**
		 * @brief Assigns a wxWidgets event handler for notifications.
		 * @param handler Pointer to the wxWidgets event handler.
		 */
		void setEventHandler(wxEvtHandler* handler) {
			wxMutexLocker lock(m_mutex);
			m_eventHandler = handler;
		}

		/**
		 * @brief Converts the value of a key to a C-style string.
		 * @param key Key whose associated value is converted.
		 * @return Pointer to the C-string or nullptr if unsupported or not found.
		 */
		const char* AsCStr(const K& key) const {
			wxMutexLocker lock(m_mutex);
			auto it = m_data.find(key);
			if (it == m_data.end()) return nullptr;

			if constexpr (std::is_same_v<V, std::string>) {
				return it->second.c_str();
			}
			else if constexpr (std::is_same_v<V, wxString>) {
				return it->second.mb_str().data();
			}
			else {
				return nullptr; // Zapobiega b³êdowi kompilacji dla innych typów
			}
		}

		/**
		 * @brief Retrieves a reference to the value associated with a key.
		 * If the key does not exist, a default-constructed value is inserted.
		 * @param key Key whose associated value is to be retrieved.
		 * @return Reference to the associated value.
		 */
		V& operator[](const K& key) {
			wxMutexLocker lock(m_mutex);
			if (m_readOnly) {
				throw std::runtime_error("Modification not allowed in read-only mode");
			}

			recordHistory();
			auto it = m_data.find(key);
			if (it == m_data.end()) {
				m_data[key] = V(); // Wymusza inicjalizacjê, jeœli brak domyœlnego konstruktora
			}
			m_lastChangeTime = std::chrono::system_clock::now();
			notifyChange();
			return m_data[key];
		}

		/**
		 * @brief Retrieves a constant reference to the value associated with a key.
		 * @param key Key whose associated value is to be retrieved.
		 * @return Constant reference to the associated value.
		 * @throws std::out_of_range If the key does not exist.
		 */
		const V& operator[](const K& key) const {
			wxMutexLocker lock(m_mutex);
			auto it = m_data.find(key);
			if (it == m_data.end()) {
				throw std::out_of_range("Key not found");
			}
			return it->second;
		}

	private:
		/** @brief Notifies listeners through callbacks and wxWidgets events. */
		void notifyChange() {
			if (m_onChange) m_onChange();
			if (m_eventHandler) {
				wxCommandEvent evt(EVT_BWXPROPERTY_MAP_CHANGED);
				m_eventHandler->AddPendingEvent(evt);
			}
		}

		/** @brief Records the current state into the undo history. */
		void recordHistory() {
			if (m_historyLimit == 0) return;
			m_undoHistory.push_front(m_data);
			trimHistory(m_undoHistory);
			m_redoHistory.clear();
		}

		/** @brief Trims the history to respect the configured limit. */
		void trimHistory(std::deque<std::map<K, V>>& history) {
			while (history.size() > m_historyLimit) {
				history.pop_back();
			}
		}

		/** @brief Ensures the map respects the capacity limit by removing oldest entries. */
		void trimToCapacity() {
			while (m_capacityLimit > 0 && m_data.size() > m_capacityLimit) {
				m_data.erase(m_data.begin());
			}
		}

		std::map<K, V> m_data{};                          ///< Internal map storing key-value pairs.
		size_t m_historyLimit = 0;                        ///< Undo/redo history limit.
		size_t m_capacityLimit = 0;                       ///< Maximum allowed number of elements.
		std::deque<std::map<K, V>> m_undoHistory;         ///< Undo history states.
		std::deque<std::map<K, V>> m_redoHistory;         ///< Redo history states.
		wxEvtHandler* m_eventHandler = nullptr;           ///< wxWidgets event handler.
		ChangeCallback m_onChange = nullptr;              ///< Callback function for changes.
		Timestamp m_lastChangeTime;                       ///< Timestamp of the last change.
		bool m_readOnly = false;                          ///< Read-only mode flag.
		mutable wxMutex m_mutex;                          ///< Mutex for thread-safe access.
	};

}

#endif // _BWXOOP_H_
