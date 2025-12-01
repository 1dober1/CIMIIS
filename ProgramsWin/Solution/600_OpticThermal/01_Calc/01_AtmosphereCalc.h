#ifndef _60_OpticThermal_Calc_AtmosphereCalc_h
#define _60_OpticThermal_Calc_AtmosphereCalc_h


//=============================================================================
// Заголовочные файлы
#include "10_DataStructs.h"


namespace SysOpticThermal
{
namespace Calc
{
    namespace Atm
    {
        // Расчет показателя рассеяния при H=0 (сигма р) (зависит от спектра и осадков)
        double calculateDispersionIndex(const AtmosphereStruct& atm, const SystemStruct& sys);
        
        // Расчет показателя поглощения при H=0 (сигма п)
        double calculateAbsorptionIndex(const AtmosphereStruct& atm, const SystemStruct& sys);

        // Расчет поправочного коэффициента на высоту полета ЛА (рассеяние)
        double calculateCorrectCoefDispersion(const AtmosphereStruct& atm, const CarrierStruct& carrier);
        
        // Расчет поправочного коэффициента на высоту полета ЛА (поглощение)
        double calculateCorrectCoefAbsorption(const AtmosphereStruct& atm, const CarrierStruct& carrier);

        // Расчет оптической толщи атмосферы, соответствующей рассеянию (Lр)
        double calculateOpticalThicknessDisp(const AtmosphereResult& res, const TargetStruct& target);
        
        // Расчет оптической толщи атмосферы, соответствующей поглощению (Lп)
        double calculateOpticalThicknessAbsorp(const AtmosphereResult& res, const SystemStruct& sys, const TargetStruct& target);

        // Расчет параметра атмосферы (тауа а)
        AtmosphereResult calculateAtmosphereValue(const AtmosphereStruct& atm, const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target);
    }
}
}

#endif
