// Файл 02_CTextColor.cpp
// Цвет текста в консоли
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_CTextColor.h"




//=====================================================================================================================
// Перечень стандартных цветов текста
#ifdef SYSTEM_WINDOWS
CTextColor CTextColor::myWhite  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
CTextColor CTextColor::myGrey   (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
CTextColor CTextColor::myBlue   (FOREGROUND_BLUE);
CTextColor CTextColor::myGreen  (FOREGROUND_GREEN);
CTextColor CTextColor::myCyan   (FOREGROUND_GREEN | FOREGROUND_BLUE);
CTextColor CTextColor::myRed    (FOREGROUND_RED);
CTextColor CTextColor::myMagenta(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
CTextColor CTextColor::myYellow (FOREGROUND_RED | FOREGROUND_GREEN);
#else
CTextColor CTextColor::myWhite  ("\033[1;37m");
CTextColor CTextColor::myGrey   ("\033[0;37m");
CTextColor CTextColor::myBlue   ("\033[0;34m");
CTextColor CTextColor::myGreen  ("\033[0;32m");
CTextColor CTextColor::myCyan   ("\033[0;36m");
CTextColor CTextColor::myRed    ("\033[0;31m");
CTextColor CTextColor::myMagenta("\033[0;35m");
CTextColor CTextColor::myYellow ("\033[0;33m");
#endif




//=====================================================================================================================
// Конструктор класса
#ifdef SYSTEM_WINDOWS
CTextColor::CTextColor(WORD colorCode)
{
    myColorCode = colorCode;
}
#else
CTextColor::CTextColor(const string &colorCode)
{
    myColorCode = colorCode;
}
#endif


