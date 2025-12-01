// Файл Wide.h
// Преобразование кодировки символов между CP-1251 и WideChar
//

#ifndef Kernel_Encoding_Wide_h
#define Kernel_Encoding_Wide_h




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
wstring toWide(const string &str);
string fromWide(const wstring &str);




//=====================================================================================================================
#endif
