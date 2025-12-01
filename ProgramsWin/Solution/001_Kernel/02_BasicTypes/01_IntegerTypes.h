// Файл 01_IntegerTypes.h
// Обработка целых чисел
//

#ifndef Kernel_BasicTypes_IntegerTypes_h
#define Kernel_BasicTypes_IntegerTypes_h


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
// Определение 8-битного целого типа
#if SCHAR_MAX == 127
    typedef signed char     int8;
    typedef unsigned char   uint8;
#else
    #error "Stop compilation! There is no 8-bit integer type!"
#endif


//=====================================================================================================================
// Определение 16-битного целого типа
#if SHRT_MAX == 32767
    typedef signed short    int16;
    typedef unsigned short  uint16;
#elif INT_MAX == 32767
    typedef signed int      int16;
    typedef unsigned int    uint16;
#else
    #error "Stop compilation! There is no 16-bit integer type!"
#endif


//=====================================================================================================================
// Определение 32-битного целого типа
#if INT_MAX == 2147483647
    typedef signed int      int32;
    typedef unsigned int    uint32;
#elif LONG_MAX == 2147483647
    typedef signed long     int32;
    typedef unsigned long   uint32;
#else
    #error "ERROR! There is no 32-bit integer type!"
#endif


//=====================================================================================================================
// Определение набора констант для граничных значений
const int8   int8_min  = -127-1;
const int8   int8_max  =  127;
const int16  int16_min = -32767-1;
const int16  int16_max =  32767;
const int32  int32_min = -2147483647-1;
const int32  int32_max =  2147483647;

const uint8   uint8_max  = 0xFF;
const uint16  uint16_max = 0xFFFF;
const uint32  uint32_max = 0xFFFFFFFF;


//=====================================================================================================================
// Определения дополнительных макросов
#ifdef SIZE_MAX
#undef SIZE_MAX
#endif
#define SIZE_MAX    ((size_t)-1)

#ifdef INDEX_NOT_FOUND
#undef INDEX_NOT_FOUND
#endif
#define INDEX_NOT_FOUND     SIZE_MAX


//=====================================================================================================================
#endif



