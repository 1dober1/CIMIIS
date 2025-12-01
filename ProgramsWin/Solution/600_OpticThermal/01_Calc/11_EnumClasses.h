#ifndef _60_OpticThermal_Calc_EnumClasses_h
#define _60_OpticThermal_Calc_EnumClasses_h

namespace SysOpticThermal
{
class SpectrumRange
{
public:
    enum type
    {
        WaveLen_3_5 = 0,
        WaveLen_8_14
    };

private:
    SpectrumRange();
    ~SpectrumRange();
    SpectrumRange(const SpectrumRange&);
    SpectrumRange& operator=(const SpectrumRange&);
};

class Clouds
{
public:
    enum type
    {
        None = 0,
        Ci,
        Cu,
        St
    };

private:
    Clouds();
    ~Clouds();
    Clouds(const Clouds&);
    Clouds& operator=(const Clouds&);
};

class Terrain
{
public:
    enum type
    {
        Open = 0,
        ForestClearing,
        ForestTrees
    };

private:
    Terrain();
    ~Terrain();
    Terrain(const Terrain&);
    Terrain& operator=(const Terrain&);
};

class Surface
{
public:
    enum type
    {
        Horizontal = 0,
        Vertical,
    };

private:
    Surface();
    ~Surface();
    Surface(const Surface&);
    Surface& operator=(const Surface&);
};

class Precipitation
{
public:
    enum type
    {
        None = 0,
        Drizzle,
        Rain,
        Snow,
        Fog
    };

private:
    Precipitation();
    ~Precipitation();
    Precipitation(const Precipitation&);
    Precipitation& operator=(const Precipitation&);
};

}

#endif // !EnumClasses