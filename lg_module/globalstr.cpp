#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globalstr.h"
#include "var_def.h"

void find_str_hit_global(ray_trace1 *ray1, dot_position *dpos, opt_record *opr)  // change coordinate origin into the center of the top plane of the box containg a microstrcuture
{
	long int i, j, xi, yi, indx, begi, endi;
	double dx, dy;
	double x, y, z, r, nx, ny, nz, xc, yc, nx0, ny0, nz0;
	double x0, y0, z0;
	double xmin, xmax, ymin, ymax, zmin, zmax, rmin;
	double mx, my, mz;
	bool solved;
	
	

	if(ray1->zr >=0.0 && ray1->thar<90.0)
	{
		// light emitted from light-guide plate. record the performance;
		// herein, a normal luminance (theta<5) is recorded only for a example
		dx = xdim/opr->nx+delta; dy = ydim/opr->ny+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		indx = xi*opr->ny + yi;
		opr->inty[indx] = opr->inty[indx]+ray1->inty;

	}
	else if( (ray1->zr==0.0 && ray1->thar>90.0) || (ray1->zr<0 && ray1->zr>-zdim_in) || (ray1->zr==-zdim_in && ray1->thar<90.0) )
	{
		x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
		xmin = 0.0;		xmax = xdim;
		ymin = 0.0;		ymax = ydim;
		zmin = -zdim_in;		zmax = 0.0;
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
	}
	else if( (ray1->zr==-zdim_in && ray1->thar>90.0) || (ray1->zr<-zdim_in && ray1->zr>-zdim_out) || (ray1->zr==-zdim_out && ray1->thar<90.0) )
	{
		// find the dots in nearby partition, In this case, we consier the dots in occupied partition only.
		// In this global finding, every microstructure for a dot is effectively treated as box;
		// (1) if the ray1 is inside a box, call the find_str_hit_local to obtain the accuracy intersection point;
		// (2) if the ray1 is outside the boxes, find the box that ray1 belongs to
		dx = xdim/dpos->partnx+delta; dy = ydim/dpos->partny+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		indx = xi*dpos->partny + yi;
		if(indx>0) begi = dpos->partaccni[indx-1];
		else begi = 0;
		endi = dpos->partaccni[indx];
		rmin = pow(10.0,5.0);
		x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
		nx0 = ray1->nx;	ny0 = ray1->ny;	nz0 = ray1->nz;
		solved = false;
		for(i=begi; i<endi; i++)
		{
			
			// check whether the ray1 is inside a box
			xc = dpos->xd[i]; yc = dpos->yd[i];
			if( abs(ray1->xr-xc)<0.5*xstr_rng && abs(ray1->yr-yc)<0.5*ystr_rng )		// inside a box
			{
				ray1->xr = x0; ray1->yr = y0; ray1->zr = z0;
				ray1->nx = nx0; ray1->ny = ny0; ray1->nz = nz0;
				break;
			}

			// if not inside a box, find the nearest plane hitted
			xmin = xc-0.5*xstr_rng;		xmax = xc+0.5*xstr_rng;
			ymin = yc-0.5*ystr_rng;		ymax = yc+0.5*ystr_rng;
			zmin = -zdim_out;		zmax = -zdim_in;
			mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
			mz = cos(ray1->thar*pi/180.0);
			nx = 0.0; ny = 0.0; nz = 0.0;
			// sovle the intersection of ray and top plane z=zmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
				z = zmax;	r = (z-z0)/mz;
				x = r*mx+x0;	 y = r*my+y0;
				if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
				{
					solved = true;
					nx = 0.0; ny = 0.0; nz = 1.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
				}
			// sovle the intersection of ray and down plane z=zmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
				z = zmin;	r = (z-z0)/mz;
				x = r*mx+x0;	 y = r*my+y0;
				if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
				{
					solved = true;
					nx = 0.0; ny = 0.0; nz = -1.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
				}
			// sovle the intersection of ray and back plane y=ymin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
				y = ymin;	r = (y-y0)/my;
				x = r*mx+x0;	 z = r*mz+z0;
				if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
				{
					solved = true;
					nx = 0.0; ny = -1.0; nz = 0.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
				}
			// sovle the intersection of ray and front plane y=ymax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
				y = ymax;	r = (y-y0)/my;
				x = r*mx+x0;	 z = r*mz+z0;
				if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
				{
					solved = true;
					nx = 0.0; ny = 1.0; nz = 0.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
				}
			// sovle the intersection of ray and left plane x=xmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
				x = xmin;	r = (x-x0)/mx;
				z = r*mz+z0;	 y = r*my+y0;
				if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
				{
					solved = true;
					nx = -1.0; ny = 0.0; nz = 0.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
				}
			// sovle the intersection of ray and right plane x=xmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
				x = xmax;	r = (x-x0)/mx;
				z = r*mz+z0;	 y = r*my+y0;
				if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
				{
					solved = true;
					nx = 1.0; ny = 0.0; nz = 0.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
				}
		}
		if(solved==false){ printf("find_str_hit_global: ray1 hit no dot in considered partitions"); exit(0); }  // modify the rule according to demand.
	}
}

void part_dots(dot_position *dpos)
{
	long int i, j, nx, ny, indi, indbuf, indxvalue, indxnum;
	double dx, dy, xbuf, ybuf;

	nx = dpos->partnx;
	ny =  dpos->partny;
	dx = 1.0*xdim/nx+delta;
	dy = 1.0*ydim/ny+delta;

	for(i=0; i<dpos->ndot; i++)
	{
		indi =	int(dpos->xd[i]/dx)*ny+int(dpos->yd[i]/dy);			// the index corresponding to partition array
		dpos->partindx[i] = indi;
	}
	//sort xd/yd by partindx
	for(i=0; i<dpos->ndot; i++)
	{
		for(j=i+1; j<dpos->ndot; j++)
		{
			if(dpos->partindx[i]>dpos->partindx[j])
			{
				indbuf = dpos->partindx[i];
				dpos->partindx[i] = dpos->partindx[j];
				dpos->partindx[j] = indbuf;

				xbuf = dpos->xd[i];
				dpos->xd[i] = dpos->xd[j];
				dpos->xd[j] = xbuf;

				ybuf = dpos->yd[i];
				dpos->yd[i] = dpos->yd[j];
				dpos->yd[j] = ybuf;
			}
		}
	}
	for(i=0; i<nx*ny; i++){ dpos->partaccni[i]=0; }
	indxnum=1;
	indxvalue = dpos->partindx[0];
	for(i=1; i<dpos->ndot; i++)
	{
		if(indxvalue!=dpos->partindx[i])
		{
			dpos->partaccni[indxvalue] = indxnum;
			indxvalue = dpos->partindx[i];
			indxnum = 1;
		}
		else
		{
			indxnum++;
		}
	}
	dpos->partaccni[indxvalue] = indxnum;
	
	for(i=1; i<nx*ny; i++){ dpos->partaccni[i]=dpos->partaccni[i]+dpos->partaccni[i-1]; }

}