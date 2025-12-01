// Файл 04_CSphere3D.h
// Сфера в пространстве
//

#ifndef Math_Geometry_CSphere3D_h
#define Math_Geometry_CSphere3D_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "02_CPoint3D.h"
#include "03_CLine3D.h"


//=====================================================================================================================
// Сфера в пространстве
class CSphere3D
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    CPoint3D myCentre;      // Координаты центра сферы
    double myRadius;        // Радиус сферы


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CSphere3D();
    CSphere3D(const CPoint3D &centre, double radius);
    CSphere3D(double xCentre, double yCentre, double zCentre, double radius);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к параметрам сферы
    const CPoint3D & getCentre() const;
    double getRadius() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Изменение параметров сферы
    void setCentre(const CPoint3D &centre);
    void setCentre(double xCentre, double yCentre, double zCentre);
    void setRadius(double radius);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Проверка сфер на совпадение
    bool operator ==(const CSphere3D &other) const;
    bool operator !=(const CSphere3D &other) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Перенос начала координат в заданную точку
    void moveOrthoTo(double newOrthoX, double newOrthoY, double newOrthoZ);
    void moveOrthoTo(const CPoint3D &newOrtho);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Поворот координатной плоскости xOy вокруг оси Oz в направлении от Ox к Oy
    void rotatePlaneXY(double sinAngle, double cosAngle);
    void rotatePlaneXY(double angle);

    // Поворот координатной плоскости yOz вокруг оси Ox в направлении от Oy к Oz
    void rotatePlaneYZ(double sinAngle, double cosAngle);
    void rotatePlaneYZ(double angle);

    // Поворот координатной плоскости zOx вокруг оси Oy в направлении от Oz к Ox
    void rotatePlaneZX(double sinAngle, double cosAngle);
    void rotatePlaneZX(double angle);

    // Переход к новым координатным осям
    bool changeBasis(const CVector3D &newOx, const CVector3D &newOy, const CVector3D &newOz);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Определение принадлежности точки сфере
    bool containsPoint(const CPoint3D &point) const;

    // Определение принадлежности точки шару, ограниченному сферой
    bool containsPointInside(const CPoint3D &point) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление точек пересечения с прямой
    vector<CPoint3D> computeIntersectionWith(const CLine3D &line) const;



public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление площади сферы
    double computeArea() const;

    // Вычисление объёма шара, ограниченного сферой
    double computeVolume() const;


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


