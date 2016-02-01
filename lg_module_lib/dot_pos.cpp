#include <stdio.h>
#include "dot_pos.h"
#include "den_to_pos.h"
#include "var_def.h"
#include "mem_func.h"
#include "dbg_log.h"


void dpos_swap_xyd(dot_position *dpos, int pos1, int pos2)
{
	double v;

	v = dpos->xd[pos1];
	dpos->xd[pos1] = dpos->xd[pos2];
	dpos->xd[pos2] = v;

	v = dpos->yd[pos1];
	dpos->yd[pos1] = dpos->yd[pos2];
	dpos->yd[pos2] = v;

}

void dpos_swap_xyd_partindx(dot_position *dpos, int pos1, int pos2)
{
	double v;
	long int indbuf;
	indbuf = dpos->partindx[pos1];
	dpos->partindx[pos1] = dpos->partindx[pos2];
	dpos->partindx[pos2] = indbuf;

	v = dpos->xd[pos1];
	dpos->xd[pos1] = dpos->xd[pos2];
	dpos->xd[pos2] = v;

	v = dpos->yd[pos1];
	dpos->yd[pos1] = dpos->yd[pos2];
	dpos->yd[pos2] = v;
}

int dpos_quick_partition_by_xd(dot_position *dpos, int begin, int end)
{
	int s_index = begin;
	// int cur_index = begin;
	int i;
	double v;
	double pivot = dpos->xd[end];

	// printf("partion: %d - %d, pivot: %f\n", begin, end, pivot);
	for(i=begin; i<end; i++)
	{
		if(dpos->xd[i] < pivot)
		{
			// v = array[s_index];
			// array[s_index] = array[i];
			// array[i] = v;
			dpos_swap_xyd(dpos, s_index, i);
			s_index++;
			
		}
	}
	// swap pivot
	// v = array[s_index];
	// array[s_index] = array[end];
	// array[end] = v;

	dpos_swap_xyd(dpos, s_index, end);
	
	return s_index;

}

int dpos_quick_partition_by_partindx(dot_position *dpos, int begin, int end)
{
	int s_index = begin;
	// int cur_index = begin;
	int i;
	long int pivot = dpos->partindx[end];

	// printf("partion: %d - %d, pivot: %f\n", begin, end, pivot);
	for(i=begin; i<end; i++)
	{
		if(dpos->partindx[i] < pivot)
		{
			// v = array[s_index];
			// array[s_index] = array[i];
			// array[i] = v;
			dpos_swap_xyd_partindx(dpos, s_index, i);
			s_index++;
			
		}
	}
	// swap pivot
	// v = array[s_index];
	// array[s_index] = array[end];
	// array[end] = v;

	dpos_swap_xyd_partindx(dpos, s_index, end);
	
	return s_index;

}


void dpos_quick_sort_by_xd(dot_position *dpos, int begin, int end)
{
	int p_index;
	if (begin == end)
		return;
	p_index = dpos_quick_partition_by_xd(dpos, begin, end);
	// printf("p_index: %d\n", p_index);
	if (p_index > begin)
	{
		dpos_quick_sort_by_xd(dpos, begin, p_index-1);
	}
	if (p_index < end)
	{
		dpos_quick_sort_by_xd(dpos, p_index+1, end);
	}
}

void dpos_quick_sort_by_partindx(dot_position *dpos, int begin, int end)
{
	int p_index;
	if (begin == end)
		return;
	p_index = dpos_quick_partition_by_partindx(dpos, begin, end);
	// printf("p_index: %d\n", p_index);
	if (p_index > begin)
	{
		dpos_quick_sort_by_partindx(dpos, begin, p_index-1);
	}
	if (p_index < end)
	{
		dpos_quick_sort_by_partindx(dpos, p_index+1, end);
	}
}

bool load_dpos(const char* fname, dot_position *dpos, char hexbl, double hexlng)
{
	long int i;
	long int ndot;
	long int n1, n2, n3, n4;
	double c1, c2, c3;
	double x0;
	double y0;
	FILE *node;


	// ops->x0 = xl_or;
	// ops->y0 = yl_or;

	x0 = xl_or;
	y0 = yl_or;

	node = fopen(fname, "r");

	if (node == NULL)
	{
		fprintf(stderr, "cannot find tetgen generated file: %s\n", fname);
		return false;
	}


	fscanf(node,"%ld %ld %ld %ld\n", &n1, &n2, &n3, &n4);
	ndot = 0;
	for(i=1; i<=n1; i++)
	{
		fscanf(node,"%ld %lf %lf %lf\n", &n2, &c1, &c2, &c3);
		if( c3 ==0.0 ){ ndot = ndot+1; }
	}

	fclose(node);


	// update dpos
	deallocmem_dot_position(dpos);				// !!! need to check whether dpos is allocated or not
	n_dots = ndot;
	allocmem_dot_position(n_dots, hex_bl, hex_lng, dpos);

	ndot = 0;
	node = fopen(fname, "r");
	fscanf(node,"%ld %ld %ld %ld\n", &n1, &n2, &n3, &n4);
	printf("\n");
	for(i=1; i<=n1; i++)
	{
		printf("\r%d/%d", i, n1);
		fscanf(node,"%ld %lf %lf %lf\n", &n2, &c1, &c2, &c3);
		if( c3 == 0.0 )
		{
			dpos->xd[ndot] = c1+x0;
			dpos->yd[ndot] = c2+y0;
			ndot = ndot+1;
		}
	}
	fclose(node);

	if (hexbl=='T' || hexbl=='t') hex_fit(dpos, hexlng);	// put dots onto hexagon lattice
	return true;

}