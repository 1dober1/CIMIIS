// Файл 04_CSystemLog.h
// Протокол работы программы
//

#ifndef Kernel_Output_CSystemLog_h
#define Kernel_Output_CSystemLog_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "../01_Environment/01_StdLib.h"
#include "../01_Environment/02_DetectSystem.h"
#include "../01_Environment/03_CheckSystem.h"
#include "../01_Environment/04_DetectCompiler.h"
#include "../01_Environment/05_CheckCompiler.h"
#include "../01_Environment/06_SysLib.h"
#include "../02_BasicTypes/01_IntegerTypes.h"
#include "../05_System/04_CTextFile.h"
#include "01_CControlChar.h"
#include "02_CTextColor.h"


//=====================================================================================================================
// Протокол работы программы
class CSystemLog
{
public:
    //-----------------------------------------------------------------------------------------------------------------
    // Единственный на всю программу экземпляр протокола работы
    static CSystemLog myGlobalLog;


private:
    //-----------------------------------------------------------------------------------------------------------------
    CTextFile myFile;               // Файл протокола


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Запрет создания и копирования напрямую
    CSystemLog();
    CSystemLog(const CSystemLog &);
    CSystemLog & operator =(const CSystemLog &);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Деструктор класса
    ~CSystemLog();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Управление протоколом
    bool start(const string &fileName);
    void stop();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вывод стандартных строк и числовых значений в протокол
    CSystemLog & operator <<(const string &str);
    CSystemLog & operator <<(const char *str);
    CSystemLog & operator <<(double val);
    CSystemLog & operator <<(size_t val);
    CSystemLog & operator <<(uint32 val);
    CSystemLog & operator <<(uint16 val);
    CSystemLog & operator <<(uint8 val);
    CSystemLog & operator <<(int32 val);
    CSystemLog & operator <<(int16 val);
    CSystemLog & operator <<(int8 val);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вывод управляющих символов на консоль
    CSystemLog & operator <<(const CControlChar &cmd);

    // Управление цветом текста консоли (фиктивный метод)
    CSystemLog & operator <<(const CTextColor &);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
// Быстрый доступ к протоколу работы программы
static CSystemLog &Log = CSystemLog::myGlobalLog;


//=====================================================================================================================
#endif

