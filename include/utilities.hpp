#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <stdlib.h>

inline int taxicab_dist(const int a[3], const int b[3]) { return (abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2])); }

#endif
