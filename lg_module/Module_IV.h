#ifndef _MODLUE_IV_H_
#define _MODLUE_IV_H_

#include "var_type.h"

double CalcThetaOne(double thetar, double phir, double nx, double ny, double nz);
double CalcThetaTwo(double ThetaOne, double n1, double n2);
RayProfile CalcMainReflectiveRay (struct ray_struct IncidentRay);
RayProfile CalcMainTransmittanceRay (struct ray_struct IncidentRay);
RayProfile CalcGaussScatteredRay (struct ray_struct MainRay);


#endif