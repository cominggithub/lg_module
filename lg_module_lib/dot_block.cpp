#include "dot_block.h"
#include "var_def.h"
#include "math.h"
#include "stdio.h"
#include "dbg_log.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "data_file_util.h"
using namespace std;



dot_block* alloc_dot_block_array(int size)
{
    int i;
    dot_block* dar;
    dar = new dot_block[size]();

    if (dar == NULL)
    {
        fprintf(stderr, "alloc_dot_block_array out of memory\n");
        exit(0);
    }

    for(i=0; i<size; i++)
    {
        dar[i].index = i;
    }
    return dar;
}

void free_dot_block_array(dot_block **dar, int size)
{
    delete *dar;
    *dar = NULL;
}

dot_node* alloc_dot_node(long int dpos_index, double xd, double yd)
{
    dot_node* dotn = new dot_node;

    if (dotn == NULL)
    {
        fprintf(stderr, "alloc_dot_node out of memory\n");
        exit(0);
    }

    dotn->next = NULL;
    dotn->xd = xd;
    dotn->yd = yd;

    return dotn;
}

// we dont calculate the square root of the distance
double get_distance(double x1, double y1, double x2, double y2)
{
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

void add_dot_node(dot_block *dot_blk, long int dops_index, double xd, double yd)
{
    dot_node *dotn = alloc_dot_node(dops_index, xd, yd);
    dot_block *cur_blk;
    cur_blk = dot_blk+ (BLOCK_IDX(xd)*BLOCK_Y_SIZE + BLOCK_IDX(yd));

    if (cur_blk->head == NULL)
    {
        cur_blk->head = dotn;
    }
    else
    {
        cur_blk->tail->next = dotn;
    }
    cur_blk->tail = dotn;
    cur_blk->count++;
}

int get_start_radius(double x, double y)
{
    // xden_or=14.7
    // yden_or=4.15
    // den_2_pos_scale=10000.0
    // xden_rng=163.0
    // yden_rng=122.5

    // int dot_start_idx_x;
    // int dot_end_idx_x;
    // int dot_start_idx_y;
    // int dot_end_idx_y;
    int radius_x;
    int radius_y;

    // dot_start_idx_x = BLOCK_IDX(xden_or);
    // dot_end_idx_x = BLOCK_IDX(xden_rng);

    // dot_start_idx_y = BLOCK_IDX(yden_or);
    // dot_end_idx_y = BLOCK_IDX(yden_rng);

    radius_x = std::min(abs(BLOCK_IDX(x) - BLOCK_IDX(xden_or)), abs(BLOCK_IDX(x) - BLOCK_IDX(xden_rng)));
    radius_y = std::min(abs(BLOCK_IDX(x) - BLOCK_IDX(xden_or)), abs(BLOCK_IDX(x) - BLOCK_IDX(xden_rng)));

    return std::min(radius_x, radius_y);


}

bool find_nearest_dot(dot_block *dot_blk, ray_trace1 *ray, int *hit_index, double *hit_xd, double *hit_yd)
{
    int radius = 1;
    int radius_width = 1;


    if (BLOCK_IDX(ray->xr) < BLOCK_X_SIZE/2)
    {
        radius_width = 1;
    }

    radius = get_start_radius(ray->xr, ray->yr);
    pInt(radius);

    return find_nearest_dot_r(dot_blk, ray, radius, radius+500, radius_width, hit_index, hit_xd, hit_yd);

}

bool out_of_block_range(double x, double y, int radius)
{
    return  BLOCK_IDX(x)+radius > BLOCK_X_SIZE  &&
            BLOCK_IDX(y)+radius > BLOCK_Y_SIZE  &&
            BLOCK_IDX(x)-radius < 0            &&
            BLOCK_IDX(x)-radius < 0;
}


bool find_nearest_dot_r(dot_block *dot_blk, ray_trace1 *ray, int radius, int radius_max, int radius_width, int *hit_index, double *hit_xd, double *hit_yd)
{
    int i;
    int blk_idx;
    int tmp_hit_idx;
    double tmp_hit_xd;
    double tmp_hit_yd;
    double double_max = 99999;
    double min_distance;
    double tmp_distance;
    double required_min_distance;

    dot_block_index* dot_blk_idx;

    required_min_distance = 10.0;

    // printf("ray (%.5f, %.5f), radius %d, radius width: %d\n",
    //         ray->xr,
    //         ray->yr,
    //         radius,
    //         radius_width
    //     );

    // out of range, stop
    if (out_of_block_range(ray->xr, ray->yr, radius))
    {
        printf("ray %d, (%.5f, %.5f), radius %d, radius width: %d, out_of_block_range\n",
            ray->no,
            ray->xr,
            ray->yr,
            radius,
            radius_width
        );

        append_block_hit_log(
            ray->no,
            ray->xr,
            ray->yr,
            tmp_hit_xd,
            tmp_hit_yd,
            88888,
            radius,
            radius_width
        );

        return false;
    }

    if (radius > radius_max)
    {
        printf("ray %d, (%.5f, %.5f), radius %d, radius width: %d, out of radius\n",
            ray->no,
            ray->xr,
            ray->yr,
            radius,
            radius_width
        );

        append_block_hit_log(
            ray->no,
            ray->xr,
            ray->yr,
            tmp_hit_xd,
            tmp_hit_yd,
            99999,
            radius,
            radius_width
        );

        return false;
    }


    dot_blk_idx     = get_block_neighbor_w(BLOCK_IDX(ray->xr), BLOCK_IDX(ray->yr), radius, radius_width);
    pInt(radius);
    pInt(dot_blk_idx->count);

    if (dot_blk_idx->count == 0)
    {
        return find_nearest_dot_r(dot_blk, ray, radius+radius_width, radius_max, radius_width, hit_index, hit_xd, hit_yd);
    }

    min_distance    = double_max;  // max distance value, should be large enough
    tmp_hit_idx     = -1;

    tmp_distance = double_max;

    // find_min_distance_in_blk(dot_block *dot_blk, dot_block_index* dot_blk_idx, double x, double y, double *distance, int *index, double *xd, double *yd);

    find_min_distance_in_blk(dot_blk, dot_blk_idx, ray->xr, ray->yr, &tmp_distance, &tmp_hit_idx, &tmp_hit_xd, &tmp_hit_yd);

    // free(dot_blk_idx->indices_x);
    // free(dot_blk_idx->indices_y);
    // free(dot_blk_idx->indices_x);
    free_dot_block_index(&dot_blk_idx);

    // if (tmp_distance > required_min_distance)
    // {
    //     return find_nearest_dot_r(dot_blk, ray, radius+radius_width, radius_max, radius_width, hit_index, hit_xd, hit_yd);
    // }

    *hit_index  = tmp_hit_idx;
    *hit_xd     = tmp_hit_xd;
    *hit_yd     = tmp_hit_yd;

    if(enable_block_hit_log)
    {
        append_block_hit_log(
            ray->no,
            ray->xr,
            ray->yr,
            tmp_hit_xd,
            tmp_hit_yd,
            tmp_distance,
            radius,
            radius_width
        );
    }

    printf("ray (%.5f, %.5f), radius %d, radius width: %d, hit (%.5f, %.5f), distance: %.5f\n",
        ray->xr,
        ray->yr,
        radius,
        radius_width,
        *hit_xd,
        *hit_yd,
        tmp_distance
    );


    return true;
}

bool find_min_distance_in_blk(dot_block *dot_blk, dot_block_index* dot_blk_idx, double x, double y, double *distance, int *index, double *xd, double *yd)
{
    int i;
    int blk_idx;
    int tmp_hit_idx;
    double tmp_hit_xd;
    double tmp_hit_yd;
    int xidx;
    int yidx;
    int block_offset;
    double double_max = 99999999;
    double min_distance;
    double tmp_distance;
    dot_block *cur_dot_blk;
    dot_node *cur_dot_node;

    min_distance = double_max;
    pl();
    pInt(dot_blk_idx->count);
    for(i=0; i<dot_blk_idx->count; i++)
    {

        xidx = dot_blk_idx->indices_x[i];
        yidx = dot_blk_idx->indices_y[i];

        block_offset = xidx * BLOCK_Y_SIZE + yidx;
        cur_dot_blk = dot_blk + block_offset;
        cur_dot_node = cur_dot_blk->head;

        while(cur_dot_node != NULL)
        {
            pl();
            tmp_distance = get_distance(x, y, cur_dot_node->xd, cur_dot_node->yd);
            // printf("  (%.5f, %.5f) -> (%.5f, %.5f): %.5f\n", x, y, cur_dot_node->xd, cur_dot_node->yd, tmp_distance);
            if (min_distance > tmp_distance)
            {
                pl();
                min_distance    = tmp_distance;
                tmp_hit_idx     = cur_dot_node->index;
                tmp_hit_xd      = cur_dot_node->xd;
                tmp_hit_yd      = cur_dot_node->yd;
            }
            cur_dot_node = cur_dot_node->next;
        }
    }

    pDouble(min_distance);
    if (min_distance != double_max)
    {
        *distance   = min_distance;
        *index      = tmp_hit_idx;
        *xd         = tmp_hit_xd;
        *yd         = tmp_hit_yd;
        return true;
    }

    return false;
}

void dump_dot_blk(dot_block *dot_blk)
{
    int i;
    dot_block *cur_blk = dot_blk;

    int size = BLOCK_X_SIZE*BLOCK_Y_SIZE;


    for(i=0; i<size; i++)
    {
        printf("[%d][%d](%d): %d\n", i/BLOCK_Y_SIZE, i%BLOCK_Y_SIZE, cur_blk->index, cur_blk->count);
        dot_node *cur_node = cur_blk->head;
        while(cur_node != NULL)
        {
            printf("  (%.5f, %.5f)\n", cur_node->xd, cur_node->yd);
            cur_node = cur_node->next;
        }
        cur_blk++;
    }
}

void dump_dot_blk_single(dot_block *dot_blk)
{
    dot_block *cur_blk = dot_blk;
    dot_node *cur_node = cur_blk->head;
    while(cur_node != NULL)
    {
        printf("  (%.5f, %.5f)\n", cur_node->xd, cur_node->yd);
        cur_node = cur_node->next;
    }
}

void dump_dot_blK_3dfile(const char* fname, dot_block *dot_blk)
{
    int i;
    dot_block *cur_blk = dot_blk;

    int size = BLOCK_X_SIZE*BLOCK_Y_SIZE;

    FILE *fp;
    fp = fopen(fname, "w");

    for(i=0; i<size; i++)
    {
        fprintf(fp, "%d %d %d\n", i/BLOCK_Y_SIZE, i%BLOCK_Y_SIZE, cur_blk->count);
        cur_blk++;
    }

    fflush(fp);
    fclose(fp);
}

void load_dot_block_from_dot_position(dot_block *dot_blk, dot_position *dpos)
{
    long int i;
    for(i=0; i<dpos->ndot; i++)
    {
        add_dot_node(dot_blk, i, dpos->xd[i], dpos->yd[i]);
    }
    // dump_dot_blk(dot_blk);
    // dump_dot_blK_3dfile("block3d.txt", dot_blk);
}

// dot_block_index* merge_dot_block_index(const dot_block_index* src1, const dot_block_index* src2)
// {
//     dot_block_index* merged;
//     int i;
//     merged = alloc_dot_block_index(src1->count+src2->count);


//     for(i=0; i<src1->count; i++)
//     {
//         merged->indices_x[merged->count] = src1->indices_x[i];
//         merged->indices_y[merged->count] = src1->indices_y[i];
//         merged->count++;
//     }

//     for(i=0; i<src2->count; i++)
//     {
//         merged->indices_x[merged->count] = src2->indices_x[i];
//         merged->indices_y[merged->count] = src2->indices_y[i];
//         merged->count++;
//     }

//     return merged;
// }

dot_block_index* get_block_neighbor_w(int xc, int yc, int radius, int radius_width)
{
    int i;
    int j;
    int count;

    dot_block_index *merged_idx;
    dot_block_index* blk_idx[radius_width];

    count = 0;
    for(i=0; i<radius_width; i++)
    {
        blk_idx[i] = get_block_neighbor(xc, yc, radius+i);
        count += blk_idx[i]->count;
        // dump_dot_block_index(blk_idx[i]);
    }

    merged_idx          = alloc_dot_block_index(count);
    merged_idx->count   = 0;

    for(i=0;i<radius_width; i++)
    {
        for(j=0; j<blk_idx[i]->count; j++)
        {
            merged_idx->indices_x[merged_idx->count] = blk_idx[i]->indices_x[j];
            merged_idx->indices_y[merged_idx->count] = blk_idx[i]->indices_y[j];
            merged_idx->count++;
        }
    }

    for(i=0; i<radius_width; i++)
    {
        free_dot_block_index(&blk_idx[i]);
    }


    // dump_dot_block_index(merged_idx);
    return merged_idx;
}

dot_block_index* get_block_neighbor(int xc, int yc, int radius)
{
    int max_x = BLOCK_X_SIZE;
    int max_y = BLOCK_Y_SIZE;

    int x;
    int y;
    int left_x;
    int right_x;

    int top_y;
    int bottom_y;
    int count;

    dot_block_index* blk_idx;
    blk_idx = alloc_dot_block_index(get_block_count(radius));
    count = 0;

    left_x      = xc-radius;
    right_x     = xc+radius;
    top_y       = yc+radius;
    bottom_y    = yc-radius;

    // pInt(xc);
    // pInt(yc);
    // pInt(radius);
    // pInt(left_x);
    // pInt(right_x);
    // pInt(max_x);
    // pInt(top_y);
    // pInt(bottom_y);
    // pInt(max_y);
    // top ------
    y = top_y;
    if (y >=0 && y < max_y)
    {
        for(x=left_x; x<=right_x; x++)
        {
            if (x>=0 && x<max_x)
            {
                blk_idx->indices_x[count] = x;
                blk_idx->indices_y[count] = y;
                count++;
            }
        }
    }

    // bottom ------
    y = bottom_y;
    if (y >=0 && y < max_y)
    {
        for(x=left_x; x<=right_x; x++)
        {
            if (x>=0 && x<max_x)
            {
                blk_idx->indices_x[count] = x;
                blk_idx->indices_y[count] = y;
                count++;
            }
        }
    }

    // left |
    x = left_x;
    if (x >=0 && x < max_x)
    {
        for(y=bottom_y+1; y<=top_y-1; y++)
        {
            if (y>=0 && y<max_y)
            {
                blk_idx->indices_x[count] = x;
                blk_idx->indices_y[count] = y;
                count++;
            }
        }
    }

    // right |
    x = right_x;
    if (x >=0 && x < max_x)
    {
        for(y=bottom_y+1; y<=top_y-1; y++)
        {
            if (y >= 0 && y < max_y)
            {
                blk_idx->indices_x[count] = x;
                blk_idx->indices_y[count] = y;
                count++;
            }
        }
    }

    blk_idx->count = count;

    return blk_idx;
}

dot_block_index* alloc_dot_block_index(int size)
{
    dot_block_index *blk_idx;
    try
    {
        blk_idx = new dot_block_index;

        blk_idx->indices_x = new int[size];
        blk_idx->indices_y = new int[size];

        if (blk_idx == nullptr)
        {
           fprintf(stderr, "alloc_dot_block_index out of memory\n");
            exit(0);
        }

        if (blk_idx->indices_x == nullptr)
        {
            fprintf(stderr, "alloc_dot_block_index out of memory\n");
            exit(0);
        }

        if (blk_idx->indices_y == nullptr)
        {
            fprintf(stderr, "alloc_dot_block_index out of memory\n");
            exit(0);
        }

        blk_idx->size = size;

    }
    catch(std::bad_alloc&)
    {
        fprintf(stderr, "alloc_dot_block_index out of memory\n");
        exit(0);
    }

    return blk_idx;
}

void free_dot_block_index(dot_block_index **blk_idx)
{
    dot_block_index *node;
    node = *blk_idx;
    delete node->indices_x;
    delete node->indices_y;
    delete node;
    *blk_idx = NULL;
}

int get_block_count(int radius)
{
    if (radius <= 0)
        return 1;

    return radius*2*4;
}

void dump_dot_block_index(dot_block_index *blk_idx)
{
    int i;
    printf("block count: %d\n", blk_idx->count);
    for(i=0;i<blk_idx->count; i++)
    {
        printf("%2d: = [%3d, %3d]\n", i, blk_idx->indices_x[i], blk_idx->indices_y[i]);
    }
}
