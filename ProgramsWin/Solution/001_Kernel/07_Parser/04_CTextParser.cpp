// Файл 04_CTextParser.cpp
// Синтаксический анализатор текстовых данных
//

//=====================================================================================================================
// Заголовочные файлы
#include "04_CTextParser.h"
#include "../03_Encoding/UTF8.h"




//=====================================================================================================================
// Единственный на всю программу экземпляр синтаксического анализатора
CTextParser CTextParser::myGlobalParser;




//=====================================================================================================================
// Конструктор класса
CTextParser::CTextParser()
{
    myWasError = true;
    myErrorLine = 0;
    myErrorColumn = 0;
    myErrorExtraLine = 0;
    myErrorExtraColumn = 0;
    myErrorMessage = "Входной текст не задан!";
}




//=====================================================================================================================
// Разбор текста
void CTextParser::parse(const string &text)
{
    // Очистить результат
    myData.clear();

    // Если текст отсутствует
    if (text.empty() == true)
    {
        myWasError = true;
        myErrorLine = 0;
        myErrorColumn = 0;
        myErrorExtraLine = 0;
        myErrorExtraColumn = 0;
        myErrorMessage = "Входной текст не задан!";
        return;
    }

    // Преобразование входного текста из UTF-8 в нормальную кодировку
    string txt = fromUTF8(text);

    // Подготовка лексического препроцессора
    myScanner.init(txt);
    myScanResult.clear();

    // Сброс состояния ошибки
    myWasError = false;
    myErrorMessage.clear();

    // Последовательное извлечение лексем и анализ их взаимного расположения
    int state = 1;                      // Текущее состояние анализируеющего автомата
    CTextLexeme lexeme;                 // Последняя выделенная лексема
    size_t columnIndex = 0;             // Первый свободный индекс столбца в таблице
    CStringTable value;                 // Накопленное значение параметра или таблицы
    list<CTextLexeme> lexemeStack;      // Иерархия вложенных лексем
    while (lexeme = myScanner.read(), lexeme.myType != CTextLexeme::END_OF_TEXT)
    {
        // Запись данных в структуру обработанного текста
        _collect(lexeme.myType);

        // Если при выделении очередной лексемы произошла ошибка
        if (myScanner.wasError() == true)
        {
            myWasError = true;
            myErrorLine = myScanner.getErrorLine();
            myErrorColumn = myScanner.getErrorExtraColumn();
            myErrorExtraLine = myErrorLine;
            myErrorExtraColumn = myScanner.getErrorExtraColumn();
            myErrorMessage = myScanner.getErrorMessage();
            return;
        }

        // Обработка состояния автомата
        switch (state)
        {
        case 1:
            // Пропуск пустых строк
            if(lexeme.myType == CTextLexeme::END_OF_LINE)
                continue;

            // Прочитан идентификатор
            if (lexeme.myType == CTextLexeme::IDENTIFIER)
            {
                state = 2;
                lexemeStack.push_back(lexeme);
                continue;
            }

            // Прочитано начало таблицы
            if (lexeme.myType == CTextLexeme::TABLE_BRACKET)
            {
                state = 5;
                columnIndex = 0;
                value.removeAll();
                value.insertRow(0);
                lexemeStack.push_back(lexeme);
                continue;
            }

            // Прочитано начало группы
            if (lexeme.myType == CTextLexeme::GROUP_BRACKET)
            {
                state = 9;
                lexemeStack.push_back(lexeme);
                continue;
            }

            // Прочитан конец группы
            if (lexeme.myType == CTextLexeme::CLOSE_BRACKET)
            {
                // Если пара для данной лексемы в принципе не существует
                if (lexemeStack.empty() == true)
                {
                    myWasError = true;
                    myErrorLine = lexeme.myLine;
                    myErrorColumn = lexeme.myColumn;
                    myErrorExtraLine = myErrorLine;
                    myErrorExtraColumn = lexeme.myColumn;
                    myErrorMessage = _errorPos() + "непарный закрывающий дескриптор группы! <" + lexeme.myValue + ">";
                    return;
                }

                // Если пара для данной лексемы не совпадает с ней по имени
                if (lexemeStack.back().myValue != lexeme.myValue)
                {
                    myWasError = true;
                    myErrorLine = lexeme.myLine;
                    myErrorColumn = lexeme.myColumn;
                    myErrorExtraLine = lexemeStack.back().myLine;
                    myErrorExtraColumn = lexemeStack.back().myColumn;
                    myErrorMessage = _errorPos() + "имена в закрывающем и открывающем " + _extraPos() + " дескрипторах группы не совпадают! <" + lexeme.myValue + " != " + lexemeStack.back().myValue + ">";
                    return;
                }

                // Если нашлась парная лексема
                state = 10;
                lexemeStack.pop_back();
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается имя параметра, дескриптор группы или таблицы>";
            return;


        case 2:
            // Прочитан оператор присваивания
            if (lexeme.myType == CTextLexeme::ASSIGNMENT)
            {
                state = 3;
                value.removeAll();
                value.insertRow(0);
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается оператор присваивания>";
            return;


        case 3:
            // Прочитано числовое значение
            if (lexeme.myType == CTextLexeme::NUMBER_VALUE)
            {
                state = 4;
                value.insertColumn(value.getColumnCount());
                value.setValue(0, value.getColumnCount() - 1, lexeme.myValue);
                continue;
            }

            // Прочитано строковое значение
            if (lexeme.myType == CTextLexeme::STRING_VALUE)
            {
                state = 4;
                value.insertColumn(value.getColumnCount());
                value.setValue(0, value.getColumnCount() - 1, lexeme.myValue);
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается числовое или строковое значение>";
            return;


        case 4:
            // Прочитано числовое значение
            if (lexeme.myType == CTextLexeme::NUMBER_VALUE)
            {
                value.insertColumn(value.getColumnCount());
                value.setValue(0, value.getColumnCount() - 1, lexeme.myValue);
                continue;
            }

            // Прочитано строковое значение
            if (lexeme.myType == CTextLexeme::STRING_VALUE)
            {
                value.insertColumn(value.getColumnCount());
                value.setValue(0, value.getColumnCount() - 1, lexeme.myValue);
                continue;
            }

            // Прочитана новая строка
            if (lexeme.myType == CTextLexeme::END_OF_LINE)
            {
                state = 1;
                myData.insert(make_pair(_construct(lexemeStack), value));
                lexemeStack.pop_back();
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается числовое значение, строковое значение или новая строка>";
            return;


        case 5:
            // Прочитана новая строка
            if (lexeme.myType == CTextLexeme::END_OF_LINE)
            {
                state = 6;
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается новая строка>";
            return;


        case 6:
            // Пропуск пустых строк
            if(lexeme.myType == CTextLexeme::END_OF_LINE)
                continue;

            // Прочитано числовое значение
            if (lexeme.myType == CTextLexeme::NUMBER_VALUE)
            {
                state = 7;
                if (columnIndex + 1 > value.getColumnCount())
                    value.insertColumn(value.getColumnCount());
                value.setValue(value.getRowCount() - 1, columnIndex, lexeme.myValue);
                columnIndex++;
                continue;
            }

            // Прочитано строковое значение
            if (lexeme.myType == CTextLexeme::STRING_VALUE)
            {
                state = 7;
                if (columnIndex + 1 > value.getColumnCount())
                    value.insertColumn(value.getColumnCount());
                value.setValue(value.getRowCount() - 1, columnIndex, lexeme.myValue);
                columnIndex++;
                continue;
            }

            // Прочитан конец таблицы
            if (lexeme.myType == CTextLexeme::CLOSE_BRACKET)
            {
                // Если пара для данной лексемы в принципе не существует
                if (lexemeStack.empty() == true)
                {
                    myWasError = true;
                    myErrorLine = lexeme.myLine;
                    myErrorColumn = lexeme.myColumn;
                    myErrorExtraLine = myErrorLine;
                    myErrorExtraColumn = lexeme.myColumn;
                    myErrorMessage = _errorPos() + "непарный закрывающий дескриптор таблицы! <" + lexeme.myValue + ">";
                    return;
                }

                // Если пара для данной лексемы не совпадает с ней по имени
                if (lexemeStack.back().myValue != lexeme.myValue)
                {
                    myWasError = true;
                    myErrorLine = lexeme.myLine;
                    myErrorColumn = lexeme.myColumn;
                    myErrorExtraLine = lexemeStack.back().myLine;
                    myErrorExtraColumn = lexemeStack.back().myColumn;
                    myErrorMessage = _errorPos() + "имена в закрывающем и открывающем " + _extraPos() + " дескрипторах таблицы не совпадают! <" + lexeme.myValue + " != " + lexemeStack.back().myValue + ">";
                    return;
                }

                // Если нашлась парная лексема
                state = 8;
                value.removeRow(value.getRowCount() - 1);
                myData.insert(make_pair(_construct(lexemeStack), value));
                lexemeStack.pop_back();
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается числовое значение, строковое значение или закрывающий дескриптор таблицы>";
            return;


        case 7:
            // Прочитано числовое значение
            if (lexeme.myType == CTextLexeme::NUMBER_VALUE)
            {
                if (columnIndex + 1 > value.getColumnCount())
                    value.insertColumn(value.getColumnCount());
                value.setValue(value.getRowCount() - 1, columnIndex, lexeme.myValue);
                columnIndex++;
                continue;
            }

            // Прочитано строковое значение
            if (lexeme.myType == CTextLexeme::STRING_VALUE)
            {
                if (columnIndex + 1 > value.getColumnCount())
                    value.insertColumn(value.getColumnCount());
                value.setValue(value.getRowCount() - 1, columnIndex, lexeme.myValue);
                columnIndex++;
                continue;
            }

            // Прочитан переход на новую строку в таблице
            if (lexeme.myType == CTextLexeme::END_OF_LINE)
            {
                state = 7;
                if(value.isEmptyRow(value.getRowCount() - 1) == false)
                    value.insertRow(value.getRowCount());
                columnIndex = 0;
                continue;
            }

            // Прочитан конец таблицы
            if (lexeme.myType == CTextLexeme::CLOSE_BRACKET)
            {
                // Если пара для данной лексемы в принципе не существует
                if (lexemeStack.empty() == true)
                {
                    myWasError = true;
                    myErrorLine = lexeme.myLine;
                    myErrorColumn = lexeme.myColumn;
                    myErrorExtraLine = myErrorLine;
                    myErrorExtraColumn = lexeme.myColumn;
                    myErrorMessage = _errorPos() + "непарный закрывающий дескриптор таблицы! <" + lexeme.myValue + ">";
                    return;
                }

                // Если пара для данной лексемы не совпадает с ней по имени
                if (lexemeStack.back().myValue != lexeme.myValue)
                {
                    myWasError = true;
                    myErrorLine = lexeme.myLine;
                    myErrorColumn = lexeme.myColumn;
                    myErrorExtraLine = lexemeStack.back().myLine;
                    myErrorExtraColumn = lexemeStack.back().myColumn;
                    myErrorMessage = _errorPos() + "имена в закрывающем и открывающем " + _extraPos() + " дескрипторах таблицы не совпадают! <" + lexeme.myValue + " != " + lexemeStack.back().myValue + ">";
                    return;
                }

                // Если нашлась парная лексема
                state = 8;
                value.removeRow(value.getRowCount() - 1);
                myData.insert(make_pair(_construct(lexemeStack), value));
                lexemeStack.pop_back();
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается числовое значение, строковое значение, новая строка или закрывающий дескриптор таблицы>";
            return;


        case 8:
            // Прочитана новая строка
            if (lexeme.myType == CTextLexeme::END_OF_LINE)
            {
                state = 1;
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается новая строка>";
            return;


        case 9:
            // Прочитана новая строка
            if (lexeme.myType == CTextLexeme::END_OF_LINE)
            {
                state = 1;
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается новая строка>";
            return;


        case 10:
            // Прочитана новая строка
            if (lexeme.myType == CTextLexeme::END_OF_LINE)
            {
                state = 1;
                continue;
            }

            // Ошибка! Прочитана недопустимая лексема!
            myWasError = true;
            myErrorLine = lexeme.myLine;
            myErrorColumn = lexeme.myColumn;
            myErrorExtraLine = lexeme.myLine;
            myErrorExtraColumn = lexeme.myColumn;
            myErrorMessage = _errorPos() + "недопустимая лексема! <ожидается новая строка>";
            return;
        }
    }
    _collect(lexeme.myType);

    // Обработка остановки в состоянии 4 (прочитан, но не сохранён параметр)
    if (state == 4)
    {
        myData.insert(make_pair(_construct(lexemeStack), value));
        lexemeStack.pop_back();
    }

    // Обработка некорректной остановки автомата в нефинальных состояниях
    switch (state)
    {
    case 2:
        myWasError = true;
        myErrorLine = lexeme.myLine;
        myErrorColumn = lexeme.myColumn;
        myErrorExtraLine = lexeme.myLine;
        myErrorExtraColumn = lexeme.myColumn;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает оператора присваивания>";
        return;

    case 3:
        myWasError = true;
        myErrorLine = lexeme.myLine;
        myErrorColumn = lexeme.myColumn;
        myErrorExtraLine = lexeme.myLine;
        myErrorExtraColumn = lexeme.myColumn;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает числового или строкового значения>";
        return;

    case 5:
    case 9:
        myWasError = true;
        myErrorLine = lexeme.myLine;
        myErrorColumn = lexeme.myColumn;
        myErrorExtraLine = lexeme.myLine;
        myErrorExtraColumn = lexeme.myColumn;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает новой строки>";
        return;

    case 6:
    case 7:
        myWasError = true;
        myErrorLine = lexeme.myLine;
        myErrorColumn = lexeme.myColumn;
        myErrorExtraLine = lexeme.myLine;
        myErrorExtraColumn = lexeme.myColumn;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает закрывающего дескриптора таблицы>";
        return;
    }

    // Обработка остановки с непустым стеком
    if (lexemeStack.empty() == false)
    {
        myWasError = true;
        myErrorLine = lexeme.myLine;
        myErrorColumn = lexeme.myColumn;
        myErrorExtraLine = lexeme.myLine;
        myErrorExtraColumn = lexeme.myColumn;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает закрывающего дескриптора группы>";
        return;
    }
}




//=====================================================================================================================
// Доступ к результатам разбора
const CDataMap & CTextParser::getData() const
{
    return myData;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к результатам разбора
const string & CTextParser::getScanResult() const
{
    return myScanResult;
}




//=====================================================================================================================
// Доступ к информации об ошибках анализа
bool CTextParser::wasError() const
{
    return myWasError;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
size_t CTextParser::getErrorLine() const
{
    return myErrorLine;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
size_t CTextParser::getErrorColumn() const
{
    return myErrorColumn;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
size_t CTextParser::getErrorExtraLine() const
{
    return myErrorExtraLine;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
size_t CTextParser::getErrorExtraColumn() const
{
    return myErrorExtraColumn;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
const string & CTextParser::getErrorMessage() const
{
    return myErrorMessage;
}




//=====================================================================================================================
// Добавление данных в структуру текста
void CTextParser::_collect(CTextLexeme::type t)
{
    switch (t)
    {
    case CTextLexeme::IDENTIFIER:
        myScanResult +=  "i";
        break;

    case CTextLexeme::GROUP_BRACKET:
        myScanResult +=  "G";
        break;

    case CTextLexeme::TABLE_BRACKET:
        myScanResult +=  "T";
        break;

    case CTextLexeme::CLOSE_BRACKET:
        myScanResult +=  "E";
        break;

    case CTextLexeme::STRING_VALUE:
        myScanResult +=  "s";
        break;

    case CTextLexeme::NUMBER_VALUE:
        myScanResult +=  "n";
        break;

    case CTextLexeme::ASSIGNMENT:
        myScanResult +=  "=";
        break;

    case CTextLexeme::END_OF_LINE:
        myScanResult +=  "#";
        break;

    case CTextLexeme::END_OF_TEXT:
        myScanResult +=  "$";
        break;

    case CTextLexeme::UNKNOWN:
        myScanResult +=  "U";
        break;
    }
}




//=====================================================================================================================
// Конструирование описания позиции с ошибкой "[строка, столбец]: "
string CTextParser::_errorPos()
{
    ostringstream out;
    out << "[" << myErrorLine << ", " << myErrorColumn << "]: ";
    return out.str();
}




//=====================================================================================================================
// Конструирование описания вспомогательной позиции "[строка, столбец]"
string CTextParser::_extraPos()
{
    ostringstream out;
    out << "[" << myErrorExtraLine << ", " << myErrorExtraColumn << "]";
    return out.str();
}




//=====================================================================================================================
// Конструирование полного имени элемента по стеку
string CTextParser::_construct(const list<CTextLexeme> &lexemeStack)
{
    string result;
    list<CTextLexeme>::const_iterator itr;
    for(itr = lexemeStack.begin(); itr != lexemeStack.end(); ++itr)
        result += string("/") + itr->myValue;
    return result;
}

