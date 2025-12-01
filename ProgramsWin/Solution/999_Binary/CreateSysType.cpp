// Файл CreateSysType.cpp
// Вызов моделируемых систем
//

//=====================================================================================================================
// Заголовочные файлы
#include "Settings.h"


//=====================================================================================================================

CSysType * CreateSysType(const string &key)
{ 
    if (key == "launchmissile")
        return new CSysLaunchType();

    if (key == "missilesimple")
        return new CSysMotionMissileSimpleType();
        
    if (key == "navedenienpc")
        return new CSysMotionNavedenieNPCType();

    if (key == "navedeniezps")
        return new CSysMotionNewType();
       
    if (key == "opticthermal")
        return new SysOpticThermal::CSysOpticThermalType();
       
    return NULL;
}
