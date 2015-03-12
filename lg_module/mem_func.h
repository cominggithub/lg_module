
void allocmem_opm(opt_mat *opm)
{
	// allocate memory for materials
	opm->wln = n_wl;
	opm->matn = n_mat;
	opm->refx = new double[opm->wln*opm->matn];
	if( opm->refx == nullptr ) { printf("allocmem_opm: materials error\n"); exit(0); }
	return;
}

void allocmem_ops(opt_source *ops)
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

void allocmem_record(opt_record *opr)
{
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
	opr->zrng = zrcd_rng;
	opr->inty = new double[opr->nx*opr->ny*opr->ntha*opr->nphi];
	if( opr->inty == nullptr ) { printf("allocmem_record: light recording error\n"); exit(0); }
	return;
}

void allocmem_rays(ray_traces *rays, ray_trace1 *ray1)
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
	return;
}

void allocmem_local_str(local_str *lstr)
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

void allocmem_dot_density(dot_density *dden)
{
	// allocate memory for global dot density
	dden->nx = nx_den;
	dden->ny = ny_den;
	dden->x0 = xden_or;
	dden->y0 = yden_or;
	dden->xrng = xden_rng;
	dden->yrng = yden_rng;
	dden->den = new double[dden->nx*dden->ny];
	if( dden->den == nullptr ) { printf("allocmem_dot_density: density error\n"); exit(0); }
	return;
}

void allocmem_dot_position(dot_position *dpos)
{
	// allocate memory for global dot position
	dpos->ndot = n_dots;
	dpos->xline = n_xline;
	dpos->yline = n_yline;
	dpos->xd = new double[dpos->ndot];
	if( dpos->xd == nullptr ) { printf("allocmem_dot_position: dot potision error\n"); exit(0); }
	dpos->yd = new double[dpos->ndot];
	if( dpos->xd == nullptr ) { printf("allocmem_dot_position: dot potision error\n"); exit(0); }
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
	delete [] opr->inty;
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