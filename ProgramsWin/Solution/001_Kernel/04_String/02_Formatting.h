// Файл 02_Formatting.h
// Форматный вывод в строку
//

#ifndef Kernel_String_Formatting_h
#define Kernel_String_Formatting_h


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
// Варианты выравнивания строки
enum stringAlignType
{
    alignLeft = 0,
    alignRight
};


//=====================================================================================================================
// Варианты заполнения строки
enum stringFillType
{
    fillSpace = 1,
    fillZero
};


//=====================================================================================================================
// Форматирование строкового значения
string format(const string &value, int32 width = 0, stringAlignType alignType = alignLeft);

// Форматирование целочисленного значения
string format(int32  value, int32 width = 0, stringFillType fillType = fillSpace);
string format(uint32 value, int32 width = 0, stringFillType fillType = fillSpace);

// Форматирование действительного значения
string format(double value, int32 width = 0, int32 precision = 0);


//=====================================================================================================================
#endif
