
// for visual-studio header
#include "stdafx.h"

// for regular C header
#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "math.h"
#include "malloc.h"
#include "time.h"
#include "tchar.h"

// for private header
#include "var_def.h"
#include "mem_func.h"
#include "plt_figure.h"
#include "den_to_pos.h"
#include "debug_fun.h"

// function for memory arrangement
void allocmem_opm(opt_mat *opm);
void allocmem_ops(opt_source *ops);
void allocmem_rays(ray_traces *rays, ray_trace1 *ray1);
void allocmem_record(opt_record *opr);
void allocmem_local_str(local_str *lstr);
void allocmem_dot_density(dot_density *dden);
void allocmem_dot_position(dot_position *dpos);
void allocmem_gnuplot_2d(long int n, plot_2d *plt2d);
void allocmem_gnuplot_3d(long int n, plot_3d *plt3d);
void allocmem_gnuplot_3d_matrix(long int n1, long int n2, plot_3d_matrix *plt3dm);
void deallocmem_opm(opt_mat *opm);
void deallocmem_ops(opt_source *ops);
void deallocmem_rays(ray_traces *rays, ray_trace1 *ray1);
void deallocmem_record(opt_record *opr);
void deallocmem_local_str(local_str *lstr);
void deallocmem_dot_density(dot_density *dden);
void deallocmem_dot_position(dot_position *dpos);
void deallocmem_gnuplot_2d(plot_2d *plt2d);
void deallocmem_gnuplot_3d(plot_3d *plt3d);
void deallocmem_gnuplot_3d_matrix(plot_3d_matrix *plt3dm);

// function for moudles
void den2pos(dot_density *dden, dot_position *dpos);

// functino for figure
void gnuplot_2d(plot_2d *plt2d);
void gnuplot_3d(plot_3d *plt3d);
void gnuplot_3d_matrix(plot_3d_matrix *plt3dm);

// function for debug
void debug_den_to_pos(dot_density *dden, dot_position *dpos);

// main program
int _tmain(int argc, _TCHAR* argv[])
{
	// define variables
	opt_mat opm;
	opt_source ops;
	opt_record opr;
	ray_traces rays;
	ray_trace1 ray1;
	local_str lstr;
	dot_density dden;
	dot_position dpos;

	srand((unsigned)time(NULL));	// initiate rand seed 

	// allocate memory
	allocmem_opm(&opm);				// for optic materials
	allocmem_ops(&ops);				// for optic source
	allocmem_rays(&rays, &ray1);	// for samplings of ray tracing
	allocmem_record(&opr);			// for recording rays emitted from light guides
	allocmem_local_str(&lstr);		// for local microstructure
	allocmem_dot_density(&dden);	// for global dot density
	allocmem_dot_position(&dpos);	// for global dot potision

	// program body
	debug_den_to_pos(&dden, &dpos);		// debug for dot_density to dot_position
	// moduel 1...
	// moduel 2...
	// moduel 3...
	// moduel 4...
	// moduel 5...

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

