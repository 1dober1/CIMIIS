// Файл 41_CTaskMsgQueue.cpp
// Очередь сообщений из задачи моделирования
//

//=====================================================================================================================
// Заголовочные файлы
#include "41_CTaskMsgQueue.h"




//=====================================================================================================================
// Единственное на всю программу пустое сообщение
CTaskMsg CTaskMsgQueue::myEmptyMsg(DBL_MAX, CTaskMsg::MSG_OBJECT_EMPTY, SIZE_MAX, SIZE_MAX);




//=====================================================================================================================
// Конструктор класса
CTaskMsgQueue::CTaskMsgQueue()
{
    myCurMsgIndex = 0;
}




//=====================================================================================================================
// Доступ к состоянию очереди сообщений
const CTaskMsg & CTaskMsgQueue::peekMsg() const
{
    // Если очередь пуста
    if (myCurMsgIndex >= myMsgQueue.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return myEmptyMsg;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return myMsgQueue[myCurMsgIndex];
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к состоянию очереди сообщений
const CTaskMsg & CTaskMsgQueue::popMsg()
{
    // Если очередь пуста
    if (myCurMsgIndex >= myMsgQueue.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return myEmptyMsg;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return myMsgQueue[myCurMsgIndex++];
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к состоянию очереди сообщений
bool CTaskMsgQueue::isEmpty() const
{
    if (myCurMsgIndex >= myMsgQueue.size())
        return true;
    return false;
}




//=====================================================================================================================
// Заполнение очереди сообщений
void CTaskMsgQueue::pushMsg(const CTaskMsg &msg)
{
    myMsgQueue.push_back(msg);
}




//=====================================================================================================================
// Упорядочивание очереди сообщений
void CTaskMsgQueue::sort()
{
    std::sort(myMsgQueue.begin(), myMsgQueue.end());
    myCurMsgIndex = 0;
}

