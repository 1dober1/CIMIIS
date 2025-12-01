// Файл 03_CLine3D.h
// Прямая в пространстве
//

#ifndef Math_Geometry_CLine3D_h
#define Math_Geometry_CLine3D_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CVector3D.h"
#include "02_CPoint3D.h"


//=====================================================================================================================
// Прямая в пространстве
class CSphere3D;
class CLine3D
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    // Опорная точка
    CPoint3D myPoint;

    // Направляющий вектор
    CVector3D myDirection;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CLine3D(const CPoint3D &p1, const CPoint3D &p2);
    CLine3D(const CPoint3D &point, const CVector3D &direction);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к направляющему вектору
    const CVector3D & getDirection() const;

    // Доступ к опорной точке
    const CPoint3D & getPoint() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Изменение параметров прямой
    void set(const CPoint3D &p1, const CPoint3D &p2);
    void set(const CPoint3D &point, const CVector3D &direction);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Проверка прямых на совпадение
    bool operator ==(const CLine3D &other) const;
    bool operator !=(const CLine3D &other) const;


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
    // Определение параллельности прямых и векторов
    bool isParallelTo(const CLine3D &other) const;
    bool isParallelTo(const CVector3D &vect) const;

    // Определение перпендикулярности прямых и векторов
    bool isNormalTo(const CLine3D &other) const;
    bool isNormalTo(const CVector3D &vect) const;

    // Определение компланарности прямых
    bool isCoplanarTo(const CLine3D &other) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Определение принадлежности точки прямой
    bool containsPoint(const CPoint3D &point) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление расстояния до точки
    double computeDistanceTo(const CPoint3D &point) const;

	// Вычисление расстояния до прямой
	double computeDistanceTo(const CLine3D& other) const;

    // Вычисление точки пересечения двух прямых
    vector<CPoint3D> computeIntersectionWith(const CLine3D &other) const;

    // Вычисление точек пересечения прямой со сферой
    vector<CPoint3D> computeIntersectionWith(const CSphere3D &sphere) const;


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление опорной точки и направляющего вектора по двум точкам
    void _computeLine(const CPoint3D &p1, const CPoint3D &p2);

    // Нормирование длины направляющего вектора на единицу
    void _normalizeDirection();


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


