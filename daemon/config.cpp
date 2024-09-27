#include <cstring>
#include "config.hpp"

std::vector<Data> Config::read()
{
    std::cout << "Config::read1\n";
    std::ifstream in_file(path);
    if (!in_file.is_open())
    {
        syslog(LOG_ERR, "File could not be opened! %s", path.c_str());
        exit(EXIT_FAILURE);
    }
    std::cout << "Config::read2\n";
    std::string line;
    std::vector<Data> data;

    while (std::getline(in_file, line))
    {
        std::cout << line << std::endl;
        std::istringstream ss(line);
        Data entry;
        ss >> entry.folder1 >> entry.folder2 >> entry.time;
        data.push_back(entry);
    }
    std::cout << "Config::read3\n";
    in_file.close();
    syslog(LOG_INFO, "Config file %s was read successfully!", path.c_str());
    std::cout << "Config::read4\n";
    return data;
}