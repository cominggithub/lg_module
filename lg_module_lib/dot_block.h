#ifndef _DOT_BLOCK_H_
#define _DOT_BLOCK_H_

#include "var_type.h"

#define BLOCK_X_SIZE ((int)ceil(xden_rng*block_scale))
#define BLOCK_Y_SIZE ((int)ceil(yden_rng*block_scale))
#define BLOCK_IDX(x) ((int)(x*block_scale))

struct dot_node
{
	struct dot_node *next;
	double xd;
	double yd;
	long int index;
};

struct dot_block
{
	int count;
	int index;
	struct dot_node *head;
	struct dot_node *tail;
};

struct dot_block_index
{
	int* indices_x;
	int* indices_y;
	int count;
	int size;
};

void load_dot_block_from_dot_position(dot_block *dot_blk, dot_position *dpos);

dot_block_index* get_block_neighbor(dot_block *dot_blk, int radius);
int get_block_count(int radius);
void free_dot_block_index(dot_block_index **blk_idx);
dot_block_index* alloc_dot_block_index(int size);
void dump_dot_block_index(dot_block_index *blk_idx);
void dump_dot_blk(dot_block *dot_blk);
void dump_dot_blk_single(dot_block *dot_blk);

dot_block* alloc_dot_block_array(int size);
void free_dot_block_array(int size);

bool find_nearest_dot(dot_block *dot_blk, ray_trace1 *ray, int *hit_index, double *hit_xd, double *hit_yd);
bool find_nearest_dot_r(dot_block *dot_blk, ray_trace1 *ray, int radius, int radius_width, int *hit_index, double *hit_xd, double *hit_yd);
dot_block_index* get_block_neighbor_w(int xc, int yc, int radius, int radius_width);
dot_block_index* get_block_neighbor(int xc, int yc, int radius);

bool find_min_distance_in_blk(dot_block *dot_blk, dot_block_index* dot_blk_idx, double x, double y, double *distance, int *index, double *xd, double *yd);


#endif