// Файл 02_CRequestTimeToMeetWeapon.cpp
// Запрос боекомплекта
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_CRequestTimeToMeetWeapon.h"




//=====================================================================================================================
// Конструктор класса
CRequestTimeToMeetWeapon::CRequestTimeToMeetWeapon()
{

}




//=====================================================================================================================
// Деструктор класса
CRequestTimeToMeetWeapon::~CRequestTimeToMeetWeapon()
{
}




//=====================================================================================================================
// Управление результатами запроса
double CRequestTimeToMeetWeapon::getDeltaTimeToMeet() const
{
	return myDeltaTimeToMeet;
}




//---------------------------------------------------------------------------------------------------------------------
// Управление результатами запроса
const CTaskPoint & CRequestTimeToMeetWeapon::getStartWeaponPoint() const
{
	return myStartWeaponPoint;
}




//---------------------------------------------------------------------------------------------------------------------
// Управление результатами запроса
const CTaskPoint & CRequestTimeToMeetWeapon::getTargetPoint() const
{
	return myTargetPoint;
}




//---------------------------------------------------------------------------------------------------------------------
// Управление результатами запроса
void CRequestTimeToMeetWeapon::setDeltaTimeToMeet(double inDeltaTimeToMeet)
{
	myDeltaTimeToMeet = inDeltaTimeToMeet;
}




//---------------------------------------------------------------------------------------------------------------------
// Управление результатами запроса
void CRequestTimeToMeetWeapon::setStartWeaponPoint(const CTaskPoint& inStartWeaponPoint)
{
	myStartWeaponPoint = inStartWeaponPoint;
}




//---------------------------------------------------------------------------------------------------------------------
// Управление результатами запроса
void CRequestTimeToMeetWeapon::setTargetPoint(const CTaskPoint& inTargetPoint)
{
	myTargetPoint = inTargetPoint;
}





