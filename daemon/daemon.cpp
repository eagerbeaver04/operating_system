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
    time_points = std::vector<std::chrono::time_point<std::chrono::steady_clock>>
        (table.size(), std::chrono::steady_clock::now()) ;
}

void Daemon::replace_folder(const Data & data)
{
    auto cur_path1 = current_path / data.folder1;
    auto cur_path2 = current_path / data.folder2;
    auto path1 = std::filesystem::absolute(cur_path1);
    auto path2 = std::filesystem::absolute(cur_path2);

    if (!std::filesystem::exists(cur_path2))
        std::filesystem::create_directory(cur_path2);

    for (const auto &entry : std::filesystem::directory_iterator(cur_path1))
        if (entry.is_regular_file())
            std::filesystem::rename(entry.path(), cur_path2 / entry.path().filename());

    std::string message = "Files moved successfully from " + 
        std::string(cur_path1.c_str()) + " to " + std::string(cur_path2.c_str());
    syslog(LOG_INFO, "%s", message.c_str());
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
    snprintf(str, sizeof(str), "%d\n", getpid());
    write(pid_file_handle, str, strlen(str));

    close(pid_file_handle);
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

void Daemon::run(const std::filesystem::path & current_dir, const std::string & filename)
{
    current_path = current_dir;
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
            if (diff >= table[i].time)
            {   
                try
                {
                    replace_folder(table[i]);
                    std::cout << "data from folder %s to folder %s was replaced successfully!\n"; 
                }
                catch(const std::exception& e)
                {
                    std::cout << "Folder can not be replaced\n";
                    syslog(LOG_ERR, "Folder can not be replaced: %s", e.what());
                }
                time_points[i] = std::chrono::steady_clock::now();
            }
        }
    }
    std::cout << "daemon_run_after_all\n";
}