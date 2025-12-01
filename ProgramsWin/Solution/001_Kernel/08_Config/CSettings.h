// Файл CSettings.h
// Настройки программы
//

#ifndef Kernel_Config_CSettings_h
#define Kernel_Config_CSettings_h


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
// Настройки программы
class CSettings
{
public:
    //-----------------------------------------------------------------------------------------------------------------
    // Единственный на всю программу экземпляр глобальных настроек
    static CSettings myGlobalSettings;


private:
    //-----------------------------------------------------------------------------------------------------------------
    bool myInDebugMode;         // Признак включения режима отладки
    bool myInFilmMode;          // Признак включения режима записи фильма
    bool myInNoTaskMode;        // Признак работы без использования задачи моделирования 
    bool myAutoClose;           // Признак автоматического завершения программы
    bool myRewriteLog;          // Признак перезаписи протокола
    bool myRewriteFilm;         // Признак перезаписи фильма
   
    uint32 mySessionId;         // Номер статистической реализации

    string myObjectsDir;        // Каталог с описанием моделируемых объектов
    string mySystemsDir;        // Каталог с описанием функциональных систем
    string myTasksDir;          // Каталог с описанием задач моделирования
    string myRedScenesDir;      // Каталог с описанием сценариев за "Красных"
    string myBlueScenesDir;     // Каталог с описанием сценариев за "Синих"
    string myFilmsDir;          // Каталог для хранения фильмов
    
    string myLogsDir;           // Каталог для хранения протоколов

    string myTaskName;          // Имя задачи моделирования
    string myTaskFileName;      // Имя файла задачи моделирования
    string myFilmFileName;      // Имя файла фильма, формируемого в результате моделирования
    string myLogFileName;       // Имя файла протокола, формируемого в результате моделирования

    double myEarthMajorRadius;  // Экваториальный радиус Земли
    double myEarthMinorRadius;  // Полярный радиус Земли
    double myEarthGravity;      // Ускорение свободного падения

    string mySurfaceFileName;   // Имя файла с подстилающей поверхностью
    
    bool myIsDay;               // Время суток (день или ночь)

private:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CSettings();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Загрузка настроек из файла
    bool loadFromFile(const string &fileName);

    // Извлечение настроек из командной строки
    bool loadFromCmdLine(const vector<string> &argList);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к общим параметрам программы
    bool inDebugMode() const;
    bool inFilmMode() const;
    bool inNoTaskMode() const;
    bool getAutoClose() const;
    bool getRewriteLog() const;
    bool getRewriteFilm() const;
  

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к настройкам программы
    const string & getObjectsDir() const;
    const string & getSystemsDir() const;
    const string & getTasksDir() const;
    const string & getRedScenesDir() const;
    const string & getBlueScenesDir() const;
    const string & getFilmsDir() const;
    const string & getLogsDir() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к параметрам моделирования
    const string & getTaskName() const;
    const string & getTaskFileName() const;
    const string & getFilmFileName() const;
    const string & getLogFileName() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к параметрам Земли
    double getEarthMajorRadius() const;
    double getEarthMinorRadius() const;
    double getEarthGravity() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к параметрам подстилающей поверхности
    const string & getSurfaceFileName() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к внешним условиям (время суток, погода и т.д.)
    bool isDay() const;


private:
    //-----------------------------------------------------------------------------------------------------------------
    // Вспомогательные функции
    bool _processTaskFileName(const string &taskFileName);
    void _constructFilmFileName();
    void _constructLogFileName();


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
// Быстрый доступ к настройкам программы
static CSettings &Settings = CSettings::myGlobalSettings;


//=====================================================================================================================
#endif

