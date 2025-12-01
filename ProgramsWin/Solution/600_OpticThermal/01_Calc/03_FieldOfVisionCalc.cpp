//=============================================================================
// Заголовочные файлы
#include "03_FieldOfVisionCalc.h"
#include "../Settings.h"

namespace optic = SysOpticThermal;


//=============================================================================
// Расчет коэффициента, учитывающего влияние объектива на разрешающую способность
double optic::Calc::FOV::calculateLensResCoeff(const SystemStruct& sys)
{
    const double component_a = 0.22 * (std::pow(sys.lens_diff, 2) - 1);
    const double denom = std::pow(static_cast<double>(sys.scan), 2) * std::pow(sys.fill, 2);
    const double component_b = 0.55 / denom;
    const  double component_c = 0.22 * (1 / (denom * std::pow(sys.zoom, 2)) - 1);
    return sqrt(1 + component_a + component_b + component_c);
}


//=============================================================================
// Расчет табличного коэффициента кси (не учитывается в расчетах)
double optic::Calc::FOV::calculateKsiCoeff(const SystemStruct& sys)
{
    const double x = sys.scan * sys.fill;
    if (x <= 0.55)
        return 1.06;
    if (x > 0.55 && x <= 0.65)
        return 1.05;
    if (x > 0.65 && x <= 0.75)
        return 1.04;
    if (x > 0.75 && x <= 0.85)
        return 1.03;
    if (x > 0.85 && x <= 0.95)
        return 1.025;
    return 1.02;
}


//=============================================================================
// Расчет номинального значения элементарного поля зрения
double optic::Calc::FOV::calculateNominalFOV(const SystemStruct& sys)
{
    return sys.elevation / static_cast<double>(sys.height) * sys.fill;
}


//=============================================================================
// Расчет суммарного (эффективного) поля зрения
double optic::Calc::FOV::calculateEffectiveFOV(const FieldOfViewResult& res)
{
    return res.ksi * res.lens_resolution * res.nominal_fov;
}


//=============================================================================
// Расчет всех параметров, связанных с полем зрения
optic::FieldOfViewResult optic::Calc::FOV::calculateFOV(const SystemStruct& sys)
{
    FieldOfViewResult res;

    res.ksi = calculateKsiCoeff(sys);

    res.lens_resolution = calculateLensResCoeff(sys);
    res.nominal_fov = calculateNominalFOV(sys);
    res.eff_fov = calculateEffectiveFOV(res);

    return res;
}
