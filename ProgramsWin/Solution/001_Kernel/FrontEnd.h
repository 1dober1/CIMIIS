// Файл FrontEnd.h
// Точка подключения всех возможностей блока 001_Kernel
//

#ifndef Kernel_FrontEnd_h
#define Kernel_FrontEnd_h




//=====================================================================================================================
// Программное окружение
#include "01_Environment/01_StdLib.h"
#include "01_Environment/02_DetectSystem.h"
#include "01_Environment/03_CheckSystem.h"
#include "01_Environment/04_DetectCompiler.h"
#include "01_Environment/05_CheckCompiler.h"
#include "01_Environment/06_SysLib.h"




//=====================================================================================================================
// Базовые типы данных
#include "02_BasicTypes/01_IntegerTypes.h"
#include "02_BasicTypes/02_FloatingTypes.h"
#include "02_BasicTypes/03_AngleTransform.h"
#include "02_BasicTypes/04_CByteArray.h"
#include "02_BasicTypes/05_Result.h"




//=====================================================================================================================
// Кодовые таблицы символов
#include "03_Encoding/CP866.h"
#include "03_Encoding/UTF8.h"
#include "03_Encoding/Wide.h"




//=====================================================================================================================
// Дополнительные операции со строками
#include "04_String/01_FileName.h"
#include "04_String/02_Formatting.h"
#include "04_String/03_Case.h"




//=====================================================================================================================
// Системные функции
#include "05_System/01_CDateTime.h"
#include "05_System/02_CFileInfo.h"
#include "05_System/03_CDataFile.h"
#include "05_System/04_CTextFile.h"
#include "05_System/05_MakeSleep.h"
#include "05_System/06_CClock.h"




//=====================================================================================================================
// Средства вывода информации
#include "06_Output/01_CControlChar.h"
#include "06_Output/02_CTextColor.h"
#include "06_Output/03_COutputConsole.h"
#include "06_Output/04_CSystemLog.h"




//=====================================================================================================================
// Средства синтаксического анализа
#include "07_Parser/01_CDataMap.h"
#include "07_Parser/01_CStringTable.h"
#include "07_Parser/02_CTextLexeme.h"
#include "07_Parser/03_CTextScanner.h"
#include "07_Parser/04_CTextParser.h"




//=====================================================================================================================
// Настройки программы
#include "08_Config/CSettings.h"




//=====================================================================================================================
// Коды ошибок в программе
#include "09_Errors/ErrorCodes.h"




//=====================================================================================================================
// Подключение файла статической библиотеки при компиляции
#if !defined(USED_CMAKE_BUILD_TOOL)   &&   defined(COMPILER_MICROSOFT)
__pragma(comment(lib, "001_Kernel"))
#endif




//=====================================================================================================================
#endif
