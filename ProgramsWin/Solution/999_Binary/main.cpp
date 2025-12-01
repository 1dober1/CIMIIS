// Файл main.cpp
// Основная программа
//

//=====================================================================================================================
// Заголовочные файлы
#include "Settings.h"



//=====================================================================================================================
// Коды завершения работы программы
#define EXIT_OK                 0
#define EXIT_BAD_CONFIG         1
#define EXIT_BAD_ARGS           2
#define EXIT_NO_LOG             3
#define EXIT_NO_FILM            4
#define EXIT_NO_SYSTEMS         5
#define EXIT_NO_OBJECTS         6
#define EXIT_BAD_TASK           7



//=====================================================================================================================
// Внешние функции
extern CSysType * CreateSysType(const string &key);



//=====================================================================================================================
// Создание массива с параметрами командной строки
vector<string> CreateArgList(int argc, char **argv)
{
#ifdef SYSTEM_WINDOWS
    vector<string> argList;
    for (int i = 0; i < argc; ++i)
        argList.push_back(argv[i]);
#else
    vector<string> argList;
    for (int i = 0; i < argc; ++i)
        argList.push_back(fromUTF8(argv[i]));
#endif
    return argList;
}



//=====================================================================================================================
// Создание отдельного окна консоли в Windows
void CreateNewConsoleWindow(const vector<string> &argList)
{
#ifdef SYSTEM_WINDOWS
    for (size_t i = 0; i < argList.size(); ++i)
    {
        // Пропуск не интересующих нас параметров командной строки
        if (argList[i] != "CreateNewConsole")
            continue;

        // Создание отдельной консоли
        FreeConsole();
        if (AllocConsole() == TRUE)
        {
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
            freopen("CONIN$", "r", stdin);
        }
        return;
    }
#endif
}



//=====================================================================================================================
// Приостановка запуска модели
void PauseStart(const vector<string> &argList)
{
    for (size_t i = 0; i < argList.size(); ++i)
    {
        // Пропуск не интересующих нас параметров командной строки
        if (argList[i] != "+start_delay")
            continue;

        // Если после параметра "+start_delay" не указано время задержки
        if (i + 1 >= argList.size())
            return;

        // Приостановка модели на указанное время
        uint32 pauseTime = (uint32)atoi(argList[i + 1].c_str());
        Console << Magenta << "  Приостановка работы программы на " << pauseTime << " мс" << EndL;
        MakeSleep(pauseTime);
        Console << Magenta << "  Продолжение работы программы" << EndL;
        Console << EndL << EndL;
        return;
    }
}



//=====================================================================================================================
// Вывод заголовка на экран
void PrintPreamble(const vector<string> & argList)
{
    CDateTime dateTime = CDateTime::queryLocalTime();
    Console << Yellow << "Виртуальная модель Мира" << EndL;
    Console << Yellow << "Версия: " << Green << REVISION_NUMBER << Yellow << ", код сборки: " << Green << BUILD_NUMBER << EndL;
    Console << Yellow << "============================================================" << EndL;
    Console << EndL << EndL;

    // Вывод параметров командной строки
    size_t indexWidth = 0;                  // Вычисление
    size_t localSize = argList.size();      // количества
    do                                      // цифр
    {                                       // в индексах
        localSize /= 10;                    // параметров
        indexWidth++;                       // командной
    } while (localSize > 0);                // строки
    ostringstream out;
    Console << Title << "ПАРАМЕТРЫ КОМАНДНОЙ СТРОКИ (" << argList.size() << ")" << EndL;
    for (size_t i = 0; i < argList.size(); ++i)
    {
        out.str(string());
        out << "    [" << setfill('0') << setw(indexWidth) << i << "] = \"" << argList[i] << "\"";
        Console << out.str() << EndL;
    }
    Console << EndL << EndL;
}



//=====================================================================================================================
// Вывод заголовка в протокол
void LogPreamble(const vector<string> & argList)
{
    CDateTime dateTime = CDateTime::queryLocalTime();
    Log << "Виртуальная модель Мира" << EndL;
    Log << "Версия: " << REVISION_NUMBER << ", код сборки:" << BUILD_NUMBER << EndL;
    Log << "==========================================" << EndL;
    Log << EndL << EndL;

    // Вывод параметров командной строки
    size_t indexWidth = 0;                  // Вычисление
    size_t localSize = argList.size();      // количества
    do                                      // цифр
    {                                       // в индексах
        localSize /= 10;                    // параметров
        indexWidth++;                       // командной
    } while (localSize > 0);                // строки
    ostringstream out;
    Log     << "ПАРАМЕТРЫ КОМАНДНОЙ СТРОКИ (" << argList.size() << ")" << EndL;
    for (size_t i = 0; i < argList.size(); ++i)
    {
        out.str(string());
        out << "    [" << setfill('0') << setw(indexWidth) << i << "] = \"" << argList[i] << "\"";
        Log     << out.str() << EndL;
    }
    Log     << EndL << EndL;
}



//=====================================================================================================================
// Завершение работы программы
void ExitProgram(int32 exitCode)
{
    if (Settings.getAutoClose() == false)
    {
        Console << Success << "Нажмите ENTER для закрытия окна программы..." << EndL;
        getchar();
    }

    exit(exitCode);
}



//=====================================================================================================================
// Загрузка настроек программы
void LoadSettings(const vector<string> & argList)
{
    Console << Title << "ЗАГРУЗКА НАСТРОЕК ПРОГРАММЫ" << EndL;
    if (Settings.loadFromFile("config.ini") == false)
        ExitProgram(EXIT_BAD_CONFIG);
    if (Settings.loadFromCmdLine(argList) == false)
        ExitProgram(EXIT_BAD_ARGS);
    Console << EndL << EndL;
}



//=====================================================================================================================
// Запуск протоколирования работы программы
void StartLog()
{
    Console << Title << "ЗАПУСК ПРОТОКОЛА РАБОТЫ ПРОГРАММЫ" << EndL;
    if (Log.start(Settings.getLogFileName()) == false)
    {
        Console << Error << "    Ошибка! Не удаётся создать файл протокола! <" << Settings.getLogFileName() << ">" << EndL;
        ExitProgram(EXIT_NO_LOG);
    }
    Console << "  Протокол запущен в файле \"" << Settings.getLogFileName() << "\"" << EndL;
    Console << EndL << EndL;
}



//=====================================================================================================================
// Начало записи фильма
void StartFilm()
{
    // Если запись фильма выключена, то ничего не делать
    if (Settings.inFilmMode() == false)
        return;

    // Подготовка файла фильма
    Console << Title << "НАЧАЛО ЗАПИСИ ФИЛЬМА" << EndL;
    Log     << Title << "НАЧАЛО ЗАПИСИ ФИЛЬМА" << EndL;
    if (Film.start(Settings.getFilmFileName()) == false)
    {
        Console << Error << "    Ошибка! Не удаётся создать файл фильма! <" << Settings.getFilmFileName() << ">" << EndL;
        Log     << Error << "    Ошибка! Не удаётся создать файл фильма! <" << Settings.getFilmFileName() << ">" << EndL;
        ExitProgram(EXIT_NO_FILM);
    }
    Console << "  Фильм пишется в файл \"" << Settings.getFilmFileName() << "\"" << EndL;
    Log     << "  Фильм пишется в файл \"" << Settings.getFilmFileName() << "\"" << EndL;
    Console << EndL << EndL;
    Log     << EndL << EndL;
}



//=====================================================================================================================
// Основная программа
int main(int argc, char **argv)
{
   
    // Подготовка программы к работе
    vector<string> argList = CreateArgList(argc, argv);
    CreateNewConsoleWindow(argList);
    PrintPreamble(argList);
    LoadSettings(argList);
    StartLog();
    LogPreamble(argList);
    PauseStart(argList);
    SysTypeList.setCreateSysType(CreateSysType);

    // Загрузка базы исходных данных
    Console << Title << "ЗАГРУЗКА БАЗЫ ИСХОДНЫХ ДАННЫХ" << EndL;
    Log     << Title << "ЗАГРУЗКА БАЗЫ ИСХОДНЫХ ДАННЫХ" << EndL;
    if (SysTypeList.load(Settings.getSystemsDir()) == false)
        ExitProgram(EXIT_NO_SYSTEMS);
    if (ObjectTypeList.load(Settings.getObjectsDir()) == false)
        ExitProgram(EXIT_NO_OBJECTS);
    if (Settings.inNoTaskMode() == false)
        if (Task.load(Settings.getTaskFileName()) == false)
            ExitProgram(EXIT_BAD_TASK);
       
    // Начало записи фильма
    StartFilm();
    
    // Диспетчер времени начинает работу с момента появления первого объекта в задаче моделирования
    Console << Title << "НАЧАЛО МОДЕЛИРОВАНИЯ" << EndL;
    Log     << Title << "НАЧАЛО МОДЕЛИРОВАНИЯ" << EndL;

            
    // Основной цикл продвижения времени
    double curTime = 0.0;
    while (true)
    {
        // Проверить, что хотя бы один объект моделирования ещё есть
        bool doObjectsExist = false;
        for (size_t i = 1; i <= ObjectList.getObjectCount(); ++i)
        {
            if (ObjectList.getObject(i).isExist() == true)
            {
                doObjectsExist = true;
                break;
            }
        }

        // Если работа ведётся без задачи моделирования
        if (Settings.inNoTaskMode() == true)
        {
            // Если не существует ни одного объекта, и пришла команда остановки, то стоп
            if (doObjectsExist == false)
                break;
        }

        // Если работа ведётся по задаче моделирования
        else
        {
            // Если не существует ни одного объекта, и очередь сообщений пуста, то стоп
            if (doObjectsExist == false   &&   Task.isQueueEmpty() == true)
                break;
        }

        // Найти ближайшее время, на которое можно продвинуться по состоянию какой-нибудь системы какого-нибудь объекта
        double newTime = DBL_MAX;
        size_t sysIndex = SIZE_MAX;
        size_t objectNum = SIZE_MAX;
        for (size_t i = 1; i <= ObjectList.getObjectCount(); ++i)
        {
            const CObject &object = ObjectList.getObject(i);
            if (object.isExist() == false)
                continue;

            for (size_t j = 0; j < object.getSysCount(); ++j)
            {
                const CSys &sys = object.getSys(j);
                if (sys.getNextTime() < newTime)
                {
                    newTime = sys.getNextTime();
                    objectNum = i;
                    sysIndex = j;
                }
            }
        }

        // Если ближайшая активизация раньше ближайшего события по сценарию
        if (checkGT(Task.peekMsg().getTime(), newTime) == true)
        {
            
            // Если у активируемого объекта закончилось время существования
            CObject &object = ObjectList.getObject(objectNum);
            const vector<CTaskPoint> &objectRoute = object.getSysMotion().getInitialRoute();
            if (object.isExist() == true   &&   objectRoute.size() > 0)
            {
                if (checkLE(objectRoute[objectRoute.size() - 1].getPointGlobal().getTime(), newTime) == true)
                {
                    // Запросить подтверждение конца маршрута
                    if (object.routeEnd(newTime) == true)
                    {
                        object.disappear(newTime);
                        curTime = newTime;
                        continue;
                    }
                }
            }

            // Выполнить ближайшую активацию
            ObjectList.getObject(objectNum).getSys(sysIndex).activate(newTime);
            curTime = newTime;
            continue;
        }
                
        // Извлечь из задачи моделирования очередное сообщение
        const CTaskMsg &msg = Task.popMsg();

        // Обработать сообщение о создании моделируемого объекта
        if (msg.getType() == CTaskMsg::MSG_OBJECT_APPEAR)
        {
            // Поиск нужного типа объектов
            const CTaskObject &taskObject = Task.getObject(msg.getObjectIndex());
            size_t typeIndex = ObjectTypeList.find(taskObject.getTypeName());
            if (typeIndex == INDEX_NOT_FOUND)
            {
                Log << "    Ошибка в сценарии \"" << taskObject.getSceneName() << "\": неизвестный тип объекта " << taskObject.getSceneNum() << "! <" << taskObject.getTypeName() << ">" << EndL;
                continue;
            }
            const CObjectType &objectType = ObjectTypeList.get(typeIndex);

            // Создание нового моделируемого объекта
            size_t absNum;
            if (taskObject.getParentIndex() == SIZE_MAX)
                absNum = ObjectList.createObject(objectType, taskObject.isRed());
            else
            {
                const CTaskObject &parentObject = Task.getObject(taskObject.getParentIndex());
                if (ErrorCode == ERR_OK)
                    absNum = ObjectList.createObject(objectType, parentObject.getAbsNum(), taskObject.isRed());
                else
                    absNum = ObjectList.createObject(objectType, taskObject.isRed());
            }
            CObject &newObject = ObjectList.getObject(absNum);
            newObject.getSysMotion().takeInitialRoute(taskObject);
            newObject.setName(taskObject.getName());
            taskObject.saveAbsNum(absNum);

            // Активация систем созданного объекта
            newObject.appear(msg.getTime());
            continue;
        }

        // Обработать сообщение о конце маршрута движения объекта
        if (msg.getType() == CTaskMsg::MSG_OBJECT_ROUTE_END)
        {
            // Доступ к объекту
            const CTaskObject &taskObject = Task.getObject(msg.getObjectIndex());
            CObject &object = ObjectList.getObject(taskObject.getAbsNum());
            if (ErrorCode != ERR_OK)
                continue;
            if (object.isExist() == false)
                continue;

            // Запросить подтверждение конца маршрута
            if (object.routeEnd(msg.getTime()) == true)
                object.disappear(msg.getTime());
            continue;
        }

        // Обработать сообщение о точке маршрута
        if (msg.getType() == CTaskMsg::MSG_OBJECT_POINT)
        {
            // Доступ к точке маршрута
            const CTaskObject &taskObject = Task.getObject(msg.getObjectIndex());
            const CTaskPoint &taskPoint = taskObject.getPoint(msg.getPointCmdIndex());
            if (ErrorCode != ERR_OK)
                continue;

            // Доступ к объекту
            CObject &object = ObjectList.getObject(taskObject.getAbsNum());
            if (ErrorCode != ERR_OK)
                continue;
            if (object.isExist() == false)
                continue;

            // Передача точки в системы объекта
            object.processTaskPoint(taskPoint);
            continue;
        }

        // Обработать сообщение о команде объекту
        if (msg.getType() == CTaskMsg::MSG_OBJECT_CMD)
        {
            // Доступ к команде
            const CTaskObject &taskObject = Task.getObject(msg.getObjectIndex());
            const CTaskCmd &taskCmd = taskObject.getCmd(msg.getPointCmdIndex());
            if (ErrorCode != ERR_OK)
                continue;

            // Доступ к объекту
            CObject &object = ObjectList.getObject(taskObject.getAbsNum());
            if (ErrorCode != ERR_OK)
                continue;
            if (object.isExist() == false)
                continue;

            // Передача команды в системы объекта
            object.processTaskCmd(taskCmd);
            continue;
        }
    }
    Console << Title << "МОДЕЛИРОВАНИЕ ЗАВЕРШЕНО" << EndL;
    Log     << Title << "МОДЕЛИРОВАНИЕ ЗАВЕРШЕНО" << EndL;
    Console << EndL << EndL;
    Log     << EndL << EndL; 


    // Результат работы
    Film.stop();
    Log.stop();
    ExitProgram(EXIT_OK);
}
