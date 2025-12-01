// Файл 03_CTextScanner.cpp
// Лексический препроцессор текстовых данных
//

//=====================================================================================================================
// Заголовочные файлы
#include "03_CTextScanner.h"




//=====================================================================================================================
// Конструктор класса
CTextScanner::CTextScanner()
{
    myText = NULL;
    myOffset = 0;
    myLineCounter = 0;
    myColumnCounter = 0;
    myWasError = true;
    myErrorLine = 0;
    myErrorColumn = 0;
    myErrorExtraColumn = 0;
    myErrorMessage = "Входной текст не задан!";
}




//=====================================================================================================================
// Инициализация сканера строкой текста
void CTextScanner::init(const string &text)
{
    // Если задана пустая строка
    if (text.empty() == true)
    {
        myText = NULL;
        myOffset = 0;
        myLineCounter = 0;
        myColumnCounter = 0;
        myWasError = true;
        myErrorLine = 0;
        myErrorColumn = 0;
        myErrorExtraColumn = 0;
        myErrorMessage = "Входной текст не задан!";
        return;
    }

    // Если всё хорошо
    myText = &text;
    myOffset = 0;
    myLineCounter = 1;
    myColumnCounter = 1;
    myWasError = false;
    myErrorLine = 0;
    myErrorColumn = 0;
    myErrorExtraColumn = 0;
    myErrorMessage.clear();
}




//=====================================================================================================================
// Чтение очередной лексемы из текста
CTextLexeme CTextScanner::read()
{
    // Если входной текст не задан
    if (myText == NULL)
    {
        // Установка признака ошибки
        myWasError = true;
        myErrorLine = 0;
        myErrorColumn = 0;
        myErrorExtraColumn = 0;
        myErrorMessage = "Входной текст не задан!";

        // Генерация лексемы с ошибкой
        return CTextLexeme(CTextLexeme::UNKNOWN, string(), 0, 0);
    }

    // Посимвольный просмотр входного текста до нахождения чего-нибудь полезного
    while (myOffset < myText->size())
    {
        // Обработка пустых строк
        char sym = (*myText)[myOffset];
        if (sym == '\n')
            return _readNewLine();

        // Пропуск незначащих пробелов
        if (_isSpace(sym) == true)
        {
            myOffset++;
            myColumnCounter++;
            continue;
        }

        // Обработка комментария
        if (sym == '/')
        {
            if (_readComment() == false)
                return CTextLexeme(CTextLexeme::UNKNOWN, string(), myLineCounter, myColumnCounter);
            continue;
        }

        // Обработка идентификатора
        if(_isLetter(sym) == true   ||   sym == '.'   ||   sym == '_')
            return _readIdentifier();

        // Обработка операторной скобки
        if (sym == '[')
            return _readBracket();

        // Обработка числового значения
        if(_isDigit(sym) == true   ||   sym == '+'   ||   sym == '-'   ||   sym == '.')
            return _readNumber();

        // Обработка строкового значения
        if (sym == '\"')
            return _readString();

        // Обработка оператора присваивания
        if (sym == '=')
            return _readAssignment();

        // Обработка неизвестного символа
        myWasError = true;
        myErrorLine = myLineCounter;
        myErrorColumn = myColumnCounter;
        myErrorExtraColumn = myColumnCounter;
        myErrorMessage = _errorPos() + "недопустимый символ! <ожидается буква, цифра, двойная кавычка, символы \'[\' или \'=\'>";
        return CTextLexeme(CTextLexeme::UNKNOWN, string(1, sym), myLineCounter, myColumnCounter);
    }

    // Ничего не найдено - текст закончился
    return CTextLexeme(CTextLexeme::END_OF_TEXT, string(), myLineCounter, myColumnCounter);
}




//=====================================================================================================================
// Доступ к информации об ошибках анализа
bool CTextScanner::wasError() const
{
    return myWasError;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
size_t CTextScanner::getErrorLine() const
{
    return myErrorLine;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
size_t CTextScanner::getErrorColumn() const
{
    return myErrorColumn;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
size_t CTextScanner::getErrorExtraColumn() const
{
    return myErrorExtraColumn;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации об ошибках анализа
const string & CTextScanner::getErrorMessage() const
{
    return myErrorMessage;
}




//=====================================================================================================================
// Чтение комментария
bool CTextScanner::_readComment()
{
    int state = 1;
    size_t startPos = myOffset;
    while (myOffset < myText->size())
    {
        // Чтение очередного символа
        char sym = (*myText)[myOffset];

        // Обработка состояния
        switch (state)
        {
        case 1:
            // Прочитали первый слеш комментария (символ проверен ранее, перед вызовом функции)
            state = 2;
            myOffset++;
            myColumnCounter++;
            continue;

        case 2:
            // Прочитали второй слеш комментария (комментарий подтверждён)
            if (sym == '/')
            {
                state = 3;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает второго слеша>";
                return false;
            }

            // Ошибка! Нет второго слеша (комментарий задан некорректно)
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "некорректный комментарий! <пропущен второй слеш>";
            return false;


        case 3:
            // Прочитали символ новой строки (обнаружен конец комментария)
            if (sym == '\n')
                return true;

            // Пропускаем любые символы в комментарии
            myOffset++;
            myColumnCounter++;
            continue;
        }
    }

    // Если текст оборвался до прочтения двух слешей
    if (state != 3)
    {
        myWasError = true;
        myErrorLine = myLineCounter;
        myErrorColumn = myColumnCounter;
        myErrorExtraColumn = startPos;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает второго слеша>";
        return false;
    }

    // Файл прочитан, комментарий обработан
    return true;
}




//=====================================================================================================================
// Чтение идентификатора
CTextLexeme CTextScanner::_readIdentifier()
{
    int state = 1;
    string buffer;
    size_t startPos = myColumnCounter;
    while (myOffset < myText->size())
    {
        // Чтение очередного символа
        char sym = (*myText)[myOffset];

        // Обработка состояния
        switch (state)
        {
        case 1:
            // Прочитали первый символ идентификатора (символ проверен ранее, перед вызовом функции)
            state = 2;
            buffer += sym;
            myOffset++;
            myColumnCounter++;
            continue;

        case 2:
            // Читаем остальные символы идентификатора
            if (_isLetter(sym) == true   ||   _isDigit(sym) == true   ||   sym == '.'   ||   sym == '_')
            {
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Обнаружили конец идентификатора
            if (_isSpace(sym) == true   ||   sym == '\n'   ||   sym == '=')
                return CTextLexeme(CTextLexeme::IDENTIFIER, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружили недопустимый символ в идентификаторе
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в идентификаторе! <ожидается буква, цифра, точка или символ подчёркивания>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
        }
    }

    // Файл прочитан, идентификатор обработан
    return CTextLexeme(CTextLexeme::IDENTIFIER, buffer, myLineCounter, startPos);
}




//=====================================================================================================================
// Чтение операторной скобки (начало таблицы, начало группы, конец группы или таблицы)
CTextLexeme CTextScanner::_readBracket()
{
    int state = 1;
    string buffer;
    size_t startPos = myColumnCounter;
    while (myOffset < myText->size())
    {
        // Чтение очередного символа
        char sym = (*myText)[myOffset];

        // Обработка состояния
        switch (state)
        {
        case 1:
            // Прочитали левую квадратную скобку (символ проверен ранее, перед вызовом функции)
            state = 2;
            myOffset++;
            myColumnCounter++;
            continue;


        case 2:
            // Прочитали первый символ имени группы или таблицы
            if (_isLetter(sym) == true   ||   _isDigit(sym) == true   ||   sym == '.'   ||   sym == '_')
            {
                state = 3;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали признак конца группы или таблицы
            if (sym == '/')
            {
                state = 7;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает имени группы или таблицы>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в идентификаторе
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в имени группы или таблицы! <ожидается буква, цифра, точка или символ подчёркивания>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 3:
            // Читаем остальные символы имени группы или таблицы
            if (_isLetter(sym) == true   ||   _isDigit(sym) == true   ||   sym == '.'   ||   sym == '_')
            {
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали признак группы
            if (sym == ']')
            {
                state = 4;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали признак таблицы
            if (sym == '=')
            {
                state = 5;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает символа \'=\' или \']\'>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в идентификаторе
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в имени группы или таблицы! <ожидается буква, цифра, точка или символ подчёркивания>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 4:
            // Прочитали пробел после начала группы
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::GROUP_BRACKET, buffer, myLineCounter, startPos);
            }

            // Прочитали новую строку после начала группы
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::GROUP_BRACKET, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ после начала группы
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ! <ожидается пробел или новая строка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 5:
            // Прочитали последний символ начала таблицы
            if (sym == ']')
            {
                state = 6;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает символа ']'>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ после знака '='
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ! <ожидается символ ']'>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 6:
            // Прочитали пробел после начала таблицы
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::TABLE_BRACKET, buffer, myLineCounter, startPos);
            }

            // Прочитали новую строку после начала таблицы
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::TABLE_BRACKET, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ после начала таблицы
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ! <ожидается пробел или новая строка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 7:
            // Прочитали первый символ имени группы или таблицы
            if (_isLetter(sym) == true   ||   _isDigit(sym) == true   ||   sym == '.'   ||   sym == '_')
            {
                state = 8;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает имени группы или таблицы>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в идентификаторе
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в имени группы или таблицы! <ожидается буква, цифра, точка или символ подчёркивания>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 8:
            // Читаем остальные символы имени группы или таблицы
            if (_isLetter(sym) == true   ||   _isDigit(sym) == true   ||   sym == '.'   ||   sym == '_')
            {
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали признак группы
            if (sym == ']')
            {
                state = 9;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает символа ']'>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в идентификаторе
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в имени группы или таблицы! <ожидается буква, цифра, точка или символ подчёркивания>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 9:
            // Прочитали пробел после конца таблицы или группы
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::CLOSE_BRACKET, buffer, myLineCounter, startPos);
            }

            // Прочитали новую строку после конца таблицы или группы
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::CLOSE_BRACKET, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ после конца таблицы или группы
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ! <ожидается пробел или новая строка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
        }
    }

    // Файл прочитан
    switch (state)
    {
    case 4:
        // Прочитали начало группы
        return CTextLexeme(CTextLexeme::GROUP_BRACKET, buffer, myLineCounter, startPos);

    case 6:
        // Прочитали начало таблицы
        return CTextLexeme(CTextLexeme::TABLE_BRACKET, buffer, myLineCounter, startPos);

    case 9:
        // Прочитали конец группы или таблицы
        return CTextLexeme(CTextLexeme::CLOSE_BRACKET, buffer, myLineCounter, startPos);

    default:
        // Обрыв произошёл где-то внутри
        myWasError = true;
        myErrorLine = myLineCounter;
        myErrorColumn = myColumnCounter;
        myErrorExtraColumn = startPos;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает новой строки>";
        return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
    }
}




//=====================================================================================================================
// Чтение числового значения
CTextLexeme CTextScanner::_readNumber()
{
    int state = 1;
    string buffer;
    size_t startPos = myColumnCounter;
    while (myOffset < myText->size())
    {
        // Чтение очередного символа
        char sym = (*myText)[myOffset];

        // Обработка состояния
        switch (state)
        {
        case 1:
            // Прочитали первую цифру в целой части числа
            if (_isDigit(sym) == true)
            {
                state = 2;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали знак числа
            if (sym == '+'   ||   sym == '-')
            {
                state = 3;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали десятичную точку (целая часть отсутствует),
            // Такую фигню нам любит подкладывать Илья, идём навстречу
            if (sym == '.')
            {
                state = 9;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Иного варианта быть не может - первый символ был проверен ранее


        case 2:
            // Читаем остальные цифры в целой части числа
            if (_isDigit(sym) == true)
            {
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали десятичную точку (целая часть закончилась)
            if (sym == '.')
            {
                state = 4;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Пробельный символ - признак окончания числа
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);
            }

            // Новая строка - признак окончания числа
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ в целой части числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра или точка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 3:
            // Прочитали первую цифру числа после знака
            if (_isDigit(sym) == true)
            {
                state = 2;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали десятичную точку (целая часть отсутствует)
            // Такую фигню нам любит подкладывать Илья, идём навстречу
            if (sym == '.')
            {
                state = 9;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает цифры или точки>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в целой части числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра или точка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 4:
            // Читаем цифры дробной части числа
            if (_isDigit(sym) == true)
            {
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали признак экспоненты
            if (sym == 'e'   ||   sym == 'E')
            {
                state = 5;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Пробельный символ - признак окончания числа
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);
            }

            // Новая строка - признак окончания числа
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ в дробной части числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра или символ экспоненты>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 5:
            // Прочитали первую цифру из целой части экспоненты числа
            if (_isDigit(sym) == true)
            {
                state = 6;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали знак экспоненты числа
            if (sym == '+'   ||   sym == '-')
            {
                state = 7;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали десятичную точку в экспоненте числа (целая часть отсутствует)
            // Такую фигню нам любит подкладывать Илья, идём навстречу
            if (sym == '.')
            {
                state = 10;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает цифры, знака числа или точки>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в целой части экспоненты числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра, знак числа или точка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 6:
            // Читаем остальные цифры в целой части числа
            if (_isDigit(sym) == true)
            {
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали десятичную точку (целая часть экспоненты закончилась)
            if (sym == '.')
            {
                state = 8;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Пробельный символ - признак окончания числа
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);
            }

            // Новая строка - признак окончания числа
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ в целой части экспоненты числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра или точка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 7:
            // Прочитали первую цифру экспоненты числа после её знака
            if (_isDigit(sym) == true)
            {
                state = 6;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали десятичную точку (целая часть отсутствует)
            // Такую фигню нам любит подкладывать Илья, идём навстречу
            if (sym == '.')
            {
                state = 10;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает цифры или точки>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в целой части экспоненты числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра или точка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 8:
            // Читаем цифры дробной части экспоненты числа
            if (_isDigit(sym) == true)
            {
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }
            
            // Пробельный символ - признак окончания числа
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);
            }

            // Новая строка - признак окончания числа
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ в дробной части экспоненты числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра или точка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 9:
            // Прочитали первую цифру дробной части числа
            if (_isDigit(sym) == true)
            {
                state = 4;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает цифры>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в дробной части числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 10:
            // Прочитали первую цифру дробной части экспоненты числа
            if (_isDigit(sym) == true)
            {
                state = 8;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает цифры>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ в дробной части числа
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в числовом значении! <ожидается цифра>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
        }
    }

    // Файл прочитан
    switch (state)
    {
    case 2:
    case 4:
    case 6:
    case 8:
        // Прочитали числовое значение
        return CTextLexeme(CTextLexeme::NUMBER_VALUE, buffer, myLineCounter, startPos);

    default:
        // Обрыв произошёл где-то внутри
        myWasError = true;
        myErrorLine = myLineCounter;
        myErrorColumn = myColumnCounter;
        myErrorExtraColumn = startPos;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает новой строки>";
        return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
    }
}




//=====================================================================================================================
// Чтение строкового значения
CTextLexeme CTextScanner::_readString()
{
    int state = 1;
    string buffer;
    size_t startPos = myColumnCounter;
    while (myOffset < myText->size())
    {
        // Чтение очередного символа
        char sym = (*myText)[myOffset];

        // Обработка состояния
        switch (state)
        {
        case 1:
            // Прочитали открывающую двойную кавычку (проверка символа выполнена ранее)
            state = 2;
            myOffset++;
            myColumnCounter++;
            continue;


        case 2:
            // Прочитали закрывающую двойную кавычку
            if (sym == '\"')
            {
                state = 3;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали экранирующий символ
            if (sym == '\\')
            {
                state = 4;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает закрывающей двойной кавычки>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Читаем все символы в строке
            buffer += sym;
            myOffset++;
            myColumnCounter++;
            continue;


        case 3:
            // Пробельный символ - признак окончания строки
            if (_isSpace(sym) == true)
            {
                myOffset++;
                myColumnCounter++;
                return CTextLexeme(CTextLexeme::STRING_VALUE, buffer, myLineCounter, startPos);
            }

            // Новая строка - признак окончания строки
            if (sym == '\n')
                return CTextLexeme(CTextLexeme::STRING_VALUE, buffer, myLineCounter, startPos);

            // Ошибка! Обнаружен недопустимый символ после строки
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ после закрывающей двойной кавычки! <ожидается пробел или новая строка>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);


        case 4:
            // Прочитали заэкранированную кавычку
            if (sym == '\"')
            {
                state = 2;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Прочитали заэкранированный слеш
            if (sym == '\\')
            {
                state = 2;
                buffer += sym;
                myOffset++;
                myColumnCounter++;
                continue;
            }

            // Ошибка! Обрыв строки!
            if (sym == '\n')
            {
                myWasError = true;
                myErrorLine = myLineCounter;
                myErrorColumn = myColumnCounter;
                myErrorExtraColumn = startPos;
                myErrorMessage = _errorPos() + "неожиданный обрыв строки! <не хватает экранируемого символа>";
                return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
            }

            // Ошибка! Обнаружен недопустимый символ при экранировании
            myWasError = true;
            myErrorLine = myLineCounter;
            myErrorColumn = myColumnCounter;
            myErrorExtraColumn = startPos;
            myErrorMessage = _errorPos() + "недопустимый символ в строковом значении! <экранировать можно только двойную кавычку или слеш>";
            return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
        }
    }

    // Файл прочитан, но в середине произошёл обрыв текста
    if (state != 3)
    {
        myWasError = true;
        myErrorLine = myLineCounter;
        myErrorColumn = myColumnCounter;
        myErrorExtraColumn = startPos;
        myErrorMessage = _errorPos() + "неожиданный конец текста! <не хватает новой строки>";
        return CTextLexeme(CTextLexeme::UNKNOWN, buffer, myLineCounter, startPos);
    }

    // Файл прочитан, строка обработана
    return CTextLexeme(CTextLexeme::STRING_VALUE, buffer, myLineCounter - 1, startPos);
}




//=====================================================================================================================
// Чтение оператора присваивания
CTextLexeme CTextScanner::_readAssignment()
{
    myOffset++;
    myColumnCounter++;
    return CTextLexeme(CTextLexeme::ASSIGNMENT, "=", myLineCounter, myColumnCounter - 1);
}




//=====================================================================================================================
// Чтение новой строки в таблице
CTextLexeme CTextScanner::_readNewLine()
{
    myOffset++;
    size_t colNum = myColumnCounter;
    myLineCounter++;
    myColumnCounter = 1;
    return CTextLexeme(CTextLexeme::END_OF_LINE, "\n", myLineCounter - 1, colNum);
}




//=====================================================================================================================
// Идентификация буквы
bool CTextScanner::_isLetter(char sym)
{
    // Большие латинские буквы
    if (sym >= 'A'   &&   sym <= 'Z')
        return true;

    // Маленькие латинские буквы
    if (sym >= 'a'   &&   sym <= 'z')
        return true;

    // Большие русские буквы
    if (sym >= 'А'   &&   sym <= 'Я')
        return true;
    if (sym == 'Ё')
        return true;

    // Маленькие русские буквы
    if (sym >= 'а'   &&   sym <= 'я')
        return true;
    if (sym == 'ё')
        return true;

    // Всё остальное
    return false;
}




//=====================================================================================================================
// Идентификация цифры
bool CTextScanner::_isDigit(char sym)
{
    if (sym >= '0'   &&   sym <= '9')
        return true;
    return false;
}




//=====================================================================================================================
// Идентификация пробела
bool CTextScanner::_isSpace(char sym)
{
    if (sym == ' '   ||   sym == '\t'   ||   sym == '\r')
        return true;
    return false;
}




//=====================================================================================================================
// Конструирование описания позиции с ошибкой "[строка, столбец/столбец]: "
string CTextScanner::_errorPos()
{
    ostringstream out;
    out << "[" << myErrorLine << ", " << myErrorColumn << "/" << myErrorExtraColumn << "]: ";
    return out.str();
}

