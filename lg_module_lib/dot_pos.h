#ifndef _DOT_POS_H_
#define _DOT_POS_H_

#include "var_type.h"

bool load_dpos(const char* fname, dot_position *dpos, char hexbl, double hexlng);
int dpos_quick_partition(dot_position *dpos, int begin, int end);
void dpos_quick_sort(dot_position *dpos, int begin, int end);

int dpos_quick_partition_by_xd(dot_position *dpos, int begin, int end);
void dpos_quick_sort_by_xd(dot_position *dpos, int begin, int end);

int dpos_quick_partition_by_partindx(dot_position *dpos, int begin, int end);
void dpos_quick_sort_by_partindx(dot_position *dpos, int begin, int end);

#endif