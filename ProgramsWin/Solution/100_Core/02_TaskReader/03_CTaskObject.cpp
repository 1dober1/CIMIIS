// Файл 03_CTaskObject.cpp
// Данные о моделируемом объекте в сценарии боевых действий
//

//=====================================================================================================================
// Заголовочные файлы
#include "03_CTaskObject.h"




//=====================================================================================================================
// Единственный на всю программу технологический экземпляр пустой точки
CTaskPoint CTaskObject::myEmptyPoint;




//=====================================================================================================================
// Единственный на всю программу технологический экземпляр пустой команды
CTaskCmd CTaskObject::myEmptyCmd;




//=====================================================================================================================
// Конструктор класса
CTaskObject::CTaskObject(const string &typeName, const string &name, const vector<CTaskPoint> &pointList, const vector<CTaskCmd> &cmdList, bool isRed)
{
    mySceneNum = uint32_max;
    myParentIndex = SIZE_MAX;
    myTypeName = typeName;
    myIsRed = isRed;
    myName = name;

    myAbsNum = SIZE_MAX;

    myPointList = pointList;
    myCmdList = cmdList;
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CTaskObject::CTaskObject(const string &sceneName, uint32 sceneNum, const string &typeName, const string &name, const vector<CTaskPoint> &pointList, const vector<CTaskCmd> &cmdList, bool isRed)
{
    mySceneName = sceneName;
    mySceneNum = sceneNum;
    myParentIndex = SIZE_MAX;
    myTypeName = typeName;
    myIsRed = isRed;
    myName = name;

    myAbsNum = SIZE_MAX;

    myPointList = pointList;
    sort(myPointList.begin(), myPointList.end());

    myCmdList = cmdList;
    sort(myCmdList.begin(), myCmdList.end());
}




//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CTaskObject::CTaskObject(const string &sceneName, uint32 sceneNum, size_t parentIndex, const string &typeName, const string &name, const vector<CTaskPoint> &pointList, const vector<CTaskCmd> &cmdList, bool isRed)
{
    mySceneName = sceneName;
    mySceneNum = sceneNum;
    myParentIndex = parentIndex;
    myTypeName = typeName;
    myIsRed = isRed;
    myName = name;

    myAbsNum = SIZE_MAX;

    myPointList = pointList;
    sort(myPointList.begin(), myPointList.end());

    myCmdList = cmdList;
    sort(myCmdList.begin(), myCmdList.end());
}




//=====================================================================================================================
// Доступ информации об объекте
const string & CTaskObject::getSceneName() const
{
    return mySceneName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации об объекте
uint32 CTaskObject::getSceneNum() const
{
    return mySceneNum;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации об объекте
size_t CTaskObject::getParentIndex() const
{
    return myParentIndex;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации об объекте
const string & CTaskObject::getTypeName() const
{
    return myTypeName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации об объекте
const string & CTaskObject::getName() const
{
    return myName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ информации об объекте
bool CTaskObject::isRed() const
{
    return myIsRed;
}




//=====================================================================================================================
// Управление номером созданного модельного объекта
size_t CTaskObject::getAbsNum() const
{
    return myAbsNum;
}




//---------------------------------------------------------------------------------------------------------------------
// Управление номером созданного модельного объекта
void CTaskObject::saveAbsNum(size_t absNum) const
{
    myAbsNum = absNum;
}




//=====================================================================================================================
// Доступ к маршруту движения объекта
size_t CTaskObject::getPointCount() const
{
    return myPointList.size();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к маршруту движения объекта
const CTaskPoint & CTaskObject::getPoint(size_t index) const
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




//=====================================================================================================================
// Доступ к командам объекту
size_t CTaskObject::getCmdCount() const
{
    return myCmdList.size();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к командам объекту
const CTaskCmd & CTaskObject::getCmd(size_t index) const
{
    // Если индекс некорректный
    if (index >= myCmdList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return myEmptyCmd;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return myCmdList[index];
}




