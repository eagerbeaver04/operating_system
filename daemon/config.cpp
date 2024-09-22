#include <cstring>
#include "config.hpp"

void Config::request_data(std::ofstream& out_file)
{
    Data data;
    std::string time;
    while (true)
    {
        std::cout << "Enter first folder: ";
        std::getline(std::cin, data.folder1);
        if (data.folder1.empty())
            break;

        std::cout << "Enter second folder: ";
        std::getline(std::cin, data.folder2);
        if (data.folder2.empty())
            break; 

        std::cout << "Enter time: ";
        std::getline(std::cin, time);
        if (time.empty())
            break;
        data.time = std::stoi(time);

        out_file << data.folder1 << " " << data.folder2 << " " << data.time << std::endl;
    }
}


std::vector<Data> Config::read()
{
    std::ifstream in_file(path);
    if (!in_file.is_open())
    {
        std::cerr << "Cannot open file: " << path << std::endl;
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