#ifndef _MICROSTR_H_
#define _MICROSTR_H_

#include "var_type.h"

void read_microstr(char *fname, local_str *lstr);
// change coordinate origin into the center of the top plane of the box containg a microstrcuture
bool find_str_hit_local(ray_trace1 *ray1, local_str *lstr); 

#endif
