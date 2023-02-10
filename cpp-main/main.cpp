#include <string_view>
#include <vector>
#include <iostream>

int main(const int argc, const char* const* const argv)
{
	std::vector<std::string_view> args(argv,
		std::next(argv, static_cast<std::ptrdiff_t>(argc)));

	for (auto &arg : args) {
		std::cout << arg << std::endl;
	}

	return 0;
}
