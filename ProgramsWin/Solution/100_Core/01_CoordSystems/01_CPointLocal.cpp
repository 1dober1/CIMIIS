// Файл 01_CPointLocal.cpp
// Положение моделируемого объекта в местной декартовой системе координат
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CPointLocal.h"
#include "02_CPointGlobal.h"




//=====================================================================================================================
// Конструктор класса
CPointLocal::CPointLocal()
{
    myTime = 0.0;
    myRoll = 0.0;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CPointLocal::CPointLocal(double t, const CPoint3D &pos, double roll)
{
    myTime = t;
    myPosition = pos;
    myRoll = roll;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CPointLocal::CPointLocal(double t, const CPoint3D &pos, const CVector3D &vel, double roll)
{
    myTime = t;
    myPosition = pos;
    myVelocity = vel;
    myRoll = roll;
}




//=====================================================================================================================
// Пересчёт положения объекта в геоцентрическую систему координат
CPointGlobal CPointLocal::toPointGlobal(const CPoint3D &ortho) const
{
    // Будущий результат преобразования
    CPointGlobal result;
    result.setTime(myTime);
    result.setRoll(myRoll);

    // Преобразование текущего положения в геоцентрическую СК
    CPoint3D globalPos = _transformToGlobal(myPosition, ortho);
    if (ErrorCode != ERR_OK)
        return result;

    // Вычисление положения через 1 секунду
    CPoint3D newPos = myPosition;
    newPos.moveOn(myVelocity);

    // Преобразование нового положения в геоцентрическую СК
    CPoint3D newGlobalPos = _transformToGlobal(newPos, ortho);
    // Проверка ошибок при преобразовании не требуется, она выполнена при первом преобразовании

    // Вычисление вектора мгновенной скорости
    CVector3D globalVel(globalPos, newGlobalPos);

    // Формирование точки маршрута
    result.setPosition(globalPos);
    result.setVelocity(globalVel);
    return result;
}



//=====================================================================================================================
// Доступ к параметрам положения объекта
double CPointLocal::getTime() const
{
    return myTime;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам положения объекта
const CPoint3D & CPointLocal::getPosition() const
{
    return myPosition;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам положения объекта
const CVector3D & CPointLocal::getVelocity() const
{
    return myVelocity;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам положения объекта
double CPointLocal::getRoll() const
{
    return myRoll;
}




//=====================================================================================================================
// Изменение параметров положения объекта
void CPointLocal::setTime(double time)
{
    myTime = time;
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение параметров положения объекта
void CPointLocal::setPosition(const CPoint3D &pos)
{
    myPosition = pos;
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение параметров положения объекта
void CPointLocal::setVelocity(const CVector3D &vel)
{
    myVelocity = vel;
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение параметров положения объекта
void CPointLocal::setRoll(double roll)
{
    myRoll = roll;
}




//=====================================================================================================================
// Направление мгновенной скорости на заданную точку
void CPointLocal::adjustVelocityTo(const CPoint3D &nextPoint)
{
    // Если вектор скорости нулевой, то нечего направлять
    if (checkEQ(myVelocity.getLength(), 0.0) == true)
        return;

    // Если целевая точка совпадает с данной, то нечего направлять
    if (myPosition == nextPoint)
        return;

    // Скорректировать вектор скорости
    double length = myVelocity.getLength();
    double dx = nextPoint.getX() - myPosition.getX();
    double dy = nextPoint.getY() - myPosition.getY();
    double dz = nextPoint.getZ() - myPosition.getZ();
    myVelocity.setCoords(dx, dy, dz);
    myVelocity.setLength(length);
}




//=====================================================================================================================
// Перенос начала координат в заданную точку
void CPointLocal::moveOrthoTo(double newOrthoX, double newOrthoY, double newOrthoZ)
{
    myPosition.moveOrthoTo(newOrthoX, newOrthoY, newOrthoZ);
}




//---------------------------------------------------------------------------------------------------------------------
// Перенос начала координат в заданную точку
void CPointLocal::moveOrthoTo(const CPoint3D &newOrtho)
{
    moveOrthoTo(newOrtho.getX(), newOrtho.getY(), newOrtho.getZ());
}




//=====================================================================================================================
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CPointLocal::rotatePlaneXY(double sinAngle, double cosAngle)
{
    myPosition.rotatePlaneXY(sinAngle, cosAngle);
    myVelocity.rotatePlaneXY(sinAngle, cosAngle);
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CPointLocal::rotatePlaneXY(double angle)
{
    rotatePlaneXY(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CPointLocal::rotatePlaneYZ(double sinAngle, double cosAngle)
{
    myPosition.rotatePlaneYZ(sinAngle, cosAngle);
    myVelocity.rotatePlaneYZ(sinAngle, cosAngle);
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CPointLocal::rotatePlaneYZ(double angle)
{
    rotatePlaneYZ(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CPointLocal::rotatePlaneZX(double sinAngle, double cosAngle)
{
    myPosition.rotatePlaneZX(sinAngle, cosAngle);
    myVelocity.rotatePlaneZX(sinAngle, cosAngle);
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CPointLocal::rotatePlaneZX(double angle)
{
    rotatePlaneZX(sin(angle), cos(angle));
}




//=====================================================================================================================
// Переход к новым координатным осям
bool CPointLocal::changeBasis(const CVector3D &newOx, const CVector3D &newOy, const CVector3D &newOz)
{
    if (myPosition.changeBasis(newOx, newOy, newOz) == false)
        return false;
    myVelocity.changeBasis(newOx, newOy, newOz);
    return true;
}




//=====================================================================================================================
// Вычисление угла места некоторой точки относительно данного положения объекта [радианы]
double CPointLocal::computeRadianElevationOf(const CPoint3D &point) const
{
    // Вычисляем дальность до целевой точки
    double distance = myPosition.computeDistanceTo(point);

    // Если точки совпадают
    if (checkEQ(distance, 0.0) == true)
        return 0.0;

    // Вычисляем разницу высот
    double dz = point.getZ() - myPosition.getZ();

    // Вычисляем угол места точки
    return asin(dz / distance);
}




//=====================================================================================================================
// Вычисление азимута некоторой точки относительно данного положения объекта [радианы]
double CPointLocal::computeRadianAzimuthOf(const CPoint3D &point) const
{
    // Вычисляем проекцию текущей точки и направляющей точки
    // на горизонтальную плоскость
    CPoint3D curProjection(myPosition.getX(), myPosition.getY(), 0.0);
    CPoint3D pointProjection(point.getX(), point.getY(), 0.0);

    // Вычисляем расстояние между проекциями точек на горизонтальной плоскости
    double distance = curProjection.computeDistanceTo(pointProjection);
    if (checkEQ(distance, 0.0) == true)
        return 0.0;

    // Вычисляем отклонение вдоль координатных осей проекции направляющей точки
    // от проекции текущей точки на горизонтальную плоскость 
    double dx = pointProjection.getX() - curProjection.getX();
    double dy = pointProjection.getY() - curProjection.getY();

    // Направления координатных осей:
    //    ось Ox направлена на восток,
    //    ось Oy направлена на север.
    // Направление влево от оси Oy (II и III четверти) будем считать отрицательным,
    // направление вправо от оси Oy (I и IV четверти) будем считать положительным

    // Если направляющая точка лежит в I или II четверти
    if (checkGE(dy, 0.0) == true)
    {
        return 0.5*Pi - acos(dx/distance);
    }

    // Если направляющая точка лежит в III или IV четверти
    else
    {
        // Если направляющая точка лежит в III четверти
        if (checkLT(dx, 0.0) == true)
            return acos(dx/distance) - 1.5*Pi;

        // Если направляющая точка лежит в IV четверти
        else
            return 0.5*Pi + acos(dx/distance);
    }
}




//=====================================================================================================================
// Вычисление текущего направления движения объекта по азимуту [радианы]
double CPointLocal::computeRadianSelfAzimuth() const
{
    // Пролонгируем движение на одну секунду в будущее
    CPoint3D point = myPosition;
    point.moveOn(myVelocity);

    // Вычисляем направление на эту точку
    return computeRadianAzimuthOf(point);
}




//=====================================================================================================================
// Вычисление текущего направления движения объекта по углу места [радианы]
double CPointLocal::computeRadianSelfElevation() const
{
    // Пролонгируем движение на одну секунду в будущее
    CPoint3D point = myPosition;
    point.moveOn(myVelocity);

    // Вычисляем направление на эту точку
    return computeRadianElevationOf(point);
}




//=====================================================================================================================
// Вспомогательная функция для преобразования в геоцентрическую СК
CPoint3D CPointLocal::_transformToGlobal(const CPoint3D &point, const CPoint3D &ortho)
{
    // Доступ к параметрам планеты
    double a = Settings.getEarthMajorRadius();
    double b = Settings.getEarthMinorRadius();

    // Вычисление орт местной системы координат (алгоритм в преобразовании из геоцентрической в местную)
    double xC = ortho.getX();
    double yC = ortho.getY();
    double zC = ortho.getZ();
    double r = sqrt(xC*xC + yC*yC);
    if (checkEQ(r, 0.0) == true)
    {
        ErrorCode = ERR_BAD_ORTHO;
        return CPoint3D();
    }
    CVector3D newOx(-yC, xC, 0.0);
    CVector3D newOy(-a*a*xC*zC/r, -a*a*yC*zC/r, b*b*r);
    CVector3D newOz = newOx.computeVectorProductWith(newOy);
    newOx.setLength(1.0);
    newOy.setLength(1.0);
    newOz.setLength(1.0);

    // Вычисление орт геоцентрической СК в местной СК
    CVector3D globalOx(1.0, 0.0, 0.0);
    CVector3D globalOy(0.0, 1.0, 0.0);
    CVector3D globalOz(0.0, 0.0, 1.0);
    globalOx.changeBasis(newOx, newOy, newOz);
    globalOy.changeBasis(newOx, newOy, newOz);
    globalOz.changeBasis(newOx, newOy, newOz);
    globalOz = globalOx.computeVectorProductWith(globalOy);

    // Переход к новым координатным осям
    // (от осей местной СК к осям геоцентрической СК)
    CPoint3D pos = point;
    pos.changeBasis(globalOx, globalOy, globalOz);

    // Перенос начала координат с поверхности в центр планеты
    pos.moveOrthoTo(-ortho.getX(), -ortho.getY(), -ortho.getZ());
    ErrorCode = ERR_OK;
    return pos;
}


