//=============================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CEnvironment.h"
#include "../91_Utility/01_CStorage.h"

namespace optic = SysOpticThermal;

double optic::CEnvironment::MyHumidityTable[][21]= {
    {-25, 10, 0.1, 20,  0.1, 30,  0.2, 40,  0.2, 50,  0.3, 60,  0.3, 70,  0.4, 80,  0.4, 90,  0.5, 100,  0.6},
    {-20, 10, 0.1, 20,  0.2, 30,  0.3, 40,  0.4, 50,  0.4, 60,  0.5, 70,  0.6, 80,  0.7, 90,  0.8, 100,  0.9},
    {-15, 10, 0.2, 20,  0.3, 30,  0.5, 40,  0.6, 50,  0.8, 60,  1.0, 70,  1.1, 80,  1.3, 90,  1.5, 100,  1.6},
    {-10, 10, 0.2, 20,  0.5, 30,  0.7, 40,  0.9, 50,  1.2, 60,  1.4, 70,  1.6, 80,  1.9, 90,  2.1, 100,  2.3},
    { -5, 10, 0.3, 20,  0.7, 30,  1.0, 40,  1.4, 50,  1.7, 60,  2.1, 70,  2.4, 80,  2.7, 90,  3.1, 100,  3.4},
    {  0, 10, 0.5, 20,  1.0, 30,  1.5, 40,  1.9, 50,  2.4, 60,  2.9, 70,  3.4, 80,  3.9, 90,  4.4, 100,  4.8},
    {  5, 10, 0.7, 20,  1.4, 30,  2.0, 40,  2.7, 50,  3.4, 60,  4.1, 70,  4.8, 80,  5.4, 90,  6.1, 100,  6.8},
    { 10, 10, 0.9, 20,  1.9, 30,  2.8, 40,  3.8, 50,  4.7, 60,  5.6, 70,  6.6, 80,  7.5, 90,  8.5, 100,  9.4},
    { 15, 10, 1.3, 20,  2.6, 30,  3.9, 40,  5.1, 50,  6.4, 60,  7.7, 70,  9.0, 80, 10.3, 90, 11.5, 100, 12.8},
    { 20, 10, 1.7, 20,  3.5, 30,  5.2, 40,  6.9, 50,  8.7, 60, 10.4, 70, 12.1, 80, 13.8, 90, 15.6, 100, 17.3},
    { 25, 10, 2.3, 20,  4.6, 30,  6.9, 40,  9.2, 50, 11.5, 60, 13.8, 70, 16.1, 80, 18.4, 90, 20.7, 100, 23.0},
    { 30, 10, 3.0, 20,  6.1, 30,  9.1, 40, 12.1, 50, 15.2, 60, 18.2, 70, 21.3, 80, 24.3, 90, 27.3, 100, 30.4},
    { 35, 10, 4.0, 20,  7.9, 30, 11.9, 40, 15.8, 50, 19.8, 60, 23.8, 70, 27.7, 80, 31.7, 90, 35.6, 100, 39.6},
    { 40, 10, 5.1, 20, 10.2, 30, 15.3, 40, 20.5, 50, 25.6, 60, 30.7, 70, 35.8, 80, 40.9, 90, 46.0, 100, 51.1},
    { 45, 10, 6.5, 20, 13.1, 30, 19.6, 40, 26.2, 50, 32.7, 60, 39.3, 70, 45.8, 80, 52.4, 90, 58.9, 100, 65.4},
    { 50, 10, 8.3, 20, 16.6, 30, 24.9, 40, 33.2, 50, 41.5, 60, 49.8, 70, 58.1, 80, 66.4, 90, 74.8, 100, 83.0},
};
std::map<optic::CEnvironment::ExtCoefKey, double> optic::CEnvironment::MyTblCoefTable;


//=============================================================================
// Конструктор класса
optic::CEnvironment::CEnvironment()
    : mySunAngle(45), myPrec(Precipitation::None), myIsNight(false)
    , myObjTemp(22), myObjRadCoef3_5(0.74), myObjRadCoef8_14(0.93), myObjTblCoef(4)
    , myObjSunPos(1), myObjSurface(1)
    , myBgTemp(20), myBgRadCoef3_5(0.82), myBgRadCoef8_14(0.91), myBgTblCoef(12)
    , myBgSunPos(1), myBgSurface(0), myBgAngle(3.2)
    , myIsLoaded(false)
{
}


double optic::CEnvironment::getAbsHumidity(const double hum, const double temp)
{
    for (int i = 0; i < sizeof(MyHumidityTable); i++)
    {
        if (MyHumidityTable[i][0] >= temp)
        {
            for (int j = 1; j < 10 * 2; j += 2)
            {
                if (MyHumidityTable[i][j] >= hum)
                    return MyHumidityTable[i][j + 1];
            }
        }
    }
    return 10.4;
}

void optic::CEnvironment::_initExteralCoefMap()
{
    if (!MyTblCoefTable.empty())
        return;

    // Открытая местность
    //// Горизонтальная поверхность
    ////// 3-5 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::None,
                    Terrain::Open,
                    Surface::Horizontal ), 24.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Ci,
                    Terrain::Open,
                    Surface::Horizontal ), 17.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Cu,
                    Terrain::Open,
                    Surface::Horizontal ), 12.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::St,
                    Terrain::Open,
                    Surface::Horizontal ), 5.0));

    ////// 8-12 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::None,
                    Terrain::Open,
                    Surface::Horizontal ), 42.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Ci,
                    Terrain::Open,
                    Surface::Horizontal ), 25.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Cu,
                    Terrain::Open,
                    Surface::Horizontal ), 15.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::St,
                    Terrain::Open,
                    Surface::Horizontal ), 5.0));

    //// Вертикальная поверхность
    ////// 3-5 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::None,
                    Terrain::Open,
                    Surface::Vertical ), 13.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Ci,
                    Terrain::Open,
                    Surface::Vertical ), 8.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Cu,
                    Terrain::Open,
                    Surface::Vertical ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::St,
                    Terrain::Open,
                    Surface::Vertical ), 2.0));

    ////// 8-12 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::None,
                    Terrain::Open,
                    Surface::Vertical ), 17.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Ci,
                    Terrain::Open,
                    Surface::Vertical ), 9.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Cu,
                    Terrain::Open,
                    Surface::Vertical ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::St,
                    Terrain::Open,
                    Surface::Vertical ), 2.0));

    // Лесная поляна
    //// Горизонтальная поверхность
    ////// 3-5 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::None,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 14.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Ci,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 12.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Cu,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 9.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::St,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 4.0));

    ////// 8-12 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::None,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 18.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Ci,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 15.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Cu,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 10.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::St,
                    Terrain::ForestClearing,
                    Surface::Horizontal ), 4.0));

    //// Вертикальная поверхность
    ////// 3-5 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::None,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey(SpectrumRange::WaveLen_3_5,
                    Clouds::Ci,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Cu,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 3.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::St,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 2.0));

    ////// 8-12 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::None,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Ci,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Cu,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 3.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::St,
                    Terrain::ForestClearing,
                    Surface::Vertical ), 2.0));

    // Среди деревьев
    //// Горизонтальная поверхность
    ////// 3-5 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::None,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 7.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Ci,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 5.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Cu,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::St,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 3.0));

    ////// 8-12 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::None,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 7.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Ci,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 5.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Cu,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 4.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::St,
                    Terrain::ForestTrees,
                    Surface::Horizontal ), 3.0));

    //// Вертикальная поверхность
    ////// 3-5 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::None,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 2.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Ci,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 2.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::Cu,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 2.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_3_5,
                    Clouds::St,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 1.5));

    ////// 8-12 мкм
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::None,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 2.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Ci,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 2.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::Cu,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 2.0));
    MyTblCoefTable.insert(make_pair(
        ExtCoefKey( SpectrumRange::WaveLen_8_14,
                    Clouds::St,
                    Terrain::ForestTrees,
                    Surface::Vertical ), 1.5));
}

double optic::CEnvironment::_getTblCoef(const SpectrumRange::type srange, const Surface::type surface, TypeSurface ter, SecondCloudType cl) const
{
    Clouds::type clouds;
    Terrain::type terrain;

    switch (ter)
    {
    case surface_railroad:
    case surface_industrialArea:
        terrain = Terrain::ForestClearing;
        break;
    case surface_forest:
        terrain = Terrain::ForestTrees;
        break;
    default:
        terrain = Terrain::Open;
    }

    switch (cl)
    {
    case SecondCloudType_highCirrus:
        clouds = Clouds::Ci;
        break;
    case SecondCloudType_avarageCumulus:
        clouds = Clouds::Cu;
        break;
    case SecondCloudType_lowLayered:
        clouds = Clouds::St;
        break;
    default:
        clouds = Clouds::None;
    }

    map<ExtCoefKey, double>::const_iterator tblIt = MyTblCoefTable.find(ExtCoefKey(srange, clouds, terrain, surface ));
    if (tblIt != MyTblCoefTable.end())
        return tblIt->second;
    return HUGE_VAL;
}


//=============================================================================
// Загрузка внешних условий из оперативной базы данных
result optic::CEnvironment::load(const CDataMap& data, const string& fileName)
{
    if (myIsLoaded)
        return RESULT_OK;
    myIsLoaded = true;

    _initExteralCoefMap();

    bool wereWarnings = false;                      // Признак наличия предупреждений при считывании параметров
    const std::string envPath = "environment/параметры_тпоэс/";
    std::string path;

    Utility::CStorage db(data, fileName);

    path = envPath + "объект/температура";
    if (!db.extractDoubleAny(path, myObjTemp))
        return RESULT_ERROR;
    myObjTemp += 273.0;

    path = envPath + "объект/коэф_излучения_3_5_мкм";
    if (!db.extractDoubleAny(path, myObjRadCoef3_5))
        return RESULT_ERROR;

    path = envPath + "объект/коэф_излучения_8_14_мкм";
    if (!db.extractDoubleAny(path, myObjRadCoef8_14))
        return RESULT_ERROR;

    path = envPath + "объект/положение";
    std::string sunPos;
    if (db.extractString(path, sunPos))
    {
        if (sunPos == "на солнце")
            myObjSunPos = 1;
        else if (sunPos == "в тени")
            myObjSunPos = 0;
        else
        {
            Log << "    Ошибка в " << fileName << ": " <<
                "неверное значение поля \"" + path + "\". "
                "Возможные значения: 'на солнце'/'в тени'. "
                "Прочитанное значение: \"" + sunPos + "\"" << EndL;
            return RESULT_ERROR;
        }
    }
    else
        return RESULT_ERROR;

    path = envPath + "объект/поверхность";
    std::string surface;
    if (db.extractString(path, surface))
    {
        if (surface == "горизонтальная")
            myObjSurface = 0;
        else if (surface == "вертикальная")
            myObjSurface = 1;
        else
        {
            Log << "    Ошибка в " << fileName << ": " <<
                "неверное значение поля \"" + path + "\". "
                "Возможные значения: 'горизонтальная'/'вертикальная'. "
                "Прочитанное значение: \"" + surface + "\"" << EndL;
            return RESULT_ERROR;
        }
    }
    else
        return RESULT_ERROR;

    path = envPath + "объект/табличный_коэф";
    if (!db.extractDoubleAny(path, myObjTblCoef))
        return RESULT_ERROR;

    path = envPath + "фон/температура";
    if (!db.extractDoubleAny(path, myBgTemp))
        return RESULT_ERROR;
    myObjTemp += 273.0;

    path = envPath + "фон/коэф_излучения_3_5_мкм";
    if (!db.extractDoubleAny(path, myBgRadCoef3_5))
        return RESULT_ERROR;

    path = envPath + "фон/коэф_излучения_8_14_мкм";
    if (!db.extractDoubleAny(path, myBgRadCoef8_14))
        return RESULT_ERROR;

    path = envPath + "фон/положение";
    std::string bgSunPos;
    if (db.extractString(path, bgSunPos))
    {
        if (bgSunPos == "на солнце")
            myBgSunPos = 1;
        else if (bgSunPos == "в тени")
            myBgSunPos = 0;
        else
        {
            Log << "    Ошибка в " << fileName << ": " <<
                "неверное значение поля \"" + path + "\". "
                "Возможные значения: 'на солнце'/'в тени'. "
                "Прочитанное значение: \"" + bgSunPos + "\"" << EndL;
            return RESULT_ERROR;
        }
    }
    else
        return RESULT_ERROR;

    path = envPath + "фон/поверхность";
    std::string bgSurface;
    if (db.extractString(path, bgSurface))
    {
        if (surface == "горизонтальная")
            myBgSurface = 0;
        else if (surface == "вертикальная")
            myBgSurface = 1;
        else
        {
            Log << "    Ошибка в " << fileName << ": " <<
                "неверное значение поля \"" + path + "\". "
                "Возможные значения: 'горизонтальная'/'вертикальная'. "
                "Прочитанное значение: \"" + bgSurface + "\"" << EndL;
            return RESULT_ERROR;
        }
    }
    else
        return RESULT_ERROR;

    path = envPath + "фон/табличный_коэф";
    if (!db.extractDoubleAny(path, myBgTblCoef))
        return RESULT_ERROR;

    // Считывание параметров завершено
    if (wereWarnings == true)
        return RESULT_WARNING;
    return RESULT_OK;
}

optic::Precipitation::type optic::CEnvironment::getPrecipitationType(gidrometeorEnum g)
{
    Precipitation::type prec;
    switch (g)
    {
    case GIDROMETEOR_MOROS:
        prec = Precipitation::Drizzle;
        break;
    case GIDROMETEOR_RAIN:
        prec = Precipitation::Rain;
        break;
    case GIDROMETEOR_SNOW:
        prec = Precipitation::Snow;
        break;
    case GIDROMETEOR_FOG_ADVECT:
    case GIDROMETEOR_FOG_RADIAT:
        prec = Precipitation::Fog;
        break;
    default:
        prec = Precipitation::None;
    }

    return prec;
}

double optic::CEnvironment::getObjTblCoef(const SpectrumRange::type srange, TypeSurface ter, SecondCloudType cl) const
{
    const double value = _getTblCoef(srange, Surface::Vertical, ter, cl);
    if (value != HUGE_VAL)
        return value;
    return myObjTblCoef;
}

double optic::CEnvironment::getBgTblCoef(const SpectrumRange::type srange, TypeSurface ter, SecondCloudType cl) const
{
    const double value = _getTblCoef(srange, Surface::Horizontal, ter, cl);
    if (value != HUGE_VAL)
        return value;
    return myBgTblCoef;
}
