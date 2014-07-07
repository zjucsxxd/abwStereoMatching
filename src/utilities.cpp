#include "utilities.hpp"

void echo_interval(clock_t t1, clock_t t2)
{
	double interval = (double)(t2 - t1) / CLOCKS_PER_SEC;
	std::cout << interval << std::endl;
}
