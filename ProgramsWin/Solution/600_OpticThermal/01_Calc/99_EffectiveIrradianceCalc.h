#ifndef _60_OpticThermal_Calc_SunRadCalc_h
#define _60_OpticThermal_Calc_SunRadCalc_h


//=============================================================================
// Заголовочные файлы
#include "10_DataStructs.h"
#include "../Settings.h"


namespace SysOpticThermal
{
namespace Calc
{
    namespace SunRad
    {
        // Расчет интегральной облученности поверхности
        double calculateIntegralIrradianceArea(const SunRadiationResult& res, const AtmosphereStruct& atm);

        // Расчет прямого излучения Солнца
        template<typename Fragment>
        double calculateDirectSunRadiation(const SunRadiationResult& res, const AtmosphereStruct& atm, const Fragment& fragment)
        {
            if (atm.is_night || fragment.sun_pos == 0)
                return 0;
            if (fragment.surface == 0)
                return res.integral_irrad_area * sin(atm.sun_height);
            return res.integral_irrad_area * cos(atm.sun_height);
        }

        // Расчет излучения Солнца (итоговый)
        SunRadiationResult calculateSunRadiation(const AtmosphereStruct& atm, const std::pair<TargetFragment, BackgroundFragment> &fragment);
    }
}
}

#endif