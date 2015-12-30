
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "debug_fun.h"
#include "var_def.h"
#include "mem_func.h"
#include "plt_figure.h"
#include "den_to_pos.h"
#include "dbg_log.h"
#include "data_file_util.h"

void debug_den_to_pos(dot_density *dden, dot_position *dpos)
{
	plot_3d_matrix plt3m;
	plot_2d plt2;
	long int i, j, xi, yi, nx, ny, ndot;
	double x, y, dx, dy, x0, y0, xrng, yrng;
	double *data;

	x0 = dden->x0;	y0 = dden->y0;
	xrng = dden->xrng;	yrng = dden->yrng;
	nx = dden->nx;	ny = dden->ny;
	dx = xrng/nx;	dy = yrng/ny;
	char fname[256];
	
	// simulate a gaussin dot_density for test
	/*
	for (i=0; i<nx*ny; i++)
	{
		yi = int(i%ny);	xi = int(1.0*(i-yi)/ny);
		x = x0+(xi+0.5)*dx; y = y0+(yi+0.5)*dy;
		dden->den[i] = 1.0+10.0*exp( -(pow(x-0.3*xrng,2.0)+pow(y-0.6*yrng,2.0))/pow(0.2*xrng,2.0) );
		dden->den[i] = dden->den[i]+5.0*exp( -(pow(x-0.7*xrng,2.0)+pow(y-0.3*yrng,2.0))/pow(0.1*xrng,2.0) );
		dden->den[i] = dden->den[i]+25.0*exp( -(pow(x-0.8*xrng,2.0)+pow(y-0.8*yrng,2.0))/pow(0.05*xrng,2.0) );
	}
	*/
	data = new double[nx*ny];
	getFileFullPath(fname, "dot_density.txt");
	load_matrix(fname, nx, ny, data);

	for(i=0; i<ny; i++)
	{
	  	for(j=0; j<nx; j++)
	  	{
			// dden->den[ny-1+j*ny-i] = data[i*ny+j];
			dden->den[ny-1+j*ny-i] = 0.001;
			// dden->den[ny-1+j*ny-i] = 0.05;
	  	}
	}
	// plot dot_density
	
	allocmem_gnuplot_3d_matrix(nx, ny, &plt3m);
	for (i=0; i<nx*ny; i++){ plt3m.v[i] = dden->den[i]; }
	gnuplot_3d_matrix(&plt3m);									// plot 
	
	deallocmem_gnuplot_3d_matrix(&plt3m);						// clear memory

	// call den2pos function to generate dot_position
	// den2pos(dden, dpos, hex_bl, hex_lng);
	
	printf("unsorted\n");
	den2pos_tetgen("GG", dden, dpos, hex_bl, hex_lng);
	
	// printf("sorted\n");
	// den2pos_tetgen_sorted(dden, dpos, hex_bl, hex_lng);
	
	ndot = dpos->ndot;
	
	// plot dot_position
	/*
	allocmem_gnuplot_2d(ndot, &plt2);
	for (i=0; i<ndot; i++){ plt2.x[i] = dpos->xd[i]; plt2.y[i] = dpos->yd[i]; }
	gnuplot_2d(&plt2);											// plot
	
	deallocmem_gnuplot_2d(&plt2);
	*/

	return;
}