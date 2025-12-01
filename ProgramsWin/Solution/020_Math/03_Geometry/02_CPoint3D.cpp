// Файл 02_CPoint3D.cpp
// Точка в пространстве
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_CPoint3D.h"
#include "03_CLine3D.h"




//=====================================================================================================================
// Конструктор класса
CPoint3D::CPoint3D()
{
    myX = 0.0;
    myY = 0.0;
    myZ = 0.0;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CPoint3D::CPoint3D(double x, double y, double z)
{
    myX = x;
    myY = y;
    myZ = z;
}




//=====================================================================================================================
// Доступ к координатам точки
double CPoint3D::getX() const
{
    return myX;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к координатам точки
double CPoint3D::getY() const
{
    return myY;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к координатам точки
double CPoint3D::getZ() const
{
    return myZ;
}




//=====================================================================================================================
// Изменение координат точки
void CPoint3D::setCoords(double x, double y, double z)
{
    myX = x;
    myY = y;
    myZ = z;
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение координат точки
void CPoint3D::setX(double x)
{
    myX = x;
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение координат точки
void CPoint3D::setY(double y)
{
    myY = y;
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение координат точки
void CPoint3D::setZ(double z)
{
    myZ = z;
}




//=====================================================================================================================
// Проверка точек на совпадение
bool CPoint3D::operator ==(const CPoint3D &other) const
{
    if (checkEQ(myX, other.myX) == true   &&
        checkEQ(myY, other.myY) == true   &&
        checkEQ(myZ, other.myZ) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Проверка точек на несовпадение
bool CPoint3D::operator !=(const CPoint3D &other) const
{
    return !(operator ==(other));
}




//=====================================================================================================================
// Перенос начала координат в заданную точку
void CPoint3D::moveOrthoTo(double newOrthoX, double newOrthoY, double newOrthoZ)
{
    myX = myX - newOrthoX;
    myY = myY - newOrthoY;
    myZ = myZ - newOrthoZ;
}




//---------------------------------------------------------------------------------------------------------------------
// Перенос начала координат в заданную точку
void CPoint3D::moveOrthoTo(const CPoint3D &newOrtho)
{
    moveOrthoTo(newOrtho.getX(), newOrtho.getY(), newOrtho.getZ());
}




//=====================================================================================================================
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CPoint3D::rotatePlaneXY(double sinAngle, double cosAngle)
{
    double newX =  myX*cosAngle + myY*sinAngle;
    double newY = -myX*sinAngle + myY*cosAngle;

    myX = newX;
    myY = newY;
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CPoint3D::rotatePlaneXY(double a)
{
    rotatePlaneXY(sin(a), cos(a));
}




//=====================================================================================================================
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CPoint3D::rotatePlaneYZ(double sinAngle, double cosAngle)
{
    double newY =  myY*cosAngle + myZ*sinAngle;
    double newZ = -myY*sinAngle + myZ*cosAngle;

    myY = newY;
    myZ = newZ;
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CPoint3D::rotatePlaneYZ(double a)
{
    rotatePlaneYZ(sin(a), cos(a));
}




//=====================================================================================================================
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CPoint3D::rotatePlaneZX(double sinAngle, double cosAngle)
{
    double newZ =  myZ*cosAngle + myX*sinAngle;
    double newX = -myZ*sinAngle + myX*cosAngle;

    myZ = newZ;
    myX = newX;
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CPoint3D::rotatePlaneZX(double a)
{
    rotatePlaneZX(sin(a), cos(a));
}




//=====================================================================================================================
// Переход к новым координатным осям
bool CPoint3D::changeBasis(const CVector3D &newOx, const CVector3D &newOy, const CVector3D &newOz)
{
    // Проверка, что (Ox, Oy, Oz) - правая тройка
    CVector3D testOz = newOx.computeVectorProductWith(newOy);
    testOz.setLength(newOz.getLength());
    if (testOz != newOz)
    {
        ErrorCode = ERR_BAD_TRIPPLE;
        return false;
    }

    // Переход
    double newX = newOx.getCosX()*myX + newOx.getCosY()*myY + newOx.getCosZ()*myZ;
    double newY = newOy.getCosX()*myX + newOy.getCosY()*myY + newOy.getCosZ()*myZ;
    double newZ = newOz.getCosX()*myX + newOz.getCosY()*myY + newOz.getCosZ()*myZ;

    myX = newX;
    myY = newY;
    myZ = newZ;
    ErrorCode = ERR_OK;
    return true;
}




//=====================================================================================================================
// Перенос точки на вектор
void CPoint3D::moveOn(const CVector3D &vect)
{
    myX = myX + vect.getX();
    myY = myY + vect.getY();
    myZ = myZ + vect.getZ();
}




//=====================================================================================================================
// Вычисление расстояния до другой точки
double CPoint3D::computeDistanceTo(const CPoint3D &other) const
{
    double dx = myX - other.myX;
    double dy = myY - other.myY;
    double dz = myZ - other.myZ;
    return sqrt(dx*dx + dy*dy + dz*dz);
}




//=====================================================================================================================
// Вычисление расстояния до прямой
double CPoint3D::computeDistanceTo(const CLine3D &line) const
{
    const CVector3D &direction = line.getDirection();
    double ax = direction.getX();
    double ay = direction.getY();
    double az = direction.getZ();

    const CPoint3D &point = line.getPoint();
    double x = point.getX();
    double y = point.getY();
    double z = point.getZ();

    // Вспомогательные определители
    double d1 = ay*(z-myZ) - az*(y-myY);
    double d2 = az*(x-myX) - ax*(z-myZ);
    double d3 = ax*(y-myY) - ay*(x-myX);

    // Числитель
    double numer = sqrt(d1*d1 + d2*d2 + d3*d3);

    // Знаменатель
    double denom = sqrt(ax*ax + ay*ay + az*az);

    return numer / denom;
}




