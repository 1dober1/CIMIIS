//=============================================================================
// Заголовочные файлы
#include "99_EffectiveIrradianceCalc.h"

namespace optic = SysOpticThermal;


//=============================================================================
// Расчет интегральной облученности поверхности
double optic::Calc::SunRad::calculateIntegralIrradianceArea(const SunRadiationResult& res, 
    const AtmosphereStruct& atm)
{
    // Расчет промежуточного значения
    const double min_coeff = std::min(1 / std::sin(atm.sun_height), 35.0);

    // Расчет интегральной облученности поверхности 
    return 1360 / (1 + res.middle_transparency * min_coeff);
}


//=============================================================================
// Расчет излучения Солнца (итоговый)
optic::SunRadiationResult optic::Calc::SunRad::calculateSunRadiation(const AtmosphereStruct& atm,
    const std::pair<TargetFragment, BackgroundFragment>& fragment)
{
    const TargetFragment& target = fragment.first;
    const BackgroundFragment& bg = fragment.second;

    SunRadiationResult res;
    res.middle_transparency = 0.43;

    res.integral_irrad_area = calculateIntegralIrradianceArea(res, atm);
    res.direct_sun_radiation_obj = calculateDirectSunRadiation(res, atm, target);
    res.direct_sun_radiation_bg  = calculateDirectSunRadiation(res, atm, bg);

    return res;
}
