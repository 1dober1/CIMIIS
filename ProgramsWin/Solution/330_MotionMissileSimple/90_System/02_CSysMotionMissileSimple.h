// Файл 02_CSysMotionMissileSimple.h
// Описание новой функциональной системы движения
//

#ifndef _330_MotionMissileSimple_System_CSysMotionNew_h
#define _330_MotionMissileSimple_System_CSysMotionNew_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CSysMotionMissileSimpleType.h"


//=====================================================================================================================
// Описание новой функциональной системы движения
class CSysMotionMissileSimple: public CSysMotion
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    double myMeetTime;          // Время до встречи
    size_t myTargetAbsNum;      // Номер цели

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CSysMotionMissileSimple(const CSysType &sysType, CObject &object);

    // Деструктор класса
    virtual ~CSysMotionMissileSimple();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к типу системы
    virtual const CSysMotionMissileSimpleType & getType() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Сменные алгоритмы действий
    virtual void onAppear(double time);
    virtual void onDestroy(double time);
    virtual void onActivate(double time);
    virtual void onDisappear(double time);
    virtual bool onRouteEnd(double time);
    virtual void onTaskCmd(const CTaskCmd &taskCmd);
    virtual void onTaskPoint(const CTaskPoint &taskPoint);
    virtual void onImpact(double time, const CImpact &impact);
    virtual bool onRequest(double time, CRequest &request);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


