#include "ray_tracer.h"
#include <string.h>
#include <math.h>
#include "stdlib.h"
#include "stdio.h"
#include "Module_IV.h"
#include "globalstr.h"
#include "microstr.h"
#include "var_def.h"
#include "opt_reflector.h"
#include "glist.h"
#include "dbg_log.h"
#include "data_file_util.h"

#define MAX_OUTPUT_RAY 5

static unsigned long hit_local_error_count = 0;
static unsigned long iteration_count = 0;
static char name[256];

static unsigned long ray_count=0;


void get_child_prefix(const char* prefix, char* child_prefix, int child_num)
{
	if (prefix == NULL || strlen(prefix) == 0)
	{
		sprintf(child_prefix, "%d", child_num);
	}
	else
	{
		sprintf(child_prefix, "%s-%d", prefix, child_num);
	}

	if (enable_ray_index_log)
	{
		printf("%s\n", child_prefix);
	}

}

// need to check with simon
void trace_ray_type3(const char * prefix, ray_trace1 *ray, dot_position *dpos, opt_record *opr, local_str *lstr)
{
	int i, j;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];
	char child_prefix[1024];

	if (!find_str_hit_local(ray, lstr))
	{
		hit_local_error_count++;
		return;
	}

	CalcMainReflectiveRay(ray, &src_ray[0]);
	CalcMainTransmittanceRay(ray, &src_ray[1]);

	for(i=0; i<2; i++)
	{
		get_child_prefix(prefix, child_prefix, i);

		if(strlen(child_prefix)>=1000)
		{
			src_ray[i].inty=0.0;
		}
		
		
		
		if (src_ray[i].inty > IntensityThreshold)
		{
			
            trace_one_ray(child_prefix, &src_ray[i], dpos, opr, lstr);
		}
	}
}

void trace_ray_type4(const char * prefix, ray_trace1 *ray, dot_position *dpos, opt_record *opr, local_str *lstr)
{
	int i, j;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];
	char child_prefix[1024];

	CalcMainReflectiveRay(ray, &src_ray[0]);
	CalcMainTransmittanceRay(ray, &src_ray[1]);

	for(i=0; i<2; i++)
	{
		get_child_prefix(prefix, child_prefix, i);

		if(strlen(child_prefix)>=1000)
		{
			src_ray[i].inty=0.0;
		}
		//append_ray_and_opt_record_to_csv(child_prefix, &src_ray[i], NULL);
		if (src_ray[i].inty > IntensityThreshold)
		{
			
            trace_one_ray(child_prefix, &src_ray[i], dpos, opr, lstr);
		}
	}
}


void trace_ray_type5(const char * prefix, ray_trace1 *ray, dot_position *dpos, opt_record *opr, local_str *lstr)
{
	RayFromReflector(ray);
	trace_one_ray(prefix, ray, dpos, opr, lstr);
}

void trace_one_ray(const char * prefix, ray_trace1 *ray, dot_position *dpos, opt_record *opr, local_str *lstr)
{
	int i, j;
	int type;
	bool result;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];
	
	iteration_count++;
	
	append_ray_and_opt_record_to_csv_type(prefix, ray, opr, type);
	result = find_str_hit_global(ray, dpos, opr, lstr, &type);
	

	if (result)
	{
		switch(type)
		{
			case 1:
				// just skip it, and go to next round
				//append_ray_and_opt_record_to_csv_type(prefix, ray, opr, type);
				break;
			case 2:
				//append_ray_and_opt_record_to_csv_type(prefix, ray, opr, type);
				break;
			case 3:
				//append_ray_and_opt_record_to_csv_type(prefix, ray, opr, type);
				//append_ray_and_opt_record_to_csv_lstr(prefix, ray, lstr);
				trace_ray_type3(prefix, ray, dpos, opr, lstr);
				break;
			case 4:
				//append_ray_and_opt_record_to_csv_type(prefix, ray, opr, type);
				trace_ray_type4(prefix, ray, dpos, opr, lstr);
				break;
			case 5:
				//append_ray_and_opt_record_to_csv_type(prefix, ray, opr, type);
				trace_ray_type5(prefix, ray, dpos, opr, lstr);
				break;
			case 6:
				//append_ray_and_opt_record_to_csv_type(prefix, ray, opr, type);
				trace_one_ray(prefix, ray, dpos, opr, lstr);
		}
	}
	else
	{
		// printf("has no result\n");
	}
}

void trace_one_ray_single_thread(ray_trace1 *ray1, dot_position *dpos, opt_record *opr, local_str *lstr)
{
	long int i, j, xi, yi, indx, begi, endi;
	double dx, dy;
	double x, y, z, r, nx, ny, nz, xc, yc, nx0, ny0, nz0, r1, r2;
	double x0, y0, z0;
	double xmin, xmax, ymin, ymax, zmin, zmax, rmin;
	double mx, my, mz;
	double xbuf, ybuf, zbuf, nxbuf, nybuf, nzbuf;
	bool solved, found;

	// ignore the ray with intensity being lower than a threshold value
	if( ray1->inty<ray_threshold){return;}


	if(ray1->zr >=0.0 && ray1->thar<90.0)	// region 1
	{
		// light emitted from light-guide plate. record the performance;
		// herein, a normal luminance (theta<5) is recorded only for a example
		dx = xdim/opr->nx+delta; dy = ydim/opr->ny+delta;
		xi =(long int)(ray1->xr/dx); yi =(long int)(ray1->yr/dy);
		indx = xi*opr->ny + yi;
		if( abs(ray1->thar)<90.0) opr->inty[indx] = opr->inty[indx]+ray1->inty;	// set range(theta<90) for debug
		return;

	}
	else if( (ray1->zr==0.0 && ray1->thar>90.0) || (ray1->zr<0 && ray1->zr>-zdim_in) || (ray1->zr==-zdim_in && ray1->thar<90.0) ) // region 2
	{
		x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
		xmin = 0.0;				xmax = xdim;
		ymin = 0.0;				ymax = ydim;
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

				// update ray1
				ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
				ray1->n1 = guide_indx;		ray1->n2 = air_indx;
				ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
				ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
				ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
				// calculate transmittance and reflection rays
				ray_trace1 *ray2=new ray_trace1();
				moduleiv_simple(ray1, ray2);		// return (rayT, rayR)
				// continue tracing
				if(ray1->inty>ray_threshold) {trace_one_ray_single_thread(ray1, dpos, opr, lstr);}
				if(ray2->inty>ray_threshold) {trace_one_ray_single_thread(ray2, dpos, opr, lstr);}
				return;
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
				// update ray1
				ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
				ray1->n1 = guide_indx;		ray1->n2 = guide_indx;
				ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
				ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
				ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
				// continue tracing
				trace_one_ray_single_thread(ray1, dpos, opr, lstr);
				return;
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
				// out-going ray, not record in presented case.
				return;
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
				// out-going ray, not record in presented case.
				return;
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
				// out-going ray, not record in presented case.
				return;
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
				// out-going ray, not record in presented case.
				return;
			}
		}
		if(solved==false){
			// printf("%f %f %f\n%f %f %f\n%f %f %f\n%f %f\n",ray1->xr,xmin,xmax,ray1->yr,ymin,ymax,ray1->zr,zmin,zmax,ray1->thar,ray1->phir);
			printf("find_str_hit_global (region 2): Warning! ray1 hit no boundary considered partitions");
			return;
		}  // modify the rule according to demand.
	}
	else if( (ray1->zr==-zdim_in && ray1->thar>90.0) || (ray1->zr<-zdim_in && ray1->zr>-zdim_out) || (ray1->zr==-zdim_out && ray1->thar<90.0) )  // region 3
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
			if( abs(ray1->xr-xc)<0.5*(xstr_rng-delta) && abs(ray1->yr-yc)<0.5*(ystr_rng-delta) )		// inside a box
			{
				// call the microstructure subroutine
				ray1->xr=ray1->xr-xc; ray1->yr=ray1->yr-yc; ray1->zr=ray1->zr-(-zdim_in);		// move the coordinate to the cneter of top-plane of the box
				found=find_str_hit_local(ray1, lstr);
				ray1->xr=ray1->xr+xc; ray1->yr=ray1->yr+yc; ray1->zr=ray1->zr+(-zdim_in);		// move back the global coordinate
				// calculate transmittance and reflection rays
				ray_trace1 *ray3=new ray_trace1();
				if(found) {moduleiv_simple(ray1, ray3);}		// return (rayT, rayR)
				else{return;}
				// continue tracing
				if(ray1->inty>ray_threshold) {trace_one_ray_single_thread(ray1, dpos, opr, lstr);}
				if(ray3->inty>ray_threshold) {trace_one_ray_single_thread(ray3, dpos, opr, lstr);}
				return;

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
						xbuf = x; ybuf = y; zbuf = z;
						nxbuf = nx; nybuf = ny; nzbuf = nz;
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
						xbuf = x; ybuf = y; zbuf = z;
						nxbuf = nx; nybuf = ny; nzbuf = nz;
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
						xbuf = x; ybuf = y; zbuf = z;
						nxbuf = nx; nybuf = ny; nzbuf = nz;
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
						xbuf = x; ybuf = y; zbuf = z;
						nxbuf = nx; nybuf = ny; nzbuf = nz;
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
						xbuf = x; ybuf = y; zbuf = z;
						nxbuf = nx; nybuf = ny; nzbuf = nz;
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
						xbuf = x; ybuf = y; zbuf = z;
						nxbuf = nx; nybuf = ny; nzbuf = nz;
					}
				}
		}
		if(solved==true) // update the ray1
		{
			ray1->xr=xbuf;	ray1->yr=ybuf;	ray1->zr=zbuf;
			ray1->nx=nxbuf;	ray1->ny=nybuf;	ray1->nz=nzbuf;
			trace_one_ray_single_thread(ray1, dpos, opr, lstr);
			return;
		}
		else if(solved==false)  // if not hit microstructure box, than find the hit on the global boundary of region 3, modify the rule according to demand.
		{
			x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
			xmin = 0.0;				xmax = xdim;
			ymin = 0.0;				ymax = ydim;
			zmin = -zdim_out;		zmax = -zdim_in;
			mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
			mz = cos(ray1->thar*pi/180.0);
			nx = 0.0; ny = 0.0; nz = 0.0;

			// sovle the intersection of ray and top plane z=zmax; ray equation: x=r*mx+x0, y=r*my+y0, z=r*mz+z0;
			if(!solved)
			{
				r1=(zmax-z0)/mz;	r2=(zmax-pow(10.0,-3)-z0)/mz;  // refer to microstr.cpp for the adding slight base-thickness (1um)
				if(r1>delta && r2>delta && r1>r2)
				{
					z = zmax-pow(10.0,-3);	r = (z-z0)/mz;
					x = r*mx+x0;	 y = r*my+y0;
					if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
					{
						solved = true;
						nx = 0.0; ny = 0.0; nz = 1.0;
						// update ray1
						ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
						ray1->n1 = air_indx;		ray1->n2 = guide_indx;
						ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
						ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
						ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
						// calculate transmittance and reflection rays
						ray_trace1 *ray4=new ray_trace1();
						moduleiv_simple(ray1, ray4);		// return (rayT, rayR)
						// continue tracing
						if(ray1->inty>ray_threshold) {trace_one_ray_single_thread(ray1, dpos, opr, lstr);}
						if(ray4->inty>ray_threshold) {trace_one_ray_single_thread(ray4, dpos, opr, lstr);}
						return;
					}
				}
				else if(r1>delta && r2>delta && r2>r1)
				{
					// no possible for this condition in present case !
					return;
				}
				else if( (r1>delta && r2<=delta) )
				{
					z = zmax;	r = (z-z0)/mz;
					x = r*mx+x0;	 y = r*my+y0;
					if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
					{
						solved = true;
						nx = 0.0; ny = 0.0; nz = 1.0;

						// update ray1
						ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
						ray1->n1 = guide_indx;		ray1->n2 = guide_indx;
						ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
						ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
						ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
						// continue tracing
						trace_one_ray_single_thread(ray1, dpos, opr, lstr);
						return;
					}
				}
				else if( (r1<=delta && r2>delta) )
				{
					z = zmax-pow(10.0,-3);	r = (z-z0)/mz;
					x = r*mx+x0;	 y = r*my+y0;
					if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
					{
						solved = true;
						nx = 0.0; ny = 0.0; nz = 1.0;

						// update ray1
						ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
						ray1->n1 = guide_indx;		ray1->n2 = air_indx;
						ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
						ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
						ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
						// calculate transmittance and reflection rays
						ray_trace1 *ray5=new ray_trace1();
						moduleiv_simple(ray1, ray5);		// return (rayT, rayR)
						// continue tracing
						if(ray1->inty>ray_threshold) {trace_one_ray_single_thread(ray1, dpos, opr, lstr);}
						if(ray5->inty>ray_threshold) {trace_one_ray_single_thread(ray5, dpos, opr, lstr);}
						return;
					}
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

					// update ray1
					ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
					ray1->n1 = air_indx;		ray1->n2 = air_indx;
					ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
					ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
					ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
					// continue tracing
					trace_one_ray_single_thread(ray1, dpos, opr, lstr);
					return;
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
					// out-going ray, not record in presented case.
					return;
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
					// out-going ray, not record in presented case.
					return;
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
					// out-going ray, not record in presented case.
					return;
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
					// out-going ray, not record in presented case.
					return;
				}
			}
			if(solved==false){ printf("find_str_hit_global (region 3): Warning! ray1 hit no boundary in considered partitions"); return; }  // modify the rule according to demand.

		}
	}
	else if( (ray1->zr==-zdim_out && ray1->thar>90.0) || (ray1->zr<-zdim_out && ray1->zr>-zdim_out-pow(10.0,-3)) || (ray1->zr==-zdim_out-pow(10.0,-3) && ray1->thar<90.0) ) // region 4
	{
		x0 = ray1->xr;	y0 = ray1->yr;	z0 = ray1->zr;
		xmin = 0.0;				xmax = xdim;
		ymin = 0.0;				ymax = ydim;
		zmin = -zdim_out-pow(10.0,-3);		zmax = -zdim_out;
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

				// update ray1
				ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
				ray1->n1 = air_indx;		ray1->n2 = air_indx;
				ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
				ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
				ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
				// continue tracing
				trace_one_ray_single_thread(ray1, dpos, opr, lstr);
				return;
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

				// update ray1
				ray1->ngaus = ray1->ngaus;	ray1->inty = ray1->inty;
				ray1->n1 = air_indx;		ray1->n2 = refl_indx;
				ray1->xr = x;				ray1->yr = y;			ray1->zr = z;
				ray1->thar = ray1->thar;		ray1->phir = ray1->phir;
				ray1->nx = nx;				ray1->ny = ny;			ray1->nz = nz;
				// calculate transmittance and reflection rays
				ray_trace1 *ray6=new ray_trace1();
				moduleiv_simple(ray1, ray6);		// return (rayT, rayR)
				// continue tracing
				if(ray1->inty>ray_threshold) {trace_one_ray_single_thread(ray1, dpos, opr, lstr);}
				if(ray6->inty>ray_threshold) {trace_one_ray_single_thread(ray6, dpos, opr, lstr);}
				return;
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
				// out-going ray, not record in presented case.
				return;
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
				// out-going ray, not record in presented case.
				return;
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
				// out-going ray, not record in presented case.
				return;
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
				// out-going ray, not record in presented case.
				return;
			}
		}
		if(solved==false){ printf("find_str_hit_global (region 4): Warning! ray1 hit no boundary in considered partitions"); return; }  // modify the rule according to demand.
	}
	else
	{
		printf("\ntrace_one_ray_single_thread: Warning found!\n");
		return;
	}
	return;
}

void moduleiv_simple(ray_trace1 *rayT, ray_trace1 *rayR)
{
	double mx, my, mz;
	double R, T, t1, t2;
	double tc;												// total reflection angle;
	double vecrot[3], veci[3], vecf[3], angrot, lng;		// rotation variable to decide refractive/reflective unit vector
	double rotm[3][3];										// rotation matrix to decide refractive/reflective unit vector

	mx=sin(rayT->thar*pi/180.0)*cos(rayT->phir*pi/180.0);
	my=sin(rayT->thar*pi/180.0)*sin(rayT->phir*pi/180.0);
	mz=cos(rayT->thar*pi/180.0);

	t1=acos(mx*rayT->nx+my*rayT->ny+mz*rayT->nz)*180.0/pi;		// incident angle: theta_1 !! normrla director of boundary surface must be normalized to 1, i.e. sqrt(nx^2+ny^2+nz^2)=1


	// calclate reflection and transmittance coefficient
	if(t1<90.0)
	{
		// find the total-reflection angle
		if(rayT->n2 < rayT->n1) {tc = asin(rayT->n2/rayT->n1)*180.0/pi;}
		else{ tc=90.0; }
		if(t1<tc)
		{
			t2=asin(rayT->n1*sin(t1*pi/180.0)/rayT->n2)*180.0/pi;
			R=0.5*pow( tan((t1-t2)*pi/180.0)/tan((t1+t2)*pi/180.0) ,2.0)+0.5*pow( sin((t1-t2)*pi/180.0)/sin((t1+t2)*pi/180.0) ,2.0);
			T=1.0-R;
		}
		else
		{
			t2=0.0;
			R=1.0;
			T=1.0-R;
		}
	}
	else
	{
		// find the total-reflection angle
		if(rayT->n2 < rayT->n1) {tc = asin(rayT->n2/rayT->n1)*180.0/pi;}
		else{ tc=90.0; }
		if((180.0-t1)<tc)
		{
			t2=180.0-asin(rayT->n1*sin((180.0-t1)*pi/180.0)/rayT->n2)*180.0/pi;
			R=0.5*pow( tan((t1-t2)*pi/180.0)/tan((t1+t2)*pi/180.0) ,2.0)+0.5*pow( sin((t1-t2)*pi/180.0)/sin((t1+t2)*pi/180.0) ,2.0);
			T=1.0-R;
		}
		else
		{
			t2=180.0;
			R=1.0;
			T=1.0-R;
		}
	}

	// define incident vector
	veci[0]=mx; veci[1]=my; veci[2]=mz;
	// calculate vector for rotation axis
	vecrot[0]= mz*rayT->ny-my*rayT->nz;
	vecrot[1]=-mz*rayT->nx+mx*rayT->nz;
	vecrot[2]= my*rayT->nx-mx*rayT->ny;
	lng=sqrt( pow(vecrot[0],2)+pow(vecrot[1],2)+pow(vecrot[2],2) );
	vecrot[0]= vecrot[0]/lng;
	vecrot[1]= vecrot[1]/lng;
	vecrot[2]= vecrot[2]/lng;

	// calculate rotation matrix for reflection, the formula refer to google: roation matrix wiki
	angrot=(180.0-2.0*t1)*pi/180.0;
	rotm[0][0]=cos(angrot)+pow(vecrot[0],2)*(1.0-cos(angrot));
	rotm[0][1]=vecrot[0]*vecrot[1]*(1.0-cos(angrot))-vecrot[2]*sin(angrot);
	rotm[0][2]=vecrot[0]*vecrot[2]*(1.0-cos(angrot))+vecrot[1]*sin(angrot);
	rotm[1][0]=vecrot[0]*vecrot[1]*(1.0-cos(angrot))+vecrot[2]*sin(angrot);
	rotm[1][1]=cos(angrot)+pow(vecrot[1],2)*(1.0-cos(angrot));
	rotm[1][2]=vecrot[1]*vecrot[2]*(1.0-cos(angrot))-vecrot[0]*sin(angrot);
	rotm[2][0]=vecrot[0]*vecrot[2]*(1.0-cos(angrot))-vecrot[1]*sin(angrot);
	rotm[2][1]=vecrot[1]*vecrot[2]*(1.0-cos(angrot))+vecrot[0]*sin(angrot);
	rotm[2][2]=cos(angrot)+pow(vecrot[2],2)*(1.0-cos(angrot));
	vecf[0]=rotm[0][0]*veci[0]+rotm[0][1]*veci[1]+rotm[0][2]*veci[2];
	vecf[1]=rotm[1][0]*veci[0]+rotm[1][1]*veci[1]+rotm[1][2]*veci[2];
	vecf[2]=rotm[2][0]*veci[0]+rotm[2][1]*veci[1]+rotm[2][2]*veci[2];
	// update variables of reflection
	rayR->ngaus = rayT->ngaus;	rayR->inty = R*rayT->inty;
	rayR->n1 = rayT->n1;		rayR->n2 = rayT->n2;
	rayR->xr = rayT->xr;		rayR->yr = rayT->yr;	rayR->zr = rayT->zr;
	rayR->thar = acos(vecf[2])*180.0/pi;
	rayR->phir = atan2(vecf[1],vecf[0])*180.0/pi;
	rayR->nx = rayT->nx;		rayR->ny = rayT->ny;	rayR->nz = rayT->nz;

	// calculate rotation matrix for reflection, the formula refer to google: roation matrix wiki
	angrot=(t2-t1)*pi/180.0;
	rotm[0][0]=cos(angrot)+pow(vecrot[0],2)*(1.0-cos(angrot));
	rotm[0][1]=vecrot[0]*vecrot[1]*(1.0-cos(angrot))-vecrot[2]*sin(angrot);
	rotm[0][2]=vecrot[0]*vecrot[2]*(1.0-cos(angrot))+vecrot[1]*sin(angrot);
	rotm[1][0]=vecrot[0]*vecrot[1]*(1.0-cos(angrot))+vecrot[2]*sin(angrot);
	rotm[1][1]=cos(angrot)+pow(vecrot[1],2)*(1.0-cos(angrot));
	rotm[1][2]=vecrot[1]*vecrot[2]*(1.0-cos(angrot))-vecrot[0]*sin(angrot);
	rotm[2][0]=vecrot[0]*vecrot[2]*(1.0-cos(angrot))-vecrot[1]*sin(angrot);
	rotm[2][1]=vecrot[1]*vecrot[2]*(1.0-cos(angrot))+vecrot[0]*sin(angrot);
	rotm[2][2]=cos(angrot)+pow(vecrot[2],2)*(1.0-cos(angrot));
	vecf[0]=rotm[0][0]*veci[0]+rotm[0][1]*veci[1]+rotm[0][2]*veci[2];
	vecf[1]=rotm[1][0]*veci[0]+rotm[1][1]*veci[1]+rotm[1][2]*veci[2];
	vecf[2]=rotm[2][0]*veci[0]+rotm[2][1]*veci[1]+rotm[2][2]*veci[2];
	// upate variables of transmittance
	rayT->ngaus = rayT->ngaus;	rayT->inty = T*rayT->inty;
	rayT->n1 = rayR->n2;		rayT->n2 = rayR->n1;
	rayT->xr = rayT->xr;		rayT->yr = rayT->yr;	rayT->zr = rayT->zr;
	rayT->thar = acos(vecf[2])*180.0/pi;
	rayT->phir = atan2(vecf[1],vecf[0])*180.0/pi;
	rayT->nx = rayT->nx;		rayT->ny = rayT->ny;	rayT->nz = rayT->nz;


}