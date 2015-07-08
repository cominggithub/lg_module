#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_

#include "var_type.h"
#include "opt_record.h"

void trace_one_ray(ray_trace1 *ray, dot_position *dpos, opt_record_head *opr_head, local_str *lstr);

#endif