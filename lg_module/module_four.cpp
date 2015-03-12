#include <math.h>
#include "var_def.h"

#define TwoPi 6.283185307179586
#define n1 1.635  // Assign a value temporarily
#define n2 1      // Assign a value temporarily


typedef struct ray_struct { 
	long int ngaus;
	double x, y, z;
	double theta, phi;
	double IntensityP;
	double IntensityS;
	double FieldP;
	double FieldS;
} RayProfile;


// CalcThetaTwo::
// The calculation is according to Sin(Theta1)/Sin(Theta2) = n2/n1 (Snell's law)
double CalcThetaTwo(double ThetaOne)
{
	double ThetaTwo ;
	ThetaTwo = asin(sin(ThetaOne*pi/180)*n2/n1);

	return ThetaTwo;
}



// CalcReflectiveRay::
// For calculating the reflective ray
// Input : Incident Ray
// Output: Relective Ray

RayProfile CalcReflectiveRay (
	double IncidentX, double IncidentY, double IncidentZ, 
	double IncidentTheta, double IncidentPhi,
	double IncidentIntensityP, 
	double IncidentIntensityS, 
	double IncidentFieldP, 
	double IncidentFieldS)
{
	struct ray_struct ReflectiveRay;
	double ThetaR;

	ReflectiveRay.ngaus = 0;     // ???
	ReflectiveRay.x = IncidentX; // ???
	ReflectiveRay.y = IncidentY; // ???
	ReflectiveRay.z = IncidentZ; // ???

	ThetaR = CalcThetaTwo(IncidentTheta); 

	ReflectiveRay.IntensityP = IncidentIntensityP * pow(tan(IncidentTheta-ThetaR),2)/pow(tan(IncidentTheta+ThetaR),2);
	ReflectiveRay.IntensityS = IncidentIntensityS * pow(sin(IncidentTheta-ThetaR),2)/pow(sin(IncidentTheta+ThetaR),2);
	ReflectiveRay.FieldP = IncidentFieldP * (n2*cos(IncidentTheta)-n1*cos(ThetaR))/(n2*cos(IncidentTheta)+(n1*cos(ThetaR)));
	ReflectiveRay.FieldS = IncidentFieldS * (n1*cos(IncidentTheta)-n2*cos(ThetaR))/(n1*cos(IncidentTheta)+(n2*cos(ThetaR)));

	return ReflectiveRay;
}


// CalcTransmittanceRay::
// For calculating the Transmittance/refractive ray
// Input : Incident Ray
// Output: Transmittance Ray
RayProfile CalcTransmittanceRay (
	double IncidentX, double IncidentY, double IncidentZ, 
	double IncidentTheta, double IncidentPhi,
	double IncidentIntensityP, 
	double IncidentIntensityS, 
	double IncidentFieldP, 
	double IncidentFieldS)
{
	struct ray_struct TransmittanceRay;
	double ThetaR;

	TransmittanceRay.ngaus = 0;     // ???
	TransmittanceRay.x = IncidentX; // ???
	TransmittanceRay.y = IncidentY; // ???
	TransmittanceRay.z = IncidentZ; // ???


	ThetaR = CalcThetaTwo(IncidentTheta); 

	TransmittanceRay.IntensityP = IncidentIntensityP * (1 - pow(tan(IncidentTheta-ThetaR),2)/pow(tan(IncidentTheta+ThetaR),2));
	TransmittanceRay.IntensityS = IncidentIntensityS * (1 - pow(sin(IncidentTheta-ThetaR),2)/pow(sin(IncidentTheta+ThetaR),2));
	TransmittanceRay.FieldP = IncidentFieldP * 2*n1*cos(IncidentTheta)/(n2*cos(IncidentTheta)+(n1*cos(ThetaR)));
	TransmittanceRay.FieldS = IncidentFieldS * 2*n1*cos(IncidentTheta)/(n1*cos(IncidentTheta)+(n2*cos(ThetaR)));

	return TransmittanceRay;
}



// Refractive radiation beam calculation.
//Itrad = pow(IntensityTransmittance,2) / (pow(TwoPi,0.5)*IncidentTheta) * exp(-pow(ReflectiveRay.thar,2)/(2*pow(IncidentTheta,2)));

