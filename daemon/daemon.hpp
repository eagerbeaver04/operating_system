#pragma once

#include <vector>
#include <unordered_map>
#include <chrono>
#include <functional>
#include "config.hpp"

class Daemon
{
public:
    static Daemon &get_instance()
    {
        static Daemon instance;
        return instance;
    };
    void run(const std::string&);
    void reopen_config_file();

private:
    Config config;
    std::vector<Data> table;
    std::vector<std::chrono::time_point<std::chrono::steady_clock>> time_points;

    void replace_folder(const Data&);
    void create_pid_file();
    void daemonize();
    void set_data(const std::vector<Data> &);

    Daemon() = default;
    Daemon(const Daemon &) = delete;
    Daemon(Daemon &&) = delete;
    Daemon &operator=(const Daemon &) = delete;
    Daemon &operator=(Daemon &&) = delete;
};