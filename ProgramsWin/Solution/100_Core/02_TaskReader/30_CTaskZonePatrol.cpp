// Файл 30_CTaskZonePatrol.cpp
// Данные о зоне барражирования
//

//=====================================================================================================================
// Заголовочные файлы
#include "30_CTaskZonePatrol.h"




//=====================================================================================================================
// Единственный на всю программу технологический экземпляр пустой точки
CTaskPoint CTaskZonePatrol::myEmptyPoint;




//=====================================================================================================================
// Конструктор класса
CTaskZonePatrol::CTaskZonePatrol(const string &name, const vector<CTaskPoint> &pointList, bool isRed)
{
    mySceneNum = uint32_max;
    myIsRed = isRed;
    myName = name;

    myPointList = pointList;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CTaskZonePatrol::CTaskZonePatrol(const string &sceneName, uint32 sceneNum, const string &name, const vector<CTaskPoint> &pointList, bool isRed)
{
    mySceneName = sceneName;
    mySceneNum = sceneNum;
    myIsRed = isRed;
    myName = name;

    myPointList = pointList;
    sort(myPointList.begin(), myPointList.end());
}




//=====================================================================================================================
// Доступ информации о зоне
const string & CTaskZonePatrol::getSceneName() const
{
    return mySceneName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации о зоне
uint32 CTaskZonePatrol::getSceneNum() const
{
    return mySceneNum;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации о зоне
const string & CTaskZonePatrol::getName() const
{
    return myName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации о зоне
bool CTaskZonePatrol::isRed() const
{
    return myIsRed;
}




//=====================================================================================================================
// Доступ к опорным точкам зоны
size_t CTaskZonePatrol::getPointCount() const
{
    return myPointList.size();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к опорным точкам зоны
const CTaskPoint & CTaskZonePatrol::getPoint(size_t index) const
{
    // Если индекс некорректный
    if (index >= myPointList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return myEmptyPoint;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return myPointList[index];
}


