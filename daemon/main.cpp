#include "daemon.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file_path>" << std::endl;
        return EXIT_FAILURE;
    }

    openlog("daemon", LOG_PID | LOG_CONS, LOG_DAEMON);

    syslog(LOG_INFO, "Daemon starts");

    Daemon::get_instance(std::string(argv[1])).run();

    return EXIT_SUCCESS;
}