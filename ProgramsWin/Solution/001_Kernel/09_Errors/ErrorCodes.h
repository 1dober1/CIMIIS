// Файл ErrorCodes.h
// Описание ошибок времени выполнения
//

#ifndef Kernel_Errors_ErrorCodes_h
#define Kernel_Errors_ErrorCodes_h




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
// Коды ошибок
enum err_code
{
    ERR_OK = 0,                 // Ошибки нет
    ERR_DIVISION_BY_ZERO,       // Деление на нуль
    ERR_NULL_POINTER,           // Нулевой указатель
    ERR_BAD_INDEX,              // Некорректный индекс в массиве
    ERR_NO_DATA,                // Нет данных
    ERR_OUT_OF_RANGE,           // Вне допустимого диапазона
    ERR_BAD_TRIPPLE,            // Недопустимая тройка векторов
    ERR_BAD_LINE,               // Не удалось построить прямую
    ERR_BAD_ORTHO,              // Недопустимое начало координат
    ERR_BAD_POINT,              // Недопустимая точка
    ERR_ALREADY_EXIST           // Добавляемый элемент уже существует
};




//=====================================================================================================================
// Глобальная переменная для передачи информации о возникшей ошибке
extern err_code ErrorCode;




//=====================================================================================================================
#endif

