#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <iostream>
#include <time.h>

#include <opencv2/core/core.hpp>

void echo_interval(clock_t t1, clock_t t2);
inline int taxicab_dist(const uchar a[3], const uchar b[3]) { return (abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2])); }

#endif
