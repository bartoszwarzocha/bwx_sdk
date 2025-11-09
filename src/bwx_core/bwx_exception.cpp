/// @file bwx_exception.cpp
/// @brief Implementation of bwx_sdk exception classes

#include "bwx_sdk/bwx_core/bwx_exception.h"
#include <sstream>

namespace bwx {
namespace core {

std::string BwxException::formatMessage(const std::string& message, const char* file, int line) {
    std::ostringstream oss;
    oss << message << " (at " << file << ":" << line << ")";
    return oss.str();
}

} // namespace core
} // namespace bwx
