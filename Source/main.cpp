#include <iostream>
#include <ios>
#include "Hotel.h"
#include "Validator.h"
#include "HotelException.h"

using namespace std;    

static void showMenu(const string &hotelName)
{
    cout << "\n==========================================\n"
        << "   " << hotelName << " - RESERVATION SYSTEM\n"
        << "==========================================\n"
        << " 1. Add Record\n"
        << " 2. Display Records\n"
        << " 3. Search Record\n"
        << " 4. Update Record\n"
        << " 5. Delete Record\n"
        << " 6. Reservation Process (Book / Check-out / Cancel)\n"
        << " 7. Report\n"                                                                   
        << " 8. Save/Load\n"
        << " 9. Exit\n"
        << "------------------------------------------\n";
}

int main()
{
    Hotel hotel("Jagdish Hotels");
    hotel.loadData(); // START -> load existing records
    cout << "Existing records loaded.\n";

    bool running = true;
    while (running)
    {
        try
        {
            showMenu(hotel.getName());
            int choice = Validator::readInt("Enter choice (1-9): ", 1, 9);
            switch (choice)
            {
            case 1:
                hotel.addRecord();
                break;
            case 2:
                hotel.displayRecords();
                break;
            case 3:
                hotel.searchRecord();
                break;
            case 4:
                hotel.updateRecord();
                break;
            case 5:
                hotel.deleteRecord();
                break;
            case 6:
                hotel.mainTransaction();
                break;
            case 7:
                hotel.report();
                break;
            case 8:
                hotel.saveLoad();
                break;
            case 9:
                if (hotel.saveData())
                    cout << "Data saved. "; // save updated records -> EXIT
                cout << "Goodbye!\n";
                running = false;
                break;
            }
        }
        catch (const HotelException &e)
        {
            cout << "  Error: " << e.what() << "\n";
        }
        catch (const ios_base::failure &)
        {
            hotel.saveData();
            cout << "\nInput closed. Data saved. Exiting.\n";
            running = false;
        }
        catch (const exception &e)
        {
            cout << "  Unexpected error: " << e.what() << "\n";
        }
    }
    return 0;
}