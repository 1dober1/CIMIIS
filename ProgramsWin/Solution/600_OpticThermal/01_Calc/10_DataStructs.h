#ifndef _60_OpticThermal_Calc_DataStructs_h
#define _60_OpticThermal_Calc_DataStructs_h

#include "../Settings.h"
#include "11_EnumClasses.h"

namespace SysOpticThermal
{

struct TargetFragment;
struct BackgroundFragment;
typedef std::pair<TargetFragment, BackgroundFragment> Fragment;

struct BearingStruct
{
    double det_p;
    double rec_p;
    double det_ir_val;
    double rec_ir_va;
};

struct AtmosphereStruct
{
    double mdv;
    double abs_hum;
    double temp;
    double sun_height;
    Precipitation::type prec;
    bool is_night;
};

struct CarrierStruct
{
    double height;
    double johnson_det;
    double johnson_rec;
    double op_skill;
    double light;
    bool is_det;
};

struct SystemStruct
{
    bool spectrum_range;
    double azimuth;
    double elevation;
    double zoom;
    double lens_diff;
    double fill;
    double frame_freq;
    double noise;
    int scan;
    int width;
    int height;
};

struct TargetStruct
{
    double height;
    double distance;
    double hor_distance;
    double max_crit_size;
};

struct TargetFragment
{
    double crit_size;
    double temperature;
    double rad_coef;
    double table_coef;
    int sun_pos;
    int surface;
};

struct BackgroundFragment
{
    double temperature;
    double rad_coef;
    double table_coef;
    int sun_pos;
    int surface;
    double angle;
};

struct SunRadiationResult
{
    double middle_transparency;
    double integral_irrad_area;
    double direct_sun_radiation_obj;
    double direct_sun_radiation_bg;
    double diffuse_sun_radiation_obj;
    double diffuse_sun_radiation_bg;
    double eff_object_irr;
    double eff_bg_irr;
};

struct InputDataStruct
{
    BearingStruct bearing;
    AtmosphereStruct atm;
    CarrierStruct carrier;
    SystemStruct sys;
    TargetStruct target;
    std::list<Fragment> outline;

    SunRadiationResult sun_rad;
};

struct DistanceResult
{
    double det_strip_count;
    double rec_strip_count;
    double min_distance;
    double max_distance;
};

struct AtmosphereResult
{
    double dispersion_index;
    double absorption_index;
    double correct_coef_dispersion;
    double correct_coef_absorption;
    double optical_thickness_disp;
    double optical_thickness_absorption;
    double atmosphere;
};

struct InfraRedContrastResult
{
    double ksi;
    double atm_ratio;
    double ir_contrast;
};

struct FieldOfViewResult
{
    double nominal_fov;
    double lens_resolution;
    double ksi;
    double eff_fov;
};

struct StaticProbabilityResult
{
    double eye_const;
    double eye_buf;
    double signal_noise;
    double x_param;
    double mira_width;
    double static_probability;
};

struct ResultDataStruct
{
    AtmosphereResult atm;
    SunRadiationResult sun_rad;
    InfraRedContrastResult ir;
    FieldOfViewResult fov;
    StaticProbabilityResult p;
};

}

#endif