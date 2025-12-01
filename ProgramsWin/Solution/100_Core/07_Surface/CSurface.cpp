// Файл CSurface.cpp
// Описание поверхности
//

//=====================================================================================================================
// Заголовочные файлы
#include "CSurface.h"




//=====================================================================================================================
// Единая на всю программу подстилающая поверхность
CSurface CSurface::myGlobalSurface;




//=====================================================================================================================
// Сдвиги
namespace {
    const int shiftLan = 0;//0
    const int shiftLon = sizeof(double);//8
    const int shiftdLat = sizeof(double)* 2;//16
    const int shiftdLon = sizeof(double)* 3;//24
    const int shiftSizeY = sizeof(double)* 4;//32
    const int shiftSizeX = sizeof(double)* 4 + sizeof(int);//36
    const int shift = sizeof(double)* 4 + sizeof(int)*2;//40
}




//=====================================================================================================================
// Конструктор
CSurface::CSurface()
{
    m_WasInit = false;
}




//=====================================================================================================================
// Деструктор
CSurface::~CSurface()
{
    m_WasInit = false;
}




//=====================================================================================================================
// Вспомогательная функция
bool CSurface::_init(const std::vector<std::string>& path)
{
    if (m_WasInit == true)
    {
        Log << "CSurface.Init() - Класс был проинициализирован,файл не был подключен" << EndL;
        return false;
    }
    if (path.size() >= 2)
    {
        std::string fileName = path[1];
        if (m_SurfaceMemory.readFile(fileName))
        {
            m_lat = m_SurfaceMemory.getDoubleValue(::shiftLan);
            m_lon = m_SurfaceMemory.getDoubleValue(::shiftLon);
            m_dlat = m_SurfaceMemory.getDoubleValue(::shiftdLat);
            m_dlon = m_SurfaceMemory.getDoubleValue(::shiftdLon);
            m_sizeY = m_SurfaceMemory.getIntValue(::shiftSizeY);
            m_sizeX = m_SurfaceMemory.getIntValue(::shiftSizeX);
            m_WasInit = true;
            return true;
        }
        Log << "CSurface.Init() - Cant create " << path[1] << EndL;
        return false;
    }
    else
    {
        Log << "CSurface.Init() - Cant create,not enouth values " << EndL;
        return false;
    }
}




//=====================================================================================================================
//возвращает тип поверхности
TypeSurface CSurface::getType(double y, double x) const
{
    if (m_WasInit == true)
    {

        int countX = 0;
        int countY = 0;
        countY = static_cast<int>((m_lat - y) / m_dlat);
        countX = static_cast<int>((x - m_lon) / m_dlon);

        if (countX < 0 || countY < 0)
        {
            return OUTOFRANGE;
        }
        if (countX >= m_sizeX || countY >= m_sizeY)
        {

            return OUTOFRANGE ;
        }
        long long int calculateBit = static_cast<long long int>(countY) * static_cast<long long int>(m_sizeX) + static_cast<long long int>(countX);
        TypeSurface result = TypeSurface(m_SurfaceMemory.getCharValue(calculateBit + ::shift));
        return result;


    }
    return TypeSurface(OUTOFRANGE);
}




//=====================================================================================================================
//возвращает излучательную способность типа поверхности
double CSurface::getSurfaceEmissivitySmall(double y, double x)const {
    int a = getType(y, x);
    if (a==99)
    {
        return dataSurfaceEmissivitySmall.at(10);
    }
    else
    {
        return dataSurfaceEmissivitySmall.at(a);

    }   
    Log << "CSurface - somthing going wrong" << EndL;
    return 0;
};




//---------------------------------------------------------------------------------------------------------------------
//возвращает излучательную способность типа поверхности
double CSurface::getSurfaceEmissivityBig(double y, double x) const
{
    int a = getType(y, x);
    if (a == 99)
    {
        return dataSurfaceEmissivityBig.at(10);
    }
    else
    {
        return dataSurfaceEmissivityBig.at(a);

    }
    Log << "CSurface - somthing going wrong" << EndL;
    return 0;
}




//=====================================================================================================================
//возвращает удельную эффективность отражающей поверхности
double CSurface::getSurfaceSRSE(double y, double x) const
{
    int a = getType(y, x);
    if (a == 99)
    {
        return dataSurfaceSRSE.at(10);
    }
    else
    {
        return dataSurfaceSRSE.at(a);

    }
    Log << "CSurface - somthing going wrong" << EndL;
    return 0;
}
