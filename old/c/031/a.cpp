/*
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <map>
#include <list>
*/

#define new __new



extern "C" {
#define __KERNEL__
#include <linux/list.h>
};

#undef __new

int
main (void)
{
        struct list_head foo;

        return 0;
}
