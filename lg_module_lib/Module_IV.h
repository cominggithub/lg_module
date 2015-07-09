#ifndef _MODLUE_IV_H_
#define _MODLUE_IV_H_

#include "var_type.h"

void CalcMainReflectiveRay (struct ray_trace1 *IncidentRay, struct ray_trace1 *MainReflectiveRay);
void CalcMainTransmittanceRay (struct ray_trace1 *IncidentRay, struct ray_trace1 *MainTransmittanceRay);
// void CalcGaussScatteredRay (struct ray_trace1 *MainRay, struct ray_trace1 *GaussScatteredRay[5]);
void CalcGaussScatteredRay (struct ray_trace1 *MainRay, struct ray_trace1 *GaussScatteredRay);
void call_CalcGaussScatteredRay(struct ray_trace1 *src_ray);
#endif