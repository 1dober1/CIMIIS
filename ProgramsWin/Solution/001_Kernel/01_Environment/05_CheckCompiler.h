// Файл 05_CheckCompiler.h
// Проверка допустимости используемого компилятора
//

#ifndef Kernel_Environment_CheckCompiler_h
#define Kernel_Environment_CheckCompiler_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_StdLib.h"
#include "04_DetectCompiler.h"


//=====================================================================================================================
// Остановка компиляции, если компилятор не поддерживается
#if defined(COMPILER_BORLAND)
    #error "Stop compilation! Unsupported compiler! [Borland C++ or C++ Builder]"


//---------------------------------------------------------------------------------------------------------------------
#elif defined(COMPILER_WATCOM)
    #error "Stop compilation! Unsupported compiler! [Watcom C++]"


//---------------------------------------------------------------------------------------------------------------------
#elif defined(COMPILER_DIGITAL)
    #error "Stop compilation! Unsupported compiler! [Digital Mars C++]"


//---------------------------------------------------------------------------------------------------------------------
#elif defined(COMPILER_UNKNOWN)
    #error "Stop compilation! Unsupported compiler [Unknown compiler]!"
#endif


//=====================================================================================================================
// Контроль версии компилятора Microsoft
#ifdef COMPILER_MICROSOFT
    #if _MSC_VER < 1400
        #error "Stop compilation! Too old compiler! [Required Microsoft Visual C++ 2005 or newer]"
    #endif
#endif


//---------------------------------------------------------------------------------------------------------------------
// Контроль версии компилятора Intel
#ifdef COMPILER_INTEL
    #if __INTEL_COMPILER < 900
        #error "Stop compilation! Too old compiler! [Required Intel C++ 9.0 or newer]"
    #endif
#endif


//---------------------------------------------------------------------------------------------------------------------
// Контроль версии компилятора clang
#ifdef COMPILER_CLANG
    // Вычислить версию компилятора во временной макроконстанте
    #ifdef __clang_major__
        #ifdef __clang_minor__
            #define MIA_TEMP_COMPILER_CLANG_VERSION     (__clang_major__*100 + __clang_minor__)
        #else
            #define MIA_TEMP_COMPILER_CLANG_VERSION     (__clang_major__*100)
        #endif

        // Неподдерживаемая версия (ниже 2.1)
        #if MIA_TEMP_COMPILER_CLANG_VERSION < 201
            #undef MIA_TEMP_COMPILER_CLANG_VERSION
            #error "Stop compilation! Too old compiler! [Required clang++ 2.1 or newer]"
        #endif

        // Поддерживаемая версия, просто убрать временную макроконстанту
        #ifdef MIA_TEMP_COMPILER_CLANG_VERSION
            #undef MIA_TEMP_COMPILER_CLANG_VERSION
        #endif

    // Если отсутствуют основные макрокостанты для вычисления версии компилятора
    #else
        #error "Stop compilation! Unsupported compiler! [Unknown version of clang]"
    #endif
#endif


//---------------------------------------------------------------------------------------------------------------------
// Контроль версии компилятора GNU
#ifdef COMPILER_GNU
    // Вычислить версию компилятора во временной макроконстанте
    #ifdef __GNUC_MINOR__
        #ifdef __GNUC_PATCHLEVEL__
            #define MIA_TEMP_COMPILER_GNU_VERSION     (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
        #else
            #define MIA_TEMP_COMPILER_GNU_VERSION     (__GNUC__*10000 + __GNUC_MINOR__*100)
        #endif
    #else
        #ifdef __GNUC_PATCHLEVEL__
            #define MIA_TEMP_COMPILER_GNU_VERSION     (__GNUC__*10000 + __GNUC_PATCHLEVEL__)
        #else
            #define MIA_TEMP_COMPILER_GNU_VERSION     (__GNUC__*10000)
        #endif
    #endif

    // Неподдерживаемая версия (ниже 3.3.4)
    #if MIA_TEMP_COMPILER_GNU_VERSION < 30304
        #undef MIA_TEMP_COMPILER_GNU_VERSION
        #error "Stop compilation! Too old compiler! [Required GCC 3.3.4 or newer]"
    #endif

    // Поддерживаемая версия, просто убрать временную макроконстанту
    #ifdef MIA_TEMP_COMPILER_GNU_VERSION
        #undef MIA_TEMP_COMPILER_GNU_VERSION
    #endif
#endif


//=====================================================================================================================
#endif
