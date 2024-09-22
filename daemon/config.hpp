#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <iostream>

#include "data.hpp"

class Config
{
private: 
    std::string path;
    void request_data(std::ofstream&);

public:
    Config(const std::string& path) : path(path)
    {
        if (!std::filesystem::exists(path))
        {
            std::ofstream out_file(path);
            if (out_file.is_open())
            {
                std::cout << "File is not found. Create your own config file: " << path << std::endl;
                request_data(out_file);
                out_file.close();
            }
            else
            {
                std::cerr << "Cannot create file: " << path << std::endl;
                throw std::runtime_error("File could not be created!");
            }
        }
    }

    std::vector<Data> read();
};