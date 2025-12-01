// Файл 03_COutputConsole.cpp
// Консоль вывода информации на экран
//

//=====================================================================================================================
// Заголовочные файлы
#include "03_COutputConsole.h"
#include "../03_Encoding/CP866.h"
#include "../03_Encoding/UTF8.h"




//=====================================================================================================================
// Единственный на всю программу экземпляр консоли вывода
COutputConsole COutputConsole::myGlobalConsole;




//=====================================================================================================================
// Конструктор класса
COutputConsole::COutputConsole()
{
}




//=====================================================================================================================
// Вывод стандартных строк на консоль
COutputConsole & COutputConsole::operator <<(const string &str)
{
#ifdef SYSTEM_WINDOWS
    cout << to866(str);
#else
    cout << toUTF8(str);
#endif
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод стандартных строк на консоль
COutputConsole & COutputConsole::operator <<(const char *str)
{
#ifdef SYSTEM_WINDOWS
    cout << to866(string(str));
#else
    cout << toUTF8(string(str));
#endif
    return *this;
}




//=====================================================================================================================
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(double val)
{
    cout << val;
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(size_t val)
{
    cout << val;
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(uint32 val)
{
    cout << val;
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(uint16 val)
{
    cout << (uint32)val;
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(uint8 val)
{
    cout << (uint32)val;
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(int32 val)
{
    cout << val;
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(int16 val)
{
    cout << (int32)val;
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений на консоль
COutputConsole & COutputConsole::operator <<(int8 val)
{
    cout << (int32)val;
    return *this;
}




//=====================================================================================================================
// Вывод управляющих команд на консоль
COutputConsole & COutputConsole::operator <<(const CControlChar &cmd)
{
    if (&cmd == &Tab)
        cout << "\t";
    else if (&cmd == &Space)
        cout << " ";
#ifdef SYSTEM_WINDOWS
    else if (&cmd == &Line)
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        if (console != INVALID_HANDLE_VALUE   &&   console != NULL)
            SetConsoleTextAttribute(console, Default.myColorCode);
        cout << endl;
    }
#else
    else if (&cmd == &Line)
        cout << Default.myColorCode << endl;
#endif
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод управляющих команд на консоль
COutputConsole & COutputConsole::operator <<(const CTextColor &color)
{
#ifdef SYSTEM_WINDOWS
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE   &&   console != NULL)
        SetConsoleTextAttribute(console, color.myColorCode);
#else
    cout << color.myColorCode;
#endif
    return *this;
}




