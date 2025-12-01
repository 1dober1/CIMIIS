// Файл 01_CPointLocal.h
// Положение моделируемого объекта в местной декартовой системе координат
//

#ifndef Core_CoordSystems_CPointLocal_h
#define Core_CoordSystems_CPointLocal_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"


//=====================================================================================================================
// Положение моделируемого объекта в местной декартовой системе координат
class CPointGlobal;
class CPointLocal
{
protected:
    //-----------------------------------------------------------------------------------------------------------------
    double myTime;          // Момент времени [с]
    CPoint3D myPosition;    // Мгновенные координаты объекта [м]
    CVector3D myVelocity;   // Вектор мгновенной скорости объекта [м/с]
    double myRoll;          // Мгновенный крен относительно горизонта [градусы]


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CPointLocal();
    CPointLocal(double t, const CPoint3D &pos, double roll = 0.0);
    CPointLocal(double t, const CPoint3D &pos, const CVector3D &vel, double roll = 0.0);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Пересчёт положения объекта в геоцентрическую систему координат
    CPointGlobal toPointGlobal(const CPoint3D &ortho) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к параметрам положения объекта
    double getTime() const;
    const CPoint3D & getPosition() const;
    const CVector3D & getVelocity() const;
    double getRoll() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Изменение параметров положения объекта
    void setTime(double time);
    void setPosition(const CPoint3D &pos);
    void setVelocity(const CVector3D &vel);
    void setRoll(double roll);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Направление мгновенной скорости на заданную точку
    void adjustVelocityTo(const CPoint3D &nextPoint);


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
    // Вычисление угла места некоторой точки относительно данного положения объекта [радианы]
    double computeRadianElevationOf(const CPoint3D &point) const;

    // Вычисление азимута некоторой точки относительно данного положения объекта [радианы]
    double computeRadianAzimuthOf(const CPoint3D &point) const;

    // Вычисление текущего направления движения объекта по азимуту [радианы]
    double computeRadianSelfAzimuth() const;

    // Вычисление текущего направления движения объекта по углу места [радианы]
    double computeRadianSelfElevation() const;


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Вспомогательная функция для преобразования в геоцентрическую СК
    static CPoint3D _transformToGlobal(const CPoint3D &point, const CPoint3D &ortho);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


