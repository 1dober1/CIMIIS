// Файл 03_CLine3D.cpp
// Прямая в пространстве
//

//=====================================================================================================================
// Заголовочные файлы
#include "03_CLine3D.h"
#include "04_CSphere3D.h"




//=====================================================================================================================
// Конструктор класса
CLine3D::CLine3D(const CPoint3D &p1, const CPoint3D &p2)
{
    set(p1, p2);
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CLine3D::CLine3D(const CPoint3D &point, const CVector3D &direction)
{
    set(point, direction);
}




//=====================================================================================================================
// Доступ к направляющему вектору
const CVector3D & CLine3D::getDirection() const
{
    return myDirection;
}




//=====================================================================================================================
// Доступ к опорной точке
const CPoint3D & CLine3D::getPoint() const
{
    return myPoint;
}




//=====================================================================================================================
// Изменение параметров прямой
void CLine3D::set(const CPoint3D &p1, const CPoint3D &p2)
{
    _computeLine(p1, p2);
    _normalizeDirection();
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение параметров прямой
void CLine3D::set(const CPoint3D &point, const CVector3D &direction)
{
    myPoint = point;

    // Если вектор направления нулевой, то проводим через точку произвольную прямую
    if (checkEQ(direction.getLength(), 0.0) == true)
    {
        myDirection = CVector3D(1.0, 0.0, 0.0);
        ErrorCode = ERR_BAD_LINE;
    }

    // Если вектор направления не нулевой, то всё хорошо
    else
    {
        myDirection = direction;
        ErrorCode = ERR_OK;
    }

    _normalizeDirection();
}




//=====================================================================================================================
// Проверка прямых на совпадение
bool CLine3D::operator ==(const CLine3D &other) const
{
    if (myDirection.isCollinearTo(other.myDirection) == true   &&
        other.containsPoint(myPoint) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Проверка прямых на несовпадение
bool CLine3D::operator !=(const CLine3D &other) const
{
    return !(operator ==(other));
}




//=====================================================================================================================
// Перенос начала координат в заданную точку
void CLine3D::moveOrthoTo(double newOrthoX, double newOrthoY, double newOrthoZ)
{
    myPoint.moveOrthoTo(newOrthoX, newOrthoY, newOrthoZ);
}




//---------------------------------------------------------------------------------------------------------------------
// Перенос начала координат в заданную точку
void CLine3D::moveOrthoTo(const CPoint3D &newOrtho)
{
    moveOrthoTo(newOrtho.getX(), newOrtho.getY(), newOrtho.getZ());
}




//=====================================================================================================================
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CLine3D::rotatePlaneXY(double sinAngle, double cosAngle)
{
    myPoint.rotatePlaneXY(sinAngle, cosAngle);
    myDirection.rotatePlaneXY(sinAngle, cosAngle);
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CLine3D::rotatePlaneXY(double angle)
{
    rotatePlaneXY(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CLine3D::rotatePlaneYZ(double sinAngle, double cosAngle)
{
    myPoint.rotatePlaneYZ(sinAngle, cosAngle);
    myDirection.rotatePlaneYZ(sinAngle, cosAngle);
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CLine3D::rotatePlaneYZ(double angle)
{
    rotatePlaneYZ(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CLine3D::rotatePlaneZX(double sinAngle, double cosAngle)
{
    myPoint.rotatePlaneZX(sinAngle, cosAngle);
    myDirection.rotatePlaneZX(sinAngle, cosAngle);
}




//-----------------------------------------------------------------------------
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CLine3D::rotatePlaneZX(double angle)
{
    rotatePlaneZX(sin(angle), cos(angle));
}




//=====================================================================================================================
// Переход к новым координатным осям
bool CLine3D::changeBasis(const CVector3D &newOx, const CVector3D &newOy, const CVector3D &newOz)
{
    if (myPoint.changeBasis(newOx, newOy, newOz) == false)
        return false;
    myDirection.changeBasis(newOx, newOy, newOz);
    return true;
}




//=====================================================================================================================
// Определение параллельности прямых и векторов
bool CLine3D::isParallelTo(const CLine3D &other) const
{
    // Две прямые параллельны, если их направляющие вектора коллинеарны
    return myDirection.isCollinearTo(other.myDirection);
}




//---------------------------------------------------------------------------------------------------------------------
bool CLine3D::isParallelTo(const CVector3D &vect) const
{
    // Вектор параллелен прямой, если он коллинеарен её направляющему вектору
    return myDirection.isCollinearTo(vect);
}




//=====================================================================================================================
// Определение перпендикулярности прямых и векторов
bool CLine3D::isNormalTo(const CLine3D &other) const
{
    // Две прямые взаимно перпендикулярны, если перпендикулярны
    // их направляющие векторы
    return myDirection.isNormalTo(other.myDirection);
}




//---------------------------------------------------------------------------------------------------------------------
bool CLine3D::isNormalTo(const CVector3D &vect) const
{
    // Вектор перпендикулярен прямой, если он перпендикулярен
    // её направляющему вектору
    return myDirection.isNormalTo(vect);
}




//=====================================================================================================================
// Определение компланарности прямых
bool CLine3D::isCoplanarTo(const CLine3D &other) const
{
    CVector3D vect(myPoint.getX() - other.myPoint.getX(),
                   myPoint.getY() - other.myPoint.getY(),
                   myPoint.getZ() - other.myPoint.getZ());

    return vect.isCoplanarTo(myDirection, other.myDirection);
}




//=====================================================================================================================
// Определение принадлежности точки прямой
bool CLine3D::containsPoint(const CPoint3D &point) const
{
    // Частные случаи, когда прямая параллельна координатным плоскостям
    if (checkEQ(myDirection.getX(), 0.0) == true)
    {
        if (checkEQ(point.getX(), myPoint.getX()) == false)
            return false;
        else
            return true;
    }

    if (checkEQ(myDirection.getY(), 0.0) == true)
    {
        if (checkEQ(point.getY(), myPoint.getY()) == false)
            return false;
        else
            return true;
    }

    if (checkEQ(myDirection.getZ(), 0.0) == true)
    {
        if (checkEQ(point.getZ(), myPoint.getZ()) == false)
            return false;
        else
            return true;
    }

    double dx = (point.getX() - myPoint.getX()) / myDirection.getX();
    double dy = (point.getY() - myPoint.getY()) / myDirection.getY();
    double dz = (point.getZ() - myPoint.getZ()) / myDirection.getZ();

    // Через каноническое уравнение прямой
    if (checkEQ(dx, dy) == true &&
        checkEQ(dy, dz) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Вычисление расстояния до точки
double CLine3D::computeDistanceTo(const CPoint3D &point) const
{
    return point.computeDistanceTo(*this);
}




//=====================================================================================================================
// Вычисление расстояния до точки
double CLine3D::computeDistanceTo(const CLine3D& other) const
{
    // Для скрещивающихся прямых result = (s0,s1,s2)/(s1 x s2)
    // Для параллельных прямых result = (s0 x s1) / |s1|
    // (s0,s1,s2) - смешанное произведение 3ёх векторов
    // где вектор s0 = r2 - r1,     вектор s1 - это вектор myDirection,     вектор s2 - это вектор other.getDirection;
    // r2 = CVector3D(other.getPoint(), CPoint3D(0,0,0)),       а r1 = CVector3D(myPoint, CPoint3D(0,0,0))

    // вместо формирования векторов, которые содержат опорные точки рассматриваемых скрещивающихся векторов, для простоты берутся просто тчк
   
    // Первая точка - это точка данной линии, т.е. myPoint
    const CPoint3D & p_other = other.getPoint();    // Это вторая точка

    //Формируем вектор (r2-r1), где r2, r1 - вектора, содержащие опорные точки
    CVector3D s0 = CVector3D(p_other.getX() - myPoint.getX(), p_other.getY() - myPoint.getY(), p_other.getZ() - myPoint.getZ());

    double result;
    // Определяем параллельны ли прямые
    if (this->isParallelTo(other))
        result = s0.computeVectorProductWith(myDirection).getLength() / myDirection.getLength();
    else
    {
        CVector3D vec_s1s2 = myDirection.computeVectorProductWith(other.getDirection());       // векторное произведение s1 и s2 (s1 x s2)
        result = s0.computeScalarProductWith(vec_s1s2) / vec_s1s2.getLength();
    }

    return abs(result);
}




//=====================================================================================================================
// Вычисление точки пересечения двух прямых
vector<CPoint3D> CLine3D::computeIntersectionWith(const CLine3D &other) const
{
    vector<CPoint3D> result;    // Будущий набор точек пересечения

    // Если прямые некомпланарны, то они не пересекаются
    if (isCoplanarTo(other) == false)
        return result;

    // Если прямые параллельны, то они тоже не пересекаются
    if (isParallelTo(other) == true)
        return result;

    // Вычисляем точку пересечения через параметричекую форму задания прямых.
    // Для этого сначала посчитаем значение параметра в точке пересечения, а потом
    // на его основе посчитаем координаты точки пересечения.

    // Вычисляем значение параметра
    double param = 0.0;
    double denom = myDirection.getX() - other.myDirection.getX();
    if (checkEQ(denom, 0.0) == false)
    {
        param = (other.myPoint.getX() - myPoint.getX()) / denom;
    }
    else
    {
        denom = myDirection.getY() - other.myDirection.getY();
        if (checkEQ(denom, 0.0) == false)
        {
            param = (other.myPoint.getY() - myPoint.getY()) / denom;
        }
        else
        {
            denom = myDirection.getZ() - other.myDirection.getZ();
            // Все три координаты обоих векторов не могут совпадать, поскольку прямые не параллельны
            param = (other.myPoint.getZ() - myPoint.getZ()) / denom;
        }
    }

    // Вычисление координат точки пересечения
    CPoint3D point(myPoint.getX() + param * myDirection.getX(),
                   myPoint.getY() + param * myDirection.getY(),
                   myPoint.getZ() + param * myDirection.getZ());
    result.push_back(point);
    return result;
}




//=====================================================================================================================
// Вычисление точек пересечения прямой со сферой
vector<CPoint3D> CLine3D::computeIntersectionWith(const CSphere3D &sphere) const
{
    return sphere.computeIntersectionWith(*this);
}




//=====================================================================================================================
// Вычисление опорной точки и направляющего вектора по двум точкам
void CLine3D::_computeLine(const CPoint3D &p1, const CPoint3D &p2)
{
    // Если точки совпадают, то проводим через них произвольную прямую
    if (p1 == p2)
    {
        myPoint = p1;
        myDirection.setCoords(1.0, 0.0, 0.0);
        ErrorCode = ERR_BAD_LINE;
        return;
    }

    myPoint = p1;
    myDirection.setCoords(p1.getX() - p2.getX(), p1.getY() - p2.getY(), p1.getZ() - p2.getZ());
    ErrorCode = ERR_OK;
}




//=====================================================================================================================
// Нормирование длины направляющего вектора на единицу
void CLine3D::_normalizeDirection()
{
    myDirection.setLength(1.0);
}

