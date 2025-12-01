// Файл 02_CSysLaunch.h
// Описание новой функциональной системы
//

#ifndef _700_CSysLaunch_System_CSysNew_h
#define _700_CSysLaunch_System_CSysNew_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CSysLaunchType.h"


//=====================================================================================================================
// Возможные состояния цели
enum myTargetStates : int8
{
    tarEmpty = 0,
    tarLaunchPrepare = 1,
    tarReadyForAttack = 2,
    tarWeaponPrepare = 3,
    tarWeaponStart = 4,
    tarWeaponAppearance = 5,
    tarWeaponFlight = 6,
    tarWeaponExplode = 7,
    tarTargetLost = 8,
    tarTargetSingleEnd = 9,
    tarTargetGlobalEnd = 10,
    tarTargetRefuse = 11,
};


//=====================================================================================================================
// Возможные состояния оружия
enum myWeaponStates : int8
{
    wpnEmpty = 0,
    wpnReserved = 1,
    wpnApplied = 2,
    wpnExplode = 3,
    wpnDestroy = 4,
    wpnDissapear = 5
};


//=====================================================================================================================
// Формуляр цели
struct targetParam
{
    size_t targetAbsNum;
    myTargetStates targetState;
    size_t weaponAbsNum;
    myWeaponStates weaponState;
    double weaponDeltaTimeToMeet;
    double weaponDieTime;

    double stateActivationTime;                     // Время, с которого может начать обрабатываться следующее состояние цели

    targetParam()
    {
        targetAbsNum = 0;
        targetState = tarEmpty;
        weaponAbsNum = 0;
        weaponState = wpnEmpty;
        weaponDeltaTimeToMeet = 0.;
        weaponDieTime = 0.;

        stateActivationTime = 0.;
    }
};


//=====================================================================================================================
// Описание новой функциональной системы
class CSysLaunch: public CSys
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    bool myIsSysWorking;                                            // Состояние Функциональной системы "Система пуска"

    bool myIsWeaponEmpty;                                           // Нет оружия, на точках подвесок ничего не висит: оно не устанавливалось или полность израсходовано

    double myStartDelayBlockTime;                                   // Блокировка ВСЕЙ системы, а не отдельных заявок, для реализации задержки между пусками

    uint8 myFreeWeaponCount;                                        // Кол-во "свободных" ракет
    uint8 myWeaponLaunchCount;                                      // Кол-во запущенных ракет

    std::multimap<size_t, targetParam> myTargetsMap;                // Список целей/заявок целей

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CSysLaunch(const CSysType &sysType, CObject &object);

    // Деструктор класса
    virtual ~CSysLaunch();
	

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к типу системы
    virtual const CSysLaunchType & getType() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Сменные алгоритмы действий
    virtual void onAppear(double time);
    virtual void onDestroy(double time);
    virtual void onActivate(double time);
    virtual void onDisappear(double time);
    virtual void onTaskCmd(const CTaskCmd &taskCmd);
    virtual void onTaskPoint(const CTaskPoint &taskPoint);
    virtual void onImpact(double time, const CImpact &impact);
    virtual bool onRequest(double time, CRequest &request);

private:
    //-----------------------------------------------------------------------------------------------------------------
    // Блоки модели пуска ракеты
    virtual bool _moduleSystemCheck(double time);                                                                               // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarLaunchPrepare(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);         // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarReadyForAttack(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarWeaponPrepare(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);         // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarWeaponStart(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);           // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarWeaponAppearance(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);      // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarWeaponFlight(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);          // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarTargetLost(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);            // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
    virtual bool _moduleTarTargetRefuse(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);          // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
	virtual bool _moduleTarTargetSingleEnd(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);       // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
	virtual bool _moduleTarTargetGlobalEnd(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);       // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
	
    // Блоки, которые должны вызываться как реакция на импакты
    virtual bool _moduleImpTarWeaponExplode(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);      // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onImpact
    virtual bool _moduleImpTarWeaponDisapear(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);     // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onImpact
    virtual bool _moduleImpTarWeaponDestroy(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator);      // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onImpact


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Обработка воздействия "Команда на пуск ракеты по цели"
    virtual void _processImpactLaunchWeapon(double time, const CImpact& impact);
    virtual void _processImpactTargetLost(double time, const CImpact& impact);
    virtual void _processImpactWeaponExplosion(double time, const CImpact& impact);
    virtual void _processImpactDisappear(double time, const CImpact& impact);
    virtual void _processImpactDestroy(double time, const CImpact& impact);
    virtual void _processImpactFireDelay(double time, const CImpact& impact);
    virtual void _processRequestAmmunition(double time, CRequest& request);
    virtual void _processRequestAmmunitionParam(double time, CRequest& request);
    virtual void _processRequestServiceTime(double time, CRequest& request);


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Вычисление времени до встречи ракеты и цели
    virtual double _computeTimeToMeetWeapon(CPoint3D inStartPoint, CPoint3D inTargetPoint);

public: 
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к характеристикам систем
    double getStartDelayBlockTime() const;
    void   setDelayBlockTime(double time);      // time - время активации системы


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


