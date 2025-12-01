#ifndef _60_OpticThermal_Calc_DistanceCalc_h
#define _60_OpticThermal_Calc_DistanceCalc_h

#include "10_DataStructs.h"

namespace SysOpticThermal
{
namespace Calc
{
    namespace Distance
    {
        double calculateStripCount(double p, double johnson, double skill);
        DistanceResult calculateDistances(const BearingStruct& bearing, const CarrierStruct& carrier, const SystemStruct& sys, 
            const TargetStruct& target, 
            const AtmosphereResult& atm_res, const FieldOfViewResult& fov_res);
    }
}
}

#endif