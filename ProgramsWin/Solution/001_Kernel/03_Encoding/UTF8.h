// Файл UTF8.h
// Преобразование кодировки символов между CP-1251 и UTF-8
//

#ifndef Kernel_Encoding_UTF8_h
#define Kernel_Encoding_UTF8_h




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
// Объявление функций преобразования 
string toUTF8(const string &str);
string fromUTF8(const string &str);




//=====================================================================================================================
#endif
