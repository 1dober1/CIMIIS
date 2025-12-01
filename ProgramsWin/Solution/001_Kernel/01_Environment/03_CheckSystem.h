// Файл 03_CheckSystem.h
// Проверка допустимости используемой операционной системы
//
#ifndef Kernel_Environment_CheckSystem_h
#define Kernel_Environment_CheckSystem_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_StdLib.h"
#include "02_DetectSystem.h"


//=====================================================================================================================
// Остановка компиляции, если операционная система не поддерживается
#if defined(SYSTEM_FREEBSD)
#error "Stop compilation! Unsupported operating system! [FreeBSD]"


//---------------------------------------------------------------------------------------------------------------------
#elif defined(SYSTEM_OPENBSD)
    #error "Stop compilation! Unsupported operating system! [OpenBSD]"


//---------------------------------------------------------------------------------------------------------------------
#elif defined(SYSTEM_NETBSD)
    #error "Stop compilation! Unsupported operating system! [NetBSD]"


//---------------------------------------------------------------------------------------------------------------------
#elif defined(SYSTEM_UNKNOWN)
    #error "Stop compilation! Unsupported operating system! [Unknown OS]"
#endif


//=====================================================================================================================
// Остановка компиляции, если разрядность операционной системы не корректная
#ifndef SYSTEM_64BIT
    #error "Stop compilation! Unsupported operating system! [Required 64-bit OS]"
#endif


//=====================================================================================================================
#endif



