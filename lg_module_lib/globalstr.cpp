#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globalstr.h"
#include "var_def.h"
#include "dbg_log.h"
#include "box_check.h"
#include "dot_pos.h"
#include "data_file_util.h"


// input: ray1, dpos, opr
// output: ray1, opr, type
bool find_str_hit_global(ray_trace1 *ray1, dot_position *dpos, opt_record *opr, local_str *lstr, int *type)  // change coordinate origin into the center of the top plane of the box containg a microstrcuture
{

	long int i, j, xi, yi, indx, begi, endi, rcheck, thai, phii;
	double dx, dy, dtha, dphi;
	double x, y, z, zf, r, t, ru, nx, ny, nz, xc, yc, nx0, ny0, nz0; //add zf,ru definition
	double x0, y0, z0;
	double xmin, xmax, ymin, ymax, zmin, zmax, rmin;
	double mx, my, mz;
	bool solved, bcheck, c1, c2;
	
	
	*type = 0;
 	x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
	xmin = 0.0;		xmax = xdim;
	ymin = 0.0;		ymax = ydim;
	zmin = -zdim_in;		zmax = 0.0;
	zf = -z_reflector;
	mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
	mz = cos(ray1->thar*pi/180.0);
	
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
			
	}
	else if(ray1->zr >=0.0 && ray1->thar<90.0)
	{
		// light emitted from light-guide plate top surface. record the performance;
		dx = xdim/opr->nx+delta; dy = ydim/opr->ny+delta;
		dtha = 90/opr->ntha+delta; dphi = 360/opr->nphi+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		thai=(long int)(ray1->thar/dtha);
		if (ray1->phir<0.0) {phii=(long int)((ray1->phir+360)/dphi);} else {phii=(long int)(ray1->phir/dphi);}
		indx = thai*opr->nphi*opr->nx*opr->ny + phii*opr->nx*opr->ny + xi*opr->ny + yi;
		opr->r_index = indx;
		//opr->r_inty += ray1->inty;
		opr->inty[indx] = opr->inty[indx]+ray1->inty;
		*type = 1;

	}
	// light emitted from back,front,right,left surface. delete the ray.
	else if( (ray1->yr==0.0 && ray1->phir<0.0 && ray1->phir>-180.0) || (ray1->yr==ydim && ray1->phir>0.0 && ray1->phir<180.0) || (ray1->xr==0 && ray1->phir>90.0 && ray1->phir<=180.0) ||
		     (ray1->xr==0 && ray1->phir<-90.0 && ray1->phir>=-180.0) || (ray1->xr==xdim && ray1->phir>=0.0 && ray1->phir<90.0) || (ray1->xr==xdim && ray1->phir<=0.0 && ray1->phir>-90.0))
	{
		ray1->inty = 0.0;
		*type = 2;
	}

	// light between LGP top surface and bottom surface
	else if( (ray1->zr<=0.0 && ray1->zr>-zdim_in && ray1->thar>90.0) || (ray1->zr<0 && ray1->zr>=-zdim_in && ray1->thar<90.0) )
	{
		solved = false;
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
		        for(i=begi; i<endi; i++)
		        {
			        xc = dpos->xd[i]; yc = dpos->yd[i];
			        //if( abs(ray1->xr-xc)<0.5*xstr_rng && abs(ray1->yr-yc)<0.5*ystr_rng )		// inside a box
                    if( pow(pow((ray1->xr-xc),2.0)+pow((ray1->yr-yc),2.0),0.5)<= 0.5*xstr_rng )      // inside a box
			        {
			        	*type = 3;
						lstr->x0 = xc; lstr->y0 = yc;//record dot center coordinate

						return true;
			        }
			    }
			}
			if (solved == true && *type == 0)
			{
				*type = 4;
				return true;
			}
		}
		box_hitcheck_nobottom(ray1, xmin, xmax, ymin, ymax, zmin, zmax);
	    *type = 4;	
	}
	else
	{
		//if ray in the microstr
		if (ray1->n1 > 1.0)
		{
			xc=lstr->x0; yc=lstr->y0;
			xmin = xc-0.5*xstr_rng;		xmax = xc+0.5*xstr_rng;
			ymin = yc-0.5*ystr_rng;		ymax = yc+0.5*ystr_rng;
			zmin = -zdim_out;		zmax = -zdim_in;
			rcheck = box_check_function(ray1, xmin, xmax, ymin, ymax, zmin, zmax, 6, 0);
			if (rcheck==5){box_check_function(ray1, xmin, xmax, ymin, ymax, zmin, zmax, 5, 1);*type = 6;}
			else if (rcheck==1 ||rcheck==2 ||rcheck==3 ||rcheck==4 ||rcheck==6) {*type = 3;}
			else if (rcheck==0) {*type = 2;}
		    return true;
		}

		// if ray not in the microstr
		dx = xdim/dpos->partnx+delta; dy = ydim/dpos->partny+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		indx = xi*dpos->partny + yi;
		if(indx>0) begi = dpos->partaccni[indx-1];
		else begi = 0;
		endi = dpos->partaccni[indx];
		for(i=begi; i<endi; i++)
		{
			xc = dpos->xd[i]; yc = dpos->yd[i];
			t = -((x0-xc)+(y0-yc))/(pow((mx*r),2.0)+pow((my*r),2.0));
			if ((xc-x0)/mx>=0.0 && (xc-x0)/mx<=r) {c1 = true;}
			if ((yc-y0)/my>=0.0 && (yc-y0)/my<=r) {c2 = true;}
			if (c1 || c2)
			{
				ru=pow(pow((mx*r*t+x0-xc),2.0)+pow((my*r*t+y0-yc),2.0),0.5);  //distance between microstr center and ray
				if (ru<=0.5*xstr_rng)
				{
					xmin = xc-0.5*xstr_rng;		xmax = xc+0.5*xstr_rng;
			        ymin = yc-0.5*ystr_rng;		ymax = yc+0.5*ystr_rng;
			        zmin = -zdim_out;		zmax = -zdim_in;

					bcheck = box_hitcheck_w_bottom(ray1, xmin, xmax, ymin, ymax, zmin, zmax);
					if (bcheck == true){lstr->x0 = xc; lstr->y0 = yc; *type = 3;return true;}
				}
			}
		}

		xmin = 0.0;		xmax = xdim;
	    ymin = 0.0;		ymax = ydim;
		if (ray1->thar< 90.0){z = -zdim_in;} else {z = -z_reflector;}
	    r = (z-z0)/mz;
		x = r*mx+x0;	 y = r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax) 
		{
		    ray1->xr = x; ray1->yr = y; ray1->zr = z;
			ray1->nx = 0.0; ray1->ny = 0.0;
			if (ray1->thar <90.0){ray1->nz = -1.0;*type = 4;} else {ray1->nz = 1.0;*type = 5;}
		}
		else
		{
			*type = 2;
		}
	}

	return true;
}

void part_dots_unsorted(dot_position *dpos)
{
	long int i, j, nx, ny, indi, indbuf, indxvalue, indxnum;
	double dx, dy, xbuf, ybuf;

	printf("part_dots (unsorted)\n");
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
		printf("\r %ld/%ld", i+1, dpos->ndot);
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
	printf("\n");
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

void part_dots(dot_position *dpos)
{
	long int i, j, nx, ny, indi, indbuf, indxvalue, indxnum;
	double dx, dy, xbuf, ybuf;

	printf("part_dots (sorted)\n");
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
	// for(i=0; i<dpos->ndot; i++)
	// {
	// 	printf("\r %ld/%ld", i+1, dpos->ndot);
	// 	for(j=i+1; j<dpos->ndot; j++)
	// 	{
	// 		if(dpos->partindx[i]>dpos->partindx[j])
	// 		{
	// 			indbuf = dpos->partindx[i];
	// 			dpos->partindx[i] = dpos->partindx[j];
	// 			dpos->partindx[j] = indbuf;

	// 			xbuf = dpos->xd[i];
	// 			dpos->xd[i] = dpos->xd[j];
	// 			dpos->xd[j] = xbuf;

	// 			ybuf = dpos->yd[i];
	// 			dpos->yd[i] = dpos->yd[j];
	// 			dpos->yd[j] = ybuf;
	// 		}
	// 	}
	// }

	save_dot_position_txt_file("part_dos.sorted.before.txt", dpos);
	dpos_quick_sort_by_partindx(dpos, 0, dpos->ndot-1);
	save_dot_position_txt_file("part_dos.sorted.after.txt", dpos);
	printf("\n");
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