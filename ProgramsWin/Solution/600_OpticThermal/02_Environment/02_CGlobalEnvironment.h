// Файл 02_CGlobalEnviroment.h
// Информация о внешних условиях для оптической станции
//

#ifndef _600_OpticThermal_Environment_CGlobalEnvironment_h
#define _600_OpticThermal_Environment_CGlobalEnvironment_h



//=============================================================================
// Заголовочные файлы
#include "03_CEarth.h"
#include "04_CEnvInternal.h"
#include "10_EEnvTypes.h"
#include "../Settings.h"


namespace SysOpticThermal
{
    //=============================================================================
    // Класс "Параметры окружающей среды"
    class CGlobalEnvironment
    {
    private:
        //-------------------------------------------------------------------------
        CEarth myEarth;             // Параметры Земли
        CEnvInternal myEnv;         // Параметры среды


    public:
        //-------------------------------------------------------------------------
        // Загрузка внешних условий из оперативной базы данных
        result load(const CDataMap& data, const string& fileName);
        result loadEnv(const CDataMap& data, const string& fileName);


    public:
        //-------------------------------------------------------------------------
        // Доступ к параметрам Земли
        const CEarth & getEarth() const;

        
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
        gidrometeorEnum getGidrometeor() const;


        //-------------------------------------------------------------------------
        // Доступ к информации о солнце
        bool getIsNight(double lat, double lon) const;


        //-------------------------------------------------------------------------
        // Доступ к информации о поверхности
        double getSurfaceTemperature(double lat, double lon) const;
        TypeSurface getSurfaceType(double lat, double lon) const;
        double getSurfaceEmissivity(double lat, double lon) const;
        double getSurfaceRcs(double lat, double lon) const;
        double getHeight(double lat, double lon) const;


        //-------------------------------------------------------------------------
    };


    //=============================================================================
}


//=============================================================================
#endif
