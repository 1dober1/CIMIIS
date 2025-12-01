// ‘айл 01_CControlChar.h
// ”правл€ющий символ
//

#ifndef Kernel_Output_CControlChar_h
#define Kernel_Output_CControlChar_h


//=====================================================================================================================
// «аголовочные файлы
#include "../Settings.h"
#include "../01_Environment/01_StdLib.h"
#include "../01_Environment/02_DetectSystem.h"
#include "../01_Environment/03_CheckSystem.h"
#include "../01_Environment/04_DetectCompiler.h"
#include "../01_Environment/05_CheckCompiler.h"
#include "../01_Environment/06_SysLib.h"


//=====================================================================================================================
// ”правл€ющий символ
class CControlChar
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    // «апрет создани€ и копировани€ управл€ющих символов напр€мую
    CControlChar();
    CControlChar(const CControlChar &);
    CControlChar & operator =(const CControlChar &);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // ѕеречень стандартных управл€ющих символов
    static CControlChar myTab;             // —имвол табул€ции
    static CControlChar myLine;            // —имвол новой строки
    static CControlChar mySpace;           // —имвол пробела


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
//  раткие обозначени€ дл€ управл€ющих символов
static const CControlChar &Tab   = CControlChar::myTab;
static const CControlChar &Line  = CControlChar::myLine;
static const CControlChar &Space = CControlChar::mySpace;
static const CControlChar &EndL  = CControlChar::myLine;


//=====================================================================================================================
#endif

