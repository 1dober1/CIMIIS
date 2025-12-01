// Файл 01_CDataMap.cpp
// Словарь данных
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CDataMap.h"
#include "../02_BasicTypes/02_FloatingTypes.h"
#include "../02_BasicTypes/03_AngleTransform.h"
#include "../04_String/03_Case.h"
#include "../06_Output/04_CSystemLog.h"




//=====================================================================================================================
// Извлечение булева значения из словаря данных
bool extractBool(const CDataMap &data, const string &sourceName, const string &path, bool &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Если значение "да"
    value = false;
    if (toLower(itr->second.getValue(0, 0)) == "да")
        value = true;

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Извлечение строкового значения из словаря данных
bool extractString(const CDataMap &data, const string &sourceName, const string &path, string &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Если значение некорректно
    value = itr->second.getValue(0, 0);
    if (value.empty() == true)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Извлечение строкового вектора-строки из словаря данных
bool extractStringRow(const CDataMap &data, const string &sourceName, const string &path, vector<string> &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Извлечение значений из строки
    value.clear();
    for (size_t i = 0; i < itr->second.getColumnCount(); ++i)
    {
        const string &str = itr->second.getValue(0, i);
        if (str.empty() == false)
            value.push_back(str);
    }

    // Если всё пусто
    if (value.empty() == true)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Извлечение строкового вектора-столбца из словаря данных
bool extractStringColumn(const CDataMap &data, const string &sourceName, const string &path, vector<string> &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Извлечение значений из строки
    value.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        const string &str = itr->second.getValue(i, 0);
        if (str.empty() == false)
            value.push_back(str);
    }

    // Если всё пусто
    if (value.empty() == true)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Извлечение действительного значения из словаря данных
bool extractDoubleAny(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Всё хорошо
    value = atof(itr->second.getValue(0, 0).c_str());
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение действительного значения из словаря данных
bool extractDoublePositive(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    return extractDoubleGreaterThen(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg, badMsg, 0.0);
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение действительного значения из словаря данных
bool extractDoublePositiveRaw(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    return extractDoubleGreaterThenRaw(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg, badMsg, 0.0);
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение действительного значения из словаря данных
bool extractDoubleDiaposone(const CDataMap& data, const string& sourceName, const string& path, double& value, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const double minValue, const double maxValue)
{
    //чтение действительного значния с проверкой что значение больше чем mivValue
    bool isBigger = extractDoubleGreaterOrEqualThen(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg, badMsg, minValue);
    if (isBigger==false)
        return false;

    //если значение выходит за максимальную границу
    if (checkLE(value, maxValue) == false)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение действительного значения из словаря данных
bool extractDoubleGreaterThen(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold)
{
    // Чтение действительного значения
    bool result = extractDoubleAny(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg);
    if (result == false)
        return false;

    // Если значение некорректно
    if (checkGT(value, threshold) == false)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение действительного значения из словаря данных
bool extractDoubleGreaterThenRaw(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold)
{
    // Чтение действительного значения очень близкого к нулю
    bool result = extractDoubleAny(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg);
    if (result == false)
        return false;

    // Если значение некорректно
    if ((value < threshold) == true)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение действительного значения из словаря данных
bool extractDoubleGreaterOrEqualThen(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold)
{
    // Чтение действительного значения
    bool result = extractDoubleAny(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg);
    if (result == false)
        return false;

    // Если значение некорректно
    if (checkGE(value, threshold) == false)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Извлечение действительного значения из таблицы в словаре данных
bool extractDoubleFromTable(const CDataMap& data, const string& sourceName, const string& path, size_t rowIndex, size_t columnIndex, double& value, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& incorrectIndexMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0 || itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Если значение поля не попадает в границы таблицы
    if (itr->second.getRowCount() <= rowIndex || itr->second.getColumnCount() <= columnIndex)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << incorrectIndexMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << incorrectIndexMsg << EndL;
        return false;
    }

    // Всё хорошо
    value = atof(itr->second.getValue(rowIndex, columnIndex).c_str());
    return true;
}




//=====================================================================================================================
// Извлечение значения угла, выраженного в градусах, из словаря данных
bool extractDegreesAny(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Всё хорошо
    value = normalizeTo360(atof(itr->second.getValue(0, 0).c_str()));
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение значения угла, выраженного в градусах, из словаря данных
bool extractDegreesPositive(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    return extractDegreesGreaterThan(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg, badMsg, 0.0);
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение значения угла, выраженного в градусах, из словаря данных
bool extractDegreesGreaterThan(const CDataMap &data, const string &sourceName, const string &path, double &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, double threshold)
{
    // Чтение значения угла
    bool result = extractDegreesAny(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg);
    if (result == false)
        return false;

    // Если значение некорректно
    if (checkGT(value, threshold) == false)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Извлечение целочисленного значения из словаря данных
bool extractIntAny(const CDataMap &data, const string &sourceName, const string &path, int32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Всё хорошо
    value = (int32)atoi(itr->second.getValue(0, 0).c_str());
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение целочисленного значения из словаря данных
bool extractIntPositive(const CDataMap &data, const string &sourceName, const string &path, int32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    return extractIntGreaterThan(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg, badMsg, 0);
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение целочисленного значения из словаря данных
bool extractIntGreaterThan(const CDataMap &data, const string &sourceName, const string &path, int32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, int32 threshold)
{
    // Чтение беззнакового целого значения
    bool result = extractIntAny(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg);
    if (result == false)
        return false;

    // Если значение некорректно
    if (value <= threshold)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение целочисленного значения из словаря данных
bool extractUIntAny(const CDataMap &data, const string &sourceName, const string &path, uint32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Всё хорошо
    value = (uint32)atoi(itr->second.getValue(0, 0).c_str());
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение целочисленного значения из словаря данных
bool extractUIntPositive(const CDataMap &data, const string &sourceName, const string &path, uint32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg)
{
    return extractUIntGreaterThan(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg, badMsg, 0);
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение целочисленного значения из словаря данных
bool extractUIntGreaterThan(const CDataMap &data, const string &sourceName, const string &path, uint32 &value, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, uint32 threshold)
{
    // Чтение беззнакового целого значения
    bool result = extractUIntAny(data, sourceName, path, value, isWarning, notFoundMsg, emptyMsg);
    if (result == false)
        return false;

    // Если значение некорректно
    if (value <= threshold)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Извлечение сложной таблицы из словаря данных
bool extract13sdTable(const CDataMap& data, const string& sourceName, const string& path, vector<string>& valueY1, vector<string>& valueY2, vector<string>& valueY3, vector<double>& valueX1, vector<double>& valueX2, vector<double>& valueX3, vector<double>& valueX4, vector<double>& valueX5, vector<double>& valueX6, vector<double>& valueX7, vector<double>& valueX8, vector<double>& valueX9, vector<double>& valueX10, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const string& emptyItemMsg)
{
    // Masha
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }


    if (itr->second.getColumnCount() < 13)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }


    // Извлечение значений из строки
    valueY1.clear();
    valueY2.clear();
    valueY3.clear();
    valueX1.clear();
    valueX2.clear();
    valueX3.clear();
    valueX4.clear();
    valueX5.clear();
    valueX6.clear();
    valueX7.clear();
    valueX8.clear();
    valueX9.clear();
    valueX10.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        // первый столбец строковый, проверяем второй на то, он числовой
        string str = itr->second.getValue(i, 1);    //(i, 0);

        if (str.length() == 0)
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        // первый, второй и третий столбцы - строковые
        if ((isdigit(str[0]) != 0) || (isdigit(str[1]) != 0) || (isdigit(str[2]) != 0)) // Masha
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        valueY1.push_back(itr->second.getValue(i, 0).c_str());
        valueY2.push_back(itr->second.getValue(i, 1).c_str());
        valueY3.push_back(itr->second.getValue(i, 2).c_str());
        valueX1.push_back(atof(itr->second.getValue(i, 3).c_str()));
        valueX2.push_back(atof(itr->second.getValue(i, 4).c_str()));
        valueX3.push_back(atof(itr->second.getValue(i, 5).c_str()));
        valueX4.push_back(atof(itr->second.getValue(i, 6).c_str()));
        valueX5.push_back(atof(itr->second.getValue(i, 7).c_str()));
        valueX6.push_back(atof(itr->second.getValue(i, 8).c_str()));
        valueX7.push_back(atof(itr->second.getValue(i, 9).c_str()));
        valueX8.push_back(atof(itr->second.getValue(i, 10).c_str()));
        valueX9.push_back(atof(itr->second.getValue(i, 11).c_str()));
        valueX10.push_back(atof(itr->second.getValue(i, 12).c_str()));
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение сложной таблицы из словаря данных
bool extract7sdTable(const CDataMap& data, const string& sourceName, const string& path, vector<string>& valueY1, vector<string>& valueY2, vector<string>& valueY3, vector<double>& valueX1, vector<double>& valueX2, vector<double>& valueX3, vector<double>& valueX4, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const string& emptyItemMsg)
{
    // Masha
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }


    if (itr->second.getColumnCount() < 7)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }


    // Извлечение значений из строки
    valueY1.clear();
    valueY2.clear();
    valueY3.clear();
    valueX1.clear();
    valueX2.clear();
    valueX3.clear();
    valueX4.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        // первый столбец строковый, проверяем второй на то, он числовой
        string str = itr->second.getValue(i, 1);    //(i, 0);

        if (str.length() == 0)
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        // первый, второй и третий столбцы - строковые
        if ((isdigit(str[0]) != 0) || (isdigit(str[1]) != 0) || (isdigit(str[2]) != 0)) // Masha
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        valueY1.push_back(itr->second.getValue(i, 0).c_str());
        valueY2.push_back(itr->second.getValue(i, 1).c_str());
        valueY3.push_back(itr->second.getValue(i, 2).c_str());
        valueX1.push_back(atof(itr->second.getValue(i, 3).c_str()));
        valueX2.push_back(atof(itr->second.getValue(i, 4).c_str()));
        valueX3.push_back(atof(itr->second.getValue(i, 5).c_str()));
        valueX4.push_back(atof(itr->second.getValue(i, 6).c_str()));
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение сложной таблицы из словаря данных
bool extract5sTable(const CDataMap& data, const string& sourceName, const string& path, vector<string>& valueX, vector<double>& valueY1, vector<double>& valueY2, vector<double>& valueY3, vector<double>& valueY4, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg, const string& emptyItemMsg)
{
    // Арт
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }


    if (itr->second.getColumnCount() < 5)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }


    // Извлечение значений из строки
    valueX.clear();
    valueY1.clear();
    valueY2.clear();
    valueY3.clear();
    valueY4.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        // первый столбец строковый, проверяем второй на то, он числовой
        string str = itr->second.getValue(i, 1);    //(i, 0);

        if (str.length() == 0)
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        // первый столбец строковый, проверяем второй на то, он числовой
        if ((isdigit(str[0]) == 0) && (str[0] != '-')) // Art
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        valueX.push_back(itr->second.getValue(i, 0).c_str());
        valueY1.push_back(atof(itr->second.getValue(i, 1).c_str()));
        valueY2.push_back(atof(itr->second.getValue(i, 2).c_str()));
        valueY3.push_back(atof(itr->second.getValue(i, 3).c_str()));
        valueY4.push_back(atof(itr->second.getValue(i, 4).c_str()));
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение сложной таблицы из словаря данных
bool extract4Table(const CDataMap &data, const string &sourceName, const string &path, vector<double> &valueX, vector<double> &valueY1, vector<double> &valueY2, vector<double> &valueY3, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, const string &emptyItemMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }


    if (itr->second.getColumnCount() < 4)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }


    // Извлечение значений из строки
    valueX.clear();
    valueY1.clear();
    valueY2.clear();
    valueY3.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        string str = itr->second.getValue(i, 0);
        
        if (str.length() == 0)
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        if ((isdigit(str[0]) == 0)&&(str[0] != '-'))
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        valueX.push_back(atof(itr->second.getValue(i, 0).c_str()));
        valueY1.push_back(atof(itr->second.getValue(i, 1).c_str()));
        valueY2.push_back(atof(itr->second.getValue(i, 2).c_str()));
        valueY3.push_back(atof(itr->second.getValue(i, 3).c_str()));
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение сложной таблицы из словаря данных
bool extract3Table(const CDataMap &data, const string &sourceName, const string &path, vector<double> &valueX, vector<double> &valueY1, vector<double> &valueY2,bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, const string &emptyItemMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }


    if (itr->second.getColumnCount() < 3)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }


    // Извлечение значений из строки
    valueX.clear();
    valueY1.clear();
    valueY2.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        string str = itr->second.getValue(i, 0);

        if (str.length() == 0)
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        if ((isdigit(str[0]) == 0)&&(str[0] != '-'))
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        valueX.push_back(atof(itr->second.getValue(i, 0).c_str()));
        valueY1.push_back(atof(itr->second.getValue(i, 1).c_str()));
        valueY2.push_back(atof(itr->second.getValue(i, 2).c_str()));
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение сложной таблицы из словаря данных
bool extract2Table(const CDataMap &data, const string &sourceName, const string &path, vector<double> &valueX, vector<double> &valueY, bool isWarning, const string &notFoundMsg, const string &emptyMsg, const string &badMsg, const string &emptyItemMsg)
{
    // Если поле не найдено в массиве данных
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }


    if (itr->second.getColumnCount() < 2)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }


    // Извлечение значений из строки
    valueX.clear();
    valueY.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        string str = itr->second.getValue(i, 0);

        if (str.length() == 0)
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        if ((isdigit(str[0]) == 0)&&(str[0] != '-'))
        {
            if (isWarning == true)
                Log << "    Предупреждение в " << sourceName << ": " << emptyItemMsg << EndL;
            else
                Log << "    Ошибка в " << sourceName << ": " << emptyItemMsg << EndL;
            return false;
        }

        valueX.push_back(atof(itr->second.getValue(i, 0).c_str()));
        valueY.push_back(atof(itr->second.getValue(i, 1).c_str()));
    }

    // Всё хорошо
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
// Извлечение сложной таблицы из словаря данных
bool extractTable(const CDataMap& data, const string& sourceName, const string& path, vector<vector<double>>& vectorMatrix, bool isWarning, const string& notFoundMsg, const string& emptyMsg, const string& badMsg)
{
    CDataMap::const_iterator itr = data.find(path);
    if (itr == data.end())
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << notFoundMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << notFoundMsg << EndL;
        return false;
    }

    // Если значение поля не указано
    if (itr->second.getRowCount() == 0 || itr->second.getColumnCount() == 0)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << emptyMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << emptyMsg << EndL;
        return false;
    }

    // Извлечение значений в матрицу
    vectorMatrix.clear();
    for (size_t i = 0; i < itr->second.getRowCount(); ++i)
    {
        vector<double> tmpVector;
        for (size_t j = 0; j < itr->second.getColumnCount(); ++j)
        {
            const double par = atof(itr->second.getValue(i, j).c_str());
            tmpVector.push_back(par);
        }
        vectorMatrix.push_back(tmpVector);
    }
    // Если всё пусто
    if (vectorMatrix.empty()==true)
    {
        if (isWarning == true)
            Log << "    Предупреждение в " << sourceName << ": " << badMsg << EndL;
        else
            Log << "    Ошибка в " << sourceName << ": " << badMsg << EndL;
        return false;
    }

    // Всё хорошо
    return true;
}




//=====================================================================================================================
// Обёртка над извлечение действительного значения из таблицы
bool extractDoubleDiaposoneWithOptimizedText(const CDataMap& data, const string& sourceName, const string& path, double& value, bool isWarning, const double minValue, const double maxValue)
{
    return extractDoubleDiaposone(data, sourceName, path, value, isWarning,
        "поле \"" + path + "\" не найдено!",
        "в поле \"" + path + "\" не указано значение!",
        "в поле \"" + path + "\" допустимо только значение больше " + std::to_string(minValue) + " и меньше " + std::to_string(maxValue) + "!",
        minValue, maxValue);

}


