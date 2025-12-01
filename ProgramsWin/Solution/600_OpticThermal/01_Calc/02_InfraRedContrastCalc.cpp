//=============================================================================
// Заголовочные файлы
#include "02_InfraRedContrastCalc.h"
#include "../Settings.h"

namespace optic = SysOpticThermal;


//=============================================================================
// Расчет отношения коэф. пропускания атмосферы до фона к коэф. пропускания атмосферы до объекта
double optic::Calc::IRContrast::calculateAtmRatio(const AtmosphereResult& atm_res,
    const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target, 
    const std::pair<TargetFragment, BackgroundFragment>& fragment)
{
    const TargetFragment& ts = fragment.first;
    const BackgroundFragment& bs = fragment.second;

    const double param = ts.crit_size / ((carrier.height - ts.crit_size) / target.distance - 
        std::tan(bs.angle) * std::sqrt(1 - std::pow((carrier.height - ts.crit_size) / target.distance, 2)));

    double rad_attenuation = atm_res.dispersion_index * atm_res.correct_coef_dispersion;
    if (sys.spectrum_range)
        rad_attenuation += atm_res.absorption_index / 2 * std::sqrt(atm_res.correct_coef_absorption / target.distance);
    else
        rad_attenuation += atm_res.absorption_index * atm_res.correct_coef_absorption;

    const double max_angle = std::asin((carrier.height - ts.crit_size) / target.distance);
    return bs.angle < max_angle ? std::exp(-rad_attenuation * param) : 0;
}


//=============================================================================
// Расчет теплового контраста одного фрагмента
double optic::Calc::IRContrast::calculateIRContrast(const InfraRedContrastResult& res, const SunRadiationResult& sun_res,
    const AtmosphereStruct& atm, 
    const std::pair<TargetFragment, BackgroundFragment>& fragment)
{
    const TargetFragment& ts = fragment.first;
    const BackgroundFragment& bs = fragment.second;

    // Расчет температурного контраста объекта относительно температуры фона
    const double delta_object_temp = ts.temperature - bs.temperature;

    // Расчет температурного контраста атмосферы относительно температуры фона
    const double delta_atm_temp = atm.temp - bs.temperature;


    //-----------------------------------
    // Расчет контраста (на объекте)
    const double object_contrast = delta_object_temp * ts.rad_coef - (1 - ts.rad_coef) * (ts.table_coef - res.ksi * sun_res.direct_sun_radiation_obj);

    // Расчет контраста (фон)
    const double bg_contrast = (1 - bs.rad_coef) * (bs.table_coef - res.ksi * sun_res.direct_sun_radiation_bg);

    //-----------------------------------
    // Итоговый контраст
    return object_contrast + bg_contrast - (bg_contrast + delta_atm_temp) * (1 - res.atm_ratio);
}


//=============================================================================
// Расчет всех параметров, связанных с расчетом теплового контраста
optic::InfraRedContrastResult optic::Calc::IRContrast::calculateInfraRedContrast(const AtmosphereResult& atm_res, const SunRadiationResult& sun_res,
    const AtmosphereStruct& atm, const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target, 
    const std::list<Fragment>& outline)
{
    InfraRedContrastResult res;
    res.ksi = sys.spectrum_range ? 0.07 : 0.0005;

    std::list<double> ir_list;
    double sum = 0;
    for (std::list<Fragment>::const_iterator cit = outline.begin(); cit != outline.end(); cit++)
    {
        res.atm_ratio = calculateAtmRatio(atm_res, sys, carrier, target, *cit);
        const double ir = calculateIRContrast(res, sun_res, atm, *cit);
        ir_list.push_back(ir);
        sum += ir;
    }

    sum /= static_cast<double>(ir_list.size());
    double d = 0;
    if (ir_list.size() > 1)
    {
        for (std::list<double>::const_iterator cit = ir_list.begin(); cit != ir_list.end(); ++cit)
            d += std::pow(*cit - sum, 2);
        d *= 1.0 / (static_cast<double>(ir_list.size()) - 1);
    }

    res.ir_contrast = std::sqrt(std::pow(sum, 2) + d);

    return res;
}
