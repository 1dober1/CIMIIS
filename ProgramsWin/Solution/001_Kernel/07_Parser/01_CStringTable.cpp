// Файл 01_CStringTable.cpp
// Таблица строковых значений
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CStringTable.h"




//=====================================================================================================================
// Пустая строка
string CStringTable::myEmptyValue;




//=====================================================================================================================
// Конструктор класса
CStringTable::CStringTable()
{
    myColumnCount = 0;
}




//=====================================================================================================================
// Доступ к элементам таблицы
size_t CStringTable::getRowCount() const
{
    return myRowList.size();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к элементам таблицы
size_t CStringTable::getColumnCount() const
{
    return myColumnCount;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к элементам таблицы
const string & CStringTable::getValue(size_t rowIndex, size_t columnIndex) const
{
    // Если некорректный индекс строки
    if (rowIndex >= myRowList.size())
        return myEmptyValue;

    // Если некорректный индекс столбца
    const stringRow &row = myRowList[rowIndex];
    if (columnIndex >= row.size())
        return myEmptyValue;

    // Выдача значения
    return row[columnIndex];
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к элементам таблицы
bool CStringTable::setValue(size_t rowIndex, size_t columnIndex, const string &value)
{
    // Если некорректный индекс строки
    if (rowIndex >= myRowList.size())
        return false;

    // Если некорректный индекс столбца
    stringRow &row = myRowList[rowIndex];
    if (columnIndex >= row.size())
        return false;

    // Присваивание значения
    row[columnIndex] = value;
    return true;
}




//=====================================================================================================================
// Добавление элементов таблицы
void CStringTable::insertRow(size_t beforeIndex)
{
    // Формирование пустой строки
    stringRow newRow;
    if (myColumnCount != 0)
        newRow = stringRow(myColumnCount);

    // Вставка строки в конец таблицы
    if (beforeIndex >= myRowList.size())
    {
        myRowList.push_back(newRow);
        return;
    }

    // Вставка строки перед указанной строкой
    myRowList.insert(myRowList.begin() + beforeIndex, newRow);
}




//---------------------------------------------------------------------------------------------------------------------
// Добавление элементов таблицы
void CStringTable::insertColumn(size_t beforeIndex)
{
    // Вставка столбца в конец таблицы
    if (beforeIndex >= myColumnCount)
    {
        for (size_t i = 0; i < myRowList.size(); ++i)
            myRowList[i].push_back(string());
        myColumnCount++;
        return;
    }

    // Вставка столбца перед указанным
    for (size_t i = 0; i < myRowList.size(); ++i)
        myRowList[i].insert(myRowList[i].begin() + beforeIndex, string());
    myColumnCount++;
}




//=====================================================================================================================
// Удаление элементов таблицы
void CStringTable::removeRow(size_t index)
{
    // Если строка с таким индексом не существует
    if (index >= myRowList.size())
        return;

    // Удалить строку с таким индексом
    myRowList.erase(myRowList.begin() + index);
}




//---------------------------------------------------------------------------------------------------------------------
// Удаление элементов таблицы
void CStringTable::removeColumn(size_t index)
{
    // Если столбец с таким индексом не существует
    if (index >= myColumnCount)
        return;

    // Удалить столбец с таким индексом
    for(size_t i = 0; i < myRowList.size(); ++i)
        myRowList[i].erase(myRowList[i].begin() + index);
    --myColumnCount;
}




//---------------------------------------------------------------------------------------------------------------------
// Удаление элементов таблицы
void CStringTable::removeAll()
{
    myRowList.clear();
    myColumnCount = 0;
}




//=====================================================================================================================
// Очистка элементов таблицы
void CStringTable::clearRow(size_t index)
{
    // Если строка с таким индексом не существует
    if (index >= myRowList.size())
        return;

    // Очистить строку с таким индексом
    stringRow &row = myRowList[index];
    for (size_t i = 0; i < row.size(); ++i)
        row[i] = string();
}




//---------------------------------------------------------------------------------------------------------------------
// Очистка элементов таблицы
void CStringTable::clearColumn(size_t index)
{
    // Если столбец с таким индексом не существует
    if (index >= myColumnCount)
        return;

    // Очистить столбец с таким индексом
    for (size_t i = 0; i < myRowList.size(); ++i)
    {
        stringRow &row = myRowList[i];
        row[index] = string();
    }
}




//---------------------------------------------------------------------------------------------------------------------
// Очистка элементов таблицы
void CStringTable::clearAll()
{
    for (size_t i = 0; i < myRowList.size(); ++i)
    {
        stringRow &row = myRowList[i];
        for (size_t j = 0; j < row.size(); ++j)
            row[j] = string();
    }
}




//=====================================================================================================================
// Если строка в таблице пустая
bool CStringTable::isEmptyRow(size_t index) const
{
    // Если строка с таким индексом не существует
    if (index >= myRowList.size())
        return false;

    // Поиск хотя бы одного значения в строке
    bool emptyRow = true;
    const stringRow &row = myRowList[index];
    for (size_t i = 0; i < row.size(); ++i)
    {
        if (row[i].empty() == false)
        {
            emptyRow = false;
            break;
        }
    }

    // Выдача результата
    return emptyRow;
}

