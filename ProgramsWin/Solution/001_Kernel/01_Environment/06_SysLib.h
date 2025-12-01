// Файл 06_SysLib.h
// Средства операционной системы
//

#ifndef Kernel_Environment_SysLib_h
#define Kernel_Environment_SysLib_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_StdLib.h"
#include "02_DetectSystem.h"
#include "03_CheckSystem.h"
#include "04_DetectCompiler.h"
#include "05_CheckCompiler.h"


//=====================================================================================================================
// Подключение средств операционной системы Windows
#ifdef SYSTEM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <process.h>
#include <direct.h>
#endif


//=====================================================================================================================
// Подключение средств операционной системы на базе Linux
#ifdef SYSTEM_LINUX
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif


//=====================================================================================================================
#endif



