#include <string>
#include <chrono>
class Daemon
{
public:
    static Daemon &get_instance(const std::string &folder1,
    const std::string &folder2,
    const std::chrono::seconds& time)
    {
        static Daemon instance{folder1, folder2, time};
        return instance;
    };
    void run();
private:
    Daemon(const std::string &folder1,
           const std::string &folder2,
           const std::chrono::seconds &time) :
           folder1(folder1), folder2(folder2),  time(time) {}

    std::string folder1;
    std::string folder2;
    std::chrono::seconds time;

    Daemon() = delete;
    Daemon(const Daemon &) = delete;
    Daemon(Daemon &&) = delete;
    Daemon &operator=(const Daemon &) = delete;
    Daemon &operator=(Daemon &&) = delete;
};