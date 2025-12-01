// Файл 05_MakeSleep.h
// Приостановка текущего потока
//

#ifndef Kernel_System_MakeSleep_h
#define Kernel_System_MakeSleep_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "../01_Environment/01_StdLib.h"
#include "../01_Environment/02_DetectSystem.h"
#include "../01_Environment/03_CheckSystem.h"
#include "../01_Environment/04_DetectCompiler.h"
#include "../01_Environment/05_CheckCompiler.h"
#include "../01_Environment/06_SysLib.h"
#include "../02_BasicTypes/01_IntegerTypes.h"


//=====================================================================================================================
// Объявление функций приостановки текущего потока
void MakeSleep(uint32 milliseconds);


//=====================================================================================================================
#endif
