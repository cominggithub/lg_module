#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globalstr.h"
#include "var_def.h"
#include "dbg_log.h"


// input: ray1, dpos, opr
// output: ray1, opr, type
bool find_str_hit_global(ray_trace1 *ray1, dot_position *dpos, opt_record *opr, local_str *lstr, int *type)  // change coordinate origin into the center of the top plane of the box containg a microstrcuture
{
	long int i, j, xi, yi, indx, begi, endi;
	double dx, dy;
	double x, y, z, zf, r, ru, nx, ny, nz, xc, yc, nx0, ny0, nz0; //add zf,ru definition
	double x0, y0, z0;
	double xmin, xmax, ymin, ymax, zmin, zmax, rmin;
	double mx, my, mz;
	bool solved;
	
	
	*type = 0;
 	x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
	xmin = 0.0;		xmax = xdim;
	ymin = 0.0;		ymax = ydim;
	zmin = -zdim_in;		zmax = 0.0;
	mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
	mz = cos(ray1->thar*pi/180.0);
	//nx = 0.0; ny = 0.0; nz = 0.0;
	if(ray1->xr<0.0)
	{
		    x = xmin;	r = (x-x0)/mx;
			z = r*mz+z0;	 y = r*my+y0;
			if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
			{
				
				ray1->nx = -1.0; ray1->ny = 0.0; ray1->nz = 0.0;
				ray1->xr = x; ray1->yr = y; ray1->zr = z;
			}
			*type = 4;
			//printf("x: %ld, y: %ld, z: %ld, thar: %.2f, phir:%.2f\n", ray1->xr, ray1->yr, ray1->zr, ray1->thar, ray1->phir);
	}
	else if(ray1->zr >=0.0 && ray1->thar<90.0)
	{
		// light emitted from light-guide plate top surface. record the performance;
		// herein, a normal luminance (theta<5) is recorded only for a example
		dx = xdim/opr->nx+delta; dy = ydim/opr->ny+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		indx = xi*opr->ny + yi;
		opr->r_index = indx;
		opr->r_inty += ray1->inty;
		opr->inty[indx] = opr->inty[indx]+ray1->inty;
		//printf("xi: %ld, opr->ny: %ld, yi: %ld, opr->index: %ld\n", xi, opr->ny, yi, opr->index);
		*type = 1;

	}
	// light emitted from back,front,right,left surface. delete the ray.
	else if( (ray1->yr==0.0 && ray1->phir<0.0 && ray1->phir>-180.0) || (ray1->yr==ydim && ray1->phir>0.0 && ray1->phir<180.0) || (ray1->xr==0 && ray1->phir>90.0 && ray1->phir<=180.0) ||
		     (ray1->xr==0 && ray1->phir<-90.0 && ray1->phir>=-180.0) || (ray1->xr==xdim && ray1->phir>=0.0 && ray1->phir<90.0) || (ray1->xr==xdim && ray1->phir<=0.0 && ray1->phir>-90.0))
	{
		ray1->inty = 0;
		*type = 2;
	}

	// the position of ray is between z=0 and -zdim_in, polar angle >90 deg 
	// do 2 check (1)light-guide front, back , right, left, top bottom plane (2) microstr box
	else if( (ray1->zr<=0.0 && ray1->thar>90.0) || (ray1->zr>-zdim_in && ray1->thar>90.0) || (ray1->zr<0 && ray1->zr>=-zdim_in && ray1->thar<90.0) )
	{
		
		solved = false;
		// solve the intersection of ray and down plane z=zmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		// and check the ray if in the box. 
		// if the ray1 is inside a box, call the find_str_hit_local to obtain the accuracy intersection point;
		if(!solved)
		{
			z = zmin;	r = (z-z0)/mz;
			x = r*mx+x0;	 y = r*my+y0;
			if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
			{
				solved = true;
				ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = -1.0;
				ray1->xr = x;   ray1->yr = y;   ray1->zr = z;
			
			    dx = xdim/dpos->partnx+delta; dy = ydim/dpos->partny+delta;
		        xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		        indx = xi*dpos->partny + yi;
		        if(indx>0) begi = dpos->partaccni[indx-1];
		        else begi = 0;
		        endi = dpos->partaccni[indx];
		        //rmin = pow(10.0,5.0);
		        //x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
		        //nx0 = ray1->nx;	ny0 = ray1->ny;	nz0 = ray1->nz;
		        for(i=begi; i<endi; i++)
		        {
			
			        // check whether the ray1 is inside a box
					// and inside a box, call find_str_hit_local
			        xc = dpos->xd[i]; yc = dpos->yd[i];

					
			        //if( abs(ray1->xr-xc)<0.5*xstr_rng && abs(ray1->yr-yc)<0.5*ystr_rng )		// inside a box
                    if( pow(pow((ray1->xr-xc),2.0)+pow((ray1->yr-yc),2.0),0.5)<= 0.5*xstr_rng )      // inside a box
			        {
			        	//ray1->xr = x0; ray1->yr = y0; ray1->zr = z0;
				        //ray1->nx = nx0; ray1->ny = ny0; ray1->nz = nz0;
						*type = 3;
						lstr->x0 = xc; lstr->y0 = yc;//record dot center coordinate
				      //break;
						return true;
			        }
			    }
			}
			if (solved == true && *type == 0)
			{
				*type = 4;
				ray1->xr = x ; ray1->yr = y ; ray1->zr = z ;
				ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = -1.0;
			}
		}
		// solve the intersection of ray and top plane z=zmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		if(!solved)
		{
			z = zmax;	r = (z-z0)/mz;
			x = r*mx+x0;	 y = r*my+y0;
			if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
			{
				solved = true;
				ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = 1.0;
				ray1->xr = x; ray1->yr = y; ray1->zr = z;

			}
		}
		// solve the intersection of ray and back plane y=ymin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		if(!solved)
		{
			y = ymin;	r = (y-y0)/my;
			x = r*mx+x0;	 z = r*mz+z0;
			if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
			{
				solved = true;
				ray1->nx = 0.0; ray1->ny = -1.0; ray1->nz = 0.0;
				ray1->xr = x; ray1->yr = y; ray1->zr = z;
			}
		}
		// solve the intersection of ray and front plane y=ymax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		if(!solved)
		{
			y = ymax;	r = (y-y0)/my;
			x = r*mx+x0;	 z = r*mz+z0;
			if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
			{
				solved = true;
				ray1->nx = 0.0; ray1->ny = 1.0; ray1->nz = 0.0;
				ray1->xr = x; ray1->yr = y; ray1->zr = z;
			}
		}
		// solve the intersection of ray and left plane x=xmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		if(!solved)
		{
			x = xmin;	r = (x-x0)/mx;
			z = r*mz+z0;	 y = r*my+y0;
			if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
			{
				solved = true;
				ray1->nx = -1.0; ray1->ny = 0.0; ray1->nz = 0.0;
				ray1->xr = x; ray1->yr = y; ray1->zr = z;
			}
		}
		// solve the intersection of ray and right plane x=xmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		if(!solved)
		{
			x = xmax;	r = (x-x0)/mx;
			z = r*mz+z0;	 y = r*my+y0;
			if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax) 
			{
				solved = true;
				ray1->nx = 1.0; ray1->ny = 0.0; ray1->nz = 0.0;
				ray1->xr = x; ray1->yr = y; ray1->zr = z;
			}
		}
	    *type = 4;	
	}
	    // the position of ray is between zdim_in and z_refl, and polar angle >90 deg.
	    //do 2 check : (1) nearest box (2) reflector plane
	    // find the dots in nearby partition, In this case, we consier the dots in occupied partition only.
		// In this global finding, every microstructure for a dot is effectively treated as box;
		// find the box that ray1 belongs to
	else if( (ray1->zr <=-zdim_in && ray1->thar>90.0) || (ray1->zr >-z_reflector && ray1->thar>90.0) )
	{
		dx = xdim/dpos->partnx+delta; dy = ydim/dpos->partny+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		indx = xi*dpos->partny + yi;
		if(indx>0) begi = dpos->partaccni[indx-1];
		else begi = 0;
		endi = dpos->partaccni[indx];
		rmin = pow(10.0,5.0);
		x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
		nx0 = ray1->nx;	ny0 = ray1->ny;	nz0 = ray1->nz;
		zf = -z_reflector;
		solved = false;
		mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0); mz = cos(ray1->thar*pi/180.0);
	    nx = 0.0; ny = 0.0; nz = 0.0;
		for(i=begi; i<endi; i++)
		{
			// find the nearest box hitted
			xc = dpos->xd[i]; yc = dpos->yd[i];
			xmin = xc-0.5*xstr_rng;		xmax = xc+0.5*xstr_rng;
			ymin = yc-0.5*ystr_rng;		ymax = yc+0.5*ystr_rng;
			zmin = -zdim_out;		zmax = -zdim_in;
			
			// sovle the intersection of ray and top plane z=zmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and down plane z=zmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and back plane y=ymin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and front plane y=ymax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and left plane x=xmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and right plane x=xmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
		}
		if (solved)
		{
			lstr->x0 = xc; lstr->y0 = yc; 
			*type = 3;
		}

		// solve the intersection of ray and reflector plane z=zmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		// and if ray is solved on the reflector plane, call the RayFromReflector
		if (!solved)
		{       
			    xmin = 0.0;		xmax = xdim;
	            ymin = 0.0;		ymax = ydim;
	            z = zf;	r = (z-z0)/mz;
				x = r*mx+x0;	 y = r*my+y0;
				if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax) 
				{
					solved = true;
					nx = 0.0; ny = 0.0; nz = 1.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
					*type = 5;
				}
				else
				{
				    *type = 2;
				}

		}
		if(solved==false)
		{ 
			printf("find_str_hit_global: ray1 hit no dot in considered partitions"); 
			return false;
		}  // modify the rule according to demand.
		
	}

	 // the position of ray is between zdim_in and z_refl, and polar angle <90 deg.
	 // do 2 check : (1) nearest box (2) light-guide bottom plane 
	else if( (ray1->zr <-zdim_in && ray1->thar<90.0) || (ray1->zr >=-z_reflector && ray1->thar<90.0) )
	{
		//To avoid ray with large thar hits LGP bottom plane far away from original positon.
		//so let ray hits the z=-zdim_out plane.
		if (ray1->n1 > 1.0)
		{
		  z = -zdim_in;	r = (z-z0)/mz;
		  x = r*mx+x0;	 y = r*my+y0;
		  ray1->xr = x; ray1->yr = y; ray1->zr = z;
		  ray1->nx =0.0; ray1->ny =0.0; ray1->nz =-1.0;
		  *type = 6;
		  return true;
		}
		z = -zdim_out;	r = (z-z0)/mz;
		x = r*mx+x0;	 y = r*my+y0;
		ray1->xr = x; ray1->yr = y; ray1->zr = z;
		x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;

		dx = xdim/dpos->partnx+delta; dy = ydim/dpos->partny+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		indx = xi*dpos->partny + yi;
		if(indx>0) begi = dpos->partaccni[indx-1];
		else begi = 0;
		endi = dpos->partaccni[indx];
		rmin = pow(10.0,5.0);
		//nx0 = ray1->nx;	ny0 = ray1->ny;	nz0 = ray1->nz;
		zf = -z_reflector;
		solved = false;
		mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0); mz = cos(ray1->thar*pi/180.0);
	    nx = 0.0; ny = 0.0; nz = 0.0;
		
		for(i=begi; i<endi; i++)
		{
			// find the nearest box hitted
			xc = dpos->xd[i]; yc = dpos->yd[i];
			xmin = xc-0.5*xstr_rng;		xmax = xc+0.5*xstr_rng;
			ymin = yc-0.5*ystr_rng;		ymax = yc+0.5*ystr_rng;
			zmin = -zdim_out;		zmax = -zdim_in;
			
			// solve the intersection of ray and down plane z=zmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
				if (x0>=xmin && x0<=xmax && y0>=ymin && y0<=ymax) 
				{
					solved = true;
					nx = 0.0; ny = 0.0; nz = -1.0;
					if(r<rmin)
					{
						ray1->xr = x; ray1->yr = y; ray1->zr = z;
						ray1->nx = nx; ray1->ny = ny; ray1->nz = nz;
					}
				}
			}

			// solve the intersection of ray and top plane z=zmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			/* this judgement isn't necessary;
			if (!solved)
			{
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
			}
			*/

			// solve the intersection of ray and back plane y=ymin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and front plane y=ymax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and left plane x=xmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
			}
			// solve the intersection of ray and right plane x=xmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if (!solved)
			{
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
		}
		if (solved)
		{
			lstr->x0 = xc; lstr->y0 = yc;
			*type = 3;
		}

		// solve the intersection of ray and bottom plane z=zmin; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
		// and if ray is solved on the bottom plane, call the module IV
		if (!solved)
		{
				z = -zdim_in;	r = (z-z0)/mz;
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
					*type = 4;
				}
		}
		if(solved==false)
		{ 
			printf("find_str_hit_global: ray1 hit no dot in considered partitions"); 
			false;
		}  // modify the rule according to demand.
		
	}


	return true;
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