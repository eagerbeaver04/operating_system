#include "conn_fifo.hpp"

NamedChannel::NamedChannel(const std::string &name, bool create)
{
    pathname = std::filesystem::current_path() / "tmp" / name;
    if (create)
    {
        if (mkfifo(pathname.c_str(), 0777) == -1)
        {
            if (errno != EEXIST)
            {
                std::cout << std::strerror(errno) << std::endl;
                throw std::runtime_error("Failed to create FIFO channel");
            }
        }
    }

    fd = open(pathname.c_str(), O_RDWR);
    if (fd == -1)
    {
        std::cout << pathname << " " << std::strerror(errno) << std::endl;
        throw std::runtime_error("Failed to open FIFO channel");
    }
}

bool NamedChannel::Read(std::string &message)
{
    char buffer[max_msg_size];
    memset(buffer, '\0', max_msg_size);
    fd = open(pathname.c_str(), O_RDWR);
    if (fd == -1)
    {
        std::cout << pathname << " " << std::strerror(errno) << std::endl;
        return false;
    }
    ssize_t bytes_read = read(fd, buffer, max_msg_size - 1);
    if (bytes_read == -1)
        return false;

    message.assign(buffer, bytes_read);
    return true;
}

bool NamedChannel::Write(const std::string &message)
{
    fd = open(pathname.c_str(), O_RDWR);
    if (fd == -1)
    {
        std::cout << pathname << " " << std::strerror(errno) << std::endl;
        return false;
    }
    ssize_t bytesWritten = write(fd, message.c_str(), message.size());
    if (bytesWritten == -1)
    {
        std::cout << "Error writing to FIFO: " << strerror(errno) << "\n";
        return false;
    }
    return true;
}

NamedChannel::~NamedChannel()
{
    close(fd);
    unlink(pathname.c_str());
}