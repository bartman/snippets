#include <iostream>
#include <string_view>
#include <thread>
#include <vector>

int foo(int i)
{
    int x;
    return 0;
}

int main(const int argc, const char *const *const argv)
{
    std::vector<std::string_view> args(
        argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)));

    std::cout << "args=" << args.size() << std::endl;

    for (const auto &arg : args) {
        std::cout << arg << std::endl;
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10s);

    return 0;
}
