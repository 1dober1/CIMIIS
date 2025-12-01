#ifndef _226_NavedenieZPS_System_CSysMotionNew_h
#define _226_NavedenieZPS_System_CSysMotionNew_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CSysMotionNewType.h"

// Фазы работы системы наведения БпЛА
enum ENavZpsStage
{
    nzTakeoff = 0,      // взлёт / начальный участок маршрута
    nzSearch,           // поиск цели
    nzAttack,           // цель обнаружена, выполняем наведение и пуск
    nzReturn,           // цель уничтожена, возвращаемся
    nzFinished          // всё сделано, ждём исчезновения
};


//=====================================================================================================================
// Описание новой функциональной системы движения
class CSysMotionNew : public CSysMotion
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    // старые учебные параметры — можешь потом выкинуть, если не нужны
    uint32 myStateParamOne;
    double myStateParamTwo;

    // Текущее состояние наведения
    ENavZpsStage myStage;

    // Цель (синий БпЛА)
    int          myTargetAbsNum;    // абсолютный номер объекта-цели в ObjectList
    bool         myHasTarget;       // цель выбрана
    bool         myTargetVisible;   // цель видна (по данным ОЭС / упрощённо)
    bool         myWeaponLaunched;  // ракета уже пущена
    bool         myTargetKilled;    // пришёл Impact попадания по цели

    // Точка взлёта (для логики возврата)
    CPointGlobal myHomePoint;

    // Параметры невсеракурсного пуска
    bool   myMissileParamsInit; // получили ли параметры ракеты через запрос
    double myMissileMinRange;   // минимальная дальность пуска, м
    double myMissileMaxRange;   // максимальная дальность пуска, м
    double myMissileSectorDeg;  // сектор применения УР (из системы пуска), град
    double myRearHalfAngleDeg;  // полу-угол задней полусферы (по сути ограничение ракурса)

    // Вспомогательные методы
    bool  isRearHemisphereGeometryOk(double time, double& distance) const;
    void  tryInitMissileParams(double time);

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CSysMotionNew(const CSysType& sysType, CObject& object);

    // Деструктор класса
    virtual ~CSysMotionNew();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к типу системы
    virtual const CSysMotionNewType& getType() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Сменные алгоритмы действий
    virtual void onAppear(double time);
    virtual void onDestroy(double time);
    virtual void onActivate(double time);
    virtual void onDisappear(double time);

    virtual bool onRouteEnd(double time);

    virtual void onTaskCmd(const CTaskCmd& taskCmd);
    virtual void onTaskPoint(const CTaskPoint& taskPoint);
    virtual void onImpact(double time, const CImpact& impact);
    virtual bool onRequest(double time, CRequest& request);
};


//=====================================================================================================================
#endif
