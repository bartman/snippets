#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <stdint.h>
#include <linux/types.h>

typedef __attribute__ ((packed)) struct {
        uint16_t a;
        uint16_t b;
        uint16_t c;
} foo_t;

int
main (void)
{
        int rc;
        uint32_t len;
        std::string str;
        uint32_t magic;
        foo_t foo;

        std::ifstream in("foo", std::ios_base::in | std::ios_base::binary);

        rc = in.read (reinterpret_cast<char *>(&len), sizeof(len)).gcount();
        std::cout << "read " << rc << ", len=" << len << std::endl;

        in >> std::setw (len) >> str;
        rc = in.gcount();
        std::cout << "read " << rc << ", str=" << str << std::endl;

        rc = in.read (reinterpret_cast<char *>(&foo), sizeof(foo)).gcount();
        std::cout << "read " << rc << ", foo={" << foo.a << "," << foo.b <<
                "," << foo.c << "}" << std::endl;

        rc = in.read (reinterpret_cast<char *>(&magic), sizeof(magic)).gcount();
        std::cout << "read " << rc << ", magic=" << std::hex
                << magic << std::endl;

        in.close();

        return 0;
}
