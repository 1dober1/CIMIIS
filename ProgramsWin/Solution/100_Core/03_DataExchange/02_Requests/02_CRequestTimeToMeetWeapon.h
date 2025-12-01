// Файл 02_CRequestTimeToMeetWeapon.h
// Запрос боекомплекта
//

#ifndef Core_DataExchange_CRequestTimeToMeetWeapon_h
#define Core_DataExchange_CRequestTimeToMeetWeapon_h


//=====================================================================================================================
// Заголовочные файлы
#include "../../Settings.h"
#include "../../02_TaskReader/01_CTaskPoint.h"
#include "../02_CRequest.h"


//=====================================================================================================================
// Запрос боекомплекта
class CRequestTimeToMeetWeapon : public CRequest
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    double myDeltaTimeToMeet;                               

    CTaskPoint myStartWeaponPoint;
    CTaskPoint myTargetPoint;

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CRequestTimeToMeetWeapon();

    // Деструктор класса
    virtual ~CRequestTimeToMeetWeapon();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Управление результатами запроса
    double getDeltaTimeToMeet() const;
    const CTaskPoint & getStartWeaponPoint() const;
    const CTaskPoint & getTargetPoint() const;
    void setDeltaTimeToMeet(double inDeltaTimeToMeet);
    void setStartWeaponPoint(const CTaskPoint& inStartWeaponPoint);
    void setTargetPoint(const CTaskPoint& inTargetPoint);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif



