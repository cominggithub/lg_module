void den2pos(dot_density *dden, dot_position *dpos)
{
	long int i, j, k, n, nx, ny, ndot, xline, yline;
	long int ibuf1, ibuf2;
	long int begi, midi, endi;
	double dx, dy, x0, y0, xrng, yrng, den_tot, xsol, ysol, ratio;
	double rndx, rndy;
	double *nden, *accdenx, *accdeny;
	bool solving;

	// read data from dot_density setup
	x0 = dden->x0;	y0 = dden->y0;
	xrng = dden->xrng;	yrng = dden->yrng;
	nx = dden->nx;	ny = dden->ny;
	dx = xrng/nx;	dy = yrng/ny;
	// read data from dot_position	setup
	ndot = dpos->ndot; 
	xline = dpos->xline;
	yline = dpos->yline;

	// initiate memroy
	nden = new double[nx*ny];
	if(nden == nullptr) {printf("den2pos: nden initiation error!"); exit(0);}
	accdenx = new double[nx+1];
	if(accdenx == nullptr) {printf("den2pos: accdenx initiation error!"); exit(0);}
	accdeny = new double[ny+1];
	if(accdeny == nullptr) {printf("den2pos: accdeny initiation error!"); exit(0);}
	accdenx[0] = 0.0;	accdeny[0] = 0.0;

	// calculate and normalize the cumulative distribution function
	den_tot = 0.0;
	for(i=0; i<nx*ny; i++){ den_tot = den_tot+dden->den[i]; }
	for(i=0; i<nx*ny; i++){ nden[i] = dden->den[i]/den_tot; }
	for(i=0; i<nx ; i++)
	{
		if(i==0) {accdenx[i+1] = 0.0;} 
		else {accdenx[i+1] = accdenx[i];}
		for(j=0; j<ny; j++){ k = i*ny+j; accdenx[i+1] = accdenx[i+1]+nden[k]; }
	}

	// generate dot_position
	printf("\n");
	for(n=0; n<ndot; n++)
	{
		rndx = 1.0*rand()/RAND_MAX;
		rndy = 1.0*rand()/RAND_MAX;
		// solve along x direction
		begi = 0;	endi = nx;
		midi = int(0.5*(endi+begi));
		solving = true;
		while (solving)
		{
			if ( rndx>=accdenx[begi] && rndx<accdenx[midi]){ begi = begi; endi = midi; midi = int(0.5*(endi+begi)); }
			else { begi = midi; endi = endi; midi = int(0.5*(endi+begi)); };
			if( (endi-begi)<2 ){ solving = false; }
			if( endi == begi ){ printf("den2pos: x-solving error!"); exit(0); }
		}
		ratio = (rndx-accdenx[begi])/(accdenx[endi]-accdenx[begi]);
		xsol = dx*begi+dx*ratio;
		// build accdeny
		accdeny[0] = 0.0;
		for(j = 0; j<ny; j++)
		{ 
			if(begi ==0){ accdeny[j+1]= nden[j]; }
			else
			{
				ibuf1 = (begi-1)*ny+j;	ibuf2 = (endi-1)*ny+j;
				accdeny[j+1]= nden[ibuf1]+ratio*(nden[ibuf2]-nden[ibuf1]);
			}
		}
		for(j = 0; j<ny; j++) { accdeny[j+1] = accdeny[j+1]+accdeny[j]; }
		for(j = 0; j<ny; j++) { accdeny[j+1] = accdeny[j+1]/accdeny[ny]; }
		// solve along y direction
		begi = 0;	endi = ny;
		midi = int(0.5*(endi+begi));
		solving = true;
		while (solving)
		{
			if ( rndy>=accdeny[begi] && rndy<accdeny[midi]){ begi = begi; endi = midi; midi = int(0.5*(endi+begi)); }
			else { begi = midi; endi = endi; midi = int(0.5*(endi+begi)); };
			if( (endi-begi)<2 ){ solving = false; }
			if( endi == begi ){ printf("den2pos: y-solving error!"); exit(0); }
		}
		ratio = (rndy-accdeny[begi])/(accdeny[endi]-accdeny[begi]);
		ysol = dy*begi+dy*ratio;
		// record position
		dpos->xd[n] = xsol;
		dpos->yd[n] = ysol;
		printf("transfer density to position on %9d of %9d\r", n, ndot);
	}
	printf("\n");
	
	// free memory
	delete [] nden;
	delete [] accdenx;
	delete [] accdeny;

}