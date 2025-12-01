#ifndef _600_OpticThermal_Environment_IEnvWrapper_h
#define _600_OpticThermal_Environment_IEnvWrapper_h

class IEnvWrapper
{
public:
    virtual ~IEnvWrapper() {}

public:
    //функция инициализации класса через
    virtual bool init(const char* path) = 0;
   
    //Установка даты 
    virtual void setDate(const char* dateStr, const char* formatStr) = 0;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //	CWeather
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //вызов ссылки на класс CWeather, в которой хранятся функции

    virtual bool getTemperature(double lat, double lon, double height, double& result, double& deltaResult) const = 0;

    virtual bool getVx(double lat, double lon, double height, double& result, double& deltaResult) const = 0;

    virtual bool getVy(double lat, double lon, double height, double& result, double& deltaResult) const = 0;

    virtual bool getEarthTemperature(double lat, double lon, double& result, double& deltaResult) const = 0;


    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //	CWParams
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //CWParamsWrapper getCParams()const;
    //Возвращает первый тип облаков
    virtual int getTypeOfCloudFromFirstTable() const = 0;

    //Возвращает второй тип облаков
    virtual int getTypeOfCloudFromSecondTable() const = 0;

    //Возвращает МДВ км
    virtual double getMeteoRange() const = 0;

    //Возвращает относительную влажность %
    virtual double getRelativeHumidity() const = 0;

    //Возвращает бальность облачности
    virtual double getCloudinessCloud() const = 0;

    //Возвращает бальность облачности
    virtual double getCloudBottom() const = 0;

    //Возвращает состояние гидрометеоров
    virtual int getGidrometeor() const = 0;

    //Возвращает скорость выпадения осадков мм/ч
    virtual double getRainfallRate() const = 0;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //	CWFV
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    virtual double getPressure(const double height) const = 0;

    virtual double getDensity(const double height) const = 0;

    virtual double getSoundSpeed(const double height) const = 0;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //	CSun
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //расчёт восхода солнца 

    virtual double getSunrise(double lat, double lon) const = 0;

    virtual double getSunset(double lat, double lon) const = 0;

    //расчёт азимута солнца по lat lon
    virtual double getSunAz(double lat, double lon, double second) const = 0;

    //расчёт угла места солнца по lat lon
    virtual double getSunAlt(double lat, double lon, double second) const = 0;

    //получить дату

    //std::string getDate(const std::string& formatStr) const;
    virtual void getCurTime(int& hour, int& minute) = 0;

    //Функция перевода времени из double в часы и минуты pair<int,int>
    //std::pair<int, int> TimeFromDoubleToPairIntInt(double dhr) const;
    virtual void TimeFromDoubleToPairIntInt(double dhr, int& hour, int& minute) = 0;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //	CSurface
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //получение типа поверхности
    virtual int getSurfaceType(double lat, double lon) const = 0;

    //Возвращает значение коэффициента излучения фона для типа поверхности
    virtual double getSurfaceEmissivity(double lat, double lon) const = 0;

    virtual double getSurfaceSRSE(double lat, double lon) const = 0;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //	CHeight
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //высота рельефа по координатам lat lon
    virtual double getHeight(double lat, double lon) const = 0;

};

#endif
