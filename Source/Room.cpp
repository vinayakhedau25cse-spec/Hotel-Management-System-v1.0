#include "Room.h"
#include "HotelException.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Room::Room() : roomNumber(0), basePrice(0.0), booked(false) {}

Room::Room(int number, double price) : roomNumber(0), basePrice(0.0), booked(false)
{
    setRoomNumber(number);
    setBasePrice(price);
}
Room::~Room() {}

void Room::setRoomNumber(int number)
{
    if (number < 1 || number > 9999)
        throw HotelException("Room number must be between 1 and 9999.");
    roomNumber = number;
}

void Room::setBasePrice(double price)
{
    if (price < 100.0 || price > 100000.0)
        throw HotelException("Price per night must be between 100 and 100000.");
    basePrice = price;
}

double Room::calculateCharge(int nights) const
{
    return basePrice * nights;
}

void Room::display() const
{
    std::cout << std::left << std::setw(8) << roomNumber
            << std::setw(11) << getType()
            << std::setw(12) << std::fixed << std::setprecision(2) << basePrice
            << std::setw(30) << getAmenities()
            << (booked ? "Occupied" : "Available") << "\n";
}

std::string Room::serialize() const
{
    std::ostringstream os;
    os << getType() << "|" << roomNumber << "|" << std::fixed << std::setprecision(2)
    << basePrice << "|" << (booked ? 1 : 0);
    return os.str();
}

Room *Room::create(const std::string &type, int number, double price)
{
    if (type == "Standard")
        return new StandardRoom(number, price);
    if (type == "Deluxe")
        return new DeluxeRoom(number, price);
    if (type == "Suite")
        return new SuiteRoom(number, price);
    throw HotelException("Unknown room type: " + type);
}

double DeluxeRoom::calculateCharge(int nights) const
{
    return basePrice * nights * 1.10;
}

double SuiteRoom::calculateCharge(int nights) const
{
    return basePrice * nights * 1.20 + 500.0;
}
