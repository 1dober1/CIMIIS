//=============================================================================
// Заголовочные файлы
#include "04_StaticDetectionProbability.h"
#include "../Settings.h"

namespace optic = SysOpticThermal;


//=============================================================================
// Расчет постоянной времени глаза
double optic::Calc::Static::calculateEyeConst(const CarrierStruct& carrier)
{
    return 0.019 + 0.062 * std::pow((0.31 / carrier.light), 0.17);
}


//=============================================================================
// Расчет накопления кадров в зрительном канале человека
double optic::Calc::Static::calculateEyeBuf(const StaticProbabilityResult& res, const SystemStruct& sys)
{
    const double component = -1 / (sys.frame_freq * res.eye_const);
    return std::sqrt((1 + std::exp(component)) / (1 - std::exp(component)));
}


//=============================================================================
// Расчет соотношения сигнал/шум
double optic::Calc::Static::calculateSignalNoise(const StaticProbabilityResult& res,
    const SystemStruct& sys, 
    const AtmosphereResult& atm_res, const InfraRedContrastResult& ir_res, const FieldOfViewResult& fov_res)
{
    // Расчет соотношения сигнал-шум
    const double numerator = std::abs(ir_res.ir_contrast) * atm_res.atmosphere * res.eye_buf * fov_res.eff_fov;
    const double denominator = sys.noise * fov_res.nominal_fov; 
    return numerator / denominator * sys.fill;
}

//=============================================================================
// Расчет пороговой пространственной частоты
double optic::Calc::Static::calculateX(const StaticProbabilityResult& res)
{
    // Расчет параметра X
    const double component = 1 + 0.78 * std::log(res.signal_noise / 0.3);
    return std::min(0.75, 0.59 * (std::sqrt(component) - 1));
}


//=============================================================================
// Расчет ширины полосы Миры
double optic::Calc::Static::calculateStripeMiraWidth(const StaticProbabilityResult& res, const TargetStruct& target, const FieldOfViewResult& fov_res)
{
    return target.distance * fov_res.eff_fov / (2.0 * res.x_param);
}


//=============================================================================
// Расчет статической вероятности
double optic::Calc::Static::calculateStaticP(const StaticProbabilityResult& res,
    const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target)
{
    // Расчет статической вероятности
    const double johnson = carrier.is_det ? carrier.johnson_det : carrier.johnson_rec;
    const double component = carrier.op_skill * target.max_crit_size / (2.0 * johnson * res.mira_width);
    if (res.signal_noise > 0.3)
        return 1 - std::exp(-0.7 * component * component);
    return 0;
}


//=============================================================================
// Расчет всех параметров, связанных со статической вероятностью
optic::StaticProbabilityResult optic::Calc::Static::calculateStaticProbability(const SystemStruct& sys, const CarrierStruct& carrier, const TargetStruct& target,
    const AtmosphereResult& atm_res, const InfraRedContrastResult& ir_res, const FieldOfViewResult& fov_res)
{
    StaticProbabilityResult res;

    res.eye_const = calculateEyeConst(carrier);
    res.eye_buf = calculateEyeBuf(res, sys);
    res.signal_noise = calculateSignalNoise(res, sys, atm_res, ir_res, fov_res);
    res.x_param = calculateX(res);
    res.mira_width = calculateStripeMiraWidth(res, target, fov_res);
    res.static_probability = calculateStaticP(res, sys, carrier, target);

    return res;
}
