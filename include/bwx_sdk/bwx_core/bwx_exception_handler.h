/// @file bwx_exception_handler.h
/// @brief Exception handling utilities for wxWidgets applications
///
/// Provides helper functions for handling exceptions in wxApp::OnExceptionInMainLoop().
/// Use these utilities to format, log, and decide how to handle exceptions.

#pragma once

#include <exception>
#include <string>
#include <functional>

namespace bwx {
namespace core {

/// @brief Severity level for exception handling decisions
enum class ExceptionSeverity {
    Info,      ///< Informational, always continue
    Warning,   ///< Warning, continue by default
    Error,     ///< Error, continue with user confirmation
    Critical   ///< Critical, abort by default
};

/// @brief Callback function type for custom exception logging
/// @param severity Exception severity level
/// @param message Formatted exception message
/// @param details Additional exception details
using ExceptionLogCallback = std::function<void(ExceptionSeverity severity,
                                                  const std::string& message,
                                                  const std::string& details)>;

/// @brief Exception handling utilities
/// @details Static utility class for processing exceptions caught in wxApp::OnExceptionInMainLoop().
///          Provides formatting, logging, and decision-making helpers.
class ExceptionHandler {
public:
    /// @brief Format exception message for display
    /// @param e Exception to format
    /// @return Human-readable formatted message
    static std::string formatException(const std::exception& e);

    /// @brief Determine exception severity
    /// @param e Exception to analyze
    /// @return Severity level of the exception
    static ExceptionSeverity getSeverity(const std::exception& e);

    /// @brief Determine if application should continue after exception
    /// @param e Exception to analyze
    /// @return true if application should continue, false to abort
    static bool shouldContinue(const std::exception& e);

    /// @brief Log exception using callback
    /// @param e Exception to log
    /// @param callback Logging callback function (optional, nullptr = no logging)
    /// @details If callback is nullptr, no logging occurs. Otherwise, calls callback
    ///          with formatted exception information.
    static void logException(const std::exception& e, ExceptionLogCallback callback = nullptr);

    /// @brief Get user-friendly error title based on exception type
    /// @param e Exception to analyze
    /// @return Short title for error dialog
    static std::string getErrorTitle(const std::exception& e);

    /// @brief Get detailed technical information from exception
    /// @param e Exception to analyze
    /// @return Technical details (file, line, type name)
    static std::string getTechnicalDetails(const std::exception& e);

private:
    ExceptionHandler() = delete;  // Static utility class
};

} // namespace core
} // namespace bwx
