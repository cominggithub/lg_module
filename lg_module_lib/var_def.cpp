// all length-unit is mm, and all angle-unit is degree (0-180)
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "dbg_log.h"
// general constants
double pi = 3.141592653589793;
double delta = pow(10.0,-10.0);

// light-guide geometry (mm)
double xdim, ydim, zdim_in, zdim_out, z_reflector;					// zdim_out is decided by zdim_in + heightness of microstructure


// optical material
long int n_wl;											// n_wl: number of wavelength; <default value>
long int n_mat;											// n_material: number of optical materials; <default value>
double air_indx = 0, guide_indx = 1, refl_indx = 2;	// material indexes = 0 ~ (n_material-1) corresponding to n_material;

// light source
long int n_x, n_y, n_z;						// grid number along x, y, and z axes; <default value>
long int n_tha, n_phi;						// grid number along theta and phi; <default value>
double xl_or, yl_or, zl_or;					// origin of the coordinate
double xl_rng, yl_rng, zl_rng;				// dimensions along x, y, and z directions

// ray tracing
long int n_ray;								// number of sampling rays
long int n_gaus;							// number of gaussin-scattering rays;
double ray_threshold;

// lguide performance (measurement)
long int nx_rcd, ny_rcd;								// grid number along x, y, and z axes for records; <default value>
long int ntha_rcd, nphi_rcd;							// grid number along theta and phi for records; <default value>
double xrcd_or, yrcd_or, zrcd_or;						// origin of the coordinate
double xrcd_rng, yrcd_rng;								// dimensions along x, y, and z directions

// local structure
long int nx_str, ny_str;						// number of grids along x and y
double center_x, center_y;						// center of the microstructure
double xstr_rng, ystr_rng;						// dimensions along x and y
char str_file[256];

// global dot density
long int nx_den, ny_den;								// grid number along x and y axes for records;
double den_2_pos_scale;
double xden_or, yden_or;								// origin of the coordinate
double xden_rng, yden_rng;								// dimensions along x, y, and z directions

// global dot spatial pattern
long int n_dots=5000;											// total dots
long int partn_x, partn_y;
char hex_bl;													// define whether fit to hexagon 
double hex_lng = 0.02;											// define the lattice constant of hexagon

double theta_st = 0.1;             // The angale between main ray and scattered ray
double ThetaOne = 0;               // The gobal variable for saving ThetaOne temporarily.
double ThetaTwo = 0;               // The gobal variable for saving ThetaTwo temporarily.

double ReflectiveRayIntensity = 0; // The global variable for saving the intensity of ReflectiveRay temporarily.
                                   // It will be used between CalcMainReflectiveRay() and CalcMainTransmittanceRay()
double IntensityThreshold = 0.001;   // The threshold for determine the intensity of ray


// debug config
bool enable_gnu_plot = true;
<<<<<<< HEAD
bool enable_ray_index_log = false;
unsigned long gaussScatteredRaycount = 0;

// output file name
char output_dir[256] 			= "";
char tmp_output_dir[256] 		= "";
char output_opt_record_dat[256];
char output_opt_record_txt[256];
char output_data2d_txt[256] 	= "data2d.txt";
char output_data3d_txt[256] 	= "data3d.txt";
char output_data3dm_txt[256] 	= "data3dm.txt";
char output_data3dv_txt[256]	= "data3dv.txt";

char input_script[256]				= "script";
char input_dot_density_txt[256]		= "dot_density.txt";
char input_microstr_txt[256]		= "microstr.txt";

unsigned long gaussScatteredRaycount = 0;


void getFileFullPath(char *fullPath, const char* fname)
{
	if (strlen(output_dir) > 0)
	{
		sprintf(fullPath, "%s/%s", output_dir, fname);
	}
	else
	{
		sprintf(fullPath, "%s", fname);
	}
}

void getTmpFileFullPath(char *fullPath, const char* fname)
{
	
	if (strlen(tmp_output_dir) > 0)
	{
		sprintf(fullPath, "%s/%s", tmp_output_dir, fname);
	}
	else
	{
		sprintf(fullPath, "%s", fname);
	}
}





