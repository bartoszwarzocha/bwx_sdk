/// @file bwx_exception.h
/// @brief wxWidgets exception hierarchy for bwx_sdk
///
/// Provides typed exceptions for wxWidgets operations to enable proper
/// error handling in wxApp::OnExceptionInMainLoop().
///
/// @note Standard C++ try-catch doesn't work in wxWidgets event loop.
///       Use wxApp::OnExceptionInMainLoop() to catch these exceptions.

#pragma once

#include <stdexcept>
#include <string>

namespace bwx {
namespace core {

/// @brief Base exception class for all bwx_sdk exceptions
/// @details Derives from std::runtime_error for compatibility with standard C++ exception handling.
///          All bwx exceptions should derive from this class.
class BwxException : public std::runtime_error {
public:
    /// @brief Construct exception with message
    /// @param message Human-readable error description
    explicit BwxException(const std::string& message)
        : std::runtime_error(message)
    {}

    /// @brief Construct exception with message and source location
    /// @param message Human-readable error description
    /// @param file Source file where exception occurred
    /// @param line Line number where exception occurred
    BwxException(const std::string& message, const char* file, int line)
        : std::runtime_error(formatMessage(message, file, line))
    {}

    virtual ~BwxException() = default;

private:
    static std::string formatMessage(const std::string& message, const char* file, int line);
};

/// @brief Exception for layout-related operations (FitInside, Layout, sizers)
/// @details Thrown when wxWindow layout operations fail, such as:
///          - FitInside() failures
///          - Sizer calculation errors
///          - Invalid layout configurations
class LayoutException : public BwxException {
public:
    explicit LayoutException(const std::string& message)
        : BwxException("Layout Error: " + message)
    {}

    LayoutException(const std::string& message, const char* file, int line)
        : BwxException("Layout Error: " + message, file, line)
    {}
};

/// @brief Exception for widget/control operations
/// @details Thrown when wxControl operations fail, such as:
///          - Invalid control states
///          - Widget creation failures
///          - Control value errors
class WidgetException : public BwxException {
public:
    explicit WidgetException(const std::string& message)
        : BwxException("Widget Error: " + message)
    {}

    WidgetException(const std::string& message, const char* file, int line)
        : BwxException("Widget Error: " + message, file, line)
    {}
};

/// @brief Exception for window/dialog operations
/// @details Thrown when wxWindow/wxDialog operations fail, such as:
///          - Window creation failures
///          - Modal dialog errors
///          - Window hierarchy issues
class WindowException : public BwxException {
public:
    explicit WindowException(const std::string& message)
        : BwxException("Window Error: " + message)
    {}

    WindowException(const std::string& message, const char* file, int line)
        : BwxException("Window Error: " + message, file, line)
    {}
};

/// @brief Exception for resource operations (icons, bitmaps, fonts)
/// @details Thrown when resource loading or manipulation fails, such as:
///          - Icon loading failures
///          - Bitmap creation errors
///          - Font initialization issues
class ResourceException : public BwxException {
public:
    explicit ResourceException(const std::string& message)
        : BwxException("Resource Error: " + message)
    {}

    ResourceException(const std::string& message, const char* file, int line)
        : BwxException("Resource Error: " + message, file, line)
    {}
};

/// @brief Convenience macro for throwing exceptions with source location
/// @param ExceptionType Type of exception to throw (e.g., LayoutException)
/// @param message Error message string
#define BWX_THROW(ExceptionType, message) \
    throw ExceptionType(message, __FILE__, __LINE__)

} // namespace core
} // namespace bwx
