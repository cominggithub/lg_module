// all length-unit is mm, and all angle-unit is degree (0-180)

// general constants
double pi = 3.141592653589793;

// light-guide geometry (mm)
double xdim, ydim, zdim_in, zdim_out;					// zdim_out is decided by zdim_in + heightness of microstructure


// optical material
long int n_wl;											// n_wl: number of wavelength; <default value>
long int n_mat;											// n_material: number of optical materials; <default value>
long int air_indx = 0, guide_indx = 1, refl_indx = 2;	// material indexes = 0 ~ (n_material-1) corresponding to n_material;
struct opt_mat											// for refractive index of materials
{
	long int wln;
	long int matn;
	double *refx;										// refractive index Vs wl Vs material
};

// light source
long int n_x, n_y, n_z;						// grid number along x, y, and z axes; <default value>
long int n_tha, n_phi;						// grid number along theta and phi; <default value>
double xl_or, yl_or, zl_or;					// origin of the coordinate
double xl_rng, yl_rng, zl_rng;				// dimensions along x, y, and z directions
struct opt_source
{
	long int nx, ny, nz, ntha, nphi;
	double x0, y0, z0, xrng, yrng, zrng;
	double *inty;							// intensity profile
	double *accinty;						// accumulation intensity.
};

// ray tracing
long int n_ray;								// number of sampling rays
long int n_gaus;							// number of gaussin-scattering rays;
struct ray_traces							// for the initial ray-samplings
{
	long int nray;
	double *xr, *yr, *zr, *thar, *phir;
};
struct ray_trace1									// for individual tracing ray
{
	long int ngaus;
	double inty;
	double n1, n2;									// refractive index for incident and emergent region.
	double xr, yr, zr, thar, phir, nx, ny, nz;		// x,y,z are position; thar, phir are ray direction; nx, ny, nz are the unit vector normal to surface.
};

// lguide performance (measurement)
long int nx_rcd, ny_rcd;								// grid number along x, y, and z axes for records; <default value>
long int ntha_rcd, nphi_rcd;							// grid number along theta and phi for records; <default value>
double xrcd_or, yrcd_or, zrcd_or;						// origin of the coordinate
double xrcd_rng, yrcd_rng, zrcd_rng;					// dimensions along x, y, and z directions
struct opt_record
{
	long int nx, ny, ntha, nphi;
	double x0, y0, z0, xrng, yrng, zrng;
	double *inty;										// intensity profile
};

// local structure
long int nx_str, ny_str;						// number of grids along x and y
double center_x, center_y;						// center of the microstructure
double xstr_rng, ystr_rng;						// dimensions along x and y
struct local_str
{
	long int nx, ny;
	double x0, y0, xrng, yrng;
	double *thz;								// z-thickness at (x,y)
};

// global dot density
long int nx_den, ny_den;								// grid number along x and y axes for records;
double xden_or, yden_or;								// origin of the coordinate
double xden_rng, yden_rng;								// dimensions along x, y, and z directions
struct dot_density
{
	long int nx, ny;
	double x0, y0, xrng, yrng;
	double *den;												// density profile
};

// global dot spatial pattern
long int n_dots=50000;											// total dots
char hex_bl = 'T';												// define whether fit to hexagon 
double hex_lng = 0.02;											// define the lattice constant of hexagon
struct dot_position
{
	long int ndot;
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