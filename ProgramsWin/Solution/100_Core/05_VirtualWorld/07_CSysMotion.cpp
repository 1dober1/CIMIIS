// Файл 07_CSysMotion.cpp
// Базовый класс для системы движения моделируемого объекта
//

//=====================================================================================================================
// Заголовочные файлы
#include "../02_TaskReader/03_CTaskObject.h"
#include "../04_OutputWriter/01_CFilm.h"
#include "07_CSysMotion.h"
#include "10_CObject.h"



//=====================================================================================================================
// Конструктор класса
CSysMotion::CSysMotion(const CSysType &sysType, CObject &object)
    : CSys(sysType, object)
{
}



//=====================================================================================================================
// Деструктор класса
CSysMotion::~CSysMotion()
{
}



//=====================================================================================================================
// Доступ к типу системы
const CSysMotionType & CSysMotion::getType() const
{
    return (const CSysMotionType &)CSys::getType();
}



//=====================================================================================================================
// Управление работой системы
bool CSysMotion::routeEnd(double time)
{
    // Выполнение действий
    return onRouteEnd(time);
}


//=====================================================================================================================
// Сменные алгоритмы действий
bool CSysMotion::onRouteEnd(double)
{
    return true;
}



//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysMotion::onTaskPoint(const CTaskPoint &taskPoint)
{
    saveLastFixedPoint(taskPoint);
    Film.writePoint(getObject().getFilmId(), taskPoint.getPointGlobal());

    Console << "  " << format(taskPoint.getPointGlobal().getTime(), 7, 2) << "  объект " << format((uint32)getObject().getAbsNum(), 4, fillZero) << " прошёл опорную точку" << EndL;
    Log     << "  " << format(taskPoint.getPointGlobal().getTime(), 7, 2) << "  объект " << format((uint32)getObject().getAbsNum(), 4, fillZero) << " прошёл опорную точку" << EndL;
}



//=====================================================================================================================
// Вычисление текущего положения моделируемого объекта
CTaskPoint CSysMotion::computePoint(double time) const
{
    // Вычислить приращение времени
    double dt = time - myLastFixedPoint.getPointGlobal().getTime();
    if (checkLT(dt, 0.0) == true)
        return CTaskPoint(CPointGlobal(time, myLastFixedPoint.getPointGlobal().getPosition(), myLastFixedPoint.getPointGlobal().getVelocity(), myLastFixedPoint.getPointGlobal().getRoll()));

    CPoint3D newPos = myLastFixedPoint.getPointGlobal().getPosition();
    newPos.moveOn(myLastFixedPoint.getPointGlobal().getVelocity() * dt);
    return CTaskPoint(CPointGlobal(time, newPos, myLastFixedPoint.getPointGlobal().getVelocity(), myLastFixedPoint.getPointGlobal().getRoll()));
}



//---------------------------------------------------------------------------------------------------------------------
// Вычисление текущего положения моделируемого объекта
const CTaskPoint & CSysMotion::getLastFixedPoint() const
{
    return myLastFixedPoint;
}



//---------------------------------------------------------------------------------------------------------------------
// Вычисление текущего положения моделируемого объекта
void CSysMotion::saveLastFixedPoint(const CTaskPoint &point)
{
    myLastFixedPoint = point;
}



//=====================================================================================================================
// Управление начальным маршрутом движения моделируемого объекта
const vector<CTaskPoint> & CSysMotion::getInitialRoute() const
{
    return myInitialRoute;
}



//---------------------------------------------------------------------------------------------------------------------
// Управление начальным маршрутом движения моделируемого объекта
void CSysMotion::setInitialRoute(const vector<CTaskPoint> &route)
{
    myInitialRoute = route;

    if (myInitialRoute.empty() == false)
        myLastFixedPoint = myInitialRoute[0];
}



//---------------------------------------------------------------------------------------------------------------------
// Управление начальным маршрутом движения моделируемого объекта
void CSysMotion::takeInitialRoute(const CTaskObject &taskObject)
{
    myInitialRoute.clear();
    for (size_t i = 0; i < taskObject.getPointCount(); ++i)
        myInitialRoute.push_back(taskObject.getPoint(i));

    if (myInitialRoute.empty() == false)
        myLastFixedPoint = myInitialRoute[0];
}

