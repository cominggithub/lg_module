#include "dbg_log.h"
#include <stdio.h>

void dumpRay1(struct ray_trace1* ray)
{

// 	struct ray_trace1									// for individual tracing ray
// {
// 	long int ngaus;
// 	double inty;
// 	double n1, n2;									// refractive index for incident and emergent region.
// 	double xr, yr, zr, thar, phir, nx, ny, nz;		// x,y,z are position; thar, phir are ray direction; nx, ny, nz are the unit vector normal to surface.
// };

	printf("ray:\n");
	printf("\t ngaus: %ld\n", ray->ngaus);
	printf("\t  inty: %f\n", ray->inty);
	printf("\t    n1: %f\n", ray->n1);
	printf("\t    n2: %f\n", ray->n2);
	printf("\t    xr: %f\n", ray->xr);
	printf("\t    yr: %f\n", ray->yr);
	printf("\t    zr: %f\n", ray->zr);
	printf("\t  thar: %f\n", ray->thar);
	printf("\t  phir: %f\n", ray->phir);
	printf("\t    nx: %f\n", ray->nx);
	printf("\t    ny: %f\n", ray->ny);
	printf("\t    nz: %f\n", ray->nz);


}