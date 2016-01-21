#ifndef _GLOBALSTR_H_
#define _GLOBALSTR_H_

#include "var_type.h"
// change coordinate origin into the center of the top plane of the box containg a microstrcuture
bool find_str_hit_global(ray_trace1 *ray1, dot_position *dpos, opt_record *opr, local_str *lstr, int *type);
void part_dots_unsorted(dot_position *dpos);
void part_dots(dot_position *dpos);
#endif
