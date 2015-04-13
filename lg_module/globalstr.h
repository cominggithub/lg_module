#ifndef _GLOBALSTR_H_
#define _GLOBALSTR_H_

#include "var_type.h"
// change coordinate origin into the center of the top plane of the box containg a microstrcuture
void find_str_hit_global(ray_trace1 *ray1, dot_position *dpos, opt_record *opr);
void part_dots(dot_position *dpos);

#endif
