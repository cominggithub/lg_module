#ifndef _MEM_FUNC_H_
#define _MEM_FUNC_H_

#include "var_type.h"

void allocmem_opm(long int n_wl, long int n_mat, opt_mat *opm);
void allocmem_ops(long int n_x, long int n_y, long int n_z, long int n_tha, long int n_phi, double xl_or, \
				double yl_or, double zl_or, double xl_rng, double yl_rng, double zl_rng, opt_source *ops);
void allocmem_rays(long int n_ray, long int n_gaus, ray_traces *rays, ray_trace1 *ray1);
void allocmem_record(long int nx_rcd, long int ny_rcd, long int ntha_rcd, long int nphi_rcd, double xrcd_or,\
			double yrcd_or, double zrcd_or, double xrcd_rng, double yrcd_rng, opt_record *opr);
void allocmem_local_str(long int nx_str, long int ny_str, double center_x, double center_y, double xstr_rng, \
				double ystr_rng, local_str *lstr);
void allocmem_dot_density(long int nx_den, long int ny_den, double xden_or, double yden_or, double xden_rng, \
			double yden_rng, dot_density *dden);
void allocmem_dot_position(long int n_dots, char hex_bl, double hex_lng, dot_position *dpos);
void allocmem_gnuplot_2d(long int n, plot_2d *plt2d);
void allocmem_gnuplot_3d(long int n, plot_3d *plt3d);
void allocmem_gnuplot_3d_matrix(long int n1, long int n2, plot_3d_matrix *plt3dm);
void allocmem_gnuplot_3d_vector(long int n1, plot_3d_vector *plt3dv);
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

#endif