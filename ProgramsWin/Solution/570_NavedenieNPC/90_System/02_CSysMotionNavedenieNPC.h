#ifndef _570_NavedenieNPC_System_CSysMotionNavedenieNPC_h
#define _570_NavedenieNPC_System_CSysMotionNavedenieNPC_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CSysMotionNavedenieNPCType.h"
#include "570_NavedenieNPC/FD_BpLA/CFlightDynamicsBpLA.h"
#include "570_NavedenieNPC/FD_BpLA/BpLA.h"


//=====================================================================================================================
// Описание новой функциональной системы движения
class CSysMotionNavedenieNPC: public CSysMotion
{

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CSysMotionNavedenieNPC(const CSysType &sysType, CObject &object);

	MyBpLAStruct plane;
	CFlightDynamicsBpLA fl;
		

    // Деструктор класса
    virtual ~CSysMotionNavedenieNPC();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к типу системы
    virtual const CSysMotionNavedenieNPCType & getType() const;


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


