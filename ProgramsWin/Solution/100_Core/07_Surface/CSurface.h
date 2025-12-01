// Файл CSurface.h
// Описание поверхности
//

#ifndef Core_Surface_CSurface_h
#define Core_Surface_CSurface_h



//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "CSharedMemory.h"
#include "CSurfaceBase.h"



//=====================================================================================================================
// Описание поверхности
class CSurface : public CSurfaceBase
{
public:
    //-----------------------------------------------------------------------------------------------------------------
    // Единая на всю программу подстилающая поверхность
    static CSurface myGlobalSurface;


protected:
    //-----------------------------------------------------------------------------------------------------------------
    // Данные о поверхности
    CSharedMemory m_SurfaceMemory;
    int m_sizeX, m_sizeY;
    double m_lat, m_lon, m_dlat, m_dlon;
    bool m_WasInit;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор
    CSurface();

    // Деструктор
    virtual ~CSurface();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Загрузка подстилающей поверхности
    bool load(const std::string& surfaceFileName)
    {
        return _init({ "FILE", surfaceFileName });
    }


public:
    //-----------------------------------------------------------------------------------------------------------------
    //возвращает тип поверхности
    virtual TypeSurface getType(double y, double x) const override;

    //возвращает излучательную способность типа поверхности
    virtual double getSurfaceEmissivitySmall(double y, double x) const override;
    virtual double getSurfaceEmissivityBig(double y, double x) const override;

    //возвращает удельную эффективность отражающей поверхности
    virtual double getSurfaceSRSE(double y, double x) const override;

    //возвращает разрешение
    virtual double getResolutionLat() const { return m_dlat; }
    virtual double getResolutionLon() const { return m_dlon; }


protected:
    //-----------------------------------------------------------------------------------------------------------------
    // Вспомогательная функция
    virtual bool _init(const std::vector<std::string>& path) override;


    //-----------------------------------------------------------------------------------------------------------------
};



//=====================================================================================================================
// Быстрый доступ к подстилающей поверхности
static CSurface& Surface = CSurface::myGlobalSurface;



//=====================================================================================================================
#endif


