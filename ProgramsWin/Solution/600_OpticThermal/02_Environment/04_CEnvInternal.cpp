//=============================================================================
// Заголовочные файлы
#include "04_CEnvInternal.h"
#include "05_IEnvWrapper.h"
#include "../91_Utility/01_CStorage.h"
namespace optic = SysOpticThermal;


//=============================================================================
// Для работы с динамической библиотекой

typedef IEnvWrapper * (*CreateEnvironment)();
typedef void (*FreeEnvironment)(IEnvWrapper* prt);
# include <windows.h>
# define dll_open(name)             LoadLibrary(TEXT(name".dll")
# define dll_close(handle)          FreeLibrary(handle)
# define func_load(handle, name)    GetProcAddress(handle, name)
//using DllPtr = HMODULE;
typedef HMODULE DllPtr;



//=============================================================================
// Конструктор класса
optic::CEnvInternal::CEnvInternal()
    : myMdv(5.0), myHum(60), myTemp(20.0 + 273)
    , myIsNight(false)
    , mySurfaceTemp(20), mySurfaceRadCoef(0.82), mySurfaceRcs(0.1), mySurfaceType(surface_fields)
    , myFirstCloudType(firstCloudType_none), mySecondCloudType(SecondCloudType_none)
    , myGidrometeor(GIDROMETEOR_NONE)
    , myIsLoaded(false), myDllPtr(NULL), myEnvPtr(NULL)
{
}


//=============================================================================
// Деструктор класса
optic::CEnvInternal::~CEnvInternal()
{
    if (myDllPtr != NULL)
    {
        const FreeEnvironment freeEnv = reinterpret_cast<FreeEnvironment>(func_load(static_cast<DllPtr>(myDllPtr), "FreeEnvironment"));
        if (myEnvPtr != NULL && freeEnv != NULL)
        {
            freeEnv(myEnvPtr);
            myEnvPtr = NULL;
        }

        dll_close(static_cast<DllPtr>(myDllPtr));
    }
}


//=============================================================================
// Загрузка параметров по-умолчанию и инициализация ГПНИ
result optic::CEnvInternal::load(const CDataMap& data, const string& fileName)
{
    // Признак наличия предупреждений при считывании параметров типа системы
    bool wereWarnings = false;      // изначально полагаем, что предупреждений не будет

    // Путь до параметров среды
    const string path = "/environment/параметры_среды/";

    const Utility::CStorage db(data, fileName);

    // Запрос параметров атмосферы
    if (!db.extractDoublePositive(path + "атмосфера/мдв", myMdv, true))
        wereWarnings = true;

    if (!db.extractDoublePositive(path + "атмосфера/относительная_влажность", myHum, true))
        wereWarnings = true;

    if (!db.extractDoubleAny(path + "атмосфера/температура", myTemp, true))
        wereWarnings = true;
    else
        myTemp += 273.0;

    // Запрос параметров поверхности
    if (!db.extractDoubleAny(path + "поверхность/температура", mySurfaceTemp, true))
        wereWarnings = true;
    else
        mySurfaceTemp += 273.0;

    if (!db.extractDoubleAny(path + "поверхность/коэф_излучения", mySurfaceRadCoef, true))
        wereWarnings = true;

    if (!db.extractDoubleAny(path + "поверхность/эпр", mySurfaceRcs, true))
        wereWarnings = true;

    std::string type;
    if (!db.extractString(path + "поверхность/тип", type, true))
        wereWarnings = true;
    else
    {
        if (type == "город")
            mySurfaceType = surface_city;
        else if (type == "дорога")
            mySurfaceType = surface_road;
        else if (type == "железная дорога")
            mySurfaceType = surface_railroad;
        else if (type == "вода")
            mySurfaceType = surface_water;
        else if (type == "лес")
            mySurfaceType = surface_forest;
        else if (type == "поле")
            mySurfaceType = surface_fields;
        else if (type == "снег")
            mySurfaceType = surface_snow;
        else if (type == "пустыня/камни")
            mySurfaceType = surface_desertAndRock;
        else if (type == "промзона")
            mySurfaceType = surface_industrialArea;
    }

    // Запрос параметров облаков
    std::string cloudFirstType;
    if (!db.extractString(path + "облака/первый_тип", cloudFirstType, true))
        wereWarnings = true;
    else
    {
        if (cloudFirstType == "нет")
            myFirstCloudType = firstCloudType_none;
        else if (cloudFirstType == "перистые")
            myFirstCloudType = firstCloudType_cirrus;
        else if (cloudFirstType == "перисто-слоистые")
            myFirstCloudType = firstCloudType_cirrostratus;
        else if (cloudFirstType == "высоко-кучевые")
            myFirstCloudType = firstCloudType_highCumulus;
        else if (cloudFirstType == "высоко-слоистые")
            myFirstCloudType = firstCloudType_highLayered;
        else if (cloudFirstType == "слоисто-кучевые")
            myFirstCloudType = firstCloudType_layeredCumulus;
        else if (cloudFirstType == "слоистые")
            myFirstCloudType = firstCloudType_layered;
        else if (cloudFirstType == "слоисто-дождевые")
            myFirstCloudType = firstCloudType_layeredRain;
        else if (cloudFirstType == "кучево-дождевые")
            myFirstCloudType = firstCloudType_cumulonimus;
    }

    // Запрос параметров облаков
    std::string cloudSecondType;
    if (!db.extractString(path + "облака/второй_тип", cloudSecondType, true))
        wereWarnings = true;
    else
    {
        if (cloudSecondType == "нет")
            mySecondCloudType = SecondCloudType_none;
        else if (cloudSecondType == "безоблачно")
            mySecondCloudType = SecondCloudType_cloudless;
        else if (cloudSecondType == "высокая перистая")
            mySecondCloudType = SecondCloudType_highCirrus;
        else if (cloudSecondType == "средняя кучевая")
            mySecondCloudType = SecondCloudType_avarageCumulus;
        else if (cloudSecondType == "низкая слоистая")
            mySecondCloudType = SecondCloudType_lowLayered;
    }

    // Запрос параметров осадков
    std::string gType;
    if (!db.extractString(path + "осадки/тип", gType, true))
        wereWarnings = true;
    else
    {
        if (gType == "нет")
            myGidrometeor = GIDROMETEOR_NONE;
        else if (gType == "морось")
            myGidrometeor = GIDROMETEOR_MOROS;
        else if (gType == "дождь")
            myGidrometeor = GIDROMETEOR_RAIN;
        else if (gType == "снег")
            myGidrometeor = GIDROMETEOR_SNOW;
        else if (gType == "туман адекватный")
            myGidrometeor = GIDROMETEOR_FOG_ADVECT;
        else if (gType == "туман радиационный")
            myGidrometeor = GIDROMETEOR_FOG_RADIAT;
    }

    // Запрос времени суток
    std::string nType;
    if (!db.extractString(path + "время_суток", nType, true))
        wereWarnings = true;
    else
    {
        if (nType == "день")
            myIsNight = false;
        else if (nType == "ночь")
            myIsNight = true;
    }

    // Считывание параметров завершено
    if (wereWarnings == true)
        return RESULT_WARNING;
    return RESULT_OK;
}


//-----------------------------------------------------------------------------
bool optic::CEnvInternal::isLoaded() const
{
    return myIsLoaded;
}


//=============================================================================
// Доступ к параметрам атмосферы
double optic::CEnvInternal::getMeteoVisibility() const
{
    return myIsLoaded ? myEnvPtr->getMeteoRange() : myMdv;
}


//-----------------------------------------------------------------------------
double optic::CEnvInternal::getRelativeHumidity() const
{
    return myIsLoaded ? myEnvPtr->getRelativeHumidity() : myHum;
}


//-----------------------------------------------------------------------------
double optic::CEnvInternal::getTemperature(double lat, double lon, double height) const
{
    if (myIsLoaded)
    {
        double delta, temp;
        if (myEnvPtr->getTemperature(lat, lon, height, temp, delta))
            return double(temp);
    }
    return myTemp;
}


//=============================================================================
// Доступ к информации об облачности
FirstCloudType optic::CEnvInternal::getFirstTypeOfCloud() const
{
    if (myIsLoaded)
        return static_cast<FirstCloudType>(myEnvPtr->getTypeOfCloudFromFirstTable());
    return myFirstCloudType;
}


//----------------------------------------------------------------------------- 
SecondCloudType optic::CEnvInternal::getSecondTypeOfCloud() const
{
    if (myIsLoaded)
        return static_cast<SecondCloudType>(myEnvPtr->getTypeOfCloudFromSecondTable());
    return mySecondCloudType;
}


//=============================================================================
// Доступ к информации о погоде
gidrometeorEnum optic::CEnvInternal::getGidrometeor() const
{
    if (myIsLoaded)
        return static_cast<gidrometeorEnum>(myEnvPtr->getGidrometeor());
    return myGidrometeor;
}

std::pair<int, int> optic::CEnvInternal::getSunrise(double lat, double lon) const
{
    int hour = 6;
    int minute = 0;
    if (myIsLoaded)
    {
        const double sunrise = myEnvPtr->getSunrise(lat, lon);
        myEnvPtr->TimeFromDoubleToPairIntInt(sunrise, hour, minute);
    }

    return std::make_pair(hour, minute);
}

std::pair<int, int> optic::CEnvInternal::getSunset(double lat, double lon) const
{
    int hour = 18;
    int minute = 0;
    if (myIsLoaded)
    {
        const double sunrise = myEnvPtr->getSunset(lat, lon);
        myEnvPtr->TimeFromDoubleToPairIntInt(sunrise, hour, minute);
    }

    return std::make_pair(hour, minute);
}

double optic::CEnvInternal::getSunAzimuth(double lat, double lon, double second) const
{
    if (myIsLoaded)
        return myEnvPtr->getSunAz(lat, lon, second);
    return mySunAzimuth;
}

double optic::CEnvInternal::getSunElevation(double lat, double lon, double second) const
{
    if (myIsLoaded)
        return myEnvPtr->getSunAlt(lat, lon, second);
    return mySunElevation;
}

bool optic::CEnvInternal::getIsNight(double lat, double lon) const
{
    if (myIsLoaded)
    {
        const std::pair<int, int> sunrise = getSunrise(lat, lon);
        const std::pair<int, int> sunset = getSunset(lat, lon);
        if (sunset >= sunrise)
        {
            const std::pair<int, int> curTime = getCurTime();
            if (curTime >= sunrise && curTime <= sunset)
                return false;
            return true;
        }
    }
    return myIsNight;
}

std::pair<int, int> optic::CEnvInternal::getCurTime() const
{
    int hour = 12;
    int minute = 0;
    if (myIsLoaded)
        myEnvPtr->getCurTime(hour, minute);
    return std::make_pair(hour, minute);
}


//=============================================================================
// Доступ к информации о поверхности
double optic::CEnvInternal::getSurfaceTemperature(double lat, double lon) const
{
    if (myIsLoaded)
    {
        double delta, temp;
        if (myEnvPtr->getEarthTemperature(lat, lon, temp, delta))
            return double(temp);
    }
    return mySurfaceTemp;
}


//-----------------------------------------------------------------------------
TypeSurface optic::CEnvInternal::getSurfaceType(double lat, double lon) const
{
    if (myIsLoaded)
        return static_cast<TypeSurface>(myEnvPtr->getSurfaceType(lat, lon));
    return mySurfaceType;
}


//-----------------------------------------------------------------------------
double optic::CEnvInternal::getSurfaceEmissivity(double lat, double lon) const
{
    if (myIsLoaded)
        return myEnvPtr->getSurfaceEmissivity(lat, lon);
    return mySurfaceRadCoef;
}


//-----------------------------------------------------------------------------
double optic::CEnvInternal::getSurfaceSRSE(double lat, double lon) const
{
    return myIsLoaded ? myEnvPtr->getSurfaceSRSE(lat, lon) : mySurfaceRcs;
}


//-----------------------------------------------------------------------------
double optic::CEnvInternal::getHeight(double lat, double lon) const
{
    return myIsLoaded ? myEnvPtr->getHeight(lat, lon) : 0.0;
}
