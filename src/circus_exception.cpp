#include "circus_exception.hpp"

const char* CircusException::what() const noexcept {
    return std::string("Error code: " + std::to_string(err_code) + ". " + err_msg).c_str();
}


