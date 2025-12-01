#ifndef _600_OpticThermal_Environment_CEnvironment_h
#define _600_OpticThermal_Environment_CEnvironment_h


//=============================================================================
// Заголовочные файлы
#include "../01_Calc/11_EnumClasses.h"
#include "10_EEnvTypes.h"

namespace SysOpticThermal
{
    //=============================================================================
    // Класс "Параметры окружающей среды"
    class CEnvironment
    {
    private:
        struct ExtCoefKey
        {
            SpectrumRange::type srtype;
            Clouds::type ctype;
            Terrain::type ttype;
            Surface::type stype;

            ExtCoefKey(SpectrumRange::type sr, Clouds::type c, Terrain::type t, Surface::type s)
                : srtype(sr), ctype(c), ttype(t), stype(s) {}
            friend bool operator<(const ExtCoefKey& lhs, const ExtCoefKey& rhs)
            {
                if (lhs.srtype < rhs.srtype)
                    return true;
                if (rhs.srtype < lhs.srtype)
                    return false;
                if (lhs.ctype < rhs.ctype)
                    return true;
                if (rhs.ctype < lhs.ctype)
                    return false;
                if (lhs.ttype < rhs.ttype)
                    return true;
                if (rhs.ttype < lhs.ttype)
                    return false;
                return lhs.stype < rhs.stype;
            }
        };

    private:
        static double MyHumidityTable[][21];
        static std::map<ExtCoefKey, double> MyTblCoefTable;

    private:
        //-------------------------------------------------------------------------
        // Параметры атмосферы
        double mySunAngle;              // Высота Солнца над горизонтом (град.)
        Precipitation::type myPrec;     // Тип осадков
        bool myIsNight;                 // Признак ночи

        // Параметры объектов
        double myObjTemp;
        double myObjRadCoef3_5;
        double myObjRadCoef8_14;
        double myObjTblCoef;
        int myObjSunPos;
        int myObjSurface;

        // Параметры фона
        double myBgTemp;
        double myBgRadCoef3_5;
        double myBgRadCoef8_14;
        double myBgTblCoef;
        int myBgSunPos;
        int myBgSurface;
        double myBgAngle;

        // Другое
        bool myIsLoaded;                // Была ли произведена инициализация данных


    public:
        //-------------------------------------------------------------------------
        // Конструктор класса
        CEnvironment();


    public:
        static double getAbsHumidity(double hum, double temp);
        static Precipitation::type getPrecipitationType(gidrometeorEnum g);

    private:
        void _initExteralCoefMap();
        double _getTblCoef(SpectrumRange::type srange, Surface::type surface, TypeSurface ter, SecondCloudType cl) const;

    public:
        //-------------------------------------------------------------------------
        // Загрузка внешних условий из оперативной базы данных
        result load(const CDataMap& data, const string& fileName);

        //-------------------------------------------------------------------------
        // Доступ к параметрам атмосферы
        double getSunAngle() const { return mySunAngle; }

        //-------------------------------------------------------------------------
        // Доступ к параметрам объекта
        double getObjTemp() const { return myObjTemp; }
        double getObjRadCoef3_5() const { return myObjRadCoef3_5; }
        double getObjRadCoef8_14() const { return myObjRadCoef8_14; }
        double getObjTblCoef() const { return myObjTblCoef; }
        double getObjTblCoef(SpectrumRange::type srange, TypeSurface ter, SecondCloudType cl) const;
        int getObjSunPos() const { return myObjSunPos; }
        int getObjSurface() const { return myObjSurface; }

        //-------------------------------------------------------------------------
        // Доступ к параметрам фона
        double getBgRadCoef3_5() const { return myBgRadCoef3_5; }
        double getBgRadCoef8_14() const { return myBgRadCoef8_14; }
        double getBgTblCoef(SpectrumRange::type srange, TypeSurface ter, SecondCloudType cl) const;
        int getBgSunPos() const { return myBgSunPos; }
        int getBgSurface() const { return myBgSurface; }
        double getBgAngle() const { return myBgAngle; }

        double getObjTempDelta() const { return (myObjTemp - myBgTemp); }

        //-------------------------------------------------------------------------
    };

    //=============================================================================
}

//=============================================================================
#endif

