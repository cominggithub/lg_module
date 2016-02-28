#include "dot_block.h"
#include "var_def.h"
#include "math.h"
#include "stdio.h"
#include "dbg_log.h"
#include <stdlib.h>
#include <string.h>

dot_block* alloc_dot_block_array(int size)
{
    dot_block* dar;
    dar = new dot_block[size]();
    // dar = calloc(sizeof(dot_block)*size);
    // memset(dar, 0, sizeof(dot_block)*size);
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
    dotn->next = NULL;
    dotn->xd = xd;
    dotn->yd = yd;

    return dotn;
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

bool find_nearest_dot(ray_trace1 *ray, int *hit_index, double *hit_xd, double *hit_yd)
{
    int radius = 0;
    int radius_width = 1;

    for(radius=0; radius<123; radius++)
    {
        if (find_nearest_dot_r(ray, radius, radius_width, hit_index, hit_xd, hit_yd))
            return true;
    }

    return false;
}

bool find_nearest_dot_r(ray_trace1 *ray, int radius, int radius_width, int *hit_index, double *hit_xd, double *hit_yd)
{
    int i;
    int blk_idx;
    int tmp_hit_idx;
    int tmp_hit_xd;
    int tmp_hit_yd;
    double min_distance;

    dot_block_index* dot_blk_idx;
    dot_blk_idx     = get_block_neighbor_w(ray->xr, ray->yr, radius, radius_width);
    min_distance    = 0;

    for(i=0; i<dot_blk_idx->count; i++)
    {

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
        printf("[%d][%d]: %d\n", i/BLOCK_Y_SIZE, i%BLOCK_Y_SIZE, cur_blk->count);
        dot_node *cur_node = cur_blk->head;
        while(cur_node != NULL)
        {
            printf("  (%.5f, %.5f)\n", cur_node->xd, cur_node->yd);
            cur_node = cur_node->next;
        }
        cur_blk++;
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

dot_block_index* merge_dot_block_index(const dot_block_index* src1, const dot_block_index* src2)
{
    dot_block_index* merged;
    int i;
    merged = alloc_dot_block_index(src1->count+src2->count);


    for(i=0; i<src1->count; i++)
    {
        merged->indices_x[merged->count] = src1->indices_x[i];
        merged->indices_y[merged->count] = src1->indices_y[i];
        merged->count++;
    }

    for(i=0; i<src2->count; i++)
    {
        merged->indices_x[merged->count] = src2->indices_x[i];
        merged->indices_y[merged->count] = src2->indices_y[i];
        merged->count++;
    }

    return merged;
}

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
    top_y       = yc-radius;
    bottom_y    = yc-radius;


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
        for(y=bottom_y; y<=top_y; y++)
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
        for(y=bottom_y; y<=top_y; y++)
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
    pInt(blk_idx->count);
    pInt(blk_idx->size);
    return blk_idx;
}

dot_block_index* alloc_dot_block_index(int size)
{
    dot_block_index *blk_idx;
    blk_idx = new dot_block_index;
    blk_idx->indices_x = new int[size];
    blk_idx->indices_y = new int[size];
    blk_idx->size = size;
    return blk_idx;
}

void free_dot_block_index(dot_block_index **blk_idx)
{
    delete (*blk_idx)->indices_x;
    delete (*blk_idx)->indices_y;
    delete *blk_idx;
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
        printf("%2d: = [%3d, %3d]\n", i, blk_idx->indices_x[i], blk_idx->indices_x[i]);
    }
}

void www(dot_block *dot_blk, dot_position *dpos)
{
    long int i;
    printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");
    pLongInt(dpos->ndot);
    pl();
    for(i=0; i<dpos->ndot; i++)
    {
        pLongInt(i);
        add_dot_node(dot_blk, i, dpos->xd[i], dpos->yd[i]);
    }
    pl();

}