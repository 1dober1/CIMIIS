// Файл 01_CVector3D.h
// Вектор в пространстве
//

#ifndef Math_Geometry_CVector3D_h
#define Math_Geometry_CVector3D_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"


//=====================================================================================================================
// Вектор в пространстве
class CPoint3D;
class CVector3D
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    // Длина вектора
    double myLength;

    // Направляющие косинусы
    double myCosX;
    double myCosY;
    double myCosZ;

    // Координаты вектора
    double myX;
    double myY;
    double myZ;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CVector3D();
    CVector3D(double x, double y, double z);
    CVector3D(const CPoint3D &start, const CPoint3D &end);
    CVector3D(double length, double cosX, double cosY, double cosZ);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к координатам вектора
    double getX() const;
    double getY() const;
    double getZ() const;

    // Доступ к длине вектора
    double getLength() const;

    // Доступ к направляющим косинусам
    double getCosX() const;
    double getCosY() const;
    double getCosZ() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Изменение координат вектора
    void setCoords(double x, double y, double z);

    // Изменение длины вектора
    void setLength(double length);

    // Изменение направляющих косинусов вектора
    void setCos(double cosX, double cosY, double cosZ);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Проверка векторов на совпадение
    bool operator ==(const CVector3D &other) const;
    bool operator !=(const CVector3D &other) const;


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
    // Вычисление суммы векторов
    CVector3D add(const CVector3D &other) const;
    CVector3D operator +(const CVector3D &other) const;

    // Вычисление разности векторов
    CVector3D subtract(const CVector3D &other) const;
    CVector3D operator -(const CVector3D &other) const;

    // Вычисление произведения вектора на скаляр
    CVector3D multiply(double ratio) const;
    CVector3D operator *(double ratio) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление скалярного произведения с другим вектором
    double computeScalarProductWith(const CVector3D &other) const;

    // Вычисление векторного произведения с другим вектором
    CVector3D computeVectorProductWith(const CVector3D &other) const;

    // Вычисление косинуса угла между векторами
    double computeAngleWith(const CVector3D &other) const;

    // Вычисление проекции на другой вектор
    CVector3D computeProjectionOn(const CVector3D &other) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Определение сонаправленности векторов
    bool isDirectedAs(const CVector3D &other) const;

    // Определение коллинеарности векторов
    bool isCollinearTo(const CVector3D &other) const;

    // Определение перпендикулярности векторов
    bool isNormalTo(const CVector3D &other) const;

    // Определение компланарности векторов
    bool isCoplanarTo(const CVector3D &v1, const CVector3D &v2) const;


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление длины вектора по координатам
    void _computeLength();

    // Вычисление направляющих косинусов по координатам и длине вектора
    void _computeCos();

    // Вычисление координат по длине и косинусам
    void _computeCoords();


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


