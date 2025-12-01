// Файл 01_CStringTable.h
// Таблица строковых значений
//

#ifndef Kernel_Parser_CStringTable_h
#define Kernel_Parser_CStringTable_h


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
// Таблица строковых значений
class CStringTable
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    // Обозначение строки таблицы
    typedef vector<string> stringRow;


private:
    //-----------------------------------------------------------------------------------------------------------------
    vector<stringRow> myRowList;        // Массив строк таблицы
    size_t myColumnCount;               // Количество столбцов в таблице
    static string myEmptyValue;         // Пустое значение


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CStringTable();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к элементам таблицы
    size_t getRowCount() const;
    size_t getColumnCount() const;
    const string & getValue(size_t rowIndex, size_t columnIndex) const;
    bool setValue(size_t rowIndex, size_t columnIndex, const string &value);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Добавление элементов таблицы
    void insertRow(size_t beforeIndex);
    void insertColumn(size_t beforeIndex);

    // Удаление элементов таблицы
    void removeRow(size_t index);
    void removeColumn(size_t index);
    void removeAll();

    // Очистка элементов таблицы
    void clearRow(size_t index);
    void clearColumn(size_t index);
    void clearAll();

    // Если строка в таблице пустая
    bool isEmptyRow(size_t index) const;


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif

