// Файл 02_CFormat.h
// Информация об оптической станции
//

#ifndef _600_OpticThermal_Utility_CFormat_h
#define _600_OpticThermal_Utility_CFormat_h

//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"

namespace SysOpticThermal
{
namespace Utility
{
    template<typename ...Args>
    std::string format(const char* format, Args... args)
    {
        const int size = std::snprintf(NULL, 0, format, args...);
        const std::unique_ptr<char[]> c_string(new char[size + 1]);
        snprintf(c_string.get(), size + 1, format, args...);
        return std::string(c_string.get());
    }
}
}

#endif