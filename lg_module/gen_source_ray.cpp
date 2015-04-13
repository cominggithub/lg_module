
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gen_source_ray.h"
#include "var_def.h"
#include "mem_func.h"
#include "plt_figure.h"
#include "den_to_pos.h"

void gen_source_ray(opt_source *ops, ray_traces *rays)
{
	long int i, j, k, n;
	long int nray, nrayi, ibuf1, ibuf2;
	long int thai, phii, begi, endi, midi;
	double tha, phi, dtha, dphi, inty_tot;
	double thasol,phisol,ratio;
	double rndtha, rndphi;
	double *inty, *accintytha, *accintyphi;
	plot_3d_vector plt3dv;
	bool solving;


	dtha = 170.0/ops->ntha;
	dphi = 170.0/ops->nphi;
	nray = rays->nray;
	nrayi = int(1.0*nray/ops->ntha/ops->nphi/ops->ny);
	rays->nray = nrayi*ops->ntha/ops->nphi/ops->ny;

	// simulate a gaussin-type light source profiles (2d+1d)
	for (i=0; i<ops->ntha*ops->nphi; i++)
	{
		phii = int(i%ops->nphi);	thai = int(1.0*(i-phii)/ops->nphi);
		tha = (thai+0.5)*dtha; phi = (phii+0.5)*dphi;
		ops->inty[i] = 1.0*exp( -2.0*pow((tha-90.0)*pi/180.0,2.0) )*exp( -1.0*pow((phi-180.0)*pi/180.0,2.0) );  // phi is centered on 180 for symmetry, but the true value is from 0 to 170.
	}

	// initiate memroy
	inty = new double[ops->ntha*ops->nphi];
	if(inty == nullptr) {printf("gen_source_ray: inty initiation error!"); exit(0);}
	accintytha = new double[ops->ntha+1];
	if(accintytha == nullptr) {printf("gen_source_ray: accintytha initiation error!"); exit(0);}
	accintyphi = new double[ops->nphi+1];
	if(accintyphi == nullptr) {printf("gen_source_ray: accintyphi initiation error!"); exit(0);}
	accintytha[0] = 0.0;	accintyphi[0] = 0.0;

	// calculate and normalize the cumulative distribution function for dot_density
	inty_tot = 0.0;
	for(i=0; i<ops->ntha*ops->nphi; i++){ inty_tot = inty_tot+ops->inty[i]; }
	for(i=0; i<ops->ntha*ops->nphi; i++){ inty[i] = ops->inty[i]/inty_tot; }
	for(i=0; i<ops->ntha ; i++)
	{
		if(i==0) {accintytha[i+1] = 0.0;} 
		else {accintytha[i+1] = accintytha[i];}
		for(j=0; j<ops->nphi; j++){ k = i*ops->nphi+j; accintytha[i+1] = accintytha[i+1]+inty[k]; }
	}

	// generate source rays
	printf("\n");
	n=0;
	while (n<nray)
	{
		rndtha = 1.0*rand()/RAND_MAX;
		rndphi = 1.0*rand()/RAND_MAX;
		// solve along tha direction
		begi = 0;	endi = ops->ntha;
		midi = int(0.5*(endi+begi));
		solving = true;
		while (solving)
		{
			if ( rndtha>=accintytha[begi] && rndtha<accintytha[midi]){ begi = begi; endi = midi; midi = int(0.5*(endi+begi)); }
			else { begi = midi; endi = endi; midi = int(0.5*(endi+begi)); };
			if( (endi-begi)<2 ){ solving = false; }
			if( endi == begi ){ printf("gen_source_ray: ray-initial error on theta! "); exit(0); }
		}
		ratio = (rndtha-accintytha[begi])/(accintytha[endi]-accintytha[begi]);
		thasol = dtha*begi+dtha*ratio;
		// build accintyphi
		accintyphi[0] = 0.0;
		for(j = 0; j<ops->nphi; j++)
		{ 
			if(begi ==0){ accintyphi[j+1]= inty[j]; }
			else
			{
				ibuf1 = (begi-1)*ops->nphi+j;	ibuf2 = (endi-1)*ops->nphi+j;
				accintyphi[j+1]= inty[ibuf1]+ratio*(inty[ibuf2]-inty[ibuf1]);
			}
		}
		for(j = 0; j<ops->nphi; j++) { accintyphi[j+1] = accintyphi[j+1]+accintyphi[j]; }
		for(j = 0; j<ops->nphi; j++) { accintyphi[j+1] = accintyphi[j+1]/accintyphi[ops->nphi]; }
		// solve along phi direction
		begi = 0;	endi = ops->nphi;
		midi = int(0.5*(endi+begi));
		solving = true;
		while (solving)
		{
			if ( rndphi>=accintyphi[begi] && rndphi<accintyphi[midi]){ begi = begi; endi = midi; midi = int(0.5*(endi+begi)); }
			else { begi = midi; endi = endi; midi = int(0.5*(endi+begi)); };
			if( (endi-begi)<2 ){ solving = false; }
			if( endi == begi ){ printf("gen_source_ray: ray-initial error on phi! "); exit(0); }
		}
		ratio = (rndphi-accintyphi[begi])/(accintyphi[endi]-accintyphi[begi]);
		phisol = dphi*begi+dphi*ratio;
		// record position
		rays->thar[n] = thasol;	
		rays->phir[n] = phisol - 0.5*dphi*ops->nphi;     // note! phi is centered on 180 for symmetry previously, and now is revised
		rays->xr[n] = 0.0; rays->zr[n] = 0.0;
		rays->yr[n] = 1.0*ops->yrng *rand()/RAND_MAX + 1.0*ops->y0;
		n = n+1;
		printf("transfer density to position on %9d of %9d\r", n, nray);
	}
	printf("\n");

	// plot results
	allocmem_gnuplot_3d_vector(nray, &plt3dv);
	plt3dv.npnt = nray;
	for (i=0; i<nray; i++)
	{
		plt3dv.x[i] = rays->xr[i];
		plt3dv.y[i] = rays->yr[i];
		plt3dv.z[i] = rays->zr[i];
		plt3dv.vx[i] = sin(rays->thar[i]*pi/180.0)*cos(rays->phir[i]*pi/180.0);
		plt3dv.vy[i] = sin(rays->thar[i]*pi/180.0)*sin(rays->phir[i]*pi/180.0);
		plt3dv.vz[i] = cos(rays->thar[i]*pi/180.0);
	}
	gnuplot_3d_vector(&plt3dv);

	// free memory
	delete [] inty;
	delete [] accintytha;
	delete [] accintyphi;
	return;
}
