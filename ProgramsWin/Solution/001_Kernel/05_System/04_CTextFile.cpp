// Файл 04_CTextFile.cpp
// Текстовый файл на диске
//

//=====================================================================================================================
// Заголовочные файлы
#include "04_CTextFile.h"
#include "../03_Encoding/UTF8.h"
#include "../04_String/01_FileName.h"




//=====================================================================================================================
// Конструктор класса
CTextFile::CTextFile()
{
    myInReadMode = true;
    myIsOpen = false;
}




//=====================================================================================================================
// Деструктор класса
CTextFile::~CTextFile()
{
    close();
}




//=====================================================================================================================
// Доступ к имени файла
const string & CTextFile::getName() const
{
    return myName;
}




//=====================================================================================================================
// Доступ к режиму работы с файлом
bool CTextFile::inReadMode() const
{
    return myInReadMode;
}




//=====================================================================================================================
// Доступ к режиму работы с файлом
bool CTextFile::inWriteMode() const
{
    return !myInReadMode;
}




//=====================================================================================================================
// Доступ к готовности работы с файлом
bool CTextFile::isOpen() const
{
    return myIsOpen;
}




//=====================================================================================================================
// Работа с файлом на чтение
bool CTextFile::openForReading(const string &fileName)
{
    // Обработка имени файла (в Windows - ничего, в Linux - перекодирование)
#ifdef SYSTEM_WINDOWS
    const string &name = fileName;
#else
    string name = toUTF8(fileName);
#endif
    myName = fileName;


    // Открытие файла
    myHandle.open(name.c_str(), ios::in);
    if (!myHandle)
        myIsOpen = false;
    else
        myIsOpen = true;
    myInReadMode = true;
    return myIsOpen;
}




//=====================================================================================================================
// Чтение очередной строки из файла
string CTextFile::readLine()
{
    // Если файл не готов к работе
    if (myIsOpen == false)
        return string();

    // Если файл открыт не в режиме чтения
    if (myInReadMode == false)
        return string();

    // Чтение одной строки
    string buffer;
    getline(myHandle, buffer);
    return buffer;
}




//=====================================================================================================================
// Чтение всего файла целиком
string CTextFile::readAll()
{
    // Если файл не готов к работе
    if (myIsOpen == false)
        return string();

    // Если файл открыт не в режиме чтения
    if (myInReadMode == false)
        return string();

    // Чтение всего файла до конца
    string buffer;
    string result;
    getline(myHandle, result);
    while (getline(myHandle, buffer))
        result += (string("\n") + buffer);
    return result;
}




//=====================================================================================================================
// Закрытие файла
void CTextFile::close()
{
    if (myIsOpen == true)
    {
        myHandle.close();
        myIsOpen = false;
    }
}




//=====================================================================================================================
// Работа с файлом на запись
bool CTextFile::openForWriting(const string & fileName)
{
    // Обработка имени файла (в Windows - ничего, в Linux - перекодирование)
#ifdef SYSTEM_WINDOWS
    const string &name = fileName;
#else
    string name = toUTF8(fileName);
#endif
    myName = fileName;

    // Создание пути к файлу
    string path = extractFilePath(name);
    if (path.empty() == false)
    {
        // Формирование списка создаваемых каталогов
        deque<string> pathList;
        while (path.empty() == false)
        {
            if (path[path.size() - 1] == '/')
                path.erase(path.size() - 1);
            pathList.push_front(path);
            path = extractFilePath(path);
        }

        for (size_t i = 0; i < pathList.size(); i++)
        {
#ifdef SYSTEM_WINDOWS
            _mkdir(pathList[i].c_str());
#else
            mkdir(pathList[i].c_str(), 0733);
#endif
        }
    }

    // Открытие файла
    myHandle.open(name.c_str(), ios::out);
    if (!myHandle)
        myIsOpen = false;
    else
        myIsOpen = true;
    myInReadMode = false;
    return myIsOpen;
}




//=====================================================================================================================
// Запись строки в файл с признаком новой строки
void CTextFile::writeLine(const string & line)
{
    // Если файл не готов к работе
    if (myIsOpen == false)
        return;

    // Если файл открыт не в режиме записи
    if (myInReadMode == true)
        return;

    // Запись строки
    myHandle << line << endl;
}




//=====================================================================================================================
// Запись строки в файл без признака новой строки
void CTextFile::write(const string & text)
{
    // Если файл не готов к работе
    if (myIsOpen == false)
        return;

    // Если файл открыт не в режиме записи
    if (myInReadMode == true)
        return;

    // Запись текста
    myHandle << text;
}




//=====================================================================================================================
// Чтение всего файла целиком
bool CTextFile::readFile(const string & fileName, string & text)
{
    // Обработка имени файла (в Windows - ничего, в Linux - перекодирование)
#ifdef SYSTEM_WINDOWS
    const string &name = fileName;
#else
    string name = toUTF8(fileName);
#endif


    // Открытие файла
    ifstream file;
    file.open(name.c_str());
    if (!file)
        return false;

    // Чтение файла целиком
    string buffer;
    text.clear();
    getline(file, text);
    while (getline(file, buffer))
        text += (string("\n") + buffer);

    // Закрытие файла
    file.close();
    return true;
}




//=====================================================================================================================
// Запись всего файла целиком
bool CTextFile::writeFile(const string & fileName, const string & text)
{
    // Обработка имени файла (в Windows - ничего, в Linux - перекодирование)
#ifdef SYSTEM_WINDOWS
    const string &name = fileName;
#else
    string name = toUTF8(fileName);
#endif


    // Открытие файла
    ofstream file;
    file.open(name.c_str());
    if (!file)
        return false;

    // Запись файла целиком
    file << text;

    // Закрытие файла
    file.close();
    return true;
}



