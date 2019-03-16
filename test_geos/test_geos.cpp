// test_geos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define GEOS_USE_ONLY_R_API
#include "geos_c.h"

int main()
{
    std::cout << "Hello World!\n"; 

	auto x = GEOS_init_r();
	GEOS_finish_r(x);
}
