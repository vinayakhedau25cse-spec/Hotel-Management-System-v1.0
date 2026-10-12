#include "Hotel.h"
#include "FileManager.h"
#include "Validator.h"
#include "HotelException.h"
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;
const double Hotel::TAX_RATE = 0.12;
// ------------------------------------------------------------------
// Construction / destruction (dynamic memory lives here)
// ------------------------------------------------------------------
Hotel::Hotel() : Hotel("Grand Stay Hotel") {}

Hotel::Hotel(const string &hotelName)
    : name(hotelName),
    roomCount(0), roomCap(10), guestCount(0), guestCap(10),
    resCount(0), resCap(10), billCount(0), billCap(10),
    nextGuestId(1), nextResId(1), nextBillId(1)
{
    rooms = new Room *[roomCap];
    guests = new Guest[guestCap];
    reservations = new Reservation[resCap];
    bills = new Bill[billCap];
}

Hotel::~Hotel()
{
    for (int i = 0; i < roomCount; i++)
        delete rooms[i]; // each polymorphic Room
    delete[] rooms;
    delete[] guests;
    delete[] reservations;
    delete[] bills;
}

template <typename T>
void Hotel::growArray(T *&arr, int count, int &cap)
{
    int newCap = cap * 2;
    T *bigger = new T[newCap];
    for (int i = 0; i < count; i++)
        bigger[i] = arr[i];
    delete[] arr;
    arr = bigger;
    cap = newCap;
}

void Hotel::clearAll()
{
    for (int i = 0; i < roomCount; i++)
        delete rooms[i];
    roomCount = guestCount = resCount = billCount = 0;
    nextGuestId = nextResId = nextBillId = 1;
}

// ------------------------------------------------------------------
// Object insertion (also used by FileManager when loading)
// ------------------------------------------------------------------
void Hotel::addRoomObject(Room *r)
{
    if (roomCount == roomCap)
        growArray(rooms, roomCount, roomCap);
    rooms[roomCount++] = r;
}
void Hotel::addGuestObject(const Guest &g)
{
    if (guestCount == guestCap)
        growArray(guests, guestCount, guestCap);
    guests[guestCount++] = g;
}
void Hotel::addReservationObject(const Reservation &r)
{
    if (resCount == resCap)
        growArray(reservations, resCount, resCap);
    reservations[resCount++] = r;
}
void Hotel::addBillObject(const Bill &b)
{
    if (billCount == billCap)
        growArray(bills, billCount, billCap);
    bills[billCount++] = b;
}

void Hotel::recalculateIds()
{
    nextGuestId = nextResId = nextBillId = 1;
    for (int i = 0; i < guestCount; i++)
        nextGuestId = max(nextGuestId, guests[i].getGuestId() + 1);
    for (int i = 0; i < resCount; i++)
        nextResId = max(nextResId, reservations[i].getReservationId() + 1);
    for (int i = 0; i < billCount; i++)
        nextBillId = max(nextBillId, bills[i].getBillId() + 1);
}

// ------------------------------------------------------------------
// Lookup helpers
// ------------------------------------------------------------------
int Hotel::findRoomIndex(int no) const
{
    for (int i = 0; i < roomCount; i++)
        if (rooms[i]->getRoomNumber() == no)
            return i;
    return -1;
}
int Hotel::findGuestIndex(int id) const
{
    for (int i = 0; i < guestCount; i++)
        if (guests[i].getGuestId() == id)
            return i;
    return -1;
}
int Hotel::findReservationIndex(int id) const
{
    for (int i = 0; i < resCount; i++)
        if (reservations[i].getReservationId() == id)
            return i;
    return -1;
}
bool Hotel::guestHasActiveReservation(int gid) const
{
    for (int i = 0; i < resCount; i++)
        if (reservations[i].getGuestId() == gid && reservations[i].getStatus() == Reservation::ACTIVE)
            return true;
    return false;
}

// ------------------------------------------------------------------
// Table headers and input helpers
// ------------------------------------------------------------------
void Hotel::showRoomHeader() const
{
    cout << "\n"
        << left << setw(8) << "Room" << setw(11) << "Type" << setw(12) << "Price/Night"
        << setw(30) << "Amenities" << "Status\n"
        << string(70, '-') << "\n";
}
void Hotel::showGuestHeader() const
{
    cout << "\n"
        << left << setw(8) << "ID" << setw(22) << "Name" << setw(14) << "Phone" << "Email\n"
        << string(70, '-') << "\n";
}
void Hotel::showReservationHeader() const
{
    cout << "\n"
        << left << setw(8) << "ResID" << setw(8) << "Guest" << setw(8) << "Room"
        << setw(13) << "Check-in" << setw(8) << "Nights" << "Status\n"
        << string(60, '-') << "\n";
}
void Hotel::showBillHeader() const
{
    cout << "\n"
        << left << setw(8) << "BillID" << setw(8) << "ResID" << setw(8) << "Guest"
        << setw(8) << "Room" << setw(11) << "Type" << right << setw(12) << "Total (Rs)" << "\n"
        << string(55, '-') << "\n";
}

string Hotel::readValidated(const string &prompt, bool (*check)(const string &), const string &err) const
{
    while (true)
    {
        string s = Validator::readLine(prompt);
        if (check(s))
            return s;
        cout << "  " << err << "\n";
    }
}

string Hotel::readDate(const string &prompt) const
{
    return readValidated(prompt, Validator::isValidDate, "Invalid date. Use DD-MM-YYYY (e.g. 15-10-2026).");
}

void Hotel::showAvailableRooms() const
{
    int n = 0;
    showRoomHeader();
    for (int i = 0; i < roomCount; i++)
        if (!rooms[i]->isBooked())
        {
            rooms[i]->display();
            n++;
        }
    if (n == 0)
        cout << "  (no rooms available)\n";
}

// ------------------------------------------------------------------
// 1. ADD
// ------------------------------------------------------------------
void Hotel::addRecord()
{
    cout << "\n--- ADD RECORD ---\n1. Add Room\n2. Add Guest\n0. Back\n";
    int c = Validator::readInt("Choice: ", 0, 2);
    if (c == 1)
        addRoom();
    else if (c == 2)
        addGuest();
}

void Hotel::addRoom()
{
    int no = Validator::readInt("Room number (1-9999): ", 1, 9999);
    if (findRoomIndex(no) != -1)
        throw HotelException("Room number already exists.");
    cout << "Type: 1. Standard  2. Deluxe  3. Suite\n";
    int t = Validator::readInt("Type: ", 1, 3);
    const char *types[] = {"Standard", "Deluxe", "Suite"};
    double price = Validator::readDouble("Price per night (100-100000): ", 100, 100000);
    addRoomObject(Room::create(types[t - 1], no, price));
    cout << "  Room " << no << " (" << types[t - 1] << ") added.\n";
}

void Hotel::addGuest()
{
    string n = readValidated("Name: ", Validator::isValidName, "Invalid name (letters/spaces only).");
    string p = readValidated("Phone (10 digits): ", Validator::isValidPhone, "Invalid phone number.");
    string e = readValidated("Email: ", Validator::isValidEmail, "Invalid email.");
    Guest g(nextGuestId, n, p, e);
    addGuestObject(g);
    cout << "  Guest added with ID " << nextGuestId++ << ".\n";
}

// ------------------------------------------------------------------
// 2. DISPLAY
// ------------------------------------------------------------------
void Hotel::displayRecords() const
{
    cout << "\n--- DISPLAY RECORDS ---\n1. All Rooms\n2. Available Rooms\n3. All Guests\n"
            "4. All Reservations\n5. All Bills\n0. Back\n";
    int c = Validator::readInt("Choice: ", 0, 5);
    switch (c)
    {
    case 1:
        showRoomHeader();
        for (int i = 0; i < roomCount; i++)
            rooms[i]->display(); // runtime polymorphism
        if (!roomCount)
            cout << "  (no records)\n";
        break;
    case 2:
        showAvailableRooms();
        break;
    case 3:
        showGuestHeader();
        for (int i = 0; i < guestCount; i++)
            guests[i].display();
        if (!guestCount)
            cout << "  (no records)\n";
        break;
    case 4:
        showReservationHeader();
        for (int i = 0; i < resCount; i++)
            reservations[i].display();
        if (!resCount)
            cout << "  (no records)\n";
        break;
    case 5:
        showBillHeader();
        for (int i = 0; i < billCount; i++)
            bills[i].displayRow();
        if (!billCount)
            cout << "  (no records)\n";
        break;
    }
}

// ------------------------------------------------------------------
// 3. SEARCH
// ------------------------------------------------------------------
void Hotel::searchRecord() const
{
    cout << "\n--- SEARCH RECORD ---\n1. Room by number\n2. Guest by ID\n3. Guest by name\n"
            "4. Reservation by ID\n0. Back\n";
    int c = Validator::readInt("Choice: ", 0, 4);
    if (c == 1)
    {
        int i = findRoomIndex(Validator::readInt("Room number: ", 1, 9999));
        if (i < 0)
            throw HotelException("Room not found.");
        showRoomHeader();
        rooms[i]->display();
    }
    else if (c == 2)
    {
        int i = findGuestIndex(Validator::readInt("Guest ID: ", 1, 999999));
        if (i < 0)
            throw HotelException("Guest not found.");
        showGuestHeader();
        guests[i].display();
    }
    else if (c == 3)
    {
        string key = Validator::toLower(Validator::readLine("Name (or part of it): "));
        int found = 0;
        showGuestHeader();
        for (int i = 0; i < guestCount; i++)
            if (Validator::toLower(guests[i].getName()).find(key) != string::npos)
            {
                guests[i].display();
                found++;
            }
        if (!found)
            throw HotelException("No guest matches that name.");
    }
    else if (c == 4)
    {
        int i = findReservationIndex(Validator::readInt("Reservation ID: ", 1, 999999));
        if (i < 0)
            throw HotelException("Reservation not found.");
        showReservationHeader();
        reservations[i].display();
    }
}

// ------------------------------------------------------------------
// 4. UPDATE
// ------------------------------------------------------------------
void Hotel::updateRecord()
{
    cout << "\n--- UPDATE RECORD ---\n1. Room price\n2. Guest phone/email\n"
            "3. Reservation nights (active only)\n0. Back\n";
    int c = Validator::readInt("Choice: ", 0, 3);
    if (c == 1)
    {
        int i = findRoomIndex(Validator::readInt("Room number: ", 1, 9999));
        if (i < 0)
            throw HotelException("Room not found.");
        rooms[i]->setBasePrice(Validator::readDouble("New price per night: ", 100, 100000));
        cout << "  Price updated.\n";
    }
    else if (c == 2)
    {
        int i = findGuestIndex(Validator::readInt("Guest ID: ", 1, 999999));
        if (i < 0)
            throw HotelException("Guest not found.");
        guests[i].setPhone(readValidated("New phone: ", Validator::isValidPhone, "Invalid phone number."));
        guests[i].setEmail(readValidated("New email: ", Validator::isValidEmail, "Invalid email."));
        cout << "  Guest updated.\n";
    }
    else if (c == 3)
    {
        int i = findReservationIndex(Validator::readInt("Reservation ID: ", 1, 999999));
        if (i < 0)
            throw HotelException("Reservation not found.");
        if (reservations[i].getStatus() != Reservation::ACTIVE)
            throw HotelException("Only active reservations can be modified.");
        reservations[i].setNights(Validator::readInt("New number of nights (1-60): ", 1, 60));
        cout << "  Reservation updated.\n";
    }
}

// ------------------------------------------------------------------
// 5. DELETE
// ------------------------------------------------------------------
void Hotel::deleteRecord()
{
    cout << "\n--- DELETE RECORD ---\n1. Room\n2. Guest\n3. Reservation\n0. Back\n";
    int c = Validator::readInt("Choice: ", 0, 3);
    if (c == 1)
        deleteRoom();
    else if (c == 2)
        deleteGuest();
    else if (c == 3)
        deleteReservation();
}

void Hotel::deleteRoom()
{
    int i = findRoomIndex(Validator::readInt("Room number: ", 1, 9999));
    if (i < 0)
        throw HotelException("Room not found.");
    if (rooms[i]->isBooked())
        throw HotelException("Cannot delete an occupied room.");
    delete rooms[i];
    for (int k = i; k < roomCount - 1; k++)
        rooms[k] = rooms[k + 1];
    roomCount--;
    cout << "  Room deleted.\n";
}

void Hotel::deleteGuest()
{
    int id = Validator::readInt("Guest ID: ", 1, 999999);
    int i = findGuestIndex(id);
    if (i < 0)
        throw HotelException("Guest not found.");
    if (guestHasActiveReservation(id))
        throw HotelException("Guest has an active reservation.");
    for (int k = i; k < guestCount - 1; k++)
        guests[k] = guests[k + 1];
    guestCount--;
    cout << "  Guest deleted.\n";
}

void Hotel::deleteReservation()
{
    int i = findReservationIndex(Validator::readInt("Reservation ID: ", 1, 999999));
    if (i < 0)
        throw HotelException("Reservation not found.");
    if (reservations[i].getStatus() == Reservation::ACTIVE)
    { // free the room
        int r = findRoomIndex(reservations[i].getRoomNumber());
        if (r >= 0)
            rooms[r]->setBooked(false);
    }
    for (int k = i; k < resCount - 1; k++)
        reservations[k] = reservations[k + 1];
    resCount--;
    cout << "  Reservation deleted.\n";
}

// ------------------------------------------------------------------
// 6. MAIN TRANSACTION: Book -> Check-out & Bill -> Cancel
// ------------------------------------------------------------------
void Hotel::mainTransaction()
{
    cout << "\n--- RESERVATION PROCESS ---\n1. Book a Room\n2. Check-out & Generate Bill\n"
            "3. Cancel Reservation\n0. Back\n";
    int c = Validator::readInt("Choice: ", 0, 3);
    if (c == 1)
        bookRoom();
    else if (c == 2)
        checkOutAndBill();
    else if (c == 3)
        cancelReservation();
}

void Hotel::bookRoom()
{
    if (guestCount == 0)
        throw HotelException("Register a guest first (Menu 1).");
    showAvailableRooms();
    int gid = Validator::readInt("\nGuest ID: ", 1, 999999);
    if (findGuestIndex(gid) < 0)
        throw HotelException("Guest not found.");
    int rn = Validator::readInt("Room number: ", 1, 9999);
    int ri = findRoomIndex(rn);
    if (ri < 0)
        throw HotelException("Room not found.");
    if (rooms[ri]->isBooked())
        throw HotelException("Room is already occupied.");
    string date = readDate("Check-in date (DD-MM-YYYY): ");
    int nights = Validator::readInt("Number of nights (1-60): ", 1, 60);

    Reservation r(nextResId, gid, rn, date, nights, Reservation::ACTIVE);
    addReservationObject(r);
    rooms[ri]->setBooked(true);
    cout << "  Reservation " << nextResId++ << " confirmed. Estimated room charge: Rs. "
        << fixed << setprecision(2) << rooms[ri]->calculateCharge(nights) << " (before GST)\n";
}

void Hotel::checkOutAndBill()
{
    int idx = findReservationIndex(Validator::readInt("Reservation ID: ", 1, 999999));
    if (idx < 0)
        throw HotelException("Reservation not found.");
    Reservation &r = reservations[idx];
    if (r.getStatus() != Reservation::ACTIVE)
        throw HotelException("Reservation is not active.");
    int ri = findRoomIndex(r.getRoomNumber());
    int gi = findGuestIndex(r.getGuestId());
    if (ri < 0 || gi < 0)
        throw HotelException("Linked room/guest record is missing.");

    double charge = rooms[ri]->calculateCharge(r.getNights()); // polymorphic call
    Bill b(nextBillId, r.getReservationId(), r.getGuestId(), r.getRoomNumber(),
        rooms[ri]->getType(), r.getNights(), charge, TAX_RATE);
    addBillObject(b);
    nextBillId++;
    r.setStatus(Reservation::CHECKED_OUT);
    rooms[ri]->setBooked(false);
    b.printReceipt(guests[gi].getName(), name);
}

void Hotel::cancelReservation()
{
    int idx = findReservationIndex(Validator::readInt("Reservation ID: ", 1, 999999));
    if (idx < 0)
        throw HotelException("Reservation not found.");
    if (reservations[idx].getStatus() != Reservation::ACTIVE)
        throw HotelException("Reservation is not active.");
    reservations[idx].setStatus(Reservation::CANCELLED);
    int ri = findRoomIndex(reservations[idx].getRoomNumber());
    if (ri >= 0)
        rooms[ri]->setBooked(false);
    cout << "  Reservation cancelled.\n";
}

// ------------------------------------------------------------------
// 7. REPORT
// ------------------------------------------------------------------
void Hotel::report() const
{
    int occupied = 0, active = 0, done = 0, cancelled = 0;
    for (int i = 0; i < roomCount; i++)
        if (rooms[i]->isBooked())
            occupied++;
    for (int i = 0; i < resCount; i++)
    {
        switch (reservations[i].getStatus())
        {
        case Reservation::ACTIVE:
            active++;
            break;
        case Reservation::CHECKED_OUT:
            done++;
            break;
        default:
            cancelled++;
        }
    }
    double revenue = 0;
    map<string, double> byType;
    for (int i = 0; i < billCount; i++)
    {
        revenue += bills[i].getTotal();
        byType[bills[i].getRoomType()] += bills[i].getTotal();
    }
    cout << "\n=========== " << name << " - REPORT ===========\n"
        << "Total rooms        : " << roomCount << "\n"
        << "Occupied / Free    : " << occupied << " / " << (roomCount - occupied) << "\n"
        << "Occupancy rate     : " << fixed << setprecision(1)
         << (roomCount ? 100.0 * occupied / roomCount : 0.0) << " %\n"
        << "Registered guests  : " << guestCount << "\n"
        << "Reservations       : " << resCount << " (Active " << active << ", Checked-out " << done
        << ", Cancelled " << cancelled << ")\n"
        << "Bills generated    : " << billCount << "\n"
        << "Total revenue      : Rs. " << setprecision(2) << revenue << "\n";
    if (!byType.empty())
    {
        cout << "Revenue by room type:\n";
        for (map<string, double>::const_iterator it = byType.begin(); it != byType.end(); ++it)
            cout << "   " << left << setw(10) << it->first << ": Rs. " << it->second << "\n";
    }
    cout << "==============================================\n";
}

// ------------------------------------------------------------------
// 8. SAVE / LOAD
// ------------------------------------------------------------------
bool Hotel::saveData() const { return FileManager::saveAll(*this); }
bool Hotel::loadData() { return FileManager::loadAll(*this); }

void Hotel::saveLoad()
{
    cout << "\n--- SAVE / LOAD ---\n1. Save data to files\n2. Load data from files (replaces current data)\n0. Back\n";
    int c = Validator::readInt("Choice: ", 0, 2);
    if (c == 1)
    {
        if (saveData())
            cout << "  Data saved.\n";
    }
    else if (c == 2)
    {
        if (loadData())
            cout << "  Data loaded: " << roomCount << " rooms, " << guestCount
                << " guests, " << resCount << " reservations, " << billCount << " bills.\n";
    }
}
