#ifndef HOTEL_EXCEPTION_H
#define HOTEL_EXCEPTION_H

#include <exception>
#include <string>

// Custom exception used for validation and business-rule failures.
class HotelException : public std::exception
{
private:
    std::string message;

public:
    explicit HotelException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

#endif
