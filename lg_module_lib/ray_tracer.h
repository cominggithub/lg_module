#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_

#include "var_type.h"
#include "opt_record.h"

char* get_child_prefix(const char* prefix, int child_num);
// void trace_one_ray(const char * prefix, ray_trace1 *ray, dot_position *dpos, opt_record_head *opr_head, local_str *lstr);
void trace_one_ray(const char * prefix, ray_trace1 *ray, dot_position *dpos, opt_record *opr, local_str *lstr);
void trace_one_ray_single_thread(ray_trace1 *ray1, dot_position *dpos, opt_record *opr, local_str *lstr);
void moduleiv_simple(ray_trace1 *rayT, ray_trace1 *rayR);
void dump_ray_counter();
#endif