//=============================================================================
// Заголовочные файлы
#include "03_DistanceCalc.h"
#include "04_StaticDetectionProbability.h"
#include "../Settings.h"

namespace optic = SysOpticThermal;


double optic::Calc::Distance::calculateStripCount(double p, double johnson, double skill)
{
    return 2 * johnson / skill * std::sqrt(-std::log(1 - p) / 0.7);
}

optic::DistanceResult optic::Calc::Distance::calculateDistances(const BearingStruct& bearing, const CarrierStruct& carrier, const SystemStruct& sys,
    const TargetStruct& target, 
    const AtmosphereResult& atm_res, const FieldOfViewResult& fov_res)
{
    DistanceResult res;

    res.det_strip_count = calculateStripCount(bearing.det_p, carrier.johnson_det, carrier.op_skill);
    res.rec_strip_count = calculateStripCount(bearing.rec_p, carrier.johnson_rec, carrier.op_skill);

    const double tempApproxCoef = sys.spectrum_range ? 6.1 : 6.6;
    const double someCoef = 1.25;

    StaticProbabilityResult static_res;
    static_res.eye_const = Static::calculateEyeConst(carrier);
    static_res.eye_buf = Static::calculateEyeBuf(static_res, sys);

    const double secondPart = 1.5 * target.max_crit_size / fov_res.eff_fov;
    const double denomPart = tempApproxCoef * fov_res.eff_fov / (2 * target.max_crit_size);
    const double lnPart = static_res.eye_buf * 1 * sys.fill * fov_res.lens_resolution / (someCoef * sys.noise);


    double compBMax = denomPart * res.det_strip_count + atm_res.correct_coef_dispersion * atm_res.dispersion_index;
    double compBRec = denomPart * res.rec_strip_count + atm_res.correct_coef_dispersion * atm_res.dispersion_index;
    const double compCMax = std::log(lnPart * bearing.det_ir_val) + 1.4;
    const double compCRec = std::log(lnPart * bearing.rec_ir_va) + 1.4;

    const double maxDSecondPart = secondPart / res.det_strip_count;
    const double minDSecondPart = secondPart / res.rec_strip_count;

    if (sys.spectrum_range)
    {
        const double compA = std::sqrt(atm_res.correct_coef_absorption) * atm_res.absorption_index / 2;

        const double maxDFirstPart = std::pow((std::sqrt(pow(compA, 2) + compBMax * compCMax) - compA) / compBMax, 2);
        const double minDFirstPart = std::pow((std::sqrt(pow(compA, 2) + compBRec * compCRec) - compA) / compBRec, 2);

        res.max_distance = std::min(maxDFirstPart, maxDSecondPart);
        res.min_distance = std::min(minDFirstPart, minDSecondPart);
    }
    else
    {
        compBMax += atm_res.correct_coef_absorption * atm_res.absorption_index;
        compBRec += atm_res.correct_coef_absorption * atm_res.absorption_index;

        res.max_distance = std::min(compCMax / compBMax, maxDSecondPart);
        res.min_distance = std::min(compCRec / compBRec, minDSecondPart);
    }

    double angle = std::acos(carrier.height / res.max_distance);
    res.min_distance = carrier.height / std::abs(std::cos(angle - sys.elevation));

    return res;
}