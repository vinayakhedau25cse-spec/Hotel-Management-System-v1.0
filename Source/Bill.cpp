#include "Bill.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Bill::Bill() : billId(0), reservationId(0), guestId(0), roomNumber(0), roomType(""),
            nights(0), roomCharge(0), tax(0), total(0) {}

Bill::Bill(int bid, int rid, int gid, int room, const std::string &type, int n, double charge, double taxRate)
    : billId(bid), reservationId(rid), guestId(gid), roomNumber(room), roomType(type),
    nights(n), roomCharge(charge)
{
    tax = roomCharge * taxRate;
    total = roomCharge + tax;
}

Bill::Bill(int bid, int rid, int gid, int room, const std::string &type, int n,
        double charge, double t, double tot)
    : billId(bid), reservationId(rid), guestId(gid), roomNumber(room), roomType(type),
    nights(n), roomCharge(charge), tax(t), total(tot) {}

Bill::~Bill() {}

void Bill::displayRow() const
{
    std::cout << std::left << std::setw(8) << billId << std::setw(8) << reservationId
            << std::setw(8) << guestId << std::setw(8) << roomNumber << std::setw(11) << roomType
            << std::right << std::fixed << std::setprecision(2) << std::setw(12) << total << "\n";
}

void Bill::printReceipt(const std::string &guestName, const std::string &hotelName) const
{
    std::cout << "\n========================================\n"
            << "  " << hotelName << " - INVOICE\n"
            << "========================================\n"
            << std::left
            << std::setw(18) << "Bill ID" << ": " << billId << "\n"
            << std::setw(18) << "Reservation ID" << ": " << reservationId << "\n"
            << std::setw(18) << "Guest" << ": " << guestName << " (ID " << guestId << ")\n"
            << std::setw(18) << "Room" << ": " << roomNumber << " (" << roomType << ")\n"
            << std::setw(18) << "Nights" << ": " << nights << "\n"
            << "----------------------------------------\n"
            << std::fixed << std::setprecision(2)
            << std::setw(18) << "Room charges" << ": Rs. " << roomCharge << "\n"
            << std::setw(18) << "GST (12%)" << ": Rs. " << tax << "\n"
            << "----------------------------------------\n"
            << std::setw(18) << "TOTAL" << ": Rs. " << total << "\n"
            << "========================================\n";
}

std::string Bill::serialize() const
{
    std::ostringstream os;
    os << std::fixed << std::setprecision(2)
    << billId << "|" << reservationId << "|" << guestId << "|" << roomNumber << "|" << roomType
    << "|" << nights << "|" << roomCharge << "|" << tax << "|" << total;
    return os.str();
}
