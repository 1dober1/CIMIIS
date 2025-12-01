// Файл 02_FloatingTypes.h
// Обработка действительных чисел
//

#ifndef Kernel_BasicTypes_FloatingTypes_h
#define Kernel_BasicTypes_FloatingTypes_h


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
// Определение набора констант для граничных значений
const double  double_min     = DBL_MIN;
const double  double_max     = DBL_MAX;
const double  double_epsilon = DBL_EPSILON;


//=====================================================================================================================
// Объявление функций проверки особых значений действительных чисел
bool checkNaN(const double value);
bool checkInf(const double value);


//=====================================================================================================================
// Объявление функций интервального сравнения действительных чисел
bool checkEQ(double left, double right);        // равно
bool checkNE(double left, double right);        // не равно
bool checkLT(double left, double right);        // строго меньше
bool checkGT(double left, double right);        // строго больше
bool checkLE(double left, double right);        // меньше или равно
bool checkNG(double left, double right);        // меньше или равно
bool checkGE(double left, double right);        // больше или равно
bool checkNL(double left, double right);        // больше или равно


//=====================================================================================================================
#endif



