// Файл 40_CTaskMsg.cpp
// Сообщение из задачи моделирования
//

//=====================================================================================================================
// Заголовочные файлы
#include "40_CTaskMsg.h"




//=====================================================================================================================
// Конструктор класса
CTaskMsg::CTaskMsg(double time, CTaskMsg::msgType type, size_t objectIndex, size_t pointCmdIndex)
{
    myTime = time;
    myType = type;
    myObjectIndex = objectIndex;
    myPointCmdIndex = pointCmdIndex;
}




//=====================================================================================================================
// Доступ к параметрам сообщения
double CTaskMsg::getTime() const
{
    return myTime;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам сообщения
CTaskMsg::msgType CTaskMsg::getType() const
{
    return myType;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам сообщения
size_t CTaskMsg::getObjectIndex() const
{
    return myObjectIndex;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам сообщения
size_t CTaskMsg::getPointCmdIndex() const
{
    return myPointCmdIndex;
}




//=====================================================================================================================
// Оператор ранжирования двух сообщений во времени
bool CTaskMsg::operator <(const CTaskMsg &other) const
{
    // Упорядочиваем события по возрастанию модельного времени
    if (checkLT(myTime, other.myTime))
        return true;

    // Для событий с одинаковым временем
    else if (checkEQ(myTime, other.myTime) == true)
    {
        // Упорядочиваем события по старшинству типов событий
        if (myType < other.myType)
            return true;

        // Для событий с одинаковыми типами
        else if (myType == other.myType)
        {
            // Упорядочиваем в порядке упоминания объектов в сценарии
            if (myObjectIndex < other.myObjectIndex)
                return true;
            else
                return false;
        }

        else
            return false;
    }

    else
        return false;
}



