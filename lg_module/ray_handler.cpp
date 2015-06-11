
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

// // function for memory arrangement
// void read_setup(char *fpname);

// // function for moudles
// void gen_source_ray(opt_source *ops, ray_traces *rays);
// void read_microstr(char *fname, local_str *lstr);
// void find_str_hit_global(ray_trace1 *ray1, dot_position *dpos, opt_record *opr);				// find the intersection of ray and global geometry
// void find_str_hit_local(ray_trace1 *ray1, local_str *lstr);										// find the intersection of ray and local microstructure
// void part_dots(dot_position *dpos);																// partition random dots into regular grids
// void den2pos(dot_density *dden, dot_position *dpos, bool hex_bl, double hex_lng);				// transfer dot_density to dot_position
// void den2pos_tetgen(dot_density *dden, dot_position *dpos, bool hex_bl, double hex_lng);		// transfer dot_density to dot_position by tetgen software

// // functino for figure
// void gnuplot_2d(plot_2d *plt2d);
// void gnuplot_3d(plot_3d *plt3d);
// void gnuplot_3d_matrix(plot_3d_matrix *plt3dm);
// void gnuplot_3d_vector(plot_3d_vector *plt3dv);

// // function for debug
// void debug_den_to_pos(dot_density *dden, dot_position *dpos);

// main program

#define MAX_OUTPUT_RAY 5

static unsigned long hit_local_error_count = 0;
static unsigned long iteration_count = 0;
static char name[256];

bool read_ray_source_file(const char* ray_source_file, ray_traces *rays, long int *nray)
{
	data_file_header dfh;

	if(!load_ray_source_file_header(ray_source_file, &dfh))
		return false;
	
	allocmem_ray_traces(dfh.count, rays);
	printf("%s handles ray source offset: %d, ray count: %d\n", name, dfh.offset, dfh.count);

	load_ray_source_file(ray_source_file, rays);
	*nray = dfh.count;
}

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
	find_str_hit_global(ray, dpos, opr, &type);
	if (!find_str_hit_local(ray, lstr))
	{
		hit_local_error_count++;
		return;
	}
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

int ray_handler(const char *ray_source_file)
{
	int i;
	// define variables
	opt_mat opm;					// for optic materials
	opt_source ops;					// for optic source
	opt_record opr;					// for recording rays emitted from light guides
	ray_traces rays;				// for samplings of ray tracing
	ray_trace1 ray1;
	local_str lstr;					// for local microstructure
	dot_density dden;				// for global dot density
	dot_position dpos;				// for global dot potision
	char fpname[256];					// for reading parameters
	struct ray_trace1 source_ray[2];
	data_file_header dfh;


	set_start_time("Total");
	srand((unsigned)time(NULL));	// initiate rand seed 

	// read in setup-parameters
	strcpy(fpname, "parameters.txt");

	read_setup(fpname);

	read_ray_source_file(ray_source_file, &rays, &n_ray);
	
	// allocate memory
	allocmem_opm(n_wl, n_mat, &opm);			
	allocmem_ops(n_x, n_y, n_z, n_tha, n_phi, xl_or, yl_or, zl_or, xl_rng, yl_rng, zl_rng, &ops);				
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, &opr);			
	allocmem_local_str(nx_str, ny_str, center_x, center_y, xstr_rng, ystr_rng, &lstr);		
	allocmem_dot_density(nx_den, ny_den, xden_or, yden_or, xden_rng, yden_rng, &dden);	
	allocmem_dot_position(n_dots, hex_bl, hex_lng, &dpos);	

	// program body
		

	// generate light source rays & initialize microstructure
	// gen_source_ray(&ops, &rays);
	
	read_microstr(str_file, &lstr);
	
	// moduel 2...
	//debug test
	debug_den_to_pos(&dden, &dpos);	// generate dot pattern for test;
	
	part_dots(&dpos);
	
	for(i=0; i<n_ray; i++)
	// for(i=0; i<1; i++)
	{

		ray1.ngaus 	= 1; 
		ray1.inty 	= 1.0; 
		ray1.n1 	= 1.0; 
		ray1.n2 	= 1.0;
		ray1.xr 	= rays.xr[i]; 
		
		ray1.yr 	= 0.0; 
		// ray1.yr 	= rays.yr[i]; 
		
		ray1.zr 	= rays.zr[i]; 
		
		ray1.thar 	= 100; 
		// ray1.thar 	= rays.thar[i]; 
		
		ray1.phir 	= 0;
		// ray1.phir 	= rays.phir[i];

		// ray1.ngaus = 1; ray1.inty = 1.0; ray1.n1 = 1.0; ray1.n2 = 1.0;
		// ray1.xr = 0.0; ray1.yr = 0.0; ray1.zr = 0; 
		// ray1.thar = 100; ray1.phir =0.0;

		hanldOneRay(&ray1, &dpos, &opr, &lstr);

		// find_str_hit_global(&ray1, &dpos, &opr);
		// if (!find_str_hit_local(&ray1, &lstr))
		// {
		// 	dumpRay1(&ray1);
		// 	continue;	
		// }
		// CalcMainReflectiveRay(&ray1, &source_ray[0]);
		// CalcMainTransmittanceRay(&ray1, &source_ray[1]);
		// call_CalcGaussScatteredRay(&source_ray[0]);
	}
	
	// for(i=0; i<1; i++)
	// {
	// 	// CalcMainReflectiveRay(&incident_ray[i], &source_ray[i]);
	// 	// CalcMainTransmittanceRay(&incident_ray[i], &source_ray[i+100]);

	// 	CalcMainReflectiveRay(&ray1, &source_ray[i]);
	// 	CalcMainTransmittanceRay(&ray1, &source_ray[i+100]);
	// }

	// for(i=0; i<200; i++)
	// {
	// 	call_CalcGaussScatteredRay(&source_ray[i]);
	// }

	// module 5...	

	//deallocate memory
	deallocmem_opm(&opm);
	deallocmem_ops(&ops);
	deallocmem_rays(&rays, &ray1);
	deallocmem_record(&opr);
	deallocmem_local_str(&lstr);
	deallocmem_dot_density(&dden);
	deallocmem_dot_position(&dpos);
	set_end_time("Total");
	print_all_execution_time();
	return 0;
}

int main(int argc, char** argv)
{
	char fname[256];

	pI(argc);
	if (argc != 3)
		return 1;
	
	strcpy(name, argv[1]);
	strcpy(fname, argv[2]);

	pStr(name);
	pStr(fname);
	ray_handler(fname);

	printf("%s done\n", fname);
	return 0;
}
