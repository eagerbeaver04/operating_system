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

void Daemon::run()
{
}