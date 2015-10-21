#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "box_check.h"
#include "var_def.h"
#include "dbg_log.h"

bool box_examine_nobottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
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
bool box_examine_w_bottom (struct ray_trace1 *ray1, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
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
	return true;
}