// Файл 05_Result.h
// Результат обработки данных
//

#ifndef Kernel_BasicTypes_Result_h
#define Kernel_BasicTypes_Result_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "../01_Environment/01_StdLib.h"
#include "../01_Environment/02_DetectSystem.h"
#include "../01_Environment/03_CheckSystem.h"
#include "../01_Environment/04_DetectCompiler.h"
#include "../01_Environment/05_CheckCompiler.h"
#include "../01_Environment/06_SysLib.h"


//=====================================================================================================================
// Результат обработки данных
enum result
{
    RESULT_OK = 0,      // Обработка успешно завершена
    RESULT_WARNING,     // В ходе обработки были предупреждения
    RESULT_ERROR        // Обработка завершена с ошибкой
};


//=====================================================================================================================
#endif

