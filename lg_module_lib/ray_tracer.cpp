#include "ray_tracer.h"
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include "Module_IV.h"
#include "globalstr.h"
#include "microstr.h"
#include "var_def.h"
#include "opt_reflector.h"
#include "glist.h"
#include "dbg_log.h"

#define MAX_OUTPUT_RAY 5

static unsigned long hit_local_error_count = 0;
static unsigned long iteration_count = 0;
static char name[256];

void trace_ray_type3(ray_trace1 *ray, dot_position *dpos, opt_record_head *opr_head, local_str *lstr)
{
	int i, j;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];

	if (!find_str_hit_local(ray, lstr))
	{
		hit_local_error_count++;
		return;
	}

	CalcMainReflectiveRay(ray, &src_ray[0]);
	CalcMainTransmittanceRay(ray, &src_ray[1]);

	for(i=0; i<2; i++)
	{
		// output_ray[i][0].phir = 1;
		// output_ray[i][1].phir = 2;
		// output_ray[i][2].phir = 3;
		// output_ray[i][3].phir = 4;
		// output_ray[i][4].phir = 5;
		CalcGaussScatteredRay(&src_ray[i], (ray_trace1 *)&output_ray[i]);

		for(j=0; j<MAX_OUTPUT_RAY; j++)
		{
			if(output_ray[i][j].inty > IntensityThreshold)
			{
				trace_one_ray(&output_ray[i][j], dpos, opr_head, lstr);
			}
		}
	}
}

void trace_ray_type4(ray_trace1 *ray, dot_position *dpos, opt_record_head *opr_head, local_str *lstr)
{
	int i, j;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];

	CalcMainReflectiveRay(ray, &src_ray[0]);
	CalcMainTransmittanceRay(ray, &src_ray[1]);

	for(i=0; i<2; i++)
	{
		// output_ray[i][0].phir = 1;
		// output_ray[i][1].phir = 2;
		// output_ray[i][2].phir = 3;
		// output_ray[i][3].phir = 4;
		// output_ray[i][4].phir = 5;
		CalcGaussScatteredRay(&src_ray[i], (ray_trace1 *)&output_ray[i]);

		for(j=0; j<MAX_OUTPUT_RAY; j++)
		{
			if(output_ray[i][j].inty > IntensityThreshold)
			{
				trace_one_ray(&output_ray[i][j], dpos, opr_head, lstr);
			}
		}
	}
}


void trace_ray_type5(ray_trace1 *ray, dot_position *dpos, opt_record_head *opr_head, local_str *lstr)
{
	RayFromReflector(ray);
}


void trace_one_ray(ray_trace1 *ray, dot_position *dpos, opt_record_head *opr_head, local_str *lstr)
{
	int i, j;
	int type;
	bool result;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];
	opt_record *opr;


	// if(ray1->zr >=0.0 && ray1->thar<90.0)

	/*
	ray->ngaus 	= 1; 
	// ray->inty 	= 1.0; 
	ray->n1		= 1.0; 
	ray->n2 	= 1.0;
	ray->xr 	= 0.0; 
	ray->yr 	= 0.0; 
	ray->zr 	= 0.1; 
	ray->thar 	= 80; 
	ray->phir 	= 0.0;
	*/

	// dumpRay1(ray);

	iteration_count++;
	opr = new_opt_record();
	result = find_str_hit_global(ray, dpos, opr, &type);
	if (result)
	{
		switch(type)
		{
			case 1:
				// just skip it, and go to next round
				add_opt_record(opr_head, opr);
			case 2:
				break;
			case 3:
				trace_ray_type3(ray, dpos, opr_head, lstr);
				break;
			case 4:
				trace_ray_type4(ray, dpos, opr_head, lstr);
				break;
			case 5:
				trace_ray_type5(ray, dpos, opr_head, lstr);
				break;
		}
	}
}