#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

class Reservation
{
public:
    enum Status
    {
        ACTIVE = 0,
        CHECKED_OUT = 1,
        CANCELLED = 2
    };

private:
    int reservationId;
    int guestId;
    int roomNumber;
    std::string checkInDate; // DD-MM-YYYY
    int nights;
    Status status;

public:
    Reservation();
    Reservation(int id, int guestId, int roomNumber, const std::string &checkIn, int nights, Status st = ACTIVE);
    ~Reservation();

    int getReservationId() const { return reservationId; }
    int getGuestId() const { return guestId; }
    int getRoomNumber() const { return roomNumber; }
    std::string getCheckInDate() const { return checkInDate; }
    int getNights() const { return nights; }
    Status getStatus() const { return status; }

    void setNights(int n);
    void setCheckInDate(const std::string &d);
    void setStatus(Status s) { status = s; }

    std::string statusText() const;
    void display() const;
    std::string serialize() const;
};

#endif
