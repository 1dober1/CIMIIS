// Файл 02_Formatting.cpp
// Форматный вывод в строку
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_Formatting.h"




//=====================================================================================================================
// Буфер для преобразования числовых значений в строки
static char fixedBuffer[512];
static char *formatBuffer = fixedBuffer;
static size_t formatBufferSize = 512;




//=====================================================================================================================
// Подготовка буфера для преобразования числовых значений
static void _prepareBuffer(size_t size)
{
    // Если буфер достаточного размера, то ничего не делать
    if (formatBufferSize >= size)
        return;

    // Если буфер динамический, то удалить его
    if (formatBuffer != fixedBuffer)
        free(formatBuffer);

    // Выделить новый буфер
    formatBuffer = (char *)malloc(size);
    formatBufferSize = size;
}




//=====================================================================================================================
// Форматирование строкового значения
string format(const string &value, int32 width, stringAlignType alignType)
{
    // Замена всех спецсимволов пробелами
    string result = value;
    for (size_t i = 0; i < result.size(); ++i)
    {
        if (result[i] >= '\0'   &&   result[i] < ' ')
            result[i] = ' ';
    }


    // Если ширина не задана, то выдать без изменений
    if (width == 0)
        return result;

    // Обрезать строку
    if (result.size() > (size_t)width)
        result.erase((size_t)width);

    // Создать строку с пробелами
    size_t spaceCount = (size_t)width - result.size();
    string spaceString = string(spaceCount, ' ');

    // Прижать текст к нужному краю
    if (alignType == alignLeft)
        result += spaceString;
    else
        result = spaceString + result;

    // Выдать результат
    return result;
}




//=====================================================================================================================
// Форматирование целочисленного значения
string format(int32 value, int32 width, stringFillType fillType)
{
    // Если ширина не задана, то просто преобразовать в строку
    if (width == 0)
    {
        ostringstream out;
        out << value;
        return out.str();
    }


    // Преобразование числа в строку
    _prepareBuffer(width + 256);
    if (fillType == fillZero)
        sprintf(formatBuffer, "%0*d", width, value);
    else
        sprintf(formatBuffer, "%*d", width, value);
    string result(formatBuffer);

    // Обрезание строки
    if (result.size() > width)
        result.erase(width);

    // Выдача результата
    return result;
}


//---------------------------------------------------------------------------------------------------------------------
string format(uint32 value, int32 width, stringFillType fillType)
{
    // Если ширина не задана, то просто преобразовать в строку
    if (width == 0)
    {
        ostringstream out;
        out << value;
        return out.str();
    }


    // Преобразование числа в строку
    _prepareBuffer(width + 256);
    if (fillType == fillZero)
        sprintf(formatBuffer, "%0*u", width, value);
    else
        sprintf(formatBuffer, "%*u", width, value);
    string result(formatBuffer);

    // Обрезание строки
    if (result.size() > width)
        result.erase(width);

    // Выдача результата
    return result;
}




//=====================================================================================================================
// Форматирование действительного значения
string format(double value, int32 width, int32 precision)
{
    // Если ширина и точность не заданы, то просто преобразовать в строку
    if (width == 0   &&   precision == 0)
    {
        ostringstream out;
        out << value;
        return out.str();
    }


    // Преобразование числа в строку
    _prepareBuffer(width + precision + 256);
    sprintf(formatBuffer, "%*.*lf", width, precision, value);
    string result(formatBuffer);

    // Обрезание строки
    if (width > 0   &&   result.size() > width)
        result.erase(width);

    // Выдача результата
    return result;
}



