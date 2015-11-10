#include <stdlib.h>
#include <stdio.h>

#include "mem_func.h"
#include "var_def.h"
#include "dbg_log.h"

void allocmem_opm(long int n_wl, long int n_mat, opt_mat *opm)
{
	// allocate memory for materials
	opm->wln = n_wl;
	opm->matn = n_mat;
	opm->refx = new double[opm->wln*opm->matn];
	if( opm->refx == nullptr ) { printf("allocmem_opm: materials error\n"); exit(0); }
	return;
}

void allocmem_ops(long int n_x, long int n_y, long int n_z, long int n_tha, long int n_phi, double xl_or,\
				double yl_or, double zl_or, double xl_rng, double yl_rng, double zl_rng, opt_source *ops)
{
	// allocate memory for light source
	ops->nx = n_x;
	ops->ny = n_y;
	ops->nz = n_z;
	ops->ntha = n_tha;
	ops->nphi = n_phi;
	ops->x0 = xl_or;
	ops->y0 = yl_or;
	ops->z0 = zl_or;
	ops->xrng = xl_rng;
	ops->yrng = yl_rng;
	ops->zrng = zl_rng;
	ops->inty = new double[ops->nx*ops->ny*ops->nz*ops->ntha*ops->nphi];
	if( ops->inty == nullptr ) { printf("allocmem_ops: light intensity error\n"); exit(0); }
	ops->accinty = new double[ops->nx*ops->ny*ops->nz*ops->ntha*ops->nphi];
	if( ops->accinty == nullptr ) { printf("allocmem_ops: accumulation intensity error\n"); exit(0); }
	return;
}

void allocmem_record(long int nx_rcd, long int ny_rcd, long int ntha_rcd, long int nphi_rcd,double xrcd_or,\
		double yrcd_or, double zrcd_or, double xrcd_rng, double yrcd_rng, opt_record *opr)
{
	long int i;
	// allocate memory for recording emitted light from light-guide
	opr->nx = nx_rcd;
	opr->ny = ny_rcd;
	opr->ntha = ntha_rcd;
	opr->nphi = nphi_rcd;
	opr->x0 = xrcd_or;
	opr->y0 = yrcd_or;
	opr->z0 = zrcd_or;
	opr->xrng = xrcd_rng;
	opr->yrng = yrcd_rng;
	opr->r_index = 0;
	opr->r_inty = 0;
	opr->inty = new double[opr->nx*opr->ny*opr->ntha*opr->nphi];
	
	if( opr->inty == nullptr ) { printf("allocmem_record: light recording error\n"); exit(0); }
	for(i=0; i<opr->nx*opr->ny*opr->ntha*opr->nphi; i++ ){ opr->inty[i]=666.0; }
	return;
}

void allocmem_ray_traces(long int n_ray, ray_traces *rays)
{
	
	rays->nray = n_ray;
	rays->xr = new double[rays->nray];
	if( rays->xr == nullptr ) { printf("allocmem_rays: x in rays error\n"); exit(0); }
	rays->yr = new double[rays->nray];
	if( rays->yr == nullptr ) { printf("allocmem_rays: y in rays error\n"); exit(0); }
	rays->zr = new double[rays->nray];
	if( rays->zr == nullptr ) { printf("allocmem_rays: z in rays error\n"); exit(0); }
	rays->thar = new double[rays->nray];
	if( rays->thar == nullptr ) { printf("allocmem_rays: theta in rays error\n"); exit(0); }
	rays->phir = new double[rays->nray];
	if( rays->phir == nullptr ) { printf("allocmem_rays: phi in rays error\n"); exit(0); }
	rays->inty = new double[rays->nray];
	if( rays->inty == nullptr ) { printf("allocmem_rays: inty in rays error\n"); exit(0); }
	return;
}

void allocmem_rays(long int n_ray, long int n_gaus, ray_traces *rays, ray_trace1 *ray1)
{
	// allocate memory for sampling of rays
	ray1->ngaus = n_gaus;
	
	rays->nray = n_ray;
	rays->xr = new double[rays->nray];
	if( rays->xr == nullptr ) { printf("allocmem_rays: x in rays error\n"); exit(0); }
	rays->yr = new double[rays->nray];
	if( rays->yr == nullptr ) { printf("allocmem_rays: y in rays error\n"); exit(0); }
	rays->zr = new double[rays->nray];
	if( rays->zr == nullptr ) { printf("allocmem_rays: z in rays error\n"); exit(0); }
	rays->thar = new double[rays->nray];
	if( rays->thar == nullptr ) { printf("allocmem_rays: theta in rays error\n"); exit(0); }
	rays->phir = new double[rays->nray];
	if( rays->phir == nullptr ) { printf("allocmem_rays: phi in rays error\n"); exit(0); }
	rays->inty = new double[rays->nray];
	if( rays->inty == nullptr ) { printf("allocmem_rays: inty in rays error\n"); exit(0); }
	return;
}

void allocmem_local_str(long int nx_str, long int ny_str, double center_x, double center_y, double xstr_rng,\
						double ystr_rng, local_str *lstr)
{
	// allocate memory for local microstructure
	lstr->nx = nx_str;
	lstr->ny = ny_str;
	lstr->x0 = center_x;
	lstr->y0 = center_y;
	lstr->xrng = xstr_rng;
	lstr->yrng = ystr_rng;
	lstr->thz = new double[lstr->nx*lstr->ny];
	if( lstr->thz == nullptr ) { printf("allocmem_local_str: z-thickness error\n"); exit(0); }
	return;
}

void allocmem_dot_density(long int nx_den, long int ny_den, double xden_or, double yden_or, double xden_rng, \
			double yden_rng, dot_density *dden)
{
	// allocate memory for global dot density
	dden->nx = nx_den;
	dden->ny = ny_den;
	dden->scale = den_2_pos_scale;
	dden->x0 = xden_or;
	dden->y0 = yden_or;
	dden->xrng = xden_rng;
	dden->yrng = yden_rng;
	dden->den = new double[dden->nx*dden->ny];
	if( dden->den == nullptr ) { printf("allocmem_dot_density: density error\n"); exit(0); }
	return;
}

void allocmem_dot_position(long int n_dots, char hex_bl, double hex_lng, dot_position *dpos)
{
	// allocate memory for global dot position
	dpos->ndot = n_dots;
	dpos->partnx = partn_x;
	dpos->partny = partn_y;
	dpos->xd = new double[dpos->ndot];
	if( dpos->xd == nullptr ) { printf("allocmem_dot_position: dot potision error\n"); exit(0); }
	dpos->yd = new double[dpos->ndot];
	if( dpos->xd == nullptr ) { printf("allocmem_dot_position: dot potision error\n"); exit(0); }
	dpos->partaccni = new long int[dpos->partnx*dpos->partny];
	if( dpos->partaccni == nullptr ) { printf("allocmem_dot_position: accumulate-dot-number matrix error\n"); exit(0); }
	dpos->partindx = new long int[dpos->ndot];
	if( dpos->partindx == nullptr ) { printf("allocmem_dot_position: dot-index matrix error\n"); exit(0); }
	return;
}

void allocmem_gnuplot_2d(long int n, plot_2d *plt2d)
{
	// allocate memory for 2d figure
	plt2d->npnt = n;
	plt2d->x = new double[plt2d->npnt];
	if( plt2d->x == nullptr ) { printf("allocmem_gnuplot_2d: position error\n"); exit(0); }
	plt2d->y = new double[plt2d->npnt];
	if( plt2d->y == nullptr ) { printf("allocmem_gnuplot_2d: position error\n"); exit(0); }
	return;
}

void allocmem_gnuplot_3d(long int n, plot_3d *plt3d)
{
	// allocate memory for 3d figure
	plt3d->npnt = n;
	plt3d->x = new double[plt3d->npnt];
	if( plt3d->x == nullptr ) { printf("allocmem_gnuplot_3d: position error\n"); exit(0); }
	plt3d->y = new double[plt3d->npnt];
	if( plt3d->y == nullptr ) { printf("allocmem_gnuplot_3d: position error\n"); exit(0); }
	plt3d->z = new double[plt3d->npnt];
	if( plt3d->z == nullptr ) { printf("allocmem_gnuplot_3d: position error\n"); exit(0); }
	return;
}

void allocmem_gnuplot_3d_matrix(long int n1, long int n2, plot_3d_matrix *plt3dm)
{
	// allocate memory for 3d-matrix figure
	plt3dm->nx = n1;
	plt3dm->ny = n2;
	plt3dm->v = new double[plt3dm->nx*plt3dm->ny];
	if( plt3dm->v == nullptr ) { printf("allocmem_gnuplot_3d_matrix: position error\n"); exit(0); }
	return;
}

void allocmem_gnuplot_3d_vector(long int n1, plot_3d_vector *plt3dv)
{
	// allocate memory for 3d-matrix figure
	plt3dv->npnt = n1;
	plt3dv->x = new double[plt3dv->npnt];
	if( plt3dv->x == nullptr ) { printf("allocmem_gnuplot_3d_vector: x error\n"); exit(0); }
	plt3dv->y = new double[plt3dv->npnt];
	if( plt3dv->y == nullptr ) { printf("allocmem_gnuplot_3d_vector: y error\n"); exit(0); }
	plt3dv->z = new double[plt3dv->npnt];
	if( plt3dv->z == nullptr ) { printf("allocmem_gnuplot_3d_vector: z error\n"); exit(0); }
	plt3dv->vx = new double[plt3dv->npnt];
	if( plt3dv->vx == nullptr ) { printf("allocmem_gnuplot_3d_vector: vx error\n"); exit(0); }
	plt3dv->vy = new double[plt3dv->npnt];
	if( plt3dv->vy == nullptr ) { printf("allocmem_gnuplot_3d_vector: vy error\n"); exit(0); }
	plt3dv->vz = new double[plt3dv->npnt];
	if( plt3dv->vz == nullptr ) { printf("allocmem_gnuplot_3d_vector: vz error\n"); exit(0); }
	return;
}

void deallocmem_opm(opt_mat *op)
{
	// deallocate memory for materials
	delete [] op->refx;
	return;
}

void deallocmem_ops(opt_source *ops)
{
	// deallocate memory for light source
	delete [] ops->inty;
	delete [] ops->accinty;
	return;
}

void deallocmem_ray_traces(ray_traces *rays)
{
	delete [] rays->xr;
	delete [] rays->yr;
	delete [] rays->zr;
	delete [] rays->thar;
	delete [] rays->phir;
	return;
}

void deallocmem_rays(ray_traces *rays, ray_trace1 *ray1)
{
	// deallocate memory for sampling of rays
	delete [] rays->xr;
	delete [] rays->yr;
	delete [] rays->zr;
	delete [] rays->thar;
	delete [] rays->phir;
	return;
}

void deallocmem_record(opt_record *opr)
{
	// deallocate memory for recording emitted light from light-guide
	// delete [] opr->inty;
	return;
}

void deallocmem_local_str(local_str *lstr)
{
	// deallocate memory for local microstructure
	delete [] lstr->thz;
	return;
}

void deallocmem_dot_density(dot_density *dden)
{
	// deallocate memory for global dot density
	delete [] dden->den;
	return;
}

void deallocmem_dot_position(dot_position *dpos)
{
	// deallocate memory for global dot position
	delete [] dpos->xd;
	delete [] dpos->yd;
	return;
}

void deallocmem_gnuplot_2d(plot_2d *plt2d)
{
	// deallocate memory for 2d figure
	delete [] plt2d->x;
	delete [] plt2d->y;
	return;
}

void deallocmem_gnuplot_3d(plot_3d *plt3d)
{
	// deallocate memory for 3d figure
	delete [] plt3d->x;
	delete [] plt3d->y;
	delete [] plt3d->z;
	return;
}

void deallocmem_gnuplot_3d_matrix(plot_3d_matrix *plt3dm)
{
	// deallocate memory for 3d-matrix figure
	delete [] plt3dm->v;
	return;
}