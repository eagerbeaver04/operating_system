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

#include "daemon.hpp"

void Daemon::set_data(const std::vector<Data> &data) 
{
    table = data;
    for(auto&& time : time_points)
        time = std::chrono::steady_clock::now();
}

void Daemon::run()
{
}