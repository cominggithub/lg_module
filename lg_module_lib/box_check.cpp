#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "box_check.h"
#include "var_def.h"
#include "dbg_log.h"

bool box_hitcheck_nobottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
{
	bool solved;
	double x,y,z,r;
	double x0,y0,z0;
	double mx,my,mz;
	solved = false;
	x0 = ray1->xr; y0 = ray1->yr; z0 = ray1->zr;
	mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
	mz = cos(ray1->thar*pi/180.0);
	if (!solved) //left surface hit check
	{
		x=xmin; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = -1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //right surface hit check
	{
		x=xmax; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //back surface hit check
	{
		y=ymin; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = -1.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //front surface hit check
	{
		y=ymax; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 1.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //top surface hit check
	{
		z=zmax; r=(z-z0)/mz;
		y=r*my+y0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = 1.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	return true;
}
bool box_hitcheck_w_bottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
{
	bool solved;
	double x,y,z,r;
	double x0,y0,z0;
	double mx,my,mz;
	solved = false;
	x0 = ray1->xr; y0 = ray1->yr; z0 = ray1->zr;
	mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
	mz = cos(ray1->thar*pi/180.0);
	if (!solved) //left surface hit check
	{
		x=xmin; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = -1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //right surface hit check
	{
		x=xmax; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //back surface hit check
	{
		y=ymin; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = -1.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //front surface hit check
	{
		y=ymax; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 1.0; ray1->nz = 0.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //top surface hit check
	{
		z=zmax; r=(z-z0)/mz;
		y=r*my+y0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = 1.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	if (!solved) //bottom surface hit check
	{
		z=zmin; r=(z-z0)/mz;
		y=r*my+y0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = -1.0;
			ray1->xr = x; ray1->yr = y; ray1->zr = z;
		}
	}
	
	static unsigned int count = 77777777;
	ray1->no = count++;
	ray1->xr = 12345678;
	ray1->yr = 12345678;
	ray1->zr = 12345678;
	return true;
}

int box_check_w_bottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
{
	bool solved;
	double x,y,z,r;
	double x0,y0,z0;
	double mx,my,mz;
	solved = false;
	x0 = ray1->xr; y0 = ray1->yr; z0 = ray1->zr;
	mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
	mz = cos(ray1->thar*pi/180.0);
	if (!solved) //left surface hit check
	{
		x=xmin; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = -1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			return 1;
		}
	}
	if (!solved) //right surface hit check
	{
		x=xmax; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			return 2;
		}
	}
	if (!solved) //back surface hit check
	{
		y=ymin; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = -1.0; ray1->nz = 0.0;
			return 3;
		}
	}
	if (!solved) //front surface hit check
	{
		y=ymax; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 1.0; ray1->nz = 0.0;
			return 4;
		}
	}
	if (!solved) //top surface hit check
	{
		z=zmax; r=(z-z0)/mz;
		y=r*my+y0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = 1.0;
			return 5;
		}
	}
	if (!solved) //bottom surface hit check
	{
		z=zmin; r=(z-z0)/mz;
		y=r*my+y0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = -1.0;
			return 6;
		}
	}
	return 0;
}

int box_check_function (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int faces, int hc)
{
	//faces=5(check r,l,b,f,t 5faces), faces=6(check r,l,b,f,t,bottom 6 faces)
	//hc=1, ray will stay on the box surface after check;   hc=0, only check
	bool solved;
	double x,y,z,r;
	double x0,y0,z0;
	double mx,my,mz;
	solved = false;
	x0 = ray1->xr; y0 = ray1->yr; z0 = ray1->zr;
	mx = sin(ray1->thar*pi/180.0)*cos(ray1->phir*pi/180.0); my = sin(ray1->thar*pi/180.0)*sin(ray1->phir*pi/180.0);
	mz = cos(ray1->thar*pi/180.0);
	if (!solved) //left surface hit check
	{
		x=xmin; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = -1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			if (hc==1){ray1->xr = x; ray1->yr = y; ray1->zr = z;}
			return 1;
		}
	}
	if (!solved) //right surface hit check
	{
		x=xmax; r=(x-x0)/mx;
		z=r*mz+z0;; y=r*my+y0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 1.0; ray1->ny = 0.0; ray1->nz = 0.0;
			if (hc==1){ray1->xr = x; ray1->yr = y; ray1->zr = z;}
			return 2;
		}
	}
	if (!solved) //back surface hit check
	{
		y=ymin; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = -1.0; ray1->nz = 0.0;
			if (hc==1){ray1->xr = x; ray1->yr = y; ray1->zr = z;}
			return 3;
		}
	}
	if (!solved) //front surface hit check
	{
		y=ymax; r=(y-y0)/my;
		z=r*mz+z0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 1.0; ray1->nz = 0.0;
			if (hc==1){ray1->xr = x; ray1->yr = y; ray1->zr = z;}
			return 4;
		}
	}
	if (!solved) //top surface hit check
	{
		z=zmax; r=(z-z0)/mz;
		y=r*my+y0;; x=r*mx+x0;
		if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		{
			solved = true;
			ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = 1.0;
			if (hc==1){ray1->xr = x; ray1->yr = y; ray1->zr = z;}
			return 5;
		}
	}
	if (!solved) //bottom surface hit check
	{
		if (faces==6)
		{
            z=zmin; r=(z-z0)/mz;
		    y=r*my+y0;; x=r*mx+x0;
		    if (r>delta && x>=xmin && x<=xmax && y>=ymin && y<=ymax && z>=zmin && z<=zmax)
		    {
                solved = true;
			    ray1->nx = 0.0; ray1->ny = 0.0; ray1->nz = -1.0;
				if (hc==1){ray1->xr = x; ray1->yr = y; ray1->zr = z;}
			    return 6;
		    }
		}
	}
	return 0;
}