// Файл 04_CTextFile.h
// Текстовый файл на диске
//

#ifndef Kernel_System_CTextFile_h
#define Kernel_System_CTextFile_h


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


//=====================================================================================================================
// Текстовый файл на диске
class CTextFile
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    string myName;          // Имя файла
    fstream myHandle;       // Дескриптор файла
    bool myInReadMode;      // Признак режима чтения файла
    bool myIsOpen;          // Признак открытия файла


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Запрет копирования и присваивания файлов
    CTextFile(const CTextFile &);
    CTextFile & operator =(const CTextFile &);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CTextFile();

    // Деструктор класса
    ~CTextFile();


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
    string readLine();
    string readAll();
    void close();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Работа с файлом на запись
    bool openForWriting(const string &fileName);
    void writeLine(const string &line);
    void write(const string &text);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Чтение всего файла целиком
    static bool readFile(const string &fileName, string &text);

    // Запись всего файла целиком
    static bool writeFile(const string &fileName, const string &text);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif

