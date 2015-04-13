#ifndef _DEN_TO_POS_H_
#define _DEN_TO_POS_H_

#include "var_type.h"

// local function
void hex_fit(dot_position *dpos, double hexlng);				// put dots onto hexagon lattice
void den2pos_tetgen(dot_density *dden, dot_position *dpos, char hexbl, double hexlng); // preferred! due to its efficiency and uniformity
void den2pos(dot_density *dden, dot_position *dpos, char hexbl, double hexlng);
void hex_fit(dot_position *dpos, double hexlng);

#endif