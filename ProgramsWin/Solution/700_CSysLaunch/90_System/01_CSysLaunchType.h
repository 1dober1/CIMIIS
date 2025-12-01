// Файл 01_CSysLaunchType.h
// Описание нового типа функциональных систем
//

#ifndef _700_CSysLaunch_System_CSysNewType_h
#define _700_CSysLaunch_System_CSysNewType_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"


//=====================================================================================================================
// Степень подробности вывода
struct SInfoLevel
{
    enum infoLevel: int
    {
        lvl_0 = 0,
        lvl_1 = 1,
        lvl_2 = 2
    };

private:
    SInfoLevel()
    {
    };

    ~SInfoLevel()
    {
    }
};


//=====================================================================================================================
// Описание нового типа функциональных систем
class CSysLaunchType: public CSysType
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    string myWeaponName;                                    // Названия вооружения
    	
    //  Параметры визуализации
    bool myDisplayZone;                                     // Отображение зоны поражения в фильме

    int myInfoLevel;                                        // Уровень вывода информации в консоль, лог и фильм

    //  Параметры системы пуска
    bool myMyltipleTargetService;                           // Возможность неоднократного обслуживания одной цели
    bool myTimeToMeetRequest;                               // Запрашивается ли время до встречи ракеты с целью
    bool myFireAndForget;                                   // "Выстрелил и забыл"


    // Параметры ракеты
    double myPrepareDelay;                                  // Задержка на подготовку
    double myFireDelay;                                     // Период схода ракет
    double myMaxLifeTime;                                   // Максимальное время жизни ракеты

    double myVelocity;                                      // Средняя скорость ракеты

    // Параметры зоны поражения
    double myMinRange;                                      // Дальность границы зоны поражения
    double myMaxRange;
    double myMinHeight;                                     // Высота зона поражения
    double myMaxHeight;
    double myMinElevation;                                  // Угол места
    double myMaxElevation;
    double myAzimuthSector;                                 // Cектор по азимуту | Азимут носитель-цель относительно носителя 

    // Параметры подрыва ракеты
    double myHitRadius;                                     // Радиус поражения
    double myHitProbability;                                // Вероятность поражения цели

    std::vector<string> myForbiddenTargets;                 // Запрещённые цели

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CSysLaunchType();

    // Деструктор класса
    virtual ~CSysLaunchType();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Загрузка информации о типе систем из текстового файла
    virtual result load(const CDataMap &data, const string &fileName);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Создание экземпляра системы данного типа
    virtual CSys * createSystem(CObject &object) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к характеристикам типа систем
   
    const string& getWeaponName() const;
    bool getDisplayZone() const;
    int getInfoLevel() const;

    // Доступ к характеристикам типа систем
    bool getMultipleTargerService() const;
    bool getTimeToMeetRequest() const;
    bool getFireAndForget() const;

    // Доступ к характеристикам типа систем
    double getPrepareDelay() const;
    double getFireDelay() const;
    double getMaxLifeTime() const;
    double getVelocity() const;

    // Доступ к характеристикам типа систем
    double getMinRange() const;
    double getMaxRange() const;
    double getMinHeight() const;
    double getMaxHeight() const;
    double getMinElevation() const;
    double getMaxElevation() const;
    double getAzimuthSector() const;

    // Доступ к характеристикам типа систем
    double getHitRadius() const;
    double getHitProbability() const;

    // Запрещённые цели
    const std::vector<string>& getForbiddenTargets() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Регистрация зоны сканирования в фильме
    void registerZone(const std::string& filmName) const;


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


