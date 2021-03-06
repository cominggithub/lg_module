// local function
void hex_fit(dot_position *dpos, double hexlng);				// put dots onto hexagon lattice

void den2pos_tetgen(dot_density *dden, dot_position *dpos, char hexbl, double hexlng) // preferred! due to its efficiency and uniformity
{
	long int i, nx, ny, ndot, xi, yi;
	long int n1, n2, n3, n4;
	double c1, c2, c3;
	double dx, dy, x0, y0, xrng, yrng, den_tot, scl_factor, zbuf;
	double *nden;
	FILE *smesh, *mtr, *node;

	// read parameters from dot_density setup
	x0 = dden->x0;	y0 = dden->y0;
	xrng = dden->xrng;	yrng = dden->yrng;
	nx = dden->nx;	ny = dden->ny;
	dx = xrng/(nx-1);	dy = yrng/(ny-1);			// coordinates for vertes; not for grid-face now
	// read parameters from dot_position	setup
	ndot = dpos->ndot; 

	// initiate memroy
	nden = new double[nx*ny];
	if(nden == nullptr) {printf("den2pos_tetgen: nden initiation error!"); exit(0);}

	// normalize the cumulative distribution function for dot_density
	den_tot = 0.0;
	for(i=0; i<nx*ny; i++){ den_tot = den_tot+dden->den[i]; }
	for(i=0; i<nx*ny; i++){ nden[i] = dden->den[i]/den_tot; }

	// generate .smesh and .mtr for tegen
	scl_factor = dden->scale;
	smesh = fopen("P.smesh","w");
	mtr = fopen("P.mtr","w");
	zbuf = nden[0];								// !!! better to refer to the average density around edge, instead of that of a original point.
	fprintf(smesh,"%ld\t%d\t%d\t%d\n", 2*nx*ny, 3, 0, 0);
	fprintf(mtr,"%ld\t%d\n", 2*nx*ny, 1);
	for(i = 0; i<nx*ny; i++)					// for vertex on lower plane
	{
		yi = i%ny;	xi = int((i-yi)/ny);
		fprintf(smesh,"%ld\t%12.7f\t%12.7f\t%12.7f\n", i+1, dx*xi, dy*yi, 0.0);
		fprintf(mtr,"%12.7f\n", 1.0/sqrt(scl_factor*nden[i]+delta) );
	}
	for(i = 0; i<nx*ny; i++)					// for vertex on upper plane
	{
		yi = i%ny;	xi = int((i-yi)/ny);
		fprintf(smesh,"%ld\t%12.7f\t%12.7f\t%12.7f\n", i+nx*ny+1, dx*xi, dy*yi, 1.0/sqrt(scl_factor*zbuf+delta));
		fprintf(mtr,"%12.7f\n", 1.0/sqrt(scl_factor*nden[i]+delta) );
	}
	fprintf(smesh,"%ld\t%d\n", 2*(nx-1)*(ny-1)+2*(nx-1)+2*(ny-1), 0);
	for(i = 0; i<nx*ny; i++)					// for face
	{
		yi = i%ny;	xi = int((i-yi)/ny);
		if(xi>0 && yi>0) 
		{ 
			fprintf(smesh,"%ld\t%ld\t%ld\t%ld\t%ld\n",4, i+1, i+1-ny, i+1-ny-1, i+1-1);
			fprintf(smesh,"%ld\t%ld\t%ld\t%ld\t%ld\n",4, i+nx*ny+1, i+nx*ny+1-1, i+nx*ny+1-1-ny, i+nx*ny+1-ny); 
		}
	}
	for(i = 0; i<nx*ny; i++)		
	{
		yi = i%ny;	xi = int((i-yi)/ny);
		if( xi==0 && yi<ny-1 ){ fprintf(smesh,"%ld\t%ld\t%ld\t%ld\t%ld\n",4, i+1, i+1+nx*ny, i+1+nx*ny+1, i+1+1); }
		if( xi==nx-1 && yi<ny-1 ){ fprintf(smesh,"%ld\t%ld\t%ld\t%ld\t%ld\n",4, i+1, i+1+1, i+1+1+nx*ny, i+1+nx*ny); }
	}
	for(i = 0; i<nx*ny; i++)			
	{
		yi = i%ny;	xi = int((i-yi)/ny);
		if( yi==0 && xi<nx-1 ){ fprintf(smesh,"%ld\t%ld\t%ld\t%ld\t%ld\n",4, i+1, i+1+ny, i+1+ny+nx*ny, i+1+nx*ny); }
		if( yi==ny-1 && xi<nx-1 ){ fprintf(smesh,"%ld\t%ld\t%ld\t%ld\t%ld\n",4, i+1, i+1+nx*ny, i+1+nx*ny+ny, i+1+ny); }
	}
	fprintf(smesh,"%d\n%d\n", 0, 0);
	fclose(smesh);
	fclose(mtr);

	// generate mesh by tetgen
	system("tetgen -qpmQ P.smesh");
	
	// read number of valid dot_position
	ndot = 0;
	node = fopen("P.1.node","r");
	fscanf(node,"%ld %ld %ld %ld\n", &n1, &n2, &n3, &n4);
	for(i=1; i<=n1; i++)
	{
		fscanf(node,"%ld %lf %lf %lf\n", &n2, &c1, &c2, &c3);
		if( c3 ==0.0 ){ ndot = ndot+1; }
	}
	fclose(node);
	
	// update dpos
	deallocmem_dot_position(dpos);				// !!! need to check whether dpos is allocated or not
	n_dots = ndot;
	allocmem_dot_position(n_dots, hex_bl, hex_lng, dpos);	
	ndot = 0;
	node = fopen("P.1.node","r");
	fscanf(node,"%ld %ld %ld %ld\n", &n1, &n2, &n3, &n4);
	for(i=1; i<=n1; i++)
	{
		fscanf(node,"%ld %lf %lf %lf\n", &n2, &c1, &c2, &c3);
		if( c3 == 0.0 ){ dpos->xd[ndot] = c1; dpos->yd[ndot] = c2; ndot = ndot+1;}
	}
	fclose(node);

	if (hexbl=='T' || hexbl=='t') hex_fit(dpos, hexlng);	// put dots onto hexagon lattice

	// free memory
	delete [] nden;
	return;
}


void den2pos(dot_density *dden, dot_position *dpos, char hexbl, double hexlng)
{
	long int i, j, k, n, nx, ny, ndot;
	long int ibuf1, ibuf2;
	long int begi, midi, endi;
	double dx, dy, x0, y0, xrng, yrng, den_tot, xsol, ysol, ratio;
	double rndx, rndy;
	double *nden, *accdenx, *accdeny;
	bool solving;

	// read parameters from dot_density setup
	x0 = dden->x0;	y0 = dden->y0;
	xrng = dden->xrng;	yrng = dden->yrng;
	nx = dden->nx;	ny = dden->ny;
	dx = xrng/nx;	dy = yrng/ny;					// coordinates for grid-face
	// read parameters from dot_position	setup
	ndot = dpos->ndot; 

	// initiate memroy
	nden = new double[nx*ny];
	if(nden == nullptr) {printf("den2pos: nden initiation error!"); exit(0);}
	accdenx = new double[nx+1];
	if(accdenx == nullptr) {printf("den2pos: accdenx initiation error!"); exit(0);}
	accdeny = new double[ny+1];
	if(accdeny == nullptr) {printf("den2pos: accdeny initiation error!"); exit(0);}
	accdenx[0] = 0.0;	accdeny[0] = 0.0;

	// calculate and normalize the cumulative distribution function for dot_density
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
	n=0;
	while (n<ndot)
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
		dpos->xd[n] = xsol;	dpos->yd[n] = ysol;
		n = n+1;
		printf("transfer density to position on %9d of %9d\r", n, ndot);
	}
	printf("\n");
	
	if (hexbl=='T' || hexbl =='t') hex_fit(dpos, hexlng);		// put dots onto hexagon lattice

	// free memory
	delete [] nden;
	delete [] accdenx;
	delete [] accdeny;
	return;
}


void hex_fit(dot_position *dpos, double hexlng)
{
	long int i, j, ibuf, jbuf, ntot, ntotbuf;
	long int *chkrep;
	double *xbuf, *ybuf;
	double dbuf1, dbuf2;
	double a1[2]={1.0*hexlng, 0.0*hexlng}, a2[2]={-cos(pi/3.0)*hexlng, sin(pi/3.0)*hexlng};
	
	
	// basis for hexagon lattice
	for(i=0; i<dpos->ndot; i++)
	{
		dbuf1 = dpos->yd[i]/a2[1];
		jbuf = (long int)( dbuf1 );
		dbuf2 = (dpos->xd[i]-dbuf1*a2[0])/a1[0];
		ibuf = (long int)( dbuf2 );
		dpos->xd[i] = ibuf*a1[0] + jbuf*a2[0];
		dpos->yd[i] = ibuf*a1[1] + jbuf*a2[1];
	}
	
	// !!! remove repeated points and points on the xy border-lines
	chkrep = new long int[dpos->ndot];
	if( chkrep == nullptr ) { printf("hex_fit: chkrep matrix setup error\n"); exit(0); }
	xbuf = new double[dpos->ndot];
	if( xbuf == nullptr ) { printf("hex_fit: xbuf matrix setup error\n"); exit(0); }
	ybuf = new double[dpos->ndot];
	if( ybuf == nullptr ) { printf("hex_fit: ybuf matrix setup error\n"); exit(0); }
	for(i=0; i<dpos->ndot; i++){ chkrep[i]=1; xbuf[i]=dpos->xd[i]; ybuf[i]=dpos->yd[i]; }
	for(i=0; i<dpos->ndot; i++)
	{
		for(j=i+1; j<dpos->ndot; j++)
		{
			if( abs(dpos->xd[i]-dpos->xd[j])<delta && abs(dpos->yd[i]-dpos->yd[j])<delta)
			{
				chkrep[j] = 0;
			}
		}
	}
	for(i=0; i<dpos->ndot; i++)		// remove points being exactly on borders
	{
		if( dpos->xd[i]<xstr_rng || dpos->xd[i]>(xdim-xstr_rng) || dpos->yd[i]<ystr_rng || dpos->yd[i]>(ydim-ystr_rng) ) chkrep[j] = 0;
	}
	ntot = 0;
	for(i=0; i<dpos->ndot; i++)
	{ 
		if( chkrep[i]==1 ) ntot = ntot+1;  
	}
	ntotbuf = dpos->ndot;
	dpos->ndot = ntot;
	delete [] dpos->xd;
	delete [] dpos->yd;
	dpos->xd = new double[dpos->ndot];
	if( dpos->xd == nullptr ) { printf("hex_fit: xd matrix setup error\n"); exit(0); }
	dpos->yd = new double[dpos->ndot];
	if( dpos->yd == nullptr ) { printf("hex_fit: yd matrix setup error\n"); exit(0); }
	j=0;
	for(i=0; i<ntotbuf; i++)
	{ 
		if( chkrep[i]==1 )
		{
			dpos->xd[j] = xbuf[i];
			dpos->yd[j] = ybuf[i];
			j++;
		}
	}

	delete [] chkrep;
	delete [] xbuf;
	delete [] ybuf;
	return;
}