// Файл 01_CDateTime.cpp
// Доступ к системному времени
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CDateTime.h"




//=====================================================================================================================
// Конструктор класса
CDateTime::CDateTime()
{
    memset(&myTimeInfo, 0, sizeof(tm));
}




//=====================================================================================================================
// Доступ к текущей дате
uint16 CDateTime::getYear() const
{
    return (uint16)myTimeInfo.tm_year + 1900;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к текущей дате
uint16 CDateTime::getMonth() const
{
    return (uint16)myTimeInfo.tm_mon + 1;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к текущей дате
uint16 CDateTime::getDay() const
{
    return (uint16)myTimeInfo.tm_mday;
}




//=====================================================================================================================
// Доступ к текущему времени
uint16 CDateTime::getHour() const
{
    return (uint16)myTimeInfo.tm_hour;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к текущему времени
uint16 CDateTime::getMinute() const
{
    return (uint16)myTimeInfo.tm_min;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к текущему времени
uint16 CDateTime::getSecond() const
{
    return (uint16)myTimeInfo.tm_sec;
}




//=====================================================================================================================
// Формирование строки с датой
const string & CDateTime::getDateString() const
{
    if (myDateString.empty() == true)
    {
        char buffer[128];
        sprintf(buffer, "%02u.%02u.%04u", (uint32)getDay(), (uint32)getMonth(), (uint32)getYear());
        myDateString = buffer;
    }
    return myDateString;
}




//=====================================================================================================================
// Формирование строки со временем
const string & CDateTime::getTimeString() const
{
    if (myTimeString.empty() == true)
    {
        char buffer[128];
        sprintf(buffer, "%02u:%02u:%02u", (uint32)getHour(), (uint32)getMinute(), (uint32)getSecond());
        myTimeString = buffer;
    }
    return myTimeString;
}




//=====================================================================================================================
// Получение значений от операционной системы
CDateTime CDateTime::querySystemTime()
{
    time_t t;
    time(&t);
    tm *timeInfo = gmtime(&t);

    CDateTime dateTime;
    if (timeInfo != NULL)
        memcpy(&dateTime.myTimeInfo, timeInfo, sizeof(tm));
    return dateTime;
}




//---------------------------------------------------------------------------------------------------------------------
// Получение значений от операционной системы
CDateTime CDateTime::queryLocalTime()
{
    time_t t;
    time(&t);
    tm *timeInfo = localtime(&t);

    CDateTime dateTime;
    if (timeInfo != NULL)
        memcpy(&dateTime.myTimeInfo, timeInfo, sizeof(tm));
    return dateTime;
}

