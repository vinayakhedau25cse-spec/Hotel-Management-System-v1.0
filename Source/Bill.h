#ifndef BILL_H
#define BILL_H

#include <string>

class Bill
{
private:
    int billId;
    int reservationId;
    int guestId;
    int roomNumber;
    std::string roomType;
    int nights;
    double roomCharge;
    double tax;
    double total;

public:
    Bill();
    Bill(int billId, int reservationId, int guestId, int roomNumber, const std::string &roomType,
        int nights, double roomCharge, double taxRate);
    // used when loading from file (all values stored)
    Bill(int billId, int reservationId, int guestId, int roomNumber, const std::string &roomType,
        int nights, double roomCharge, double tax, double total);
    ~Bill();

    int getBillId() const { return billId; }
    int getReservationId() const { return reservationId; }
    int getGuestId() const { return guestId; }
    int getRoomNumber() const { return roomNumber; }
    std::string getRoomType() const { return roomType; }
    double getTotal() const { return total; }

    void displayRow() const;
    void printReceipt(const std::string &guestName, const std::string &hotelName) const;
    std::string serialize() const;
};

#endif
