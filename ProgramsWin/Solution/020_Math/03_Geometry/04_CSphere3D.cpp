// Файл 04_CSphere3D.cpp
// Сфера в пространстве
//

//=====================================================================================================================
// Заголовочные файлы
#include "04_CSphere3D.h"




//=====================================================================================================================
// Конструктор класса
CSphere3D::CSphere3D()
{
    myRadius = 0.0;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CSphere3D::CSphere3D(const CPoint3D &centre, double radius)
{
    myCentre = centre;
    myRadius = radius;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CSphere3D::CSphere3D(double xCentre, double yCentre, double zCentre, double radius)
{
    myCentre.setCoords(xCentre, yCentre, zCentre);
    myRadius = radius;
}




//=====================================================================================================================
// Доступ к параметрам сферы
const CPoint3D & CSphere3D::getCentre() const
{
    return myCentre;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам сферы
double CSphere3D::getRadius() const
{
    return myRadius;
}





//=====================================================================================================================
// Изменение параметров сферы
void CSphere3D::setCentre(const CPoint3D &centre)
{
    myCentre = centre;
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение параметров сферы
void CSphere3D::setCentre(double xCentre, double yCentre, double zCentre)
{
    myCentre.setCoords(xCentre, yCentre, zCentre);
}




//---------------------------------------------------------------------------------------------------------------------
// Изменение параметров сферы
void CSphere3D::setRadius(double radius)
{
    myRadius = radius;
}




//=====================================================================================================================
// Проверка сфер на совпадение
bool CSphere3D::operator ==(const CSphere3D &other) const
{
    if (myCentre == other.myCentre   &&
        checkEQ(myRadius, other.myRadius) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Проверка сфер на несовпадение
bool CSphere3D::operator !=(const CSphere3D &other) const
{
    return !(operator ==(other));
}




//=====================================================================================================================
// Перенос начала координат в заданную точку
void CSphere3D::moveOrthoTo(double newOrthoX, double newOrthoY, double newOrthoZ)
{
    myCentre.moveOrthoTo(newOrthoX, newOrthoY, newOrthoZ);
}




//---------------------------------------------------------------------------------------------------------------------
// Перенос начала координат в заданную точку
void CSphere3D::moveOrthoTo(const CPoint3D &newOrtho)
{
    moveOrthoTo(newOrtho.getX(), newOrtho.getY(), newOrtho.getZ());
}




//=====================================================================================================================
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CSphere3D::rotatePlaneXY(double sinAngle, double cosAngle)
{
    myCentre.rotatePlaneXY(sinAngle, cosAngle);
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
void CSphere3D::rotatePlaneXY(double angle)
{
    rotatePlaneXY(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CSphere3D::rotatePlaneYZ(double sinAngle, double cosAngle)
{
    myCentre.rotatePlaneYZ(sinAngle, cosAngle);
}




//---------------------------------------------------------------------------------------------------------------------
// Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
void CSphere3D::rotatePlaneYZ(double angle)
{
    rotatePlaneYZ(sin(angle), cos(angle));
}




//=====================================================================================================================
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CSphere3D::rotatePlaneZX(double sinAngle, double cosAngle)
{
    myCentre.rotatePlaneZX(sinAngle, cosAngle);
}




//-----------------------------------------------------------------------------
// Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
void CSphere3D::rotatePlaneZX(double angle)
{
    rotatePlaneZX(sin(angle), cos(angle));
}




//=====================================================================================================================
// Переход к новым координатным осям
bool CSphere3D::changeBasis(const CVector3D &newOx, const CVector3D &newOy, const CVector3D &newOz)
{
    if (myCentre.changeBasis(newOx, newOy, newOz) == false)
        return false;
    return true;
}




//=====================================================================================================================
// Определение принадлежности точки сфере
bool CSphere3D::containsPoint(const CPoint3D &point) const
{
    // Уравнение сферы: (x - xC)^2 + (y - yC)^2 + (z - zC)^2 = R^2
    double dx = point.getX() - myCentre.getX();
    double dy = point.getY() - myCentre.getY();
    double dz = point.getZ() - myCentre.getZ();
    if (checkEQ(dx*dx + dy*dy + dz*dz,   myRadius*myRadius) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Определение принадлежности точки шару, ограниченному сферой
bool CSphere3D::containsPointInside(const CPoint3D &point) const
{
    // Уравнение сферы: (x - xC)^2 + (y - yC)^2 + (z - zC)^2 = R^2
    double dx = point.getX() - myCentre.getX();
    double dy = point.getY() - myCentre.getY();
    double dz = point.getZ() - myCentre.getZ();
    if (checkLE(dx*dx + dy*dy + dz*dz,  myRadius*myRadius) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Вычисление точек пересечения с прямой
//
// Общее уравнение сферы:
//   (x - xC)^2 + (y - yC)^2 + (z - zC)^2 = R^2,
//
// где
//   (xC, yC, zC) - центр сферы,
//   R - радиус сферы
//
// Параметрическое уравнение прямой
//   x = x0 + Vx*t,
//   y = y0 + Vy*t,
//   z = z0 + Vz*t,
//
// где
//   (x0, y0, z0) - опорная точка на прямой
//   (Vx, Vy, Vz) - единичный направляющий вектор прямой
//
vector<CPoint3D> CSphere3D::computeIntersectionWith(const CLine3D &line) const
{
    vector<CPoint3D> result;        // Будущий набор точек пересечения

    const CVector3D &vect = line.getDirection();
    const CPoint3D &point = line.getPoint();

    // Доступ к компонентам направляющего вектора прямой и
    // координатам опорной точки прямой
    double Vx = vect.getX();
    double Vy = vect.getY();
    double Vz = vect.getZ();
    double x0 = point.getX();
    double y0 = point.getY();
    double z0 = point.getZ();

    // Доступ к координатам центра сферы и её радиусу
    double xC = myCentre.getX();
    double yC = myCentre.getY();
    double zC = myCentre.getZ();
    double R = myRadius;

    // Если прямая находится в плоскости yOz
    if (checkEQ(Vx, 0.0) == true)
    {
        // Если прямая параллельна оси Oz
        if (checkEQ(Vy, 0.0) == true)
        {
            // Вычисление промежуточных переменных 
            double F = y0 - yC;
            double G = x0 - xC;
            double M = R*R - F*F - G*G;

            // Если нет точек пересечения
            if (checkLT(M, 0.0) == true)
                return result;

            // Если одна точка пересечения
            if (checkEQ(M, 0.0) == true)
            {
                result.push_back(CPoint3D(x0, y0, zC));
                return result;
            }

            // Если две точки пересечения
            double sqrtM = sqrt(M);
            result.push_back(CPoint3D(x0, y0, zC - sqrtM));
            result.push_back(CPoint3D(x0, y0, zC + sqrtM));
            return result;
        }

        // Если прямая находится в плоскости yOz, но не параллельна оси Oz
        else
        {
            // Вычисление промежуточных переменных 
            double P = Vy*Vy + Vz*Vz;
            double F = y0 - yC;
            double G = x0 - xC;
            double H = z0 - zC;
            double K = Vy*Vy * (P*R*R - (G*G + H*H)*Vy*Vy + 2.0*F*H*Vy*Vz - (F*F + G*G)*Vz*Vz);
            double S = Vz*(-H*Vy + Vz*y0) + Vy*Vy*yC;
            double T = -F*Vy*Vy*Vz + Vy*Vy*Vy*z0 + Vy*Vz*Vz*zC;

            // Если нет точек пересечения
            if (checkLT(K, 0.0) == true)
                return result;

            // Если одна точка пересечения
            if (checkEQ(K, 0.0) == true)
            {
                double y = S / P;
                double z = T / (Vy*P);
                result.push_back(CPoint3D(x0, y, z));
                return result;
            }

            // Если две точки пересечения
            double sqrtK = sqrt(K);
            double y1 = (S + sqrtK) / P;
            double z1 = (T + Vz*sqrtK) / (Vy*P);
            result.push_back(CPoint3D(x0, y1, z1));

            double y2 = (S - sqrtK) / P;
            double z2 = (T - Vz*sqrtK) / (Vy*P);
            result.push_back(CPoint3D(x0, y2, z2));
            return result;
        }
    }

    // Если прямая не находится в плоскости yOz
    else
    {
        // Вычисление промежуточных переменных
        double P = Vx*Vx + Vy*Vy + Vz*Vz;
        double F = y0 - yC;
        double G = x0 - xC;
        double H = z0 - zC;
        double K = -F*Vx*Vy - H*Vx*Vz + x0*Vy*Vy + x0*Vz*Vz + xC*Vx*Vx;
        double L = Vz*x0 - Vx*z0 + Vx*zC;
        double M = K*K + P * (-L*L + R*R*Vx*Vx + 2.0*F*Vx*Vy*x0 - Vy*Vy*x0*x0 - (F*F + xC*xC)*Vx*Vx);
        double Q = x0*Vy*Vy + x0*Vz*Vz + xC*Vx*Vx - Vx*Vy*F - Vx*Vz*H;
        double S = -G*Vz*Vx*Vx + z0*Vx*Vx*Vx + Vx*Vy*(-F*Vz + Vy*z0) + Vx*zC*Vz*Vz;
        double T = -G*Vy*Vx*Vx + y0*Vx*Vx*Vx + Vx*Vz*(-H*Vy + Vz*y0) + Vx*yC*Vy*Vy;

        // Если точек пересечения нет
        if (checkLT(M, 0.0) == true)
            return result;

        // Если одна точка пересечения
        if (checkEQ(M, 0.0) == true)
        {
            double x = Q / P;
            double y = T / (Vx*P);
            double z = S / (Vx*P);
            result.push_back(CPoint3D(x, y, z));
            return result;
        }

        // Если две точки пересечения
        double sqrtM = sqrt(M);
        double x1 = (Q + sqrtM) / P;
        double y1 = (T + Vy*sqrtM) / (Vx*P);
        double z1 = (S + Vz*sqrtM) / (Vx*P);
        result.push_back(CPoint3D(x1, y1, z1));

        double x2 = (Q - sqrtM) / P;
        double y2 = (T - Vy*sqrtM) / (Vx*P);
        double z2 = (S - Vz*sqrtM) / (Vx*P);
        result.push_back(CPoint3D(x2, y2, z2));
        return result;
    }
}






//=====================================================================================================================
// Вычисление площади сферы
//   S = 4*pi*r*r
double CSphere3D::computeArea() const
{
    return 4.0*Pi * myRadius*myRadius;
}




//=====================================================================================================================
// Вычисление объёма шара, ограниченного сферой
// Вычисление объёма шара, ограниченного сферой
//   V = 4/3 *pi*r*r*r
double CSphere3D::computeVolume() const
{
    return 4.0/3.0*Pi * myRadius*myRadius*myRadius;
}

