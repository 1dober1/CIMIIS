//=============================================================================
// Заголовочные файлы
#include "01_CStorage.h"

namespace util = SysOpticThermal::Utility;


util::CStorage::CStorage(const CDataMap& data, const std::string& fileName)
    : myData(data), myFileName(fileName)
{
}

bool util::CStorage::extractBool(const string& path, bool& value, bool isWarning) const
{
    return ::extractBool(myData, myFileName, path, value, isWarning,
        "поле \"" + path + "\" не найдено!",
        "в поле \"" + path + "\" не указано значение!");
}

bool util::CStorage::extractString(const string& path, string& value, bool isWarning) const
{
    return ::extractString(myData, myFileName, path, value, isWarning,
        "поле \"" + path + "\" не найдено!",
        "в поле \"" + path + "\" не указано значение!",
        "поле \"" + path + "\" не может быть пустым!");
}

bool util::CStorage::extractDoubleAny(const string& path, double& value, bool isWarning) const
{
    return ::extractDoubleAny(myData, myFileName, path, value, isWarning,
        "поле \"" + path + "\" не найдено!",
        "в поле \"" + path + "\" не указано значение!");
}

bool util::CStorage::extractDoublePositive(const string& path, double& value, bool isWarning) const
{
    return ::extractDoublePositive(myData, myFileName, path, value, isWarning,
        "поле \"" + path + "\" не найдено!",
        "в поле \"" + path + "\" не указано значение!",
        "в поле \"" + path + "\" допустимо только положительное значение!");
}

bool util::CStorage::extractDegreesPositive(const string& path, double& value, bool isWarning) const
{
    return ::extractDegreesPositive(myData, myFileName, path, value, isWarning,
        "поле \"" + path + "\" не найдено!",
        "в поле \"" + path + "\" не указано значение!",
        "в поле \"" + path + "\" допустимо только положительное значение!");
}

