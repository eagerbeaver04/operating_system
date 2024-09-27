#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <csignal>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <dirent.h>
#include <cstring>
#include <errno.h>
#include <sys/wait.h>
#include <filesystem>

#include "daemon.hpp"

void Daemon::set_data(const std::vector<Data> &data) 
{
    table = data;
    for(auto&& time : time_points)
        time = std::chrono::steady_clock::now();
}

void Daemon::replace_folder(const Data & data)
{
    std::filesystem::copy(data.folder1, data.folder2);
}

void Daemon::run()
{
    for(int i=0; i < table.size(); ++i)
    {
        auto now_time = std::chrono::steady_clock::now();
        auto prev_time = time_points[i];
        int diff = std::chrono::duration_cast<std::chrono::seconds>(now_time - prev_time).count();
        if(diff >= table[i].time)
        {
            replace_folder(table[i]);
            time_points[i] = std::chrono::steady_clock::now();
        }
    }
}