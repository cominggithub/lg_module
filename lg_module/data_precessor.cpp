// for regular C header
//#include "stdio.h"
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include "malloc.h"
#endif
#include "time.h"
// #include "tchar.h"

// for private header
#include "conf_util.h"
#include "var_def.h"			// define parameters and structe variables
#include "plt_figure.h"			// call gnuplot for ploting figure
#include "ini_var.h"			// initialize variables
#include "mem_func.h"			// allocate/deallocate memory 
#include "microstr.h"			// local microstructure profile
#include "gen_source_ray.h"		// generate light-source rays
#include "globalstr.h"
#include "den_to_pos.h"			// module for transferring dot_density to dot_position 
#include "debug_fun.h"			// just for debugging individual module.
#include "Module_IV.h"
#include "dbg_log.h"
#include "time_util.h"
#include "data_file_util.h"
#include "checker.h"



ray_traces *get_ray_traces_by_offset(ray_traces* rays, long int offset, long int count)
{
	long int i;
	ray_traces *tmpRays;

	if (rays->nray < offset+count)
		return NULL;
	
	tmpRays = new ray_traces;
	allocmem_ray_traces(count, tmpRays);

	for(i=0; i<count; i++)
	{
		tmpRays->xr[i] 		= rays->xr[offset+i];
		tmpRays->yr[i] 		= rays->yr[offset+i];
		tmpRays->zr[i] 		= rays->zr[offset+i];
		tmpRays->thar[i] 	= rays->thar[offset+i];
		tmpRays->phir[i] 	= rays->phir[offset+i];
	}
	
	tmpRays->nray = count;
	return tmpRays;
}

void split_ray(const char* prefix, ray_traces *rays, int count)
{
	int i;
	long int offset;
	int perBlockCount;
	int blockCount;

	RETURN_ON_NULL(rays);
	RETURN_ON_ZERO(rays->nray);
	RETURN_ON_ZERO(count);

	offset 			= 0;
	perBlockCount 	= rays->nray/count;
	for(i=0; i<count; i++)
	{
		ray_traces *tmpRays;
		data_file_header dfh;
		char fname[256];
		// last block
		if (i == count-1)
		{
			blockCount = rays->nray - offset;
		}
		else
		{
			blockCount = perBlockCount;
		}

		RETURN_ON_ZERO(blockCount);
		tmpRays = get_ray_traces_by_offset(rays, offset, blockCount);
		
		sprintf(fname, "%s_ray_source_%d.dat", prefix, i);
		sprintf(dfh.prefix, "%s", fname);
		printf("%s\n", fname);
		dfh.count 	= blockCount;
		dfh.offset 	= offset;
		save_ray_source_file(fname, &dfh, tmpRays);

		offset += perBlockCount;
		deallocmem_ray_traces(tmpRays);
	}

}

int main(int argc, char** argv)
{

	int i;
	int count;
	char prefix[256];
	
	// define variables
	opt_source ops;					// for optic source
	ray_traces rays;				// for samplings of ray tracing
	char fpname[256];					// for reading parameters

	if (argc != 3)
	{
		return 1;	
	}
	
	count = atoi(argv[2]);
	strcpy(prefix, argv[1]);

	if (strlen(prefix) == 0)
	{
		strcpy(prefix, "ms");
	} 
	if (count == 0)
	{
		count = 1;
	}
	
	srand((unsigned)time(NULL));	// initiate rand seed 

	// read in setup-parameters
	strcpy(fpname, "parameters.txt");

	read_setup(fpname);


	// allocate memory
	allocmem_ops(n_x, n_y, n_z, n_tha, n_phi, xl_or, yl_or, zl_or, xl_rng, yl_rng, zl_rng, &ops);				
	allocmem_ray_traces(n_ray, &rays);	
	pI(n_ray);
	pI(rays.nray);
	gen_source_ray(&ops, &rays);
	pI(rays.nray);
	
	split_ray(prefix, &rays, count);


	deallocmem_ops(&ops);
	deallocmem_ray_traces(&rays);

	return 0;
}
