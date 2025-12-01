#ifndef _600_OpticThermal_System_CSysOpticThermalType_h
#define _600_OpticThermal_System_CSysOpticThermalType_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "../02_Environment/02_CGlobalEnvironment.h"


namespace SysOpticThermal
{
    //=====================================================================================================================
    //  Описание нового типа тепловизионных систем
    class CSysOpticThermalType: public CSysType
    {
    public:
        //-----------------------------------------------------------------------------------------------------------------
        enum ESpectrumRange
        {
            WaveLen_3_5,
            WaveLen_8_14
        };
        enum EMode
        {
            Detection,
            Detection_Follow_Wide,
            Detection_Follow_Narrow,
            Recognition
        };

    public:
        static CGlobalEnvironment MyGlobalEnv;  // Глобальные параметры среды

    private:
        //-----------------------------------------------------------------------------------------------------------------
        ESpectrumRange mySpectrumRange;         // Рабочий диапазон
        EMode myOperationMode;                  // Режим работы системы
        double myDetAzimuthSector;              // Поле зрения по азимуту (обнаружение) (град.)
        double myDetElevationSector;            // Поле зрения по углу места (обнаружение) (град.)
        double myRecAzimuthSector;              // Поле зрения по азимуту (распознавание) (град.)
        double myRecElevationSector;            // Поле зрения по углу места (распознавание) (град.)
        std::pair<double, double> myResolution; // Разрешение матрицы (Ш х В, пикс.)
        double myFillCoef;                      // Коэффициент заполнения
        double myFps;                           // Частота кадров
        double myLensDiff;                      // Дифракция объектива
        double myNoiseTemp;                     // Шумовая температура
        
        double myJohnosonDetIdle;               // Критерий Джонсона для обнаружения неподвижного объекта
        double myJohnosonDetMoving;             // Критерий Джонсона для обнаружения движущегося объекта
        double myJohnosonRec;                   // Критерий Джонсона для распознавания

        double myOperatorSkill;                 // Квалификация оператора
        double myLightVal;                      // Уровень освещенности

        double myDeltaTime;                     // Шаг моделирования

        bool myIsScanEnabled;                   // Признак включения продольного сканирования местности
        double myMinScanDistance;               // Минимальная дальность при сканировании
        double myMaxScanDistance;               // Минимальная дальность при сканировании

        double myTurnAngle;                     // Максимальный угол поворота оптической системы по азимуту

        double myOrientationHeight;             // Рабочая высота носителя
        double myOrientationMinD;               // Рабочая минимальная дальность носителя
        double myOrientationMaxD;               // Рабочая максимальная дальность носителя

        double myMinUpdateInterval;             // Шаг передачи информации о целях

        bool myIsDebugMsg;                      // Флаг вывода отладочных сообщений


    public:
        //-----------------------------------------------------------------------------------------------------------------
        // Конструктор класса
        CSysOpticThermalType();


        //-----------------------------------------------------------------------------------------------------------------
        // Деструктор класса
        virtual ~CSysOpticThermalType();


    public:
        //-----------------------------------------------------------------------------------------------------------------
        // Загрузка информации о типе систем из текстового файла
        virtual result load(const CDataMap& data, const string& fileName);


    public:
        //-----------------------------------------------------------------------------------------------------------------
        // Создание экземпляра системы данного типа
        virtual CSys* createSystem(CObject& object) const;


    public:
        //-----------------------------------------------------------------------------------------------------------------
        // Доступ к характеристикам системы
        ESpectrumRange getSpectrumRange() const { return mySpectrumRange; }
        EMode getOperationMode() const { return myOperationMode; }

        double getDetectionAzimuth() const { return myDetAzimuthSector; }
        double getDetectionElevation() const { return myDetElevationSector; }
        double getRecognitionAzimuth() const { return myRecAzimuthSector; }
        double getRecognitionElevation() const { return myRecElevationSector; }
        std::pair<double, double> getResolution() const { return myResolution; }
        double getFillCoef() const { return myFillCoef; }
        double getFps() const { return myFps; }
        double getLensDiff() const { return myLensDiff; }
        double getNoiseTemp() const { return myNoiseTemp; }
        double getJohnosonDetIdle() const { return myJohnosonDetIdle; }
        double getJohnosonDetMoving() const { return myJohnosonDetMoving; }
        double getJohnosonRec() const { return myJohnosonRec; }
        double getOperatorSkill() const { return myOperatorSkill; }
        double getLightVal() const { return myLightVal; }
        double getDeltaTime() const { return myDeltaTime; }
        double getMinUpdateInterval() const { return myMinUpdateInterval; }

        bool isScanEnabled() const { return myIsScanEnabled; }
        double getMinScanDistance() const { return myMinScanDistance; }
        double getMaxScanDistance() const { return myMaxScanDistance; }

        double getTurnAngle() const { return myTurnAngle; }
        double getOrientationHeight() const { return myOrientationHeight; }
        double getOrientationMaxD() const { return myOrientationMaxD; }
        double getOrientationMinD() const { return myOrientationMinD; }

        bool isDebugMsg() const { return myIsDebugMsg; }

        //-----------------------------------------------------------------------------------------------------------------
    };


    //=====================================================================================================================
}


//=====================================================================================================================
#endif