#include "daemon.hpp"

void signal_handler(int sig)
{
    switch (sig)
    {
    case SIGHUP:
        Daemon::get_instance().reopen_config_file();
        syslog(LOG_INFO, "Re-read config file");
        break;
    case SIGTERM:
        syslog(LOG_INFO, "Exiting");
        closelog();
        exit(EXIT_SUCCESS);
        break;
    }
}

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

void Daemon::reopen_config_file()
{
    set_data(config.read());
}

void Daemon::create_pid_file()
{
    std::string pid_file = "/var/run/daemon.pid";
    int pid_file_handle = open(pid_file.c_str(), O_RDWR | O_CREAT, 0600);

    if (pid_file_handle == -1)
    {
        syslog(LOG_ERR, "PID file %s cannot be open", pid_file.c_str());
        exit(EXIT_FAILURE);
    }

    if (lockf(pid_file_handle, F_TLOCK, 0) == -1)
    {
        syslog(LOG_ERR, "PID file %s cannot be open", pid_file.c_str());
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO, "PID file %s opened successfully", pid_file.c_str());

    char str[10];
    sprintf(str, "%d\n", getpid());
    write(pid_file_handle, str, strlen(str));  
}

void Daemon::daemonize()
{
    pid_t pid, sid;

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0)
        exit(EXIT_FAILURE);

    if ((chdir("/")) < 0)
        exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    create_pid_file();
}

void Daemon::run(const std::string& filename)
{   
    config = {filename};

    daemonize();

    reopen_config_file();

    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);

    while (true)
    {
        for(int i=0; i < table.size(); ++i)
        {
            auto now_time = std::chrono::steady_clock::now();
            auto prev_time = time_points[i];
            int diff = std::chrono::duration_cast<std::chrono::seconds>(now_time - prev_time).count();
            syslog(LOG_INFO, "diff: %d", diff);
            if(diff >= table[i].time)
            {   
                try
                {
                    replace_folder(table[i]);
                    syslog(LOG_INFO, "data from folder %s to folder %s was replaced successfully!", table[i].folder1.c_str(), table[i].folder2.c_str());
                }
                catch(const std::exception& e)
                {
                    syslog(LOG_ERR, "Folder can not be replaced: %s", e.what());
                }
                time_points[i] = std::chrono::steady_clock::now();
            }
        }
    }
}