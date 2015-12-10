#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Module_IV.h"
#include "var_def.h"


void RayFromReflector (struct ray_trace1 *rayintoref)
{
	int rayflag;
	double thartemp;
	double mx, my, mz;
		 
    rayflag=1;
	mx = sin(rayintoref->thar*pi/180.0)*cos(rayintoref->phir*pi/180.0); my = sin(rayintoref->thar*pi/180.0)*sin(rayintoref->phir*pi/180.0);
	mz = cos(rayintoref->thar*pi/180.0);
	thartemp=rayintoref->thar;
	switch (rayflag) 
	{
	    case 1://mirror type
			rayintoref->thar=pi-thartemp;
			break;
	
	
	}
}


