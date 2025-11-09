/// @file bwx_exception_handler.cpp
/// @brief Implementation of exception handling utilities

#include "bwx_sdk/bwx_core/bwx_exception_handler.h"
#include "bwx_sdk/bwx_core/bwx_exception.h"
#include <sstream>
#include <typeinfo>

namespace bwx {
namespace core {

std::string ExceptionHandler::formatException(const std::exception& e) {
    std::ostringstream oss;

    // Try to identify bwx exception types
    if (dynamic_cast<const LayoutException*>(&e)) {
        oss << "[LAYOUT] ";
    } else if (dynamic_cast<const WidgetException*>(&e)) {
        oss << "[WIDGET] ";
    } else if (dynamic_cast<const WindowException*>(&e)) {
        oss << "[WINDOW] ";
    } else if (dynamic_cast<const ResourceException*>(&e)) {
        oss << "[RESOURCE] ";
    } else if (dynamic_cast<const BwxException*>(&e)) {
        oss << "[BWX] ";
    } else {
        oss << "[C++] ";
    }

    oss << e.what();
    return oss.str();
}

ExceptionSeverity ExceptionHandler::getSeverity(const std::exception& e) {
    // Layout exceptions are usually non-critical (can continue)
    if (dynamic_cast<const LayoutException*>(&e)) {
        return ExceptionSeverity::Warning;
    }

    // Widget exceptions might be non-critical
    if (dynamic_cast<const WidgetException*>(&e)) {
        return ExceptionSeverity::Warning;
    }

    // Window exceptions are more serious
    if (dynamic_cast<const WindowException*>(&e)) {
        return ExceptionSeverity::Error;
    }

    // Resource exceptions can be handled
    if (dynamic_cast<const ResourceException*>(&e)) {
        return ExceptionSeverity::Warning;
    }

    // Other bwx exceptions
    if (dynamic_cast<const BwxException*>(&e)) {
        return ExceptionSeverity::Error;
    }

    // Unknown C++ exceptions are critical
    return ExceptionSeverity::Critical;
}

bool ExceptionHandler::shouldContinue(const std::exception& e) {
    ExceptionSeverity severity = getSeverity(e);

    switch (severity) {
        case ExceptionSeverity::Info:
        case ExceptionSeverity::Warning:
            return true;  // Always continue for info/warnings

        case ExceptionSeverity::Error:
            return true;  // Continue for errors (with logging)

        case ExceptionSeverity::Critical:
            return false; // Abort for critical errors

        default:
            return false;
    }
}

void ExceptionHandler::logException(const std::exception& e, ExceptionLogCallback callback) {
    if (!callback) {
        return;  // No callback = no logging
    }

    ExceptionSeverity severity = getSeverity(e);
    std::string message = formatException(e);
    std::string details = getTechnicalDetails(e);

    callback(severity, message, details);
}

std::string ExceptionHandler::getErrorTitle(const std::exception& e) {
    if (dynamic_cast<const LayoutException*>(&e)) {
        return "Layout Error";
    }
    if (dynamic_cast<const WidgetException*>(&e)) {
        return "Widget Error";
    }
    if (dynamic_cast<const WindowException*>(&e)) {
        return "Window Error";
    }
    if (dynamic_cast<const ResourceException*>(&e)) {
        return "Resource Error";
    }
    if (dynamic_cast<const BwxException*>(&e)) {
        return "Application Error";
    }
    return "Unexpected Error";
}

std::string ExceptionHandler::getTechnicalDetails(const std::exception& e) {
    std::ostringstream oss;
    oss << "Exception Type: " << typeid(e).name() << "\n";
    oss << "Message: " << e.what();
    return oss.str();
}

} // namespace core
} // namespace bwx
