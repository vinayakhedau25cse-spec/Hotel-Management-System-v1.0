#ifndef GUEST_H
#define GUEST_H

#include <string>

class Guest
{
private:
    int guestId;
    std::string name;
    std::string phone;
    std::string email;

public:
    Guest();
    Guest(int id, const std::string &name, const std::string &phone, const std::string &email);
    ~Guest();

    int getGuestId() const { return guestId; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }

    void setName(const std::string &n); // validated, throws HotelException
    void setPhone(const std::string &p);
    void setEmail(const std::string &e);

    void display() const;
    std::string serialize() const;
};

#endif
