// Файл 20_CTaskRouteFlight.cpp
// Данные о маршруте полёта
//

//=====================================================================================================================
// Заголовочные файлы
#include "20_CTaskRouteFlight.h"




//=====================================================================================================================
// Единственный на всю программу технологический экземпляр пустой точки
CTaskPoint CTaskRouteFlight::myEmptyPoint;




//=====================================================================================================================
// Конструктор класса
CTaskRouteFlight::CTaskRouteFlight(const string &name, const vector<CTaskPoint> &pointList, bool isRed)
{
    mySceneNum = uint32_max;
    myIsRed = isRed;
    myName = name;

    myPointList = pointList;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CTaskRouteFlight::CTaskRouteFlight(const string &sceneName, uint32 sceneNum, const string &name, const vector<CTaskPoint> &pointList, bool isRed)
{
    mySceneName = sceneName;
    mySceneNum = sceneNum;
    myIsRed = isRed;
    myName = name;

    myPointList = pointList;
    sort(myPointList.begin(), myPointList.end());
}




//=====================================================================================================================
// Доступ информации о маршруте
const string & CTaskRouteFlight::getSceneName() const
{
    return mySceneName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации о маршруте
uint32 CTaskRouteFlight::getSceneNum() const
{
    return mySceneNum;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации о маршруте
const string & CTaskRouteFlight::getName() const
{
    return myName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации о маршруте
bool CTaskRouteFlight::isRed() const
{
    return myIsRed;
}




//=====================================================================================================================
// Доступ к опорным точкам маршрута
size_t CTaskRouteFlight::getPointCount() const
{
    return myPointList.size();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к опорным точкам маршрута
const CTaskPoint & CTaskRouteFlight::getPoint(size_t index) const
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


