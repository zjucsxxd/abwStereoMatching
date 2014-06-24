#include "utilities.hpp"

int taxicab_dist(int Lp,
				 int ap,
				 int bp,
				 int Lq,
				 int aq,
				 int bq)
{
	return (abs(Lp - Lq) + abs(ap - aq) + abs(bp - bq));
}
