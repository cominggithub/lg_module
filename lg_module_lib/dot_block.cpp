#include "dot_block.h"
#include "var_def.h"
#include "math.h"
#include "stdio.h"


dot_node* new_dot_node(long int dpos_index, double xd, double yd)
{
	dot_node* dotn = new dot_node;
	dotn->next = NULL;
	dotn->xd = xd;
	dotn->yd = yd;

	return dotn;
}

void add_dot_node(dot_block *dot_blk, long int dops_index, double xd, double yd)
{
	dot_node *dotn = new_dot_node(dops_index, xd, yd);
	dot_block *cur_blk;
	
	int size = BLOCK_X_SIZE*BLOCK_Y_SIZE;

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

	dump_dot_blk(dot_blk);
	// dump_dot_blK_3dfile("block3d.txt", dot_blk);
}
