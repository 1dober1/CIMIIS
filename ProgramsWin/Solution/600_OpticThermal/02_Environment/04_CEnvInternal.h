#ifndef _600_OpticThermal_Environment_CEnvInternal_h
#define _600_OpticThermal_Environment_CEnvInternal_h

//=============================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "10_EEnvTypes.h"

class IEnvWrapper;

namespace SysOpticThermal
{
    //=============================================================================
    // Класс "Параметры среды"
    class CEnvInternal
    {
    private:
        //-------------------------------------------------------------------------
        // Параметры ГПНИ
        void* myDllPtr;             // Указатель на библиотеку ГПНИ
        IEnvWrapper* myEnvPtr;      // Интерфейс для доступа к данным ГПНИ
        bool myIsLoaded;            // Флаг успешной инициализации ГПНИ


        //-------------------------------------------------------------------------
        // Параметры по-умолчанию
        // Атмосфера
        double myMdv;                       // Метеорологическая дальность видимости (км)
        double myHum;                       // Относительная влажность (%)
        double myTemp;                      // Температура атмосферы (К)

        // Солнце
        double mySunAzimuth;                // Высота Солнца над горизонтом по азимуту (град.)
        double mySunElevation;              // Высота Солнца над горизонтом по углу-места (град.)
        bool myIsNight;                     // Признак ночи

        // Поверхность
        double mySurfaceTemp;               // Температура фона
        double mySurfaceRadCoef;            // Коэффициент излучения фона
        double mySurfaceRcs;                // ЭПР поверхности
        TypeSurface mySurfaceType;          // Тип поверхности

        // Облака
        FirstCloudType myFirstCloudType;    // Тип облаков
        SecondCloudType mySecondCloudType;  // Еще Тип облаков

        // Осадки
        gidrometeorEnum myGidrometeor;      // Тип осадков



    public:
        //-------------------------------------------------------------------------
        // Конструктор класса
        CEnvInternal();

        // Деструктор класса
        virtual ~CEnvInternal();


        //-------------------------------------------------------------------------
        // Загрузка параметров по-умолчанию и инициализация ГПНИ
        result load(const CDataMap& data, const string& fileName);
        bool isLoaded() const;

        //-------------------------------------------------------------------------
        // Доступ к параметрам атмосферы
        double getMeteoVisibility() const;
        double getRelativeHumidity() const;
        double getTemperature(double lat, double lon, double height) const;
               
        //-------------------------------------------------------------------------
        // Доступ к информации об облачности
        FirstCloudType getFirstTypeOfCloud() const;
        SecondCloudType getSecondTypeOfCloud() const;
       
        //-------------------------------------------------------------------------
        // Доступ к информации о погоде
        // Тип осадков
        gidrometeorEnum getGidrometeor() const;
                
        //-------------------------------------------------------------------------
        // Доступ к информации о солнце
        std::pair<int, int> getSunrise(double lat, double lon) const;
        std::pair<int, int> getSunset(double lat, double lon) const;
        double getSunAzimuth(double lat, double lon, double second) const;
        double getSunElevation(double lat, double lon, double second) const;
        bool getIsNight(double lat, double lon) const;

        //-------------------------------------------------------------------------
        // Доступ к текущему времени
        std::pair<int, int> getCurTime() const;

        //-------------------------------------------------------------------------
        // Доступ к информации о поверхности
        double getSurfaceTemperature(double lat, double lon) const;
        TypeSurface getSurfaceType(double lat, double lon) const;
        double getSurfaceEmissivity(double lat, double lon) const;
        double getSurfaceSRSE(double lat, double lon) const;
        double getHeight(double lat, double lon) const;


        //-------------------------------------------------------------------------
    };

}


//=============================================================================
#endif
