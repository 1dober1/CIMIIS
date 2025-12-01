// Файл 02_DetectSystem.h
// Определение используемой операционной системы
//

#ifndef Kernel_Environment_DetectSystem_h
#define Kernel_Environment_DetectSystem_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_StdLib.h"


//=====================================================================================================================
// Определение типа операционной системы
#if defined(WIN32)   ||   defined(_WIN32)   ||   defined(__WIN32__)   ||   defined(__NT__)
    #define SYSTEM_WINDOWS


//---------------------------------------------------------------------------------------------------------------------
#elif defined(__linux__)   ||   defined(__linux)   ||   defined(linux)
    #define SYSTEM_LINUX
    #define SYSTEM_UNIX


//---------------------------------------------------------------------------------------------------------------------
#elif defined(__FreeBSD__)   ||   defined(__DragonFly__)
    #define SYSTEM_FREEBSD
    #define SYSTEM_UNIX
    #define SYSTEM_BSD


//---------------------------------------------------------------------------------------------------------------------
#elif defined(__OpenBSD__)
    #define SYSTEM_OPENBSD
    #define SYSTEM_UNIX
    #define SYSTEM_BSD


//---------------------------------------------------------------------------------------------------------------------
#elif defined(__NetBSD__)
    #define SYSTEM_NETBSD
    #define SYSTEM_UNIX
    #define SYSTEM_BSD


//---------------------------------------------------------------------------------------------------------------------
#else
    #define SYSTEM_UNKNOWN
#endif




//=====================================================================================================================
// Определение разрядности операционной системы
#if defined(SYSTEM_WINDOWS)
    #if defined(WIN64)   ||   defined(_WIN64)
        #define SYSTEM_64BIT
    #else
        #define SYSTEM_32BIT
    #endif


//---------------------------------------------------------------------------------------------------------------------
#elif defined(SYSTEM_UNIX)
    // Если архитектура Intel x86-64 (она же AMD64)
    #if defined(__x86_64)   ||   defined(__x86_64__)   ||   defined(__amd64)   ||   defined(__amd64__)
        #define SYSTEM_64BIT

//    // Если архитектура MIPS
//    #elif defined(__mips__)   ||   defined(__mips)   ||   defined(__MIPS__)
//        #if defined(_MIPS_ISA_MIPS3)   ||   defined(__MIPS_ISA3__)
//            #define SYSTEM_64BIT
//        #elif defined(_MIPS_ISA_MIPS4)   ||   defined(__MIPS_ISA4__)
//            #define SYSTEM_64BIT
//        #else
//            #define SYSTEM_32BIT
//        #endif

//    // Если архитектура ARM64
//    #elif defined(__aarch64__)   ||   defined(_M_ARM64)
//        #define SYSTEM_64BIT
// 
    // Все остальные архитектуры предполагаются 32-битными
    #else
        #define SYSTEM_32BIT
    #endif
#endif


//=====================================================================================================================
#endif


