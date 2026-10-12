#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

class Hotel;

// Handles all persistence. Text files, '|' delimited, one record per line.
class FileManager
{
private:
    static std::string dataDir(); // finds / creates the Data folder
public:
    static bool saveAll(const Hotel &hotel);
    static bool loadAll(Hotel &hotel);
};

#endif
