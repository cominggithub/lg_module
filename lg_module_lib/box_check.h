#ifndef _BOX_CHECK_H_
#define _BOX_CHECK_H_

#include "var_type.h"

bool box_examine_nobottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
bool box_examine_w_bottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
#endif