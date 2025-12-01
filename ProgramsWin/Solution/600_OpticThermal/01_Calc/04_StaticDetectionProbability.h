#ifndef _60_OpticThermal_Calc_StaticCalc_h
#define _60_OpticThermal_Calc_StaticCalc_h


//=============================================================================
// Заголовочные файлы
#include "10_DataStructs.h"


namespace SysOpticThermal
{
namespace Calc
{
    namespace Static
    {
        // Расчет постоянной времени глаза
        double calculateEyeConst(const CarrierStruct& carrier);

        // Расчет накопления кадров в зрительном канале человека
        double calculateEyeBuf(const StaticProbabilityResult& res, const SystemStruct& sys);

        // Расчет соотношения сигнал/шум
        double calculateSignalNoise(const StaticProbabilityResult& res, const SystemStruct& sys,
            const AtmosphereResult& atm_res, const InfraRedContrastResult& ir_res, const FieldOfViewResult& fov_res);

        // Расчет пороговой пространственной частоты
        double calculateX(const StaticProbabilityResult& res);

        // Расчет ширины полосы Миры
        double calculateStripeMiraWidth(const StaticProbabilityResult& res, const TargetStruct& target, const FieldOfViewResult& fov_res);

        // Расчет статической вероятности
        double calculateStaticP(const StaticProbabilityResult& res, 
            const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target);

        // Расчет всех параметров, связанных со статической вероятностью
        StaticProbabilityResult calculateStaticProbability(const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target,
            const AtmosphereResult& atm_res, const InfraRedContrastResult& ir_res, const FieldOfViewResult& fov_res);
    }
}
}

#endif