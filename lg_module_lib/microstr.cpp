
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "microstr.h"
#include "var_def.h"

void read_microstr(char *fname, local_str *lstr)
{
	int i;
	double cbuf;
	FILE *fp;
	fp=fopen(fname,"r");
	if (fp!=NULL)
	{
		zdim_out = pow(10.0,5.0);
		for(i=0;i<lstr->nx*lstr->ny;i++) 
		{

			fscanf(fp,"%lf",&cbuf);
			//cbuf = cbuf*10.0;					// debug only! magnetify the height of microstructure for easy-debug
			lstr->thz[i] = cbuf/pow(10.0,3.0);  // Note! in unit of mm; In this case, transform nm to mm
			if(zdim_out>cbuf/pow(10.0,3.0)) {zdim_out = cbuf/pow(10.0,3.0);}
		}
	}
	else
	{
		printf("microstructure file: %s is not found!\n", fname);
		exit(0);
	}
	fclose(fp);
	zdim_out = zdim_in-zdim_out+pow(10.0,-6.0);			// set 1nm separation between microstructure and reflective plate.
	z_reflector = zdim_out+pow(10.0,-3.0);
}


// input: ray1, lstr

// output: ray1
// ray1->xr = x; ray1->yr = y; ray1->zr = z;
// ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
bool find_str_hit_local(ray_trace1 *ray1, local_str *lstr)  // change coordinate origin into the center of the top plane of the box containg a microstrcuture
{
	long int indx;
	long int begi, midi;
	long int yp, ym, xp, xm;
	double begr, endr, midr;
	double r, x, y, z, dx, dy;
	double xbuf, ybuf;
	double nx, ny, nz, ss;
	double x0, y0, z0;
	double xf, yf, zf;
	double mx, my, mz;
	double xmin, xmax, ymin, ymax, zmin, zmax;
	double sign0, signf;
	bool solved, nonfound, bcheck;

	x0 = ray1->xr -lstr->x0;	y0 = ray1->yr -lstr->y0;	z0 = ray1->zr +zdim_in;  // change coordinate origin into the center of the top plane of the box containg a microstrcuture
	dx = lstr->xrng/(lstr->nx);	dy = lstr->yrng/(lstr->ny);
	xmin = -0.5*lstr->xrng;		xmax = -xmin;
	ymin = -0.5*lstr->yrng;		ymax = -ymin;
	zmin = -(zdim_out-zdim_in);		zmax = 0.0;
	mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
	mz = cos(ray1->thar*pi/180.0);
	nx = 0.0; ny = 0.0; nz = 0.0;

	solved = false;
	// sovle the intersection of ray and top plane z=zmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
	if(!solved)
	{
		z = zmax;	r = (z-z0)/mz;
		x = r*mx+x0;	 y = r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
		{
			solved = true;
			nx = 0.0; ny = 0.0; nz = 1.0;
		}
	}
	// sovle the intersection of ray and down plane z=zmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
	if(!solved)
	{
		z = zmin;	r = (z-z0)/mz;
		x = r*mx+x0;	 y = r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
		{
			solved = true;
			nx = 0.0; ny = 0.0; nz = -1.0;
		}
	}
	// sovle the intersection of ray and back plane y=ymin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
	if(!solved)
	{
		y = ymin;	r = (y-y0)/my;
		x = r*mx+x0;	 z = r*mz+z0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
		{
			solved = true;
			nx = 0.0; ny = -1.0; nz = 0.0;
		}
	}
	// sovle the intersection of ray and front plane y=ymax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
	if(!solved)
	{
		y = ymax;	r = (y-y0)/my;
		x = r*mx+x0;	 z = r*mz+z0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
		{
			solved = true;
			nx = 0.0; ny = 1.0; nz = 0.0;
		}
	}
	// sovle the intersection of ray and left plane x=xmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
	if(!solved)
	{
		x = xmin;	r = (x-x0)/mx;
		z = r*mz+z0;	 y = r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
		{
			solved = true;
			nx = -1.0; ny = 0.0; nz = 0.0;
		}
	}
	// sovle the intersection of ray and right plane x=xmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
	if(!solved)
	{
		x = xmax;	r = (x-x0)/mx;
		z = r*mz+z0;	 y = r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
		{
			solved = true;
			nx = 1.0; ny = 0.0; nz = 0.0;
		}
	}

	// check whether the incident position and emit position are on the same materials
	if(solved) 
	{
		xf = x; yf = y; zf = z;
		if(xf == xmax){xf =x-delta;} else {xf =x;}
		if(yf == ymax){yf =y-delta;} else {yf =y;}
	}
	else {printf("find_str_intersection: error on find solution on box surface!\n"); return false; }
	indx = int((x0-xmin)/dx)*(lstr->ny)+int((y0-ymin)/dy); sign0 = (lstr->thz[indx])-delta-(z0);
	indx = int((xf-xmin)/dx)*(lstr->ny)+int((yf-ymin)/dy); signf = (lstr->thz[indx])-delta-(zf);
	if(sign0>0){sign0 = 1.0;}
	else{sign0 = -1.0;}
	if(signf>0){signf = 1.0;}
	else{signf = -1.0;}
	if(sign0*signf>0)	// the same matials
	{
		//return to global coordinates
		ray1->xr = x+lstr->x0; ray1->yr = y+lstr->y0; ray1->zr = z-zdim_in;
		ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
	}
	else		// find the intersection of ray and microstructure
	{
		begr = 0.0;	endr = r;	midr = 0.5*(begr+endr);
		nonfound = true;
		while (nonfound)
		{
			xbuf = begr*mx+x0; ybuf = begr*my+y0; z0 = begr*mz+z0;
			begi = int((xbuf-xmin)/dx)*(lstr->ny)+int((ybuf-ymin)/dy);
			xbuf = midr*mx+x0; ybuf = midr*my+y0; zf = midr*mz+z0;
			midi = int((xbuf-xmin)/dx)*(lstr->ny)+int((ybuf-ymin)/dy);
			sign0 = (lstr->thz[begi])-delta-(z0);
			signf = (lstr->thz[midi])-delta-(zf);
			if(sign0>0){sign0 = 1.0;}	else{sign0 = -1.0;}
			if(signf>0){signf = 1.0;}	else{signf = -1.0;}
			if ( sign0*signf>0 ){ begr = midr; endr = endr; midr = 0.5*(endr+begr); if(fabs(midr)<=delta){ray1->inty=0.0;return true;} }
			else if( sign0*signf<0 ) { begr = begr; endr = midr; midr = 0.5*(endr+begr);if(fabs(midr)<=delta){ray1->inty=0.0;return true;} }
			else {printf("find_str_intersection: error on find solution on microstructure surface!\n"); return false;}
			// check wheterh the solution is found
			if( begi==midi ){ nonfound = false; }
		}

		//return to global coordinate  //and judge if on the same position
		x= int(begi/lstr->ny)*dx+xmin+lstr->x0; y= (begi%lstr->ny)*dy+ymin+lstr->y0; z= lstr->thz[begi]-zdim_in;
		if (pow(pow((ray1->xr-x),2.0)+pow((ray1->yr-y),2.0)+pow((ray1->zr-z),2.0),0.5)<=delta	){ray1->inty = 0.0;return true;}
		else {ray1->xr=x; ray1->yr=y; ray1->zr=z;}
		
		// calculate normalized normal vector (toward the outside of light-guide materials)
		yp = begi+1;
		if(yp>=lstr->nx*lstr->ny) yp = begi;
		ym = begi-1;
		if(ym<0) ym = begi;
		xp = begi+lstr->ny;
		if(xp>=lstr->nx*lstr->ny) xp = begi;
		xm = begi-lstr->ny;
		if(xm<0) xm = begi;
		x0 = int(yp/lstr->ny)*dx; y0 = (yp%lstr->ny)*dy; z0 = lstr->thz[yp];
		x0 = x0-int(ym/lstr->ny)*dx; y0 = y0-(ym%lstr->ny)*dy; z0 = z0-lstr->thz[ym];
		xf = int(xp/lstr->ny)*dx; yf = (xp%lstr->ny)*dy; zf = lstr->thz[xp];
		xf = xf-int(xm/lstr->ny)*dx; yf = yf-(xm%lstr->ny)*dy; zf = zf-lstr->thz[xm];
		x = -yf*z0 + y0*zf;
		y = xf*z0 - x0*zf;
		z = -xf*y0 + x0*yf;
		ss = pow(pow(x,2.0)+pow(y,2.0)+pow(z,2.0),0.5);
		ray1->nx = x/ss; ray1->ny = y/ss;	ray1->nz = z/ss;
	}

	return true;
}