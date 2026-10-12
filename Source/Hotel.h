#ifndef HOTEL_H
#define HOTEL_H

#include <string>
#include "Room.h"
#include "Guest.h"
#include "Reservation.h"
#include "Bill.h"

// The "System" class. Owns all dynamic arrays and contains the business logic.
class Hotel
{
private:
    std::string name;

    Room **rooms;
    int roomCount;
    int roomCap; // array of base pointers (polymorphic)
    Guest *guests;
    int guestCount;
    int guestCap; // array of objects
    Reservation *reservations;
    int resCount;
    int resCap;
    Bill *bills;
    int billCount;
    int billCap;

    int nextGuestId, nextResId, nextBillId;
    static const double TAX_RATE;

    // helpers
    template <typename T>
    static void growArray(T *&arr, int count, int &cap);
    int findRoomIndex(int roomNo) const;
    int findGuestIndex(int guestId) const;
    int findReservationIndex(int resId) const;
    bool guestHasActiveReservation(int guestId) const;
    void showRoomHeader() const;
    void showGuestHeader() const;
    void showReservationHeader() const;
    void showBillHeader() const;
    void showAvailableRooms() const;
    std::string readValidated(const std::string &prompt, bool (*check)(const std::string &), const std::string &err) const;
    std::string readDate(const std::string &prompt) const;

    // sub-operations
    void addRoom();
    void addGuest();
    void bookRoom();
    void checkOutAndBill();
    void cancelReservation();
    void deleteRoom();
    void deleteGuest();
    void deleteReservation();

public:
    Hotel();
    explicit Hotel(const std::string &hotelName);
    ~Hotel();
    Hotel(const Hotel &) = delete; // owns raw memory: no shallow copies
    Hotel &operator=(const Hotel &) = delete;

    // ---- menu operations (called from main) ----
    void addRecord();
    void displayRecords() const;
    void searchRecord() const;
    void updateRecord();
    void deleteRecord();
    void mainTransaction();
    void report() const;
    void saveLoad(); // submenu
    bool saveData() const;
    bool loadData();
    std::string getName() const { return name; }

    // ---- access used by FileManager ----
    int getRoomCount() const { return roomCount; }
    int getGuestCount() const { return guestCount; }
    int getReservationCount() const { return resCount; }
    int getBillCount() const { return billCount; }
    const Room *getRoomAt(int i) const { return rooms[i]; }
    const Guest &getGuestAt(int i) const { return guests[i]; }
    const Reservation &getReservationAt(int i) const { return reservations[i]; }
    const Bill &getBillAt(int i) const { return bills[i]; }
    void addRoomObject(Room *r); // takes ownership
    void addGuestObject(const Guest &g);
    void addReservationObject(const Reservation &r);
    void addBillObject(const Bill &b);
    void recalculateIds();
    void clearAll();
};

#endif
