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

void Daemon::create_pid_file()
{
    const char *pid_file = "/var/run/daemon.pid";
    int pid_file_handle = open(pid_file, O_RDWR | O_CREAT, 0600);

    if (pid_file_handle == -1)
    {
        syslog(LOG_ERR, "PID file %s cannot be open", pid_file);
        exit(EXIT_FAILURE);
    }

    if (lockf(pid_file_handle, F_TLOCK, 0) == -1)
    {
        syslog(LOG_ERR, "PID file %s cannot be open", pid_file);
        exit(EXIT_FAILURE);
    }

    char str[10];
    sprintf(str, "%d\n", getpid());
    write(pid_file_handle, str, strlen(str));  
}

void Daemon::run()
{   //TODO: fix 
    while(true)
    {
        for(int i=0; i < table.size(); ++i)
        {
            auto now_time = std::chrono::steady_clock::now();
            auto prev_time = time_points[i];
            int diff = std::chrono::duration_cast<std::chrono::seconds>(now_time - prev_time).count();
            if(diff >= table[i].time)
            {
                try
                {
                    replace_folder(table[i]);
                }
                catch(const std::exception& e)
                {
                    syslog(LOG_ERR, e.what());
                }
                time_points[i] = std::chrono::steady_clock::now();
            }
        }
    }
}