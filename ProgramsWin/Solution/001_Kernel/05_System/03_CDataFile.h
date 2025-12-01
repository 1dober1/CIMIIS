// Файл 03_CDataFile.h
// Двоичный файл на диске
//

#ifndef Kernel_System_CDataFile_h
#define Kernel_System_CDataFile_h


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
#include "../02_BasicTypes/04_CByteArray.h"


//=====================================================================================================================
// Двоичный файл на диске
class CDataFile
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    string myName;          // Имя файла
    FILE *myHandle;         // Дескриптор файла
    bool myInReadMode;      // Признак режима чтения файла


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Запрет копирования и присваивания файлов
    CDataFile(const CDataFile &);
    CDataFile & operator =(const CDataFile &);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CDataFile();

    // Деструктор класса
    ~CDataFile();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к информации о файле
    const string & getName() const;
    bool inReadMode() const;
    bool inWriteMode() const;
    bool isOpen() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Работа с файлом на чтение
    bool openForReading(const string &fileName);
    CByteArray read(uint32 size);
    CByteArray readAll();
    void close();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Работа с файлом на запись
    bool openForWriting(const string &fileName);
    void write(const char *data, uint32 size);
    void write(const CByteArray &byteArray);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Чтение всего файла целиком
    static bool readFile(const string &fileName, CByteArray &byteArray);

    // Запись всего файла целиком
    static bool writeFile(const string &fileName, const char *data, uint32 size);
    static bool writeFile(const string &fileName, const CByteArray &byteArray);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif

