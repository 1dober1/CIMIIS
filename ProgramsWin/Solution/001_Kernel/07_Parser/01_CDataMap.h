// Файл 01_CDataMap.h
// Словарь данных
//

#ifndef Kernel_Parser_CDataMap_h
#define Kernel_Parser_CDataMap_h


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
#include "01_CStringTable.h"


//=====================================================================================================================
// Словарь данных
typedef map<string, CStringTable> CDataMap;


//=====================================================================================================================
// Функции извлечения данных конкретных типов из словаря данных
bool extractBool(const CDataMap &data, const string &sourceName, const string &path, bool &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg);
bool extractString(const CDataMap &data, const string &sourceName, const string &path, string &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractStringRow(const CDataMap &data, const string &sourceName, const string &path, vector<string> &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractStringColumn(const CDataMap &data, const string &sourceName, const string &path, vector<string> &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractDoubleAny(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg);
bool extractDoublePositive(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractDoublePositiveRaw(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractDoubleDiaposone(const CDataMap& data, const string& sourceName, const string& path, double& value, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const double minValue, const double maxValue);
bool extractDoubleGreaterThen(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold);
bool extractDoubleGreaterThenRaw(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold);
bool extractDoubleGreaterOrEqualThen(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold);
bool extractDoubleFromTable(const CDataMap& data, const string& sourceName, const string& path, size_t rowIndex, size_t columnIndex, double& value, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& incorrectIndexMsg);
bool extractDegreesAny(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg);
bool extractDegreesPositive(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractDegreesGreaterThan(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold);
bool extractIntAny(const CDataMap &data, const string &sourceName, const string &path, int32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg);
bool extractIntPositive(const CDataMap &data, const string &sourceName, const string &path, int32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractIntGreaterThan(const CDataMap &data, const string &sourceName, const string &path, int32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, int32 threshold);
bool extractUIntAny(const CDataMap &data, const string &sourceName, const string &path, uint32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg);
bool extractUIntPositive(const CDataMap &data, const string &sourceName, const string &path, uint32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg);
bool extractUIntGreaterThan(const CDataMap &data, const string &sourceName, const string &path, uint32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, uint32 threshold);
bool extract13sdTable(const CDataMap& data, const string& sourceName, const string& path, vector<string>& valueY1, vector<string>& valueY2, vector<string>& valueY3, vector<double>& valueX1, vector<double>& valueX2, vector<double>& valueX3, vector<double>& valueX4, vector<double>& valueX5, vector<double>& valueX6, vector<double>& valueX7, vector<double>& valueX8, vector<double>& valueX9, vector<double>& valueX10, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const string& emptyItemMsg);
bool extract7sdTable(const CDataMap& data, const string& sourceName, const string& path, vector<string>& valueY1, vector<string>& valueY2, vector<string>& valueY3, vector<double>& valueX1, vector<double>& valueX2, vector<double>& valueX3, vector<double>& valueX4, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const string& emptyItemMsg);
bool extract5sTable(const CDataMap& data, const string& sourceName, const string& path, vector<string>& valueX, vector<double>& valueY1, vector<double>& valueY2, vector<double>& valueY3, vector<double>& valueY4, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const string& emptyItemMsg);
bool extract4Table(const CDataMap &data, const string &sourceName, const string &path, vector<double> &valueX, vector<double> &valueY1, vector<double> &valueY2, vector<double> &valueY3, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, const string &emptyItemMsg);
bool extract3Table(const CDataMap &data, const string &sourceName, const string &path, vector<double> &valueX, vector<double> &valueY1, vector<double> &valueY2, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, const string &emptyItemMsg);
bool extract2Table(const CDataMap &data, const string &sourceName, const string &path, vector<double> &valueX, vector<double> &valueY, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, const string &emptyItemMsg);
bool extractTable(const CDataMap& data, const string& sourceName, const string& path, vector<vector<double>>& vectorMatrix, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg);
bool extractDoubleDiaposoneWithOptimizedText(const CDataMap& data, const string& sourceName, const string& path, double& value, bool isWarning, const double minValue, const double maxValue);


//=====================================================================================================================
#endif

