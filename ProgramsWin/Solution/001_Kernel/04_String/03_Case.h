// Файл 03_Case.h
// Преобразование регистра символов в кодировке CP-1251
//

#ifndef Kernel_String_Case_h
#define Kernel_String_Case_h


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
// Преобразование строки к нижнему регистру
string toLower(const string &str);

// Преобразование строки к верхнему регистру
string toUpper(const string &str);


//=====================================================================================================================
#endif
