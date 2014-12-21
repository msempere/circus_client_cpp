#ifndef __CIRCUS_EXCEPTION_HPP__
#define __CIRCUS_EXCEPTION_HPP__

#include <exception>
#include <string>

class CircusException: public std::exception{
    public:
        CircusException(const std::string &msg, const int code) : err_msg(msg), err_code(code) {};
        virtual const char* what() const noexcept override;
        virtual ~CircusException() noexcept {};
    private:
        std::string err_msg;
        int err_code;
};

#endif
