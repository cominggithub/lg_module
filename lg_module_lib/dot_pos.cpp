#include <stdio.h>
#include "dot_pos.h"
#include "den_to_pos.h"
#include "var_def.h"
#include "mem_func.h"
#include "dbg_log.h"


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
	pI(n_dots);
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