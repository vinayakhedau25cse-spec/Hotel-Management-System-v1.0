#include "FileManager.h"
#include "Hotel.h"
#include "HotelException.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <direct.h>
#endif

using namespace std;

static vector<string> split(const string &line, char d = '|')
{
    vector<string> parts;
    string tok;
    istringstream is(line);
    while (getline(is, tok, d))
        parts.push_back(tok);
    return parts;
}

static bool dirExists(const string &path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0 && (st.st_mode & S_IFDIR);
}

string FileManager::dataDir()
{
    if (dirExists("Data"))
        return "Data/";
    if (dirExists("../Data"))
        return "../Data/";
#ifdef _WIN32
    _mkdir("Data");
#else
    mkdir("Data", 0755);
#endif
    return "Data/";
}

bool FileManager::saveAll(const Hotel &h)
{
    string dir = dataDir();
    try
    {
        ofstream fr(dir + "rooms.txt"), fg(dir + "guests.txt"), fv(dir + "reservations.txt"), fb(dir + "bills.txt");
        if (!fr || !fg || !fv || !fb)
            throw HotelException("Cannot open data files for writing in " + dir);
        for (int i = 0; i < h.getRoomCount(); i++)
            fr << h.getRoomAt(i)->serialize() << "\n";
        for (int i = 0; i < h.getGuestCount(); i++)
            fg << h.getGuestAt(i).serialize() << "\n";
        for (int i = 0; i < h.getReservationCount(); i++)
            fv << h.getReservationAt(i).serialize() << "\n";
        for (int i = 0; i < h.getBillCount(); i++)
            fb << h.getBillAt(i).serialize() << "\n";
        return true;
    }
    catch (const exception &e)
    {
        cout << "  Save failed: " << e.what() << "\n";
        return false;
    }
}

bool FileManager::loadAll(Hotel &h)
{
    string dir = dataDir();
    h.clearAll();
    int bad = 0;
    string line;

    // Missing file = first run, treated as empty (not an error).
    {
        ifstream f(dir + "rooms.txt");
        while (getline(f, line))
        {
            if (line.empty())
                continue;
            try
            {
                vector<string> p = split(line);
                if (p.size() != 4)
                    throw HotelException("bad field count");
                Room *r = Room::create(p[0], stoi(p[1]), stod(p[2]));
                r->setBooked(p[3] == "1");
                h.addRoomObject(r);
            }
            catch (const exception &)
            {
                bad++;
            }
        }
    }
    {
        ifstream f(dir + "guests.txt");
        while (getline(f, line))
        {
            if (line.empty())
                continue;
            try
            {
                vector<string> p = split(line);
                if (p.size() != 4)
                    throw HotelException("bad field count");
                h.addGuestObject(Guest(stoi(p[0]), p[1], p[2], p[3]));
            }
            catch (const exception &)
            {
                bad++;
            }
        }
    }
    {
        ifstream f(dir + "reservations.txt");
        while (getline(f, line))
        {
            if (line.empty())
                continue;
            try
            {
                vector<string> p = split(line);
                if (p.size() != 6)
                    throw HotelException("bad field count");
                int st = stoi(p[5]);
                if (st < 0 || st > 2)
                    throw HotelException("bad status");
                h.addReservationObject(Reservation(stoi(p[0]), stoi(p[1]), stoi(p[2]), p[3], stoi(p[4]),
                                                (Reservation::Status)st));
            }
            catch (const exception &)
            {
                bad++;
            }
        }
    }
    {
        ifstream f(dir + "bills.txt");
        while (getline(f, line))
        {
            if (line.empty())
                continue;
            try
            {
                vector<string> p = split(line);
                if (p.size() != 9)
                    throw HotelException("bad field count");
                h.addBillObject(Bill(stoi(p[0]), stoi(p[1]), stoi(p[2]), stoi(p[3]), p[4],
                                    stoi(p[5]), stod(p[6]), stod(p[7]), stod(p[8])));
            }
            catch (const exception &)
            {
                bad++;
            }
        }
    }

    h.recalculateIds();
    if (bad)
        cout << "  Warning: " << bad << " corrupt line(s) were skipped while loading.\n";
    return true;
}
