#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Module_IV.h"
#include "var_def.h"
#include "dbg_log.h"

#define MAX_OUTPUT_RAY 5


// CalcThetaOne::
// The calculation is according to: (provided by Simon)
// ax*bx+ay*by+az*bz = sqrt(ax^2+ay^2+az^2)*sqrt(bx^2+by^2+bz^2)*cos(theta)
// or according to: (provided by Dr.Ho)
// 1. theta_1 = dot(in_vec_1, vec_N)/length(in_vec_1)/length(vec_N)
// 2. in_vec_1 = (in_vec_x, in_vec_y, in_vec_z) = ( sin(thar)cos(phir), sin(thar)sin(phir), cos(thar) )
// 3. vec_N = (nx, ny, nz)
// Input : in_vec_1, vec_N
// Output: theta_1
//
double CalcThetaOne(double thetar, double phir, double nx, double ny, double nz)
{
	double theta_1;

	theta_1 = acos( (sin(thetar*pi/180)*cos(phir*pi/180)*nx + sin(thetar*pi/180)*sin(phir*pi/180)*ny + cos(thetar*pi/180)*nz) /
		pow( pow(nx,2) + pow(ny,2) + pow(nz,2), 0.5) /
		pow( (pow(sin(thetar*pi/180)*cos(phir*pi/180),2) +  pow(sin(thetar*pi/180)*sin(phir*pi/180),2) + pow(cos(thetar*pi/180),2)),0.5) * pi/180);

	return theta_1;
}


// CalcThetaTwo::
// The calculation is according to:
// Sin(Theta1)/Sin(Theta2) = n2/n1 (Snell's law)
// Input : theta_1, n1, n2
// Output: theta_2
//
double CalcThetaTwo(double ThetaOne, double n1, double n2)
{
	double theta_2;
	theta_2 = asin(sin(ThetaOne*pi/180)*n2/n1);

	return theta_2;
}


// CalcMainReflectiveRay::
// For calculating the Main reflective ray
// Input : The pointer to the structure of Incident Ray.
// Output: The pointer to the structure of Relective Ray
//
void CalcMainReflectiveRay (struct ray_trace1 *IncidentRay, struct ray_trace1 *MainReflectiveRay)
{	
	double q, Rrx, Rry, Rrz, r;

	// Calculate ThetaOne and ThetaTwo (Shown in Fig. 7.a)
	// These two will be used in CalcMainTransmittanceRay() as well.
	ThetaOne = CalcThetaOne(IncidentRay->thar, IncidentRay->phir, IncidentRay->xr, IncidentRay->yr, IncidentRay->zr);
	ThetaTwo = CalcThetaTwo(ThetaOne, IncidentRay->n1, IncidentRay->n2); 

	MainReflectiveRay->ngaus=0;              // 0: Main ray, 1~4: Scattered ray
	MainReflectiveRay->n1 = IncidentRay->n1; // n1 of ReflectiveRay = n1 of IncidentRay
	MainReflectiveRay->n2 = IncidentRay->n2; // n2 of ReflectiveRay = n2 of IncidentRay

	q = -2*IncidentRay->n1*(sin(IncidentRay->thar*pi/180)*cos(IncidentRay->phir*pi/180)*IncidentRay->nx + 
		sin(IncidentRay->thar*pi/180)*sin(IncidentRay->phir*pi/180)*IncidentRay->ny + 
		cos(IncidentRay->thar*pi/180)*IncidentRay->nz);

	Rrx = 1/IncidentRay->n1 * (IncidentRay->n1*sin(IncidentRay->thar*pi/180)*cos(IncidentRay->phir*pi/180)+q*IncidentRay->nx);
	Rry = 1/IncidentRay->n1 * (IncidentRay->n1*sin(IncidentRay->thar*pi/180)*sin(IncidentRay->phir*pi/180)+q*IncidentRay->ny);
	Rrz = 1/IncidentRay->n1 * (IncidentRay->n1*cos(IncidentRay->thar*pi/180)                             +q*IncidentRay->nz);

	// Followings are calculated according to formulas provided by Dr.Ho.
	// a. (x,y,z) = ( r*sin(thar)cos(phir), r*sin(thar)sin(phir), r*cos(thar) )
	// b. r = sqrt(x^2+y^2+z^2)
	// c. thar = acos(z/r)
	// d. phi = atan2(y,z)
	r= pow( (pow(Rrx,2)+pow(Rry,2)+pow(Rrz,2)), 0.5);
	MainReflectiveRay->thar = acos(Rrz/r);
	MainReflectiveRay->phir   = atan2(Rry, Rrz);		

	// Intensity of Reflective Ray = Intensity of Incident Ray * 0.5 * (Rs + Rp)
	MainReflectiveRay->inty = IncidentRay->inty * 0.5 *  (
		pow(tan((ThetaOne-ThetaTwo)*pi/180),2)/pow(tan((ThetaOne+ThetaTwo)*pi/180),2) +
		pow(sin((ThetaOne-ThetaTwo)*pi/180),2)/pow(sin((ThetaOne+ThetaTwo)*pi/180),2));

	// Set the global variable for the usage in CalcMainTransmittanceRay() to reduce the calculation.
	ReflectiveRayIntensity = MainReflectiveRay->inty; 
	
	// (xr, yr, zr) and (nx, ny, nz) don't need to be calculated here. 
}


// CalcMainTransmittanceRay::
// For calculating the Transmittance/refractive ray
// Input : The pointer to the structure of Incident Ray
// Output: The pointer the structure of Transmittance Ray
//
void CalcMainTransmittanceRay (struct ray_trace1 *IncidentRay, struct ray_trace1 *MainTransmittanceRay)
{	
	double p, a, b, c;
	double Rtx, Rty, Rtz, r;

	// gThetaOne and gThetaTwo have been calculated in CalcMainReflectiveRay()
	// They don't need to be calculated here again.

	MainTransmittanceRay->ngaus=0;              // 0: Main ray, 1~4: Scattered ray
	MainTransmittanceRay->n1 = IncidentRay->n2; // n1 of TransmittanceRay = n2 of IncidentRay. Is this correct?
	MainTransmittanceRay->n2 = IncidentRay->n1; // n2 of TransmittanceRay = n1 of IncidentRay. Is this correct? 

	// Following items are calculated according to formulas provided by Simon.	
	b = sin(IncidentRay->thar*pi/180)*cos(IncidentRay->phir*pi/180)*IncidentRay->nx+
		sin(IncidentRay->thar*pi/180)*sin(IncidentRay->phir*pi/180)*IncidentRay->ny+
		cos(IncidentRay->thar*pi/180)                      *IncidentRay->nz;
	c = pow(IncidentRay->n1,2) - pow(IncidentRay->n2,2);
	p = (-b+ pow((pow(b,2) - 4*c),0.5))/2;

	if ((pow(b,2)-4*c) >= 0)
	{
		Rtx = 1/IncidentRay->n2 * (IncidentRay->n1*sin(IncidentRay->thar*pi/180)*cos(IncidentRay->phir*pi/180)+p*IncidentRay->nx);
		Rty = 1/IncidentRay->n2 * (IncidentRay->n1*sin(IncidentRay->thar*pi/180)*sin(IncidentRay->phir*pi/180)+p*IncidentRay->ny);
		Rtz = 1/IncidentRay->n2 * (IncidentRay->n1*cos(IncidentRay->thar*pi/180)                             +p*IncidentRay->nz);

		// Followings are calculated according to formulas provided by Dr.Ho.
		// a. (x,y,z) = ( r*sin(thar)cos(phir), r*sin(thar)sin(phir), r*cos(thar) )
		// b. r = sqrt(x^2+y^2+z^2)
		// c. thar = acos(z/r)
		// d. phi = atan2(y,z)
		r= pow( (pow(Rtx,2)+pow(Rty,2)+pow(Rtz,2)), 0.5);
		MainTransmittanceRay->thar = acos(Rtz/r);
		MainTransmittanceRay->phir   = atan2(Rty, Rtz);

		// Intensity of Transmittance Ray = Intensity of Incident Ray - Intensity of Reflective Ray;
		// Intensity of Reflective Ray (gReflectiveRayIntensity) has been calculated in in CalcMainReflectiveRay().
		MainTransmittanceRay->inty = IncidentRay->inty - ReflectiveRayIntensity;
	}
	else // This case means no refractive/transmittance ray. 
	{
		MainTransmittanceRay->thar = 0;
		MainTransmittanceRay->phir = 0;
		MainTransmittanceRay->inty = 0;
	}
	
	// (xr, yr, zr) and (nx, ny, nz) don't need to be calculated here.
}


// CalcGaussScatteredRay::
// For calculating the reflective ray after Gaussian scattering
// Input  : The point to the main ray which needs to be gasussian scaterred. (No matter reflective or refractive one)
// Output : The point to the structure of Gaussian scattered ray array
//
void CalcGaussScatteredRay (struct ray_trace1 *MainRay, struct ray_trace1 *GaussScatteredRay)
{
	int i;                               // 0  : Center ray
	                                     // 1~4: Scattered rays on the first "cone"
	double I0,I1_4;                      // Intensity of I0 and I1~4 
	double r;                            // The radius
	double input_x, input_y, input_z;    // Used for calculating (theta, phi)
	double output_x, output_y, output_z; // Used for calculating (theta, phi)

	
	for (i=0; i<MAX_OUTPUT_RAY;i++)
	{
	
		GaussScatteredRay[i].ngaus = i;      // 0: Main ray, 1~4: Scattered ray
		GaussScatteredRay[i].n1 = MainRay->n1; // n1 of GaussScatteredRay = n1 of Main Ray.
		GaussScatteredRay[i].n2 = MainRay->n2; // n2 of GaussScatteredRay = n2 of Main Ray.
		if (i==0) // Main ray
		{
			GaussScatteredRay[i].thar = MainRay->thar;
			GaussScatteredRay[i].phir = MainRay->phir;
		}
		else // Scattered ray
		{
			// Followings are calculated according to formulas provided by Simon.
			input_x = sin(theta_st*pi/180)*cos(2*pi/4*i);
			input_y = sin(theta_st*pi/180)*sin(2*pi/4*i);
			input_z = cos(theta_st*pi/180);

			output_x =  cos(MainRay->thar*pi/180)*cos(MainRay->phir*pi/180)*input_x + (-sin(MainRay->phir*pi/180))*input_y + sin(MainRay->thar*pi/180)*cos(MainRay->phir*pi/180)  *input_z;
			output_y =  cos(MainRay->thar*pi/180)*sin(MainRay->phir*pi/180)*input_x +   cos(MainRay->phir*pi/180) *input_y + sin(MainRay->thar*pi/180)*sin(MainRay->phir*pi/180)  *input_z;
			output_z = -sin(MainRay->thar*pi/180)*                         input_x +                                         cos(MainRay->thar*pi/180)                            *input_z;

			// Followings are calculated according to formulas provided by Dr.Ho.
			// a. (x,y,z) = ( r*sin(thar)cos(phir), r*sin(thar)sin(phir), r*cos(thar) )
			// b. r = sqrt(x^2+y^2+z^2)
			// c. thar = acos(z/r)
			// d. phi = atan2(y,z)
			r= pow( (pow(output_x,2)+pow(output_y,2)+pow(output_z,2)), 0.5);
			GaussScatteredRay[i].thar = acos(output_z/r);
			GaussScatteredRay[i].phir = atan2(output_y, output_z);
		}

		// Followings are calculated by:
		// [x=      0, I0   = exp(-(      0)^2/2/(x_st)^2)]
		// [x=   x_st, I1_4 = exp(-(   x_st)^2/2/(x_st)^2)]
		I0 = exp(0.0);                                     
		I1_4 = exp(-pow(theta_st,2)/(2*pow(theta_st,2)));    
		// Nomalization
		if (i==0)
		{
			GaussScatteredRay[i].inty = MainRay->inty * I0/(I0+I1_4*4);
		}
		else
		{
			GaussScatteredRay[i].inty = MainRay->inty * I1_4/(I0+I1_4*4);
		}

	}
	// (xr, yr, zr) and (nx, ny, nz) don't need to be calculated here.
}

void call_CalcGaussScatteredRay(struct ray_trace1 *src_ray)
{
	int i;
	struct ray_trace1 output_ray[MAX_OUTPUT_RAY];
	
	CalcGaussScatteredRay(src_ray, output_ray);
	
	for(i=0; i<MAX_OUTPUT_RAY; i++)
	{
        // §PÂ_²£¥Í¥Xªº¥ú½u°÷¤£°÷«G
        if(output_ray[i].inty > IntensityThreshold)
	    {
		    // recursive here
	        call_CalcGaussScatteredRay(&output_ray[i]);
	    }
	}
	gaussScatteredRaycount++;
}
