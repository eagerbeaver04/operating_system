#include <cstring>
#include "config.hpp"

std::vector<Data> Config::read()
{
    std::ifstream in_file(path);
    if (!in_file.is_open())
    {
        syslog(LOG_ERR, "File could not be opened! %s", path.c_str());
        throw std::runtime_error("File could not be opened!");
    }

    std::string line;
    std::vector<Data> data;

    while (std::getline(in_file, line))
    {
        std::istringstream ss(line);
        Data entry;
        ss >> entry.folder1 >> entry.folder2 >> entry.time;
        data.push_back(entry);
    }

    in_file.close();

    return data;
}