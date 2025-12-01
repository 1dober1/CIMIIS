// Файл 01_FileName.cpp
// Обработка имён файлов
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_FileName.h"




//=====================================================================================================================
// Извлечение имени непосредственно файла из полного имени файла
string extractFileName(const string &fullFileName)
{
    // Поиск последнего разделителя в пути
    size_t index = fullFileName.find_last_of("\\/");

    // Если разделитель не найден, то вся строка - имя файла
    if (index == string::npos)
        return fullFileName;

    // Если разделитель найден, то взять последнюю часть строки (не включая разделитель)
    else
    {
        if (index == fullFileName.size()-1)
            return string();
        else
            return fullFileName.substr(index + 1);
    }
}




//=====================================================================================================================
// Извлечение пути к файлу из полного имени файла
string extractFilePath(const string &fullFileName)
{
    // Поиск последнего разделителя в пути
    size_t index = fullFileName.find_last_of("\\/");

    // Если разделитель не найден, то в строке нет пути
    if (index == string::npos)
        return string();

    // Если разделитель найден, то взять первую часть строки (включая разделитель)
    else
        return fullFileName.substr(0, index+1);
}




//=====================================================================================================================
// Извлечение чистого имени файла (без расширения) из полного имени файла
string extractPureFileName(const string &fullFileName)
{
    // Извлечение имени файла
    string fileName = extractFileName(fullFileName);

    // Поиск последнего разделителя имени и расширения
    size_t index = fileName.rfind('.');

    // Если разделитель не найден, то всё имя в строке чистое
    if (index == string::npos)
        return fileName;

    // Если разделитель найден, то взять первую часть строки (не включая разделитель)
    else
        return fileName.substr(0, index);
}




//=====================================================================================================================
// Извлечение расширения имени файла из полного имени файла
string extractFileNameExtension(const string &fullFileName)
{
    // Извлечение имени файла
    string fileName = extractFileName(fullFileName);

    // Поиск последнего разделителя имени и расширения
    size_t index = fileName.rfind('.');

    // Если разделитель не найден, то в строке нет расширения имени
    if (index == string::npos)
        return string();

    // Если разделитель найден, то взять последнюю часть строки (включая разделитель)
    else
        return fileName.substr(index);
}




//=====================================================================================================================
// Нормализация разделителей имён каталогов в пути
string normalizePathSeparators(const string &fullFileName)
{
    string result = fullFileName;
    size_t pos = 0;
    while (true)
    {
        pos = result.find("\\", pos);
        if (pos == string::npos)
            break;

        result[pos] = '/';
        ++pos;

        if (pos == result.size())
            break;
    }

    return result;
}


