#ifndef _BOX_CHECK_H_
#define _BOX_CHECK_H_

#include "var_type.h"

bool box_hitcheck_nobottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
bool box_hitcheck_w_bottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
int box_check_w_bottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
int box_check_function (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int faces, int hc);
#endif