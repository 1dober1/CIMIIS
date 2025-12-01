// Файл 03_AngleTransform.h
// Преобразование углов
//

#ifndef Kernel_BasicTypes_AngleTransform_h
#define Kernel_BasicTypes_AngleTransform_h


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
// Определение набора констант для работы с углами
const double Pi = 3.14159265358979323846;
const double Pi2 = 1.57079632679489661923;


//=====================================================================================================================
// Объявление функций преобразования углов
double deg2rad(double degrees);
double rad2deg(double radians);


//=====================================================================================================================
// Объявление функций работы с минутами и секундами
double uniteDegrees(double degrees, double minutes, double seconds);
void expandDegrees(double source, double &degrees, double &minutes, double &seconds);


//=====================================================================================================================
// Объявление функций нормирования углов, выраженных в градусах
double normalizeTo180(double degrees);
double normalizeTo360(double degrees);

// Объявление функций нормирования углов, выраженных в радианах
double normalizeToPi(double radians);
double normalizeTo2Pi(double radians);


//=====================================================================================================================
#endif



