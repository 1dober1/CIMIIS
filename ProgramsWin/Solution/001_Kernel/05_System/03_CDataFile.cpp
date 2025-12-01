// Файл 03_CDataFile.cpp
// Двоичный файл на диске
//

//=====================================================================================================================
// Заголовочные файлы
#include "03_CDataFile.h"
#include "../03_Encoding/UTF8.h"
#include "../04_String/01_FileName.h"




//=====================================================================================================================
// Конструктор класса
CDataFile::CDataFile()
{
    myHandle = NULL;
    myInReadMode = true;
}




//=====================================================================================================================
// Деструктор класса
CDataFile::~CDataFile()
{
    close();
}




//=====================================================================================================================
// Доступ к имени файла
const string & CDataFile::getName() const
{
    return myName;
}




//=====================================================================================================================
// Доступ к режиму работы с файлом
bool CDataFile::inReadMode() const
{
    return myInReadMode;
}



//=====================================================================================================================
// Доступ к режиму работы с файлом
bool CDataFile::inWriteMode() const
{
    return !myInReadMode;
}




//=====================================================================================================================
// Доступ к готовности работы с файлом
bool CDataFile::isOpen() const
{
    if (myHandle != NULL)
        return true;
    return false;
}




//=====================================================================================================================
// Работа с файлом на чтение
bool CDataFile::openForReading(const string &fileName)
{
    // Обработка имени файла (в Windows - ничего, в Linux - перекодирование)
#ifdef SYSTEM_WINDOWS
    const string &name = fileName;
#else
    string name = toUTF8(fileName);
#endif
    myName = fileName;


    // Открытие файла
    myHandle = fopen(name.c_str(), "rb");
    myInReadMode = true;
    return isOpen();
}




//=====================================================================================================================
// Чтение последовательности байт из файла
CByteArray CDataFile::read(uint32 size)
{
    // Если файл не готов к работе
    if (myHandle == NULL)
        return CByteArray();

    // Если файл открыт не в режиме чтения
    if (myInReadMode == false)
        return CByteArray();

    // Подготовка буфера для приёма данных из файла
    CByteArray result;
    if (result.alloc(size) == false)
        return CByteArray();

    // Чтение указанного числа байтов из файла
    size_t bytesRead = fread(result.getData(), 1, size, myHandle);
    if (bytesRead == 0)
    {
        result.free();
        return CByteArray();
    }

    // Выдача результата
    return CByteArray(result.getData(), (uint32)bytesRead);
}




//=====================================================================================================================
// Чтение всего файла целиком
CByteArray CDataFile::readAll()
{
    // Если файл не готов к работе
    if (myHandle == NULL)
        return CByteArray();

    // Если файл открыт не в режиме чтения
    if (myInReadMode == false)
        return CByteArray();

    // Определение размера файла
    long oldPos = ftell(myHandle);
    uint32 startFrom = (uint32)oldPos;
    fseek(myHandle, 0L, SEEK_END);
    uint32 size = (uint32)ftell(myHandle) - startFrom;
    fseek(myHandle, (long)oldPos, SEEK_SET);

    // Чтение данных из файла
    return read(size);
}


//=====================================================================================================================
// Закрытие файла
void CDataFile::close()
{
    if (myHandle != NULL)
        fclose(myHandle);
    myHandle = NULL;
}




//=====================================================================================================================
// Работа с файлом на запись
bool CDataFile::openForWriting(const string & fileName)
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
    myHandle = fopen(name.c_str(), "wb");
    myInReadMode = false;
    return isOpen();
}




//=====================================================================================================================
// Запись в файл последовательности байтов
void CDataFile::write(const char *data, uint32 size)
{
    // Если файл не готов к работе
    if (myHandle == NULL)
        return;

    // Если файл открыт не в режиме записи
    if (myInReadMode == true)
        return;

    // Если данные указаны некорректно
    if (data == NULL   ||   size == 0)
        return;

    // Запись данных
    fwrite(data, 1, size, myHandle);
}




//=====================================================================================================================
// Запись в файл последовательности байтов
void CDataFile::write(const CByteArray &byteArray)
{
    // Если файл не готов к работе
    if (myHandle == NULL)
        return;

    // Если файл открыт не в режиме записи
    if (myInReadMode == true)
        return;

    // Если данные указаны некорректно
    if (byteArray.getData() == NULL   ||   byteArray.getSize() == 0)
        return;

    // Запись данных
    fwrite(byteArray.getData(), 1, byteArray.getSize(), myHandle);
}




//=====================================================================================================================
// Чтение всего файла целиком
bool CDataFile::readFile(const string & fileName, CByteArray &byteArray)
{
    // Обработка имени файла (в Windows - ничего, в Linux - перекодирование)
#ifdef SYSTEM_WINDOWS
    const string &name = fileName;
#else
    string name = toUTF8(fileName);
#endif


    // Открытие файла
    FILE *file = fopen(name.c_str(), "rb");
    if (file == NULL)
        return false;

    // Определение размера файла
    fseek(file, 0L, SEEK_END);
    uint32 size = (uint32)ftell(file);
    fseek(file, 0L, SEEK_SET);

    // Если файл пустой
    if (size == 0)
    {
        byteArray.set(NULL, 0);
        fclose(file);
        return true;
    }

    // Подготовка буфера для приёма данных из файла
    CByteArray result;
    if (result.alloc(size) == false)
    {
        fclose(file);
        return false;
    }

    // Чтение файла целиком
    size_t bytesRead = fread(result.getData(), 1, size, file);
    if (bytesRead == 0)
    {
        result.free();
        fclose(file);
        return false;
    }

    // Выдача результата
    byteArray.set(result.getData(), (uint32)bytesRead);
    fclose(file);
    return true;
}




//=====================================================================================================================
// Запись всего файла целиком
bool CDataFile::writeFile(const string & fileName, const char *data, uint32 size)
{
    // Обработка имени файла (в Windows - ничего, в Linux - перекодирование)
#ifdef SYSTEM_WINDOWS
    const string &name = fileName;
#else
    string name = toUTF8(fileName);
#endif


    // Если данные некорректны
    if (data == NULL   ||   size == 0)
        return false;

    // Открытие файла
    FILE *file = fopen(name.c_str(), "wb");
    if (file == NULL)
        return false;

    // Запись файла целиком
    fwrite(data, 1, size, file);

    // Закрытие файла
    fclose(file);
    return true;
}




//=====================================================================================================================
// Запись всего файла целиком
bool CDataFile::writeFile(const string &fileName, const CByteArray &byteArray)
{
    return writeFile(fileName, byteArray.getData(), byteArray.getSize());
}


