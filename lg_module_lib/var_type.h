#ifndef _VAR_TYPE_H_
#define _VAR_TYPE_H_

// all length-unit is mm, and all angle-unit is degree (0-180)

// optical material

struct opt_mat											// for refractive index of materials
{
	long int wln;
	long int matn;
	double *refx;										// refractive index Vs wl Vs material
};

// light source

struct opt_source
{
	long int nx, ny, nz, ntha, nphi;
	double x0, y0, z0, xrng, yrng, zrng;
	double *inty;							// intensity profile
	double *accinty;						// accumulation intensity.
};

// ray tracing
struct ray_traces							// for the initial ray-samplings
{
	long int nray;
	double *xr, *yr, *zr, *thar, *phir, *inty;
};
struct ray_trace1									// for individual tracing ray
{
	long int ngaus;
	double inty;
	double n1, n2;									// refractive index for incident and emergent region.
	double xr, yr, zr, thar, phir, nx, ny, nz;		// x,y,z are position; thar, phir are ray direction; nx, ny, nz are the unit vector normal to surface.
};

// lguide performance (measurement)
struct opt_record
{
	long int nx, ny, ntha, nphi;
	double x0, y0, z0, xrng, yrng;
	int r_index;
	double r_inty;
	double *inty;										// intensity profile
};

// local structure
struct local_str
{
	long int nx, ny;
	double x0, y0, xrng, yrng;
	double *thz;								// z-hieght at (x,y) for microstructure
};

// global dot density
struct dot_density
{
	long int nx, ny;
	double scale;
	double x0, y0, xrng, yrng;
	double *den;												// density profile
};

// global dot spatial pattern
struct dot_position
{
	long int ndot;
	long int partnx, partny;									// partition all dots into grids=npartnx*npartny
	long int *partaccni, *partindx;								// partaccni is for accumlate dots for each grid, and partindx is the responsible array index of xd/yd;
	double *xd, *yd;											// positions of dots
};

// for gnuplot
struct plot_2d
{
	long int npnt;
	double *x, *y;
};
struct plot_3d
{
	long int npnt;
	double *x, *y, *z;
};
struct plot_3d_matrix
{
	long int nx, ny;
	double *v;
};
struct plot_3d_vector
{
	long int npnt;
	double *x, *y, *z, *vx, *vy, *vz;
};


typedef struct ray_struct { 
	unsigned char ngauss; // gaussian-rays numbers
	double intensity;     // Intensity of incident ray
	double n1, n2;        // Index of refrection
	double xr, yr, zr;    // Intersection of incident ray and the boundary
	double thetar, phir;  // This means where Incident ray is from.
	double nx, ny, nz;    // Normal direction of (xr, yr, zr)
} RayProfile;


#endif