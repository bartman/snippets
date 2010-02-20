#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>

int
main (void)
{
        std::string foo = "1234aBc";
        std::string::size_type pos;

        pos = foo.find ('1');

        std::cout << "find 1 in " << foo << " -> " << std::hex << pos << "\n";

        pos = foo.find ('4');

        std::cout << "find 4 in " << foo << " -> " << std::hex << pos << "\n";

        pos = foo.find ('5');

        std::cout << "find 5 in " << foo << " -> " << std::hex << pos << "\n";

        std::transform(foo.begin(), foo.end(), foo.begin(), tolower);

        std::cout << ">> " << foo << "\n";

        return 0;
}
