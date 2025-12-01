// Файл 02_CTextColor.h
// Цвет текста в консоли
//

#ifndef Kernel_Output_CTextColor_h
#define Kernel_Output_CTextColor_h


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
// Цвет текста в консоли
class CTextColor
{
#ifdef SYSTEM_WINDOWS
private:
    //-----------------------------------------------------------------------------------------------------------------
    WORD myColorCode;
    CTextColor(WORD colorCode);
#else
private:
    //-----------------------------------------------------------------------------------------------------------------
    string myColorCode;
    CTextColor(const string &colorCode);
#endif


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Запрет копирования цветов
    CTextColor(const CTextColor &);
    CTextColor & operator =(const CTextColor &);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Перечень стандартных цветов текста
    static CTextColor myWhite;
    static CTextColor myGrey;
    static CTextColor myBlue;
    static CTextColor myGreen;
    static CTextColor myCyan;
    static CTextColor myRed;
    static CTextColor myMagenta;
    static CTextColor myYellow;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Дружественные классы
    friend class COutputConsole;


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
// Краткие обозначения для цветов текста
static const CTextColor &White   = CTextColor::myWhite;
static const CTextColor &Grey    = CTextColor::myGrey;
static const CTextColor &Blue    = CTextColor::myBlue;
static const CTextColor &Green   = CTextColor::myGreen;
static const CTextColor &Cyan    = CTextColor::myCyan;
static const CTextColor &Red     = CTextColor::myRed;
static const CTextColor &Magenta = CTextColor::myMagenta;
static const CTextColor &Yellow  = CTextColor::myYellow;
static const CTextColor &Default = CTextColor::myGrey;

static const CTextColor &Regular = CTextColor::myGrey;         // Обычный текст
static const CTextColor &Success = CTextColor::myGreen;        // Положительный результат
static const CTextColor &Warning = CTextColor::myYellow;       // Предупреждение
static const CTextColor &Error   = CTextColor::myRed;          // Ошибка
static const CTextColor &Title   = CTextColor::myCyan;         // Заголовок


//=====================================================================================================================
#endif

