
// for regular C header
//#include "stdio.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "malloc.h"
#include "time.h"
#include "tchar.h"

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
int _tmain(int argc, _TCHAR* argv[])
{
	// define variables
	opt_mat opm;					// for optic materials
	opt_source ops;					// for optic source
	opt_record opr;					// for recording rays emitted from light guides
	ray_traces rays;				// for samplings of ray tracing
	ray_trace1 ray1;
	local_str lstr;					// for local microstructure
	dot_density dden;				// for global dot density
	dot_position dpos;				// for global dot potision
	char *fpname;					// for reading parameters

	srand((unsigned)time(NULL));	// initiate rand seed 

	// read in setup-parameters
	fpname="parameters.txt";
	read_setup(fpname);

	// allocate memory
	allocmem_opm(n_wl, n_mat, &opm);			
	allocmem_ops(n_x, n_y, n_z, n_tha, n_phi, xl_or, yl_or, zl_or, xl_rng, yl_rng, zl_rng, &ops);				
	allocmem_rays(n_ray, n_gaus, &rays, &ray1);	
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, &opr);			
	allocmem_local_str(nx_str, ny_str, center_x, center_y, xstr_rng, ystr_rng, &lstr);		
	allocmem_dot_density(nx_den, ny_den, xden_or, yden_or, xden_rng, yden_rng, &dden);	
	allocmem_dot_position(n_dots, hex_bl, hex_lng, &dpos);	

	// program body
		


	// generate light source rays & initialize microstructure
	gen_source_ray(&ops, &rays);
	read_microstr(str_file, &lstr);

	// moduel 2...
	//debug test
	debug_den_to_pos(&dden, &dpos);	// generate dot pattern for test;
	part_dots(&dpos);
	find_str_hit_global(&ray1, &dpos, &opr);

	// moduel 3...
	// debug test
	ray1.ngaus = 1; ray1.inty = 1.0; ray1.n1 = 1.0; ray1.n2 = 1.0;
	ray1.xr = 0.0; ray1.yr = 0.0; ray1.zr = 0; 
	ray1.thar = 100; ray1.phir =0.0;
	printf("\n incident pos in microstructure box: %lf\t%lf\t%lf\n",ray1.xr, ray1.yr, ray1.zr);
	find_str_hit_local(&ray1, &lstr);	
	printf(" hit pos in microstructure box: %lf\t%lf\t%lf\n",ray1.xr, ray1.yr, ray1.zr);

	// moduel 4...

	// module 5...	


	//deallocate memory
	deallocmem_opm(&opm);
	deallocmem_ops(&ops);
	deallocmem_rays(&rays, &ray1);
	deallocmem_record(&opr);
	deallocmem_local_str(&lstr);
	deallocmem_dot_density(&dden);
	deallocmem_dot_position(&dpos);
	return 0;
}

