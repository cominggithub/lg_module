void debug_den_to_pos(dot_density *dden, dot_position *dpos)
{
	plot_3d_matrix plt3m;
	plot_2d plt2;
	long int i, xi, yi, nx, ny, ndot;
	double x, y, dx, dy, x0, y0, xrng, yrng;

	x0 = dden->x0;	y0 = dden->y0;
	xrng = dden->xrng;	yrng = dden->yrng;
	nx = dden->nx;	ny = dden->ny;
	dx = xrng/nx;	dy = yrng/ny;
	ndot = dpos->ndot;
	
	// simulate a gaussin dot_density for test
	for (i=0; i<nx*ny; i++)
	{
		yi = int(i%ny);	xi = int(1.0*(i-yi)/ny);
		x = x0+(xi+0.5)*dx; y = y0+(yi+0.5)*dy;
		dden->den[i] = 1.0+10.0*exp( -(pow(x-0.3*xrng,2.0)+pow(y-0.6*yrng,2.0))/pow(0.2*xrng,2.0) );
		dden->den[i] = dden->den[i]+5.0*exp( -(pow(x-0.7*xrng,2.0)+pow(y-0.3*yrng,2.0))/pow(0.1*xrng,2.0) );
		dden->den[i] = dden->den[i]+25.0*exp( -(pow(x-0.8*xrng,2.0)+pow(y-0.8*yrng,2.0))/pow(0.05*xrng,2.0) );
	}
	
	// plot dot_density
	plt3m.v = new double[nx*ny]; plt3m.nx = nx;	plt3m.ny = ny;	// set parameter
	if(plt3m.v == nullptr) {printf("debug_den_to_pos: plt3mv initiation error!"); exit(0);}
	for (i=0; i<nx*ny; i++){ plt3m.v[i] = dden->den[i]; }
	gnuplot_3d_matrix(&plt3m);									// plot 
	delete [] plt3m.v;											// clear memory


	// call den2pos function to generate dot_position
	den2pos_tetgen(dden, dpos);
	
	ndot = dpos->ndot;

	// plot dot_position
	plt2.npnt = ndot;
	plt2.x = new double[ndot];
	if(plt2.x ==nullptr) { printf("debug_den_to_pos: plt2.x initiation error!"); exit(0); }
	plt2.y = new double[ndot];
	if(plt2.y ==nullptr) { printf("debug_den_to_pos: plt2.y initiation error!"); exit(0); }
	for (i=0; i<ndot; i++){ plt2.x[i] = dpos->xd[i]; plt2.y[i] = dpos->yd[i]; }
	gnuplot_2d(&plt2);											// plot
	delete [] plt2.x;											// clear memory
	delete [] plt2.y;

	return;
}