//=============================================================================
// Заголовочные файлы
#include "01_CSysOpticThermalType.h"
#include "02_CSysOpticThermal.h"
#include "../91_Utility/01_CStorage.h"

namespace optic = SysOpticThermal;

optic::CGlobalEnvironment optic::CSysOpticThermalType::MyGlobalEnv;

//=============================================================================
// Конструктор класса
optic::CSysOpticThermalType::CSysOpticThermalType()
    : myOrientationHeight(0.0)
    , myOrientationMinD(0.0)
    , myTurnAngle(40.0)
    , myIsScanEnabled(false), myMinScanDistance(0.0)
    , myIsDebugMsg(false)
{
}


//=============================================================================
// Деструктор класса
optic::CSysOpticThermalType::~CSysOpticThermalType()
{
}


//=====================================================================================================================
// Загрузка информации о типе системы из текстового файла
result optic::CSysOpticThermalType::load(const CDataMap& data, const string& fileName)
{
    // Признак наличия предупреждений при считывании параметров типа системы
    bool wereWarnings = false;      // изначально полагаем, что предупреждений не будет
    std::string path;

    // Считывание общей информации, присутствующей в любом файле с описанием типа системы
    if (CSysType::load(data, fileName) == RESULT_ERROR)
        return RESULT_ERROR;

    myDeltaTime = getTimeStep();

    MyGlobalEnv.load(data, fileName);

    Utility::CStorage db(data, fileName);

    path = "/рабочий_диапазон";
    std::string spectrumRange;
    if (!db.extractString(path, spectrumRange))
        return RESULT_ERROR;
    if (spectrumRange == "3-5")
        mySpectrumRange = WaveLen_3_5;
    else if (spectrumRange == "8-14")
        mySpectrumRange = WaveLen_8_14;
    else
    {
        Log << "    Ошибка в " << fileName << ": " <<
            "неверное значение поля \"" + path + "\". "
            "Возможные значения: 3-5/8-14. "
            "Прочитанное значение: \"" + spectrumRange + "\"" << EndL;
        return RESULT_ERROR;
    }

    path = "/режим_работы";
    std::string operationMode;
    if (!db.extractString("/режим_работы", operationMode))
        return RESULT_ERROR;
    if (operationMode == "обнаружение")
        myOperationMode = Detection;
    else if (operationMode == "обнаружение_следование_широкий")
        myOperationMode = Detection_Follow_Wide;
    else if (operationMode == "обнаружение_следование_узкий")
        myOperationMode = Detection_Follow_Narrow;
    else if (operationMode == "распознавание")
        myOperationMode = Recognition;
    else
    {
        Log << "    Ошибка в " << fileName << ": " <<
            "неверное значение параметра \"режим_работы\". "
            "Возможные значения: обнаружение/обнаружение_следование_широкий/обнаружение_следование_узкий/распознавание. "
            "Прочитанное значение: \"" + operationMode + "\"" << EndL;
        return RESULT_ERROR;
    }

    path = "/поле_зрения/обнаружение/азимут";
    if (!db.extractDegreesPositive(path, myDetAzimuthSector))
        return RESULT_ERROR;

    path = "/поле_зрения/обнаружение/угол_места";
    if (!db.extractDegreesPositive(path, myDetElevationSector))
        return RESULT_ERROR;

    path = "/поле_зрения/обнаружение/ориентация/высота";
    if (!db.extractDoubleAny(path, myOrientationHeight, true))
        wereWarnings = true;

    path = "/поле_зрения/обнаружение/ориентация/максимальная_дальность";
    if (!db.extractDoublePositive(path, myOrientationMaxD, true))
        return RESULT_ERROR;

    path = "/поле_зрения/обнаружение/ориентация/минимальная_дальность";
    if (!db.extractDoublePositive(path, myOrientationMinD, true))
        wereWarnings = true;

    path = "/поле_зрения/распознавание/азимут";
    if (!db.extractDegreesPositive(path, myRecAzimuthSector))
        return RESULT_ERROR;

    path = "/поле_зрения/распознавание/угол_места";
    if (!db.extractDegreesPositive( path, myRecElevationSector))
        return RESULT_ERROR;

    path = "/поле_зрения/распознавание/поворот";
    if (!db.extractDegreesPositive(path, myTurnAngle, true))
        wereWarnings = true;

    path = "/разрешение/высота";
    double height;
    if (!db.extractDoublePositive(path, height, true))
        return RESULT_ERROR;

    path = "/разрешение/ширина";
    double width;
    if (!db.extractDoublePositive(path, width, true))
        return RESULT_ERROR;

    myResolution = make_pair(width, height);

    path = "/коэффициент_заполнения";
    if (!db.extractDoubleAny(path, myFillCoef))
        return RESULT_ERROR;

    path = "/частота_кадров";
    if (!db.extractDoubleAny(path, myFps, false))
        return RESULT_ERROR;

    path = "/дифракция_объектива";
    if (!db.extractDoubleAny(path, myLensDiff))
        return RESULT_ERROR;

    path = "/шумовая_температура";
    if (!db.extractDoubleAny(path, myNoiseTemp))
        return RESULT_ERROR;

    path = "/критерий_джонсона/обнаружение_неподвижный";
    if (!db.extractDoubleAny(path, myJohnosonDetIdle))
        return RESULT_ERROR;

    path = "/критерий_джонсона/обнаружение_движущийся";
    if (!db.extractDoubleAny(path, myJohnosonDetMoving))
        return RESULT_ERROR;

    path = "/критерий_джонсона/распознавание";
    if (!db.extractDoubleAny(path, myJohnosonRec))
        return RESULT_ERROR;

    path = "/квалификация_оператора";
    if (!db.extractDoubleAny(path, myOperatorSkill))
        return RESULT_ERROR;

    path = "/освещение";
    if (!db.extractDoubleAny(path, myLightVal))
        return RESULT_ERROR;

    path = "/мин_интервал_обновления_информации";
    if (!db.extractDoublePositive(path, myMinUpdateInterval, true))
        return RESULT_ERROR;

    path = "/освещение";
    if (!db.extractDoubleAny(path, myLightVal))
        return RESULT_ERROR;

    path = "/режим_сканирования_местности";
    db.extractBool(path, myIsScanEnabled, true);
    
    if (myIsScanEnabled)
    {
        path = "/сканирование_местности/минимальная_дальность";
        if (!db.extractDoublePositive(path, myMinScanDistance, true))
            wereWarnings = true;

        path = "/сканирование_местности/максимальная_дальность";
        if (!db.extractDoublePositive(path, myMaxScanDistance, true))
            wereWarnings = true;

        if (myMaxScanDistance < myMinScanDistance)
        {
            Log << "    Ошибка в " << fileName << ": " <<
                "значение параметра \"сканирование_местности/максимальная_дальность\", должно быть больше "
                "значения параметра \"сканирование_местности/минимальная_дальность\"" << EndL;
            return RESULT_ERROR;
        }
    }

  
    // Считывание параметров завершено
    if (wereWarnings == true)
        return RESULT_WARNING;
    return RESULT_OK;
}

//=====================================================================================================================
// Создание экземпляра системы данного типа
CSys* optic::CSysOpticThermalType::createSystem(CObject& object) const
{
    return new CSysOpticThermal(*this, object);
}
