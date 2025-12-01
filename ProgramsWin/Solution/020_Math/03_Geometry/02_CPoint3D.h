// Файл 02_CPoint3D.h
// Точка в пространстве
//

#ifndef Math_Geometry_CPoint3D_h
#define Math_Geometry_CPoint3D_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CVector3D.h"


//=====================================================================================================================
// Точка в пространстве
class CLine3D;
class CPoint3D
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    // Координаты точки
    double myX;
    double myY;
    double myZ;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CPoint3D();
    CPoint3D(double x, double y, double z);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к координатам точки
    double getX() const;
    double getY() const;
    double getZ() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Изменение координат точки
    void setCoords(double x, double y, double z);
    void setX(double x);
    void setY(double y);
    void setZ(double z);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Проверка точек на совпадение
    bool operator ==(const CPoint3D &other) const;
    bool operator !=(const CPoint3D &other) const;


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
    // Перенос точки на вектор
    void moveOn(const CVector3D &vect);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление расстояния до другой точки
    double computeDistanceTo(const CPoint3D &other) const;

    // Вычисление расстояния до прямой
    double computeDistanceTo(const CLine3D &line) const;


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


