// Файл CSettings.cpp
// Настройки программы
//

//=====================================================================================================================
// Заголовочные файлы
#include "CSettings.h"
#include "../02_BasicTypes/02_FloatingTypes.h"
#include "../04_String/01_FileName.h"
#include "../04_String/03_Case.h"
#include "../05_System/04_CTextFile.h"
#include "../06_Output/03_COutputConsole.h"
#include "../07_Parser/04_CTextParser.h"



//=====================================================================================================================
// Единственный на всю программу экземпляр глобальных настроек
CSettings CSettings::myGlobalSettings;


//=====================================================================================================================
// Конструктор класса
CSettings::CSettings()
{
    myInDebugMode = false;
    myInFilmMode = true;
    myInNoTaskMode = false;
    myAutoClose = true;
    myRewriteLog = false;
    myRewriteFilm = false;
   
    mySessionId = 1;

    myObjectsDir = "./";
    mySystemsDir = "./";
    myTasksDir = "./";
    myRedScenesDir = "./";
    myBlueScenesDir = "./";
    myFilmsDir = "./";
    myLogsDir = "./";

    myTaskName = string();
    myTaskFileName = string();
    myFilmFileName = "Unnamed.3df";
    myLogFileName = "Unnamed.log";

    myEarthMajorRadius = 6378137.0;
    myEarthMinorRadius = 6356752.3142;
    myEarthGravity = 9.81;

    mySurfaceFileName = string();
    
    myIsDay = true;
}



//=====================================================================================================================
// Загрузка настроек из файла
bool CSettings::loadFromFile(const string &fileName)
{
    // Проверка корректности имени файла
    if (fileName.empty() == true)
        return false;

    // Чтение файла
    string text;
    Console << "  Загрузка данных из конфигурационного файла..." << EndL;
    if (CTextFile::readFile(fileName, text) == false)
    {
        Console << Error << "    Ошибка в " << fileName << ": нет доступа к файлу!" << EndL;
        return false;
    }
    text += "\n";

    // Синтаксический разбор файла
    Parser.parse(text);
    if (Parser.wasError() == true)
    {
        Console << Error << "    " << fileName << Parser.getErrorMessage() << EndL;
        return false;
    }

    // Доступ к результатам разбора файла
    const map<string, CStringTable> &data = Parser.getData();
    map<string, CStringTable>::const_iterator itr;

    // Сохранение признака работы в режиме отладки
    itr = data.find("/режим_отладки");
    if (itr != data.end())
    {
        if (toLower(itr->second.getValue(0, 0)) == "да")
            myInDebugMode = true;
        else
            myInDebugMode = false;
    }

    // Сохранение признака автоматического завершения прораммы
    itr = data.find("/закрыть_окно");
    if (itr != data.end())
    {
        if (toLower(itr->second.getValue(0, 0)) == "да")
            myAutoClose = true;
        else
            myAutoClose = false;
    }

    // Сохранение признака перезаписи протокола
    itr = data.find("/перезаписывать_протокол");
    if (itr != data.end())
    {
        if (toLower(itr->second.getValue(0, 0)) == "да")
            myRewriteLog = true;
        else
            myRewriteLog = false;
    }

    // Сохранение признака перезаписи фильма
    itr = data.find("/перезаписывать_фильм");
    if (itr != data.end())
    {
        if (toLower(itr->second.getValue(0, 0)) == "да")
            myRewriteFilm = true;
        else
            myRewriteFilm = false;
    }

    
    // Сохранение признака работы без использования задачи моделирования
    itr = data.find("/использовать_задачу_моделирования");
    if (itr != data.end())
    {
        if (toLower(itr->second.getValue(0, 0)) == "да")
            myInNoTaskMode = false;
        else
            myInNoTaskMode = true;
    }

    // Сохранение каталога объектов
    itr = data.find("/каталоги/база_моделируемых_объектов");
    if (itr != data.end())
    {
        const string &dir = itr->second.getValue(0, 0);
        if (dir.empty() == false)
        {
            myObjectsDir = normalizePathSeparators(dir);
            if (myObjectsDir[myObjectsDir.size()-1] != '/')
                myObjectsDir.append("/");
        }
    }

    // Сохранение каталога функциональных систем
    itr = data.find("/каталоги/база_функциональных_систем");
    if (itr != data.end())
    {
        const string &dir = itr->second.getValue(0, 0);
        if (dir.empty() == false)
        {
            mySystemsDir = normalizePathSeparators(dir);
            if (mySystemsDir[mySystemsDir.size()-1] != '/')
                mySystemsDir.append("/");
        }
    }

    // Сохранение каталога задач моделирования
    itr = data.find("/каталоги/база_задач_моделирования");
    if (itr != data.end())
    {
        const string &dir = itr->second.getValue(0, 0);
        if (dir.empty() == false)
        {
            myTasksDir = normalizePathSeparators(dir);
            if (myTasksDir[myTasksDir.size()-1] != '/')
                myTasksDir.append("/");
        }
    }

    // Сохранение каталога сценариев за "Красных"
    itr = data.find("/каталоги/база_красных_сценариев");
    if (itr != data.end())
    {
        const string &dir = itr->second.getValue(0, 0);
        if (dir.empty() == false)
        {
            myRedScenesDir = normalizePathSeparators(dir);
            if (myRedScenesDir[myRedScenesDir.size()-1] != '/')
                myRedScenesDir.append("/");
        }
    }

    // Сохранение каталога сценариев за "Синих"
    itr = data.find("/каталоги/база_синих_сценариев");
    if (itr != data.end())
    {
        const string &dir = itr->second.getValue(0, 0);
        if (dir.empty() == false)
        {
            myBlueScenesDir = normalizePathSeparators(dir);
            if (myBlueScenesDir[myBlueScenesDir.size()-1] != '/')
                myBlueScenesDir.append("/");
        }
    }

    // Сохранение каталога протоколов
    itr = data.find("/каталоги/протоколы");
    if (itr != data.end())
    {
        const string &logsDir = itr->second.getValue(0, 0);
        if (logsDir.empty() == false)
        {
            myLogsDir = normalizePathSeparators(logsDir);
            if (myLogsDir[myLogsDir.size()-1] != '/')
                myLogsDir.append("/");
        }
    }

    // Сохранение каталога фильмов
    itr = data.find("/каталоги/фильмы");
    if (itr != data.end())
    {
        const string &filmsDir = itr->second.getValue(0, 0);
        if (filmsDir.empty() == false)
        {
            myFilmsDir = normalizePathSeparators(filmsDir);
            if (myFilmsDir[myFilmsDir.size() - 1] != '/')
                myFilmsDir.append("/");
        }
    }

        
    // Сохранение экваториального радиуса Земли
    itr = data.find("/параметры_Земли/экваториальный_радиус");
    if (itr != data.end())
        myEarthMajorRadius = atof(itr->second.getValue(0, 0).c_str());

    // Сохранение полярного радиуса Земли
    itr = data.find("/параметры_Земли/полярный_радиус");
    if (itr != data.end())
        myEarthMinorRadius = atof(itr->second.getValue(0, 0).c_str());

    // Сохранение ускорения свободного падения
    itr = data.find("/параметры_Земли/ускорение_свободного_падения");
    if (itr != data.end())
        myEarthGravity = atof(itr->second.getValue(0, 0).c_str());

      
    // Сохранение времени суток
    itr = data.find("/внешние_условия/время_суток");
    if (itr != data.end())
    {
        if (toLower(itr->second.getValue(0, 0)) == "день")
            myIsDay = true;
        else
            myIsDay = false;
    }

    // Файл подстилающей поверхности
    itr = data.find("/внешние_условия/подстилающая_поверхность");
    if (itr != data.end())
        mySurfaceFileName = normalizePathSeparators(itr->second.getValue(0, 0));


    return true;
}



//=====================================================================================================================
// Извлечение настроек из командной строки
bool CSettings::loadFromCmdLine(const vector<string> &argList)
{
    Console << "  Загрузка данных из командной строки..." << EndL;

    // Ожидается не менее двух аргументов: имя исполняемого файла и имя файла задачи моделирования
    if (argList.size() < 2)
    {
        Console << Error << "    Ошибка! Недостаточно параметров командной строки для запуска модели!" << EndL;
        Console << Error << "    Для запуска модели используйте программу управления!" << EndL;
        return false;
    }

    // Обработка имени файла задачи моделирования из первого аргумента
    if (_processTaskFileName(argList[1]) == false)
        return false;

    // Обработка остальных аргументов, начиная со второго
    for (size_t i = 2; i < argList.size(); ++i)
    {
        
        // Извлечение признака записи фильма
        if (argList[i] == "+Film")
        {
            myInFilmMode = true;
            continue;
        }
        if (argList[i] == "-Film")
        {
            myInFilmMode = false;
            continue;
        }
    }

    // Конструирование полных имён для файлов фильма, статистики и протокола
    _constructFilmFileName();
    _constructLogFileName();
    return true;
}



//=====================================================================================================================
// Доступ к общим параметрам программы
bool CSettings::inDebugMode() const
{
    return myInDebugMode;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к общим параметрам программы
bool CSettings::inFilmMode() const
{
    return myInFilmMode;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к общим параметрам программы
bool CSettings::inNoTaskMode() const
{
    return myInNoTaskMode;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к общим параметрам программы
bool CSettings::getAutoClose() const
{
    return myAutoClose;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к общим параметрам программы
bool CSettings::getRewriteLog() const
{
    return myRewriteLog;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к общим параметрам программы
bool CSettings::getRewriteFilm() const
{
    return myRewriteFilm;
}



//=====================================================================================================================
// Доступ к настройкам каталогов размещения файлов
const string & CSettings::getObjectsDir() const
{
    return myObjectsDir;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к настройкам каталогов размещения файлов
const string & CSettings::getSystemsDir() const
{
    return mySystemsDir;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к настройкам каталогов размещения файлов
const string & CSettings::getTasksDir() const
{
    return myTasksDir;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к настройкам каталогов размещения файлов
const string & CSettings::getRedScenesDir() const
{
    return myRedScenesDir;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к настройкам каталогов размещения файлов
const string & CSettings::getBlueScenesDir() const
{
    return myBlueScenesDir;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к настройкам каталогов размещения файлов
const string & CSettings::getFilmsDir() const
{
    return myFilmsDir;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к настройкам каталогов размещения файлов
const string & CSettings::getLogsDir() const
{
    return myLogsDir;
}



//=====================================================================================================================
// Доступ к параметрам моделирования
const string & CSettings::getTaskName() const
{
    return myTaskName;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам моделирования
const string & CSettings::getTaskFileName() const
{
    return myTaskFileName;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам моделирования
const string & CSettings::getFilmFileName() const
{
    return myFilmFileName;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам моделирования
const string & CSettings::getLogFileName() const
{
    return myLogFileName;
}



//=====================================================================================================================
// Доступ к параметрам Земли
double CSettings::getEarthMajorRadius() const
{
    return myEarthMajorRadius;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам Земли
double CSettings::getEarthMinorRadius() const
{
    return myEarthMinorRadius;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам Земли
double CSettings::getEarthGravity() const
{
    return myEarthGravity;
}



//=====================================================================================================================
// Доступ к параметрам подстилающей поверхности
const string & CSettings::getSurfaceFileName() const
{
    return mySurfaceFileName;
}



//=====================================================================================================================
// Доступ к внешним условиям (время суток, погода и т.д.)
bool CSettings::isDay() const
{
    return myIsDay;
}



//=====================================================================================================================
// Вспомогательные функции
bool CSettings::_processTaskFileName(const string &taskFileName)
{
    // Извлечь расширение имени файла задачи
    string fileExt = extractFileNameExtension(taskFileName);
    if (fileExt != ".tas")
    {
        Console << Error << "    Ошибка! Недопустимое расширение имени файла задачи моделирования!" << EndL;
        return false;
    }

    // Извлечь расширение имя задачи
    myTaskName = extractPureFileName(taskFileName);
    if (myTaskName.empty() == true)
    {
        Console << Error << "    Ошибка! Недопустимое имя задачи моделирования!" << EndL;
        return false;
    }

    // Если указан путь к файлу задачи, то использовать его вместо указанного в конфигурационном файле
    string taskFilePath = extractFilePath(taskFileName);
    if (taskFilePath.empty() == false)
        myTasksDir = taskFilePath;

    // Формирование полного имени файла задачи
    myTaskFileName = myTasksDir + myTaskName + fileExt;

    // Обработка завершена
    return true;
}



//---------------------------------------------------------------------------------------------------------------------
// Вспомогательные функции
void CSettings::_constructFilmFileName()
{
    char session[256];
    string fileName;

    // Если включена перезапись фильма, то суффикс не используется
    if (myRewriteFilm == true)
    {
        // Сконструировать имя файла фильма
        sprintf(session, "%03u", mySessionId);
        fileName = myFilmsDir + myTaskName + string(".") + string(session) + string(".3df");
    }

    
    // В противном случае последовательно подбирается свободное имя за счёт изменения суффикса
    else
    {
        // Последовательный подбор имени файла фильма
        char suffix[256];
        sprintf(session, "%03u", mySessionId);
        for(int32 counter = 1; counter < 10000; counter++)
        {
            // Сконструировать имя файла фильма
            sprintf(suffix, "%04d", counter);
            fileName = myFilmsDir + myTaskName + string(".") + string(session) + "_" + string(suffix) + string(".3df");

            // Проверить, что такого файла нет
            CTextFile tempFile;
            if (tempFile.openForReading(fileName) == false)
                break;
        }
    }

    myFilmFileName = fileName;
}



//---------------------------------------------------------------------------------------------------------------------
// Вспомогательные функции
void CSettings::_constructLogFileName()
{
    char session[256];
    string fileName;

    // Если включена перезапись протокола, то суффикс не используется
    if (myRewriteLog == true)
    {
        // Сконструировать имя файла протокола
        sprintf(session, "%03u", mySessionId);
        fileName = myLogsDir + myTaskName + string(".") + string(session) + string(".log");
    }


    // В противном случае последовательно подбирается свободное имя за счёт изменения суффикса
    else
    {
        // Последовательный подбор имени файла протокола
        char suffix[256];
        sprintf(session, "%03u", mySessionId);
        for (int32 counter = 1; counter < 10000; counter++)
        {
            // Сконструировать имя файла фильма
            sprintf(suffix, "%04d", counter);
            fileName = myLogsDir + myTaskName + string(".") + string(session) + "_" + string(suffix) + string(".log");

            // Проверить, что такого файла нет
            CTextFile tempFile;
            if (tempFile.openForReading(fileName) == false)
                break;
        }
    }

    myLogFileName = fileName;
}


