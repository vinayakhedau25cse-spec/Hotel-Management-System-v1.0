#include "Reservation.h"
#include "Validator.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Reservation::Reservation()
    : reservationId(0), guestId(0), roomNumber(0), checkInDate(""), nights(1), status(ACTIVE) {}

Reservation::Reservation(int id, int gid, int room, const std::string &checkIn, int n, Status st)
    : reservationId(id), guestId(gid), roomNumber(room), nights(1), status(st)
{
    setCheckInDate(checkIn);
    setNights(n);
}

Reservation::~Reservation() {}

void Reservation::setNights(int n)
{
    if (n < 1 || n > 60)
        throw HotelException("Nights must be between 1 and 60.");
    nights = n;
}

void Reservation::setCheckInDate(const std::string &d)
{
    if (!Validator::isValidDate(d))
        throw HotelException("Invalid date. Use DD-MM-YYYY.");
    checkInDate = d;
}

std::string Reservation::statusText() const
{
    switch (status)
    {
    case ACTIVE:
        return "Active";
    case CHECKED_OUT:
        return "Checked-out";
    default:
        return "Cancelled";
    }
}

void Reservation::display() const
{
    std::cout << std::left << std::setw(8) << reservationId << std::setw(8) << guestId
            << std::setw(8) << roomNumber << std::setw(13) << checkInDate
            << std::setw(8) << nights << statusText() << "\n";
}

std::string Reservation::serialize() const
{
    std::ostringstream os;
    os << reservationId << "|" << guestId << "|" << roomNumber << "|" << checkInDate
    << "|" << nights << "|" << (int)status;
    return os.str();
}