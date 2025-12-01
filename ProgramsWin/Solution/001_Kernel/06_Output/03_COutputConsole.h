// Файл 03_COutputConsole.h
// Консоль вывода информации на экран
//

#ifndef Kernel_Output_COutputConsole_h
#define Kernel_Output_COutputConsole_h


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
#include "01_CControlChar.h"
#include "02_CTextColor.h"


//=====================================================================================================================
// Консоль вывода информации на экран
class COutputConsole
{
public:
    //-----------------------------------------------------------------------------------------------------------------
    // Единственный на всю программу экземпляр консоли вывода
    static COutputConsole myGlobalConsole;


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Запрет создания напрямую и копирования консоли
    COutputConsole();
    COutputConsole(const COutputConsole &);
    COutputConsole & operator =(const COutputConsole &);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вывод стандартных строк и числовых значений на консоль
    COutputConsole & operator <<(const string &str);
    COutputConsole & operator <<(const char *str);
    COutputConsole & operator <<(double val);
    COutputConsole & operator <<(size_t val);
    COutputConsole & operator <<(uint32 val);
    COutputConsole & operator <<(uint16 val);
    COutputConsole & operator <<(uint8 val);
    COutputConsole & operator <<(int32 val);
    COutputConsole & operator <<(int16 val);
    COutputConsole & operator <<(int8 val);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Вывод управляющих символов на консоль
    COutputConsole & operator <<(const CControlChar &cmd);

    // Управление цветом текста консоли
    COutputConsole & operator <<(const CTextColor &color);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
// Быстрый доступ к консоли вывода
static COutputConsole &Console = COutputConsole::myGlobalConsole;


//=====================================================================================================================
#endif

