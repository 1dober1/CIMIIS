#ifndef _60_OpticThermal_Calc_IRContrastCalc_h
#define _60_OpticThermal_Calc_IRContrastCalc_h


//=============================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "10_DataStructs.h"


namespace SysOpticThermal
{
namespace Calc
{
    namespace IRContrast
    {
        // Расчет отношения коэф. пропускания атмосферы до фона к коэф. пропускания атмосферы до объекта
        double calculateAtmRatio(const AtmosphereResult& atm_res, const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target,
            const std::pair<TargetFragment, BackgroundFragment>& fragment);

        // Расчет теплового контраста одного фрагмента
        double calculateIRContrast(const InfraRedContrastResult& res, const SunRadiationResult& sun_res,
            const AtmosphereStruct& atm,
            const std::pair<TargetFragment, BackgroundFragment>& fragment);

        // Расчет всех параметров, связанных с расчетом теплового контраста
        InfraRedContrastResult calculateInfraRedContrast(const AtmosphereResult& atm_res, const SunRadiationResult& sun_res,
            const AtmosphereStruct& atm, const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target,
            const std::list<Fragment>& outline);
    }
}
}
#endif