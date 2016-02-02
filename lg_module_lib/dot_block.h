#ifndef _DOT_BLOCK_H_
#define _DOT_BLOCK_H_

#include "var_type.h"

#define BLOCK_X_SIZE ((int)ceil(xden_rng*block_scale))
#define BLOCK_Y_SIZE ((int)ceil(yden_rng*block_scale))
#define BLOCK_IDX(x) ((int)(x*block_scale))


void load_dot_block_from_dot_position(dot_block *dot_blk, dot_position *dops);


#endif