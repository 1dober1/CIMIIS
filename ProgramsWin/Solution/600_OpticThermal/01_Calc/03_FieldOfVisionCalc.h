#ifndef _60_OpticThermal_Calc_FOVCalc_h
#define _60_OpticThermal_Calc_FOVCalc_h

//=============================================================================
// Заголовочные файлы
#include "10_DataStructs.h"

namespace SysOpticThermal
{
namespace Calc
{
    namespace FOV
    {
        // Расчет коэффициента, учитывающего влияние объектива на разрешающую способность
        double calculateLensResCoeff(const SystemStruct& sys);

        // Расчет табличного коэффициента кси (не учитывается в расчетах)
        double calculateKsiCoeff(const SystemStruct& sys);

        // Расчет номинального значения элементарного поля зрения
        double calculateNominalFOV(const SystemStruct& sys);

        // Расчет суммарного (эффективного) поля зрения
        double calculateEffectiveFOV(const FieldOfViewResult& res);

        // Расчет всех параметров, связанных с полем зрения
        FieldOfViewResult calculateFOV(const SystemStruct& sys);
    }
}
}
#endif