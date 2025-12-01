#ifndef _600_OpticThermal_Utility_CStorage_h
#define _600_OpticThermal_Utility_CStorage_h

#include "../Settings.h"

namespace SysOpticThermal
{
namespace Utility
{
    class CStorage
    {
    private:
        const CDataMap& myData;
        const std::string& myFileName;

    public:
        CStorage(const CDataMap& data, const std::string& fileName);

    public:
        bool extractBool(const string& path, bool& value, bool isWarning = false) const;
        bool extractString(const string& path, string& value, bool isWarning = false) const;
        bool extractDoubleAny(const string& path, double& value, bool isWarning = false) const;
        bool extractDoublePositive(const string& path, double& value, bool isWarning = false) const;
        bool extractDegreesPositive(const string& path, double& value, bool isWarning = false) const;
    };
}
}

#endif