#ifndef _226_NavedenieZPS_FrontEnd_h
#define _226_NavedenieZPS_FrontEnd_h

// Сюда надо подключать заголовочные файлы данного проекта
#include "Settings.h"

// Описание новой системы и её типа
#include "90_System/01_CSysMotionNewType.h"
#include "90_System/02_CSysMotionNew.h"

// Подключение файла статической библиотеки при компиляции
#if !defined(USED_CMAKE_BUILD_TOOL)   &&   defined(COMPILER_MICROSOFT)
__pragma(comment(lib, "226_NavedenieZPS"))
#endif

#endif
