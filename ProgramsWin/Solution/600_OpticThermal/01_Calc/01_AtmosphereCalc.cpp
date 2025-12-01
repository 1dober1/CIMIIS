//=============================================================================
// Заголовочные файлы
#include "01_AtmosphereCalc.h"
#include "../Settings.h"

namespace optic = SysOpticThermal;


//=============================================================================
// Расчет показателя рассеяния при H=0 (сигма р) (зависит от спектра и осадков)
double optic::Calc::Atm::calculateDispersionIndex(const AtmosphereStruct& atm, const SystemStruct& sys)
{
    // Если нет осадков
    if (atm.prec == Precipitation::None)
    {
        if (sys.spectrum_range)
            return (2.26 - 0.49 * std::log(atm.mdv)) / atm.mdv; // 3-5 мкм
        // 8-12 мкм
        return (1.66 - 0.35 * std::log(atm.mdv)) / atm.mdv;
    }

    // Если осадки есть
    double precCoef = 0;
    switch (atm.prec)
    {
    case Precipitation::Drizzle:
        precCoef = 1.06;
        break;
    case Precipitation::Rain:
        precCoef = 1.22;
        break;
    case Precipitation::Snow:
        precCoef = 1.37;
        break;
    case Precipitation::Fog:
        precCoef = 1;
        break;
    default:
        break;
    }

    return 3.9 * precCoef / (atm.mdv * 1);
}


//=============================================================================
// Расчет показателя поглощения при H=0 (сигма п)
double optic::Calc::Atm::calculateAbsorptionIndex(const AtmosphereStruct& atm, const SystemStruct& sys)
{
    // Расчет показателя рассеяния (зависит от спектра)
    if (sys.spectrum_range)
        return 0.15 * std::log(atm.abs_hum / 217) + 1.2; // 3-5 мкм
    return  0.05 + 0.015 * atm.abs_hum; // 8-12 мкм
}


//=============================================================================
// Расчет поправочного коэффициента на высоту полета ЛА (рассеяние)
double optic::Calc::Atm::calculateCorrectCoefDispersion(const AtmosphereStruct& atm, const CarrierStruct& carrier)
{
    // Расчет поправочного коэффициента
    if (atm.prec == Precipitation::None && checkEQ(carrier.height, 0.0))
        return (1.2 * (1 - std::exp(-carrier.height / 1.2))) / carrier.height;
    return 1;
}


//=============================================================================
// Расчет поправочного коэффициента на высоту полета ЛА (поглощение)
double optic::Calc::Atm::calculateCorrectCoefAbsorption(const AtmosphereStruct& atm, const CarrierStruct& carrier)
{
    // Расчет поправочного коэффициента
    if (atm.prec == Precipitation::None && checkEQ(carrier.height, 0.0))
        return (2.2 * (1 - exp(-carrier.height / 2.2))) / carrier.height;
    return 1;
}


//=============================================================================
// Расчет оптической толщи атмосферы, соответствующей рассеянию (Lр)
double optic::Calc::Atm::calculateOpticalThicknessDisp(const AtmosphereResult& res, const TargetStruct& target)
{
    return res.dispersion_index * res.correct_coef_dispersion * target.distance;
}


//=============================================================================
// Расчет оптической толщи атмосферы, соответствующей поглощению (Lп)
double optic::Calc::Atm::calculateOpticalThicknessAbsorp(const AtmosphereResult& res, const SystemStruct& sys, const TargetStruct& target)
{
    // Расчет значения оптической толщи
    if (sys.spectrum_range)
        return res.absorption_index * sqrt(res.correct_coef_absorption * target.distance); // 3-5 мкм
        return res.absorption_index * res.correct_coef_absorption * target.distance;       // 8-12 мкм
}


//=============================================================================
// Расчет параметра атмосферы (тауа а)
optic::AtmosphereResult optic::Calc::Atm::calculateAtmosphereValue(const AtmosphereStruct& atm, const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target)
{
    AtmosphereResult res;

    // Расчет показателя рассеяния при H = 0
    res.dispersion_index = calculateDispersionIndex(atm, sys);
    
    // Расчет поправочного коэффициента на высоту полета ЛА (рассеяние)
    res.correct_coef_dispersion = calculateCorrectCoefDispersion(atm, carrier);
   
    // Расчет значения оптической толщи
    res.optical_thickness_disp = calculateOpticalThicknessDisp(res, target);

    // Расчет показателя поглощения при H=0
    res.absorption_index = calculateAbsorptionIndex(atm, sys);
   
    // Расчет поправочного коэффициента на высоту полета ЛА (поглощение)
    res.correct_coef_absorption = calculateCorrectCoefAbsorption(atm, carrier);
    
    // Расчет значения оптической толщи
    res.optical_thickness_absorption = calculateOpticalThicknessAbsorp(res, sys, target);

    // Расчет параметра атмосферы
    res.atmosphere = exp(-res.optical_thickness_disp - res.optical_thickness_absorption);

    return res;
}
