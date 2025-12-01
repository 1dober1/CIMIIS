// Файл 01_FileName.h
// Обработка имён файлов
//

#ifndef Kernel_String_FileName_h
#define Kernel_String_FileName_h


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
// Извлечение имени непосредственно файла из полного имени файла
string extractFileName(const string &fullFileName);

// Извлечение пути к файлу из полного имени файла
string extractFilePath(const string &fullFileName);

// Извлечение чистого имени файла (без расширения) из полного имени файла
string extractPureFileName(const string &fullFileName);

// Извлечение расширения имени файла из полного имени файла
string extractFileNameExtension(const string &fullFileName);

// Нормализация разделителей имён каталогов в пути
string normalizePathSeparators(const string &fullFileName);


//=====================================================================================================================
#endif
