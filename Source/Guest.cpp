#include "Guest.h"
#include "Validator.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Guest::Guest() : guestId(0), name(""), phone(""), email("") {}

Guest::Guest(int id, const std::string &n, const std::string &p, const std::string &e)
    : guestId(id)
{
    setName(n);
    setPhone(p);
    setEmail(e);
}

Guest::~Guest() {}

void Guest::setName(const std::string &n)
{
    if (!Validator::isValidName(n))
        throw HotelException("Invalid name (letters/spaces only, max 40 chars).");
    name = n;
}

void Guest::setPhone(const std::string &p)
{
    if (!Validator::isValidPhone(p))
        throw HotelException("Invalid phone (10 digits, starting with 6-9).");
    phone = p;
}

void Guest::setEmail(const std::string &e)
{
    if (!Validator::isValidEmail(e))
        throw HotelException("Invalid email address.");
    email = e;
}

void Guest::display() const
{
    std::cout << std::left << std::setw(8) << guestId << std::setw(22) << name
            << std::setw(14) << phone << email << "\n";
}

std::string Guest::serialize() const
{
    std::ostringstream os;
    os << guestId << "|" << name << "|" << phone << "|" << email;
    return os.str();
}
