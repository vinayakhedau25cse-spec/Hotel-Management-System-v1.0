#ifndef ROOM_H
#define ROOM_H

#include <string>

// Abstract base class. Derived classes override pricing and amenities,
// which gives us runtime polymorphism (Room* -> StandardRoom/DeluxeRoom/SuiteRoom).
class Room
{
protected:
    int roomNumber;
    double basePrice; // price per night
    bool booked;

public:
    Room();
    Room(int number, double price);
    virtual ~Room();

    // getters / setters (encapsulation, setters validate)
    int getRoomNumber() const { return roomNumber; }
    double getBasePrice() const { return basePrice; }
    bool isBooked() const { return booked; }
    void setRoomNumber(int number);
    void setBasePrice(double price);
    void setBooked(bool b) { booked = b; }

    // polymorphic interface
    virtual std::string getType() const = 0;
    virtual std::string getAmenities() const = 0;
    virtual double calculateCharge(int nights) const; // may be overridden
    virtual void display() const;
    virtual std::string serialize() const;

    // Factory: builds the right derived object from a type name
    static Room *create(const std::string &type, int number, double price);
};

class StandardRoom : public Room
{
public:
    StandardRoom() : Room() {}
    StandardRoom(int n, double p) : Room(n, p) {}
    std::string getType() const override { return "Standard"; }
    std::string getAmenities() const override { return "Fan/AC, TV"; }
};

class DeluxeRoom : public Room
{
public:
    DeluxeRoom() : Room() {}
    DeluxeRoom(int n, double p) : Room(n, p) {}
    std::string getType() const override { return "Deluxe"; }
    std::string getAmenities() const override { return "AC, TV, Minibar, Breakfast"; }
    double calculateCharge(int nights) const override; // +10% service fee
};

class SuiteRoom : public Room
{
public:
    SuiteRoom() : Room() {}
    SuiteRoom(int n, double p) : Room(n, p) {}
    std::string getType() const override { return "Suite"; }
    std::string getAmenities() const override { return "AC, Lounge, Jacuzzi, Butler"; }
    double calculateCharge(int nights) const override; // +20% + flat butler fee
};

#endif
