// Файл 01_CVector3D.cpp
// Вектор в пространстве
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CVector3D.h"
#include "02_CPoint3D.h"




//=====================================================================================================================
// Конструктор класса
CVector3D::CVector3D()
{
    myX = 1.0;
    myY = 0.0;
    myZ = 0.0;
    _computeLength();
    _computeCos();
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CVector3D::CVector3D(double x, double y, double z)
{
    myX = x;
    myY = y;
    myZ = z;
    _computeLength();
    _computeCos();
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CVector3D::CVector3D(const CPoint3D &start, const CPoint3D &end)
{
    myX = end.getX() - start.getX();
    myY = end.getY() - start.getY();
    myZ = end.getZ() - start.getZ();
    _computeLength();
    _computeCos();
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CVector3D::CVector3D(double length, double cosX, double cosY, double cosZ)
{
    myLength = length;
    myCosX = cosX;
    myCosY = cosY;
    myCosZ = cosZ;
    _computeCoords();
}




//=====================================================================================================================
// Доступ к координатам вектора
double CVector3D::getX() const
{
    return myX;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к координатам вектора
double CVector3D::getY() const
{
    return myY;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к координатам вектора
double CVector3D::getZ() const
{
    return myZ;
}




//=====================================================================================================================
// Доступ к длине вектора
double CVector3D::getLength() const
{
    return myLength;
}




//=====================================================================================================================
// Доступ к направляющим косинусам
double CVector3D::getCosX() const
{
    return myCosX;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к направляющим косинусам
double CVector3D::getCosY() const
{
    return myCosY;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к направляющим косинусам
double CVector3D::getCosZ() const
{
    return myCosZ;
}




//=====================================================================================================================
// Изменение координат вектора
void CVector3D::setCoords(double x, double y, double z)
{
    myX = x;
    myY = y;
    myZ = z;
    _computeLength();
    _computeCos();
}




//=====================================================================================================================
// Изменение длины вектора
void CVector3D::setLength(double length)
{
    if (length >= 0.0)
    {
        myLength = length;
        _computeCoords();
    }
    else
    {
        myLength = -length;
        _computeCoords();
        myX = -myX;
        myY = -myY;
        myZ = -myZ;
        _computeCos();
    }
}




//=====================================================================================================================
// Изменение направляющих косинусов вектора
void CVector3D::setCos(double cosX, double cosY, double cosZ)
{
    myCosX = cosX;
    myCosY = cosY;
    myCosZ = cosZ;
    _computeCoords();
}




//=====================================================================================================================
// Проверка векторов на совпадение
bool CVector3D::operator ==(const CVector3D &other) const
{
    if (checkEQ(myX, other.myX) == true   &&
        checkEQ(myY, other.myY) == true   &&
        checkEQ(myZ, other.myZ) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Проверка векторов на несовпадение
bool CVector3D:: operator !=(const CVector3D &other) const
{
    return !(operator ==(other));
}




//=====================================================================================================================
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CVector3D::rotatePlaneXY(double sinAngle, double cosAngle)
{
    double newX =  myX*cosAngle + myY*sinAngle;
    double newY = -myX*sinAngle + myY*cosAngle;

    myX = newX;
    myY = newY;

    _computeCos();
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CVector3D::rotatePlaneXY(double angle)
{
    rotatePlaneXY(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CVector3D::rotatePlaneYZ(double sinAngle, double cosAngle)
{
    double newY =  myY*cosAngle + myZ*sinAngle;
    double newZ = -myY*sinAngle + myZ*cosAngle;

    myY = newY;
    myZ = newZ;

    _computeCos();
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CVector3D::rotatePlaneYZ(double angle)
{
    rotatePlaneYZ(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CVector3D::rotatePlaneZX(double sinAngle, double cosAngle)
{
    double newZ =  myZ*cosAngle + myX*sinAngle;
    double newX = -myZ*sinAngle + myX*cosAngle;

    myZ = newZ;
    myX = newX;

    _computeCos();
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CVector3D::rotatePlaneZX(double angle)
{
    rotatePlaneZX(sin(angle), cos(angle));
}




//=====================================================================================================================
// Переход к новым координатным осям
bool CVector3D::changeBasis(const CVector3D &newOx, const CVector3D &newOy, const CVector3D &newOz)
{
    // Проверка, что (Ox, Oy, Oz) - правая тройка
    CVector3D testOz = newOx.computeVectorProductWith(newOy);
    testOz.setLength(newOz.getLength());
    if (testOz != newOz)
    {
        ErrorCode = ERR_BAD_TRIPPLE;
        return false;
    }

    // Переход к новым осям координат
    double newX = newOx.getCosX()*myX + newOx.getCosY()*myY + newOx.getCosZ()*myZ;
    double newY = newOy.getCosX()*myX + newOy.getCosY()*myY + newOy.getCosZ()*myZ;
    double newZ = newOz.getCosX()*myX + newOz.getCosY()*myY + newOz.getCosZ()*myZ;

    myX = newX;
    myY = newY;
    myZ = newZ;

    _computeCos();
    ErrorCode = ERR_OK;
    return true;
}




//=====================================================================================================================
// Вычисление суммы векторов
//   a = (ax, ay, az);
//   b = (bx, by, bz);
//   c = a + b = (ax+bx, ay+by, az+bz);
CVector3D CVector3D::add(const CVector3D &other) const
{
    return CVector3D(myX+other.myX, myY+other.myY, myZ+other.myZ);
}




//---------------------------------------------------------------------------------------------------------------------
// Вычисление суммы векторов
CVector3D CVector3D::operator +(const CVector3D &other) const
{
    return add(other);
}




//=====================================================================================================================
// Вычисление разности векторов
//   c = a - b = a + (-b)
CVector3D CVector3D::subtract(const CVector3D &other) const
{
    return CVector3D(myX-other.myX, myY-other.myY, myZ-other.myZ);
}




//-----------------------------------------------------------------------------
// Вычисление разности векторов
CVector3D CVector3D::operator -(const CVector3D &other) const
{
    return subtract(other);
}




//=====================================================================================================================
// Вычисление произведения вектора на скаляр
//    a = (ax, ay);
//    k*a = (k*ax, k*ay);
CVector3D CVector3D::multiply(double ratio) const
{
    return CVector3D(myX*ratio, myY*ratio, myZ*ratio);
}


//-----------------------------------------------------------------------------
// Вычисление произведения вектора на скаляр
CVector3D CVector3D::operator *(double ratio) const
{
    return multiply(ratio);
}




//=====================================================================================================================
// Вычисление скалярного произведения с другим вектором
//    a = (ax, ay);
//    b = (bx, by);
//    (a,b) = ax*bx + ay*by
double CVector3D::computeScalarProductWith(const CVector3D &other) const
{
    return (myX*other.myX + myY*other.myY + myZ*other.myZ);
}




//=====================================================================================================================
// Вычисление векторного произведения с другим вектором
//    a = (ax, ay);
//    b = (bx, by);
//    [a,b] = (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)
CVector3D CVector3D::computeVectorProductWith(const CVector3D &other) const
{
    return CVector3D(myY*other.myZ - myZ*other.myY,
                     myZ*other.myX - myX*other.myZ,
                     myX*other.myY - myY*other.myX);
}




//=====================================================================================================================
// Вычисление угла между векторами
//    a = (ax, ay, az);
//    b = (bx, by, bz);
//    (a,b) = |a| * |b| * cos(f)
//
//                (a,b)
//    cos(f) = ------------
//              |a| * |b|
double CVector3D::computeAngleWith(const CVector3D &other) const
{
    // Если хотя бы один из векторов нулевой
    if (checkEQ(myLength, 0.0) == true   ||   checkEQ(other.myLength, 0.0) == true)
        return 0.0;

    // Если оба вектора ненулевые
    return acos(computeScalarProductWith(other) / (myLength*other.myLength));
}




//=====================================================================================================================
// Вычисление проекции на другой вектор
CVector3D CVector3D::computeProjectionOn(const CVector3D &other) const
{
    // Косинус угла между векторами
    double cosA = cos(computeAngleWith(other));

    // Результарующий вектор - это вектор, на который строится проекция,
    // длина которого равна длине проецируемого вектора, умноженной на
    // косинус угла между векторами
    CVector3D result = other;
    result.setLength(myLength*cosA);
    return result;
}




//=====================================================================================================================
// Определение сонаправленности векторов
bool CVector3D::isDirectedAs(const CVector3D &other) const
{
    if (checkEQ(myCosX, other.myCosX) == true   &&
        checkEQ(myCosY, other.myCosY) == true   &&
        checkEQ(myCosZ, other.myCosZ) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Определение коллинеарности векторов
//   если [a,b] == 0, то коллинеарны
bool CVector3D::isCollinearTo(const CVector3D &other) const
{
    double vectorProductLength = computeVectorProductWith(other).getLength();
    if (checkEQ(vectorProductLength, 0.0) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Определение перпендикулярности векторов
//   если (a,b) == 0, то перпендикулярны
bool CVector3D::isNormalTo(const CVector3D &other) const
{
    double scalarProduct = computeScalarProductWith(other);
    if (checkEQ(scalarProduct, 0.0) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Определение компланарности векторов
bool CVector3D::isCoplanarTo(const CVector3D &v1, const CVector3D &v2) const
{
    if (checkEQ(computeScalarProductWith(v1.computeVectorProductWith(v2)), 0.0) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Вычисление длины вектора по координатам
void CVector3D::_computeLength()
{
    myLength = sqrt(myX*myX + myY*myY + myZ*myZ);
}




//=====================================================================================================================
// Вычисление направляющих косинусов по координатам и длине вектора
void CVector3D::_computeCos()
{
    // Если вектор не нулевой
    if (checkEQ(myLength, 0.0) == false)
    {
        myCosX = myX / myLength;
        myCosY = myY / myLength;
        myCosZ = myZ / myLength;
    }

    // Если вектор нулевой
    else
    {
        myCosX = 0.0;
        myCosY = 0.0;
        myCosZ = 0.0;
    }
}




//=====================================================================================================================
// Вычисление координат по длине и косинусам
void CVector3D::_computeCoords()
{
    myX = myLength * myCosX;
    myY = myLength * myCosY;
    myZ = myLength * myCosZ;
}


