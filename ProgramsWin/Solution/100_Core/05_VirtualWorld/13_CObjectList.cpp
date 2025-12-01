// Файл 13_CObjectList.cpp
// Массив моделируемых объектов
//

//=====================================================================================================================
// Заголовочные файлы
#include "11_CObjectType.h"
#include "13_CObjectList.h"




//=====================================================================================================================
// Единый на всю программу глобальный массив моделируемых объектов
CObjectList CObjectList::myGlobalObjectList;




//=====================================================================================================================
// Конструктор класса
CObjectList::CObjectList()
{
    // Добавление в массив фиктивного объекта, чтобы нумерация была с 1
    myObjectList.push_back(new CObject(ObjectTypeNull, 0, true));
}




//=====================================================================================================================
// Деструктор класса
CObjectList::~CObjectList()
{
    for (size_t i = 0; i < myObjectList.size(); ++i)
    {
        CObject *object = myObjectList[i];
        if (object == NULL)
            continue;
        delete object;
    }

    myObjectList.clear();
}




//=====================================================================================================================
// Создание нового моделируемого объекта
size_t CObjectList::createObject(const CObjectType &objectType, bool isRed)
{
    myObjectList.push_back(new CObject(objectType, myObjectList.size(), isRed));
    return myObjectList.size()-1;
}




//---------------------------------------------------------------------------------------------------------------------
// Создание нового моделируемого объекта
size_t CObjectList::createObject(const CObjectType &objectType, size_t parentNum, bool isRed)
{
    myObjectList.push_back(new CObject(objectType, myObjectList.size(), parentNum, isRed));
    return myObjectList.size()-1;
}




//=====================================================================================================================
// Доступ к моделируемым объектам
const CObject & CObjectList::getObject(size_t absNum) const
{
    // Если запрашивается фиктивный объект
    if (absNum == 0)
    {
        ErrorCode = ERR_BAD_INDEX;
        return ObjectNull;
    }

    // Если некорректный номер объекта
    if (absNum >= myObjectList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return ObjectNull;
    }

    // Если нулевой указатель
    if (myObjectList[absNum] == NULL)
    {
        ErrorCode = ERR_NULL_POINTER;
        return ObjectNull;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return *myObjectList[absNum];
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к моделируемым объектам
CObject & CObjectList::getObject(size_t absNum)
{
    // Если запрашивается фиктивный объект
    if (absNum == 0)
    {
        ErrorCode = ERR_BAD_INDEX;
        return ObjectNull;
    }

    // Если некорректный номер объекта
    if (absNum >= myObjectList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return ObjectNull;
    }

    // Если нулевой указатель
    if (myObjectList[absNum] == NULL)
    {
        ErrorCode = ERR_NULL_POINTER;
        return ObjectNull;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return *myObjectList[absNum];
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к моделируемым объектам
size_t CObjectList::getObjectCount() const
{
    return myObjectList.size() - 1;
}




//=====================================================================================================================
// Рассылка воздействия существующим объектам
void CObjectList::processImpact(size_t senderAbsNum, double time, const CImpact &impact)
{
    for (size_t i = 1; i < myObjectList.size(); ++i)
    {
        // Пропуск несуществующих объектов
        CObject *object = myObjectList[i];
        if(object == NULL)
            continue;
        if (object->isExist() == false)
            continue;

        // Пропуск самого отправителя
        if (object->getAbsNum() == senderAbsNum)
            continue;

        // Передать воздействие
        object->processImpact(time, impact);
    }
}



