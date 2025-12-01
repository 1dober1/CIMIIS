// Файл 05_MakeSleep.cpp
// Приостановка текущего потока
//

//=====================================================================================================================
// Заголовочные файлы
#include "05_MakeSleep.h"




//=====================================================================================================================
// Приостановка текущего потока
void MakeSleep(uint32 milliseconds)
{
#ifdef SYSTEM_WINDOWS
    Sleep((DWORD)milliseconds);
#else
    timespec ts;
    ts.tv_sec = (time_t)(milliseconds / 1000);
    ts.tv_nsec = (long)((milliseconds % 1000) * 1000000);
    nanosleep(&ts, NULL);
#endif
}

