//=============================================================================
// Заголовочные файлы
#include "03_CEarth.h"
namespace optic = SysOpticThermal;

//=============================================================================
// Конструкторы класса
optic::CEarth::CEarth()
{
    myMinorRadius = 6356751.3618;
    myMajorRadius = 6378136.0;
}


//=============================================================================
// Доступ к параметрам Земли
double optic::CEarth::getMinorRadius() const
{
    return myMinorRadius;
}


//-----------------------------------------------------------------------------
double optic::CEarth::getMajorRadius() const
{
    return myMajorRadius;
}


//=============================================================================
// Настройка параметров Земли
void optic::CEarth::setSize(double minorRadius, double majorRadius)
{
    myMinorRadius = minorRadius;
    myMajorRadius = majorRadius;
}


//=============================================================================

