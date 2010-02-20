#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <map>

class Test
{
public:
        Test() 
        { 
                std::cout << "constructor\n";
                throw std::string ("error"); 
        }

        ~Test() 
        { 
                std::cout << "constructor\n";
        }

};

int
main (void)
{
        try {
                Test foo;

        } catch (std::string x) {
                std::cout << x << "\n";
        }

        try {
                std::map<int, std::string*> m;
                std::string a = "foo";
                std::string b = "bar";

                m[0] = &a;
                m[1] = &b;

                std::string* x = m[2];

                std::cout << "x = " << x << "\n";

        } catch (...) {
                std::cout << "exception\n";
        }

        return 0;
}
