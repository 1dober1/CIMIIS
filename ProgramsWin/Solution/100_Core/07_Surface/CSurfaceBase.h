// Файл CSurfaceBase.h
// Описание поверхности
//

#ifndef Core_Surface_CSurfaceBase_h
#define Core_Surface_CSurfaceBase_h



//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "ETypeSurface.h"



//=====================================================================================================================
// Описание поверхности
class CSurfaceBase
{
protected:
    //-----------------------------------------------------------------------------------------------------------------
    std::vector <double> dataSurfaceSRSE;
    std::vector <double> dataSurfaceEmissivitySmall;
    std::vector <double> dataSurfaceEmissivityBig;
    std::vector<double> dataSurfaceDefauldSpeed;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор
    CSurfaceBase() {}

    // Деструктор
    virtual ~CSurfaceBase() {}


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Возвращает тип поверхности
    virtual TypeSurface getType(double y, double x) const = 0;

    // Возвращает излучательную способность типа поверхности
    virtual double getSurfaceEmissivitySmall(double y, double x) const = 0;
    virtual double getSurfaceEmissivityBig(double y, double x) const = 0;

    // Возвращает удельную эффективность отражающей поверхности
    virtual double getSurfaceSRSE(double y, double x) const = 0;

    // Возвращают разрешение (градусы/пкс) карты типов поверхностей по широте и долготе
    virtual double getResolutionLat() const { return -1; }
    virtual double getResolutionLon() const { return -1; }


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Общие функции
    virtual double getDefaultSpeed(TypeSurface type) const;
    static std::shared_ptr<CSurfaceBase> getSurface(const std::vector<std::string> name);
    virtual void getSurfaceInfo(int& sizeY, int& sizeX, double& stepY, double& stepX, int& Y_centre, int& X_centre)  const { return; }

protected:
    //-----------------------------------------------------------------------------------------------------------------
    // Вспомогательные функции
    virtual bool _init(const std::vector<std::string>& path) = 0;


    //-----------------------------------------------------------------------------------------------------------------
};




//=====================================================================================================================
#endif


