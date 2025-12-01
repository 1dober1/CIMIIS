//=============================================================================
// Заголовочные файлы
#include "02_CGlobalEnvironment.h"
#include "../91_Utility/01_CStorage.h"

namespace optic = SysOpticThermal;


//=============================================================================
// Загрузка внешних условий из оперативной базы данных
result optic::CGlobalEnvironment::load(const CDataMap& data, const string& fileName)
{
    // Признак наличия предупреждений при считывании параметров типа системы
    bool wereWarnings = false;      // изначально полагаем, что предупреждений не будет
    std::string path;

    const Utility::CStorage db(data, fileName);

    path = "/environment/параметры_планеты/";
    // Запрос экваториального и полярного радиусов планеты
    double majorRadius = 0.0;
    double minorRadius = 0.0;
    if(!db.extractDoublePositive(path + "полярный_радиус", minorRadius))
        return RESULT_ERROR;
    if (!db.extractDoublePositive(path + "экваториальный_радиус", majorRadius))
        return RESULT_ERROR;
    if(majorRadius > 0.0 && minorRadius > 0.0)
        myEarth.setSize(minorRadius, majorRadius);

    return loadEnv(data, fileName);
}


//-----------------------------------------------------------------------------
result optic::CGlobalEnvironment::loadEnv(const CDataMap& data, const string& fileName)
{
    return myEnv.load(data, fileName);
}


//=============================================================================
// Доступ к параметрам Земли
const optic::CEarth & optic::CGlobalEnvironment::getEarth() const
{
    return myEarth;
}

//=============================================================================
// Доступ к параметрам атмосферы
double optic::CGlobalEnvironment::getMeteoVisibility() const
{
    return myEnv.getMeteoVisibility();
}


//-----------------------------------------------------------------------------
double optic::CGlobalEnvironment::getRelativeHumidity() const
{
    return myEnv.getMeteoVisibility();
}


//-----------------------------------------------------------------------------
double optic::CGlobalEnvironment::getTemperature(double lat, double lon, double height) const
{
    return myEnv.getTemperature(lat, lon, height);
}


//=============================================================================
// Доступ к информации об облачности
FirstCloudType optic::CGlobalEnvironment::getFirstTypeOfCloud() const
{
    return myEnv.getFirstTypeOfCloud();
}


//-----------------------------------------------------------------------------
SecondCloudType optic::CGlobalEnvironment::getSecondTypeOfCloud() const
{
    return myEnv.getSecondTypeOfCloud();
}


//=============================================================================
// Доступ к информации о погоде
gidrometeorEnum optic::CGlobalEnvironment::getGidrometeor() const
{
    return myEnv.getGidrometeor();
}

bool optic::CGlobalEnvironment::getIsNight(double lat, double lon) const
{
    return myEnv.getIsNight(lat, lon);
}


//=============================================================================
// Доступ к информации о поверхности
double optic::CGlobalEnvironment::getSurfaceTemperature(double lat, double lon) const
{
    return myEnv.getSurfaceTemperature(lat, lon);
}


//-----------------------------------------------------------------------------
TypeSurface optic::CGlobalEnvironment::getSurfaceType(double lat, double lon) const
{
    return myEnv.getSurfaceType(lat, lon);
}


//-----------------------------------------------------------------------------
double optic::CGlobalEnvironment::getSurfaceEmissivity(double lat, double lon) const
{
    return myEnv.getSurfaceEmissivity(lat, lon);
}

double optic::CGlobalEnvironment::getSurfaceRcs(double lat, double lon) const
{
    return myEnv.getSurfaceSRSE(lat, lon);
}


//-----------------------------------------------------------------------------
double optic::CGlobalEnvironment::getHeight(double lat, double lon) const
{
    return myEnv.getHeight(lat, lon);
}