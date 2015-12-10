
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
#include "ray_tracer.h"

#define MAX_OUTPUT_RAY 5

static unsigned long hit_local_error_count = 0;
static unsigned long iteration_count = 0;

void hanldOneRay(ray_trace1 *ray, dot_position *dpos, opt_record *opr, local_str *lstr)
{
	int i, j;
	int type;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];

	ray->ngaus 	= 1; 
	// ray->inty 	= 1.0; 
	ray->n1		= 1.0; 
	ray->n2 	= 1.0;
	ray->xr 	= 0.0; 
	ray->yr 	= 0.0; 
	ray->zr 	= 0; 
	ray->thar 	= 100; 
	ray->phir 	= 0.0;

	iteration_count++;
	/*
	find_str_hit_global(ray, dpos, opr, &type);
	if (!find_str_hit_local(ray, lstr))
	{
		hit_local_error_count++;
		return;
	}
	*/
	CalcMainReflectiveRay(ray, &src_ray[0]);
	CalcMainTransmittanceRay(ray, &src_ray[1]);

	for(i=0; i<2; i++)
	{
		output_ray[i][0].phir = 1;
		output_ray[i][1].phir = 2;
		output_ray[i][2].phir = 3;
		output_ray[i][3].phir = 4;
		output_ray[i][4].phir = 5;
		CalcGaussScatteredRay(&src_ray[i], (ray_trace1 *)&output_ray[i]);

		for(j=0; j<MAX_OUTPUT_RAY; j++)
		{
			if(output_ray[i][j].inty > IntensityThreshold)
			{
				hanldOneRay(&output_ray[i][j], dpos, opr, lstr);
			}
		}
	}
	// call_CalcGaussScatteredRay(&source_ray[0]);
}
