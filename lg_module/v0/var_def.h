
// general constants
double pi = 3.141592653589793;

// optical material
long int n_wl=1;											// n_wl: number of wavelength; <default value>
long int n_mat=3;										// n_material: number of optical materials; <default value>
long int air_indx = 0, guide_indx = 1, refl_indx = 2;	// material indexes = 0 ~ (n_material-1) corresponding to n_material;
struct opt_mat										// for refractive index of materials
{
	long int wln;
	long int matn;
	double *refx;									// refractive index Vs wl Vs material
};

// light source
long int n_x = 1, n_y = 100, n_z = 1;						// grid number along x, y, and z axes; <default value>
long int n_tha = 10, n_phi = 30;								// grid number along theta and phi; <default value>
double xl_or = 10.0, yl_or = 0.0, zl_or = 0.0;			// origin of the coordinate
double xl_rng = 10.0, yl_rng = 10.0, zl_rng = 10.0;		// dimensions along x, y, and z directions
struct opt_source
{
	long int nx, ny, nz, ntha, nphi;
	double x0, y0, z0, xrng, yrng, zrng;
	double *inty;							// intensity profile
	double *accinty;						// accumulation intensity.
};

// ray tracing
long int n_ray = 1;								// number of sampling rays
long int n_gaus = 1;								// number of gaussin-scattering rays;
struct ray_traces							// for the initial ray-samplings
{
	long int nray;
	double *xr, *yr, *zr, *thar, *phir;
};
struct ray_trace1							// for individual tracing ray
{
	long int ngaus;
	double xr, yr, zr, thar, phir;
};

// lguide performance (measurement)
long int nx_rcd = 100, ny_rcd = 100;									// grid number along x, y, and z axes for records; <default value>
long int ntha_rcd = 10, nphi_rcd = 20;								// grid number along theta and phi for records; <default value>
double xrcd_or = 0.0, yrcd_or = 0.0, zrcd_or = 0.0;				// origin of the coordinate
double xrcd_rng = 10.0, yrcd_rng = 10.0, zrcd_rng = 10.0;		// dimensions along x, y, and z directions
struct opt_record
{
	long int nx, ny, ntha, nphi;
	double x0, y0, z0, xrng, yrng, zrng;
	double *inty;								// intensity profile
};

// local structure
long int nx_str = 10, ny_str = 10;					// number of grids along x and y
double center_x = 0.0, center_y = 0.0;			// center of the microstructure
double xstr_rng = 1.0, ystr_rng = 1.0;			// dimensions along x and y
struct local_str
{
	long int nx, ny;
	double x0, y0, xrng, yrng;
	double *thz;								// z-thickness at (x,y)
};

// global dot density
long int nx_den = 100, ny_den = 100;									// grid number along x and y axes for records;
double xden_or = 0.0, yden_or = 0.0;							// origin of the coordinate
double xden_rng = 10.0, yden_rng = 10.0;						// dimensions along x, y, and z directions
struct dot_density
{
	long int nx, ny;
	double x0, y0, xrng, yrng;
	double *den;												// density profile
};

// global dot spatial pattern
long int n_dots=50000;												// total dots
long int n_xline = 30;												// number of grid-line on x
long int n_yline = 100;												// number of grid-line on y
struct dot_position
{
	long int xline, yline, ndot;
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