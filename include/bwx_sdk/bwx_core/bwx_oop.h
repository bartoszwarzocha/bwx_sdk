/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_oop.h
// Purpose:     BWX_SDK Library; OOP stuff
// Author:      Bartosz Warzocha
// Created:     2025-02-24
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWXOOP_H_
#define _BWXOOP_H_

#include <wx/event.h>
#include <wx/thread.h>

#include <algorithm>
#include <chrono>
#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <vector>

// wxWidgets events declaration for value change notifications
wxDECLARE_EVENT(EVT_BWXPROPERTY_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_BWXPROPERTY_VECTOR_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EVT_BWXPROPERTY_MAP_CHANGED, wxCommandEvent);

//-----------------------------------------------------------------------------------

namespace bwx_sdk {

template <typename T>
class bwxProperty {
public:
    using Validator = std::function<bool(const T&)>;
    using ChangeCallback = std::function<void(const T&, const T&)>;
    using RejectCallback = std::function<void(const T&)>;
    using Timestamp = std::chrono::system_clock::time_point;

    explicit bwxProperty(const T& defaultValue = T(), Validator validator = nullptr, ChangeCallback onChange = nullptr,
                         RejectCallback onReject = nullptr, size_t historyLimit = 0,
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

    bwxProperty<T>& operator=(const T& newValue) {
        set(newValue);
        return *this;
    }

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

    [[nodiscard]] const T& get() const noexcept {
        wxMutexLocker lock(m_mutex);
        return m_value;
    }

    const char* AsCStr() const {
        if constexpr (std::is_same_v<T, std::string>) {
            return m_value.c_str();
        } else if constexpr (std::is_same_v<T, wxString>) {
            return m_value.mb_str().data();
        } else {
            static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, wxString>,
                          "AsCStr() is only supported for std::string or wxString.");
            return nullptr;
        }
    }

    void reset() { set(m_defaultValue); }

    void setValidator(Validator validator) {
        wxMutexLocker lock(m_mutex);
        m_validator = std::move(validator);
    }

    void setOnChangeCallback(ChangeCallback onChange) {
        wxMutexLocker lock(m_mutex);
        m_onChange = std::move(onChange);
    }

    void setOnRejectCallback(RejectCallback onReject) {
        wxMutexLocker lock(m_mutex);
        m_onReject = std::move(onReject);
    }

    void setReadOnly(bool readOnly) {
        wxMutexLocker lock(m_mutex);
        m_readOnly = readOnly;
    }

    [[nodiscard]] bool isReadOnly() const noexcept {
        wxMutexLocker lock(m_mutex);
        return m_readOnly;
    }

    bool undo() {
        wxMutexLocker lock(m_mutex);
        if (m_undoHistory.empty()) return false;

        auto now = std::chrono::system_clock::now();
        if (m_undoTimeout &&
            std::chrono::duration_cast<std::chrono::seconds>(now - m_lastChangeTime) > *m_undoTimeout) {
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

    void setHistoryLimit(size_t limit) {
        wxMutexLocker lock(m_mutex);
        m_historyLimit = limit;
        trimHistory(m_undoHistory);
        trimHistory(m_redoHistory);
    }

    [[nodiscard]] size_t getHistoryLimit() const noexcept {
        wxMutexLocker lock(m_mutex);
        return m_historyLimit;
    }

    void clearHistory() {
        wxMutexLocker lock(m_mutex);
        m_undoHistory.clear();
        m_redoHistory.clear();
    }

    void setUndoTimeout(std::chrono::seconds timeout) {
        wxMutexLocker lock(m_mutex);
        m_undoTimeout = timeout;
    }

    [[nodiscard]] std::optional<std::chrono::seconds> getUndoTimeout() const noexcept {
        wxMutexLocker lock(m_mutex);
        return m_undoTimeout;
    }

    Timestamp getLastChangeTime() const {
        wxMutexLocker lock(m_mutex);
        return m_lastChangeTime;
    }

    void bind(bwxProperty<T>& other) {
        wxMutexLocker lock(m_mutex);
        m_boundProperties.push_back(&other);
    }

    // Arithmetic operators
    bwxProperty<T>& operator+=(const T& rhs) {
        set(m_value + rhs);
        return *this;
    }
    bwxProperty<T>& operator-=(const T& rhs) {
        set(m_value - rhs);
        return *this;
    }
    bwxProperty<T>& operator*=(const T& rhs) {
        set(m_value * rhs);
        return *this;
    }
    bwxProperty<T>& operator/=(const T& rhs) {
        set(m_value / rhs);
        return *this;
    }

    // Comparison operators
    bool operator==(const T& rhs) const { return m_value == rhs; }
    bool operator!=(const T& rhs) const { return m_value != rhs; }
    bool operator<(const T& rhs) const { return m_value < rhs; }
    bool operator<=(const T& rhs) const { return m_value <= rhs; }
    bool operator>(const T& rhs) const { return m_value > rhs; }
    bool operator>=(const T& rhs) const { return m_value >= rhs; }

private:
    void recordHistory(const T& oldValue) {
        if (m_historyLimit == 0) return;
        m_undoHistory.push_front(oldValue);
        trimHistory(m_undoHistory);
    }

    void clearRedoHistory() { m_redoHistory.clear(); }

    void trimHistory(std::deque<T>& history) {
        while (history.size() > m_historyLimit) {
            history.pop_back();
        }
    }

    void notifyChange() {
        if (m_eventHandler) {
            wxCommandEvent evt(EVT_BWXPROPERTY_CHANGED);
            m_eventHandler->AddPendingEvent(evt);
        }
    }

    void propagateBinding(const T& newValue) {
        for (auto* boundProp : m_boundProperties) {
            if (boundProp) boundProp->set(newValue);
        }
    }

    T m_value{};
    T m_defaultValue{};
    Validator m_validator = nullptr;
    ChangeCallback m_onChange = nullptr;
    RejectCallback m_onReject = nullptr;
    size_t m_historyLimit = 0;
    std::optional<std::chrono::seconds> m_undoTimeout = std::nullopt;
    bool m_readOnly = false;
    wxEvtHandler* m_eventHandler = nullptr;
    Timestamp m_lastChangeTime;
    std::deque<T> m_undoHistory;
    std::deque<T> m_redoHistory;
    std::vector<bwxProperty<T>*> m_boundProperties;
    mutable wxMutex m_mutex;
};

//-----------------------------------------------------------------------------------

template <typename T>
class bwxPropertyVector {
public:
    using ChangeCallback = std::function<void()>;
    using Timestamp = std::chrono::system_clock::time_point;

    explicit bwxPropertyVector(wxEvtHandler* handler = nullptr, ChangeCallback callback = nullptr,
                               size_t historyLimit = 0, size_t capacityLimit = 0)
        : m_eventHandler(handler),
          m_onChange(std::move(callback)),
          m_historyLimit(historyLimit),
          m_capacityLimit(capacityLimit),
          m_lastChangeTime(std::chrono::system_clock::now()) {}

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

    std::optional<T> get(size_t index) const {
        wxMutexLocker lock(m_mutex);
        return (index < m_data.size()) ? std::optional<T>(m_data[index]) : std::nullopt;
    }

    T& at(size_t index) {
        wxMutexLocker lock(m_mutex);
        if (index >= m_data.size()) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    void clear() {
        wxMutexLocker lock(m_mutex);
        recordHistory();
        m_data.clear();
        notifyChange();
    }

    size_t size() const {
        wxMutexLocker lock(m_mutex);
        return m_data.size();
    }

    typename std::vector<T>::iterator begin() { return m_data.begin(); }
    typename std::vector<T>::iterator end() { return m_data.end(); }
    typename std::vector<T>::const_iterator cbegin() const { return m_data.cbegin(); }
    typename std::vector<T>::const_iterator cend() const { return m_data.cend(); }

    void setReadOnly(bool readOnly) {
        wxMutexLocker lock(m_mutex);
        m_readOnly = readOnly;
    }

    bool isReadOnly() const noexcept {
        wxMutexLocker lock(m_mutex);
        return m_readOnly;
    }

    bool undo() {
        wxMutexLocker lock(m_mutex);
        if (m_undoHistory.empty()) return false;

        m_redoHistory.push_front(m_data);
        m_data = m_undoHistory.front();
        m_undoHistory.pop_front();
        notifyChange();
        return true;
    }

    bool redo() {
        wxMutexLocker lock(m_mutex);
        if (m_redoHistory.empty()) return false;

        m_undoHistory.push_front(m_data);
        m_data = m_redoHistory.front();
        m_redoHistory.pop_front();
        notifyChange();
        return true;
    }

    void setHistoryLimit(size_t limit) {
        wxMutexLocker lock(m_mutex);
        m_historyLimit = limit;
        trimHistory(m_undoHistory);
        trimHistory(m_redoHistory);
    }

    size_t getHistoryLimit() const noexcept {
        wxMutexLocker lock(m_mutex);
        return m_historyLimit;
    }

    void clearHistory() {
        wxMutexLocker lock(m_mutex);
        m_undoHistory.clear();
        m_redoHistory.clear();
    }

    void setCapacityLimit(size_t limit) {
        wxMutexLocker lock(m_mutex);
        m_capacityLimit = limit;
        trimToCapacity();
    }

    size_t getCapacityLimit() const noexcept {
        wxMutexLocker lock(m_mutex);
        return m_capacityLimit;
    }

    Timestamp getLastChangeTime() const {
        wxMutexLocker lock(m_mutex);
        return m_lastChangeTime;
    }

    void setOnChangeCallback(ChangeCallback callback) {
        wxMutexLocker lock(m_mutex);
        m_onChange = std::move(callback);
    }

    void setEventHandler(wxEvtHandler* handler) {
        wxMutexLocker lock(m_mutex);
        m_eventHandler = handler;
    }

    const char* AsCStr(size_t index = 0) const {
        wxMutexLocker lock(m_mutex);
        if (index >= m_data.size()) return nullptr;

        if constexpr (std::is_same_v<T, std::string>) {
            return m_data[index].c_str();
        } else if constexpr (std::is_same_v<T, wxString>) {
            return m_data[index].mb_str().data();
        } else {
            static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, wxString>,
                          "AsCStr() is only supported for std::string or wxString.");
            return nullptr;
        }
    }

private:
    void notifyChange() {
        if (m_onChange) m_onChange();
        if (m_eventHandler) {
            wxCommandEvent evt(EVT_BWXPROPERTY_VECTOR_CHANGED);
            m_eventHandler->AddPendingEvent(evt);
        }
    }

    void recordHistory() {
        if (m_historyLimit == 0) return;
        m_undoHistory.push_front(m_data);
        trimHistory(m_undoHistory);
        m_redoHistory.clear();
    }

    void trimHistory(std::deque<std::vector<T>>& history) {
        while (history.size() > m_historyLimit) {
            history.pop_back();
        }
    }

    void trimToCapacity() {
        while (m_capacityLimit > 0 && m_data.size() > m_capacityLimit) {
            m_data.erase(m_data.begin());
        }
    }

    std::vector<T> m_data{};
    size_t m_historyLimit = 0;
    size_t m_capacityLimit = 0;
    std::deque<std::vector<T>> m_undoHistory;
    std::deque<std::vector<T>> m_redoHistory;
    wxEvtHandler* m_eventHandler = nullptr;
    ChangeCallback m_onChange = nullptr;
    Timestamp m_lastChangeTime;
    bool m_readOnly = false;
    mutable wxMutex m_mutex;
};

//-----------------------------------------------------------------------------------

template <typename K, typename V>
class bwxPropertyMap {
public:
    using ChangeCallback = std::function<void()>;
    using Timestamp = std::chrono::system_clock::time_point;

    explicit bwxPropertyMap(wxEvtHandler* handler = nullptr, ChangeCallback callback = nullptr, size_t historyLimit = 0,
                            size_t capacityLimit = 0)
        : m_eventHandler(handler),
          m_onChange(std::move(callback)),
          m_historyLimit(historyLimit),
          m_capacityLimit(capacityLimit),
          m_lastChangeTime(std::chrono::system_clock::now()) {}

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
