#pragma once

#include <vector>
#include <unordered_map>
#include <csignal>
#include <chrono>
#include "data.hpp"

class Daemon
{
public:
    static Daemon &get_instance(const std::string &config_file)
    {
        static Daemon instance{config_file};
        return instance;
    };
    void run();
    void set_data(const std::vector<Data>& data);

private:
    Daemon(const std::string &config_file) :
          config_file(config_file), table(), time_points() {}

    std::string config_file;
    std::vector<Data> table;
    std::vector<std::chrono::time_point<std::chrono::steady_clock>> time_points;
    Daemon() = delete;
    Daemon(const Daemon &) = delete;
    Daemon(Daemon &&) = delete;
    Daemon &operator=(const Daemon &) = delete;
    Daemon &operator=(Daemon &&) = delete;
};