// Файл 01_CSysLaunchType.cpp
// Описание нового типа функциональных систем
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CSysLaunchType.h"
#include "02_CSysLaunch.h"




//=====================================================================================================================
// Конструктор класса
CSysLaunchType::CSysLaunchType()
{
    // Указание начальных значений параметров
   
    myDisplayZone = false;                                      // Отображение зоны поражения

    myInfoLevel= SInfoLevel::lvl_1;                             // Вывод информации на консоль, в лог, в фильм

    myMyltipleTargetService = false;                            // Множественное обслуживание цели
    myTimeToMeetRequest = true;                                 // Запрашивается ли время до встречи ракеты с целью
    myFireAndForget = false;                                    // "Выстрелил и забыл"

    myPrepareDelay = 0.;                                        // Задержка на подготову
    myFireDelay = 0.;                                           // Период схода ракет с лафета
    myMaxLifeTime = double_max;                                 // Максимальное время полёта

    myVelocity = 0.;                                            // Средняя скорость поражения

    // Параметры зоны поражения
    myMinRange = 0.;                                            // Дальность границы зоны поражения
    myMaxRange = 0.;                              
    myMinHeight = 0.;                                           // Высота зона поражения 
    myMaxHeight = 0.;                            
    myMinElevation = 0.;                                        // Угол места 
    myMaxElevation = 0.;                      
    myAzimuthSector = 0.;                                       // Cектор по азимуту

    // Параметры подрыва ракеты
	myHitRadius = 150.;                                         // Радиус поражения
	myHitProbability = 1.;                                      // Вероятность поражения цели
}


//=====================================================================================================================
// Деструктор класса
CSysLaunchType::~CSysLaunchType()
{
}


//=====================================================================================================================
// Загрузка информации о типе системы из текстового файла
result CSysLaunchType::load(const CDataMap &data, const string &fileName)
{
    // Признак наличия предупреждений при считывании параметров типа системы
    bool wereWarnings = false;      // изначально полагаем, что предупреждений не будет

    // Считывание общей информации, присутствущей в любом файле с описанием типа системы
    if (CSysType::load(data, fileName) == RESULT_ERROR)
        return RESULT_ERROR;
    
    // Название оружия
    if (extractString(data, fileName, "/название_оружия", myWeaponName, false,
        "поле \"название_оружия\" не найдено!",
        "в поле \"название_оружия\" не указано значение!",
        "в поле \"название_оружия\" некорректное значение!") == false)
        return RESULT_ERROR;


    // Отображение зоны поражения
    extractBool(data, fileName, "/отображение_зоны_поражения", myDisplayZone, false,
        "поле \"отображение_зоны_поражения\" не найдено!",
        "в поле \"отображение_зоны_поражения\" не указано значение!");

    
    // Возможность множественного обслуживания одной цели
    if (extractBool(data, fileName, "/множественное_обслуживание_цели", myMyltipleTargetService, false,
        "поле \"множественное_обслуживание_цели\" не найдено!",
        "в поле \"множественное_обслуживание_цели\" не указано значение!") == false)
        return RESULT_ERROR;

    if (extractBool(data, fileName, "/запрос_момента_встречи", myTimeToMeetRequest, false,
        "поле \"запрос_момента_встречи\" не найдено!",
        "в поле \"запрос_момента_встречи\" не указано значение!") == false)
        return RESULT_ERROR;
	
    // "Выстрелил и забыл"
    if (extractBool(data, fileName, "/выстрелил_и_забыл", myFireAndForget, false,
        "поле \"выстрелил_и_забыл\" не найдено!",
        "в поле \"выстрелил_и_забыл\" не указано значение!") == false)
        return RESULT_ERROR;

    // Время_подготовки_оружия
    if (extractDoubleGreaterOrEqualThen(data, fileName, "/время_подготовки_оружия", myPrepareDelay, false,
        "поле \"время_подготовки_оружия\" не найдено!",
        "в поле \"время_подготовки_оружия\" не указано значение!",
        "в поле \"время_подготовки_оружия\" допустимо только положительное значение!", 0.) == false)
        return RESULT_ERROR;

    // Период схода ракет                                
    if (extractDoubleGreaterOrEqualThen(data, fileName, "/период_схода_ракет", myFireDelay, false,
        "поле \"период_схода_ракет\" не найдено!",
        "в поле \"период_схода_ракет\" не указано значение!",
        "в поле \"период_схода_ракет\" допустимо только положительное значение!",0.) == false)
        return RESULT_ERROR;

    // Максимальное время существования ракеты                               
    if (extractDoubleGreaterOrEqualThen(data, fileName, "/максимальное_время_жизни_оружия", myMaxLifeTime, false,
        "поле \"максимальное_время_жизни_оружия\" не найдено!",
        "в поле \"максимальное_время_жизни_оружия\" не указано значение!",
        "в поле \"максимальное_время_жизни_оружия\" допустимо только положительное значение!", 0.) == false)
        return RESULT_ERROR;

    // Скорость ракеты цели
    if (extractDoubleGreaterOrEqualThen(data, fileName, "/средняя_скорость_оружия", myVelocity, false,
        "поле \"средняя_скорость_оружия\" не найдено!",
        "в поле \"средняя_скорость_оружия\" не указано значение!",
        "в поле \"средняя_скорость_оружия\" допустимо только положительное значение!", 0.) == false)
        return RESULT_ERROR;
	
    // ____________________________Считывание параметров зоны поражения___________________________________
    // 
    // Считывание дальней границы зоны поражения
    if (extractDoublePositive(data, fileName, "/дальняя_граница", myMaxRange, false,
        "поле \"дальняя_граница\" не найдено!",
        "в поле \"дальняя_граница\" не указано значение!",
        "в поле \"дальняя_граница\" допустимо только положительное значение!") == false)
        return RESULT_ERROR;

    // Считывание ближней границы зоны поражения
    if (extractDoublePositive(data, fileName, "/ближняя_граница", myMinRange, false,
        "поле \"ближняя_граница\" не найдено!",
        "в поле \"ближняя_граница\" не указано значение!",
        "в поле \"ближняя_граница\" допустимо только положительное значение!") == false)
        return RESULT_ERROR;

    // Считывание верхней границы зоны поражения
    if (extractDoubleAny(data, fileName, "/верхняя_граница", myMaxHeight, false,
        "поле \"верхняя_граница\" не найдено!",
        "в поле \"верхняя_граница\" не указано значение!") == false)
        return RESULT_ERROR;

    // Считывание нижней границы зоны поражения
    if (extractDoubleAny(data, fileName, "/нижняя_граница", myMinHeight, false,
        "поле \"нижняя_граница\" не найдено!",
        "в поле \"нижняя_граница\" не указано значение!") == false)
        return RESULT_ERROR;

    // Считывание максимального угла места
    if (extractDoubleAny(data, fileName, "/максимальный_угол_места", myMaxElevation, false,
        "поле \"максимальный_угол_места\" не найдено!",
        "в поле \"максимальный_угол_места\" не указано значение!") == false)
        return RESULT_ERROR;

    // Считывание минимального угла места
    if (extractDoubleAny(data, fileName, "/минимальный_угол_места", myMinElevation, false,
        "поле \"минимальный_угол_места\" не найдено!",
        "в поле \"минимальный_угол_места\" не указано значение!") == false)
        return RESULT_ERROR;

    // Считывание сектора по азимуту
    if (extractDoubleAny(data, fileName, "/сектор_по_азимуту", myAzimuthSector, false,
        "поле \"сектор_по_азимуту\" не найдено!",
        "в поле \"сектор_по_азимуту\" не указано значение!") == false)
        return RESULT_ERROR;

    // ____________________________Конец считывания параметров зоны поражения___________________________________
    
    // Радиус поражения при подрыве ракеты
    if (extractDoublePositive(data, fileName, "/радиус_поражения_цели", myHitRadius, false,
        "поле \"радиус_поражения_цели\" не найдено!",
        "в поле \"радиус_поражения_цели\" не указано значение!",
        "в поле \"радиус_поражения_цели\" допустимо только положительное значение!") == false)
        return RESULT_ERROR;

    // Вероятность поражения цели
    if (extractDoubleGreaterOrEqualThen(data, fileName, "/вероятность_поражения_цели", myHitProbability, false,
        "поле \"вероятность_поражения_цели\" не найдено!",
        "в поле \"вероятность_поражения_цели\" не указано значение!",
        "в поле \"вероятность_поражения_цели\" допустимо только положительное значение!", 0) == false)
        return RESULT_ERROR;

    // Запрещённые цели
    if (extractStringColumn(data, fileName, "/запрещенные_к_обслуживанию_цели/обладающие_свойством", myForbiddenTargets, false,
        "вектор-столбец \"обладающие_свойством\" в группе \"запрещенные_к_обслуживанию_цели\" не найден!",
        "в векторе-столбце \"обладающие_свойством\" в группе \"запрещенные_к_обслуживанию_цели\" нет ни одной строки!",
        "в векторе-столбце \"обладающие_свойством\" в группе \"запрещенные_к_обслуживанию_цели\" все значения пустые!") == false)
        wereWarnings = true;
    for (int i = 0; i < myForbiddenTargets.size(); i++)
        myForbiddenTargets[i] = toLower(myForbiddenTargets[i]);

    // Считывание параметров завершено
    if (wereWarnings == true)
        return RESULT_WARNING;
    return RESULT_OK;
}


//=====================================================================================================================
// Создание экземпляра системы данного типа
CSys * CSysLaunchType::createSystem(CObject &object) const
{
    return new CSysLaunch(*this, object);
}


const string& CSysLaunchType::getWeaponName() const
{
    return myWeaponName;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
bool CSysLaunchType::getDisplayZone() const
{
    return myDisplayZone;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
int CSysLaunchType::getInfoLevel() const
{
    return myInfoLevel;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
bool  CSysLaunchType::getMultipleTargerService() const
{
    return myMyltipleTargetService;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
bool CSysLaunchType::getTimeToMeetRequest() const
{
    return myTimeToMeetRequest;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
bool CSysLaunchType::getFireAndForget() const
{
    return myFireAndForget;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getPrepareDelay() const
{
    return myPrepareDelay;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getFireDelay() const
{
    return myFireDelay;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getMaxLifeTime() const
{
    return myMaxLifeTime;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getVelocity() const
{
    return myVelocity;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getMinRange() const
{
    return myMinRange;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getMaxRange() const
{
    return myMaxRange;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getMinHeight() const
{
    return myMinHeight;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getMaxHeight() const
{
    return myMaxHeight;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getMinElevation() const
{
    return myMinElevation;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getMaxElevation() const
{
    return myMaxElevation;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getAzimuthSector() const
{
    return myAzimuthSector;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getHitRadius() const
{
    return myHitRadius;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа систем
double CSysLaunchType::getHitProbability() const
{
    return myHitProbability;
}


//=====================================================================================================================
// Запрещённые цели
const std::vector <string>& CSysLaunchType::getForbiddenTargets() const    
{
    return myForbiddenTargets;
}


//=====================================================================================================================
// Регистрация зоны сканирования в фильме
void CSysLaunchType::registerZone(const std::string& filmName) const
{

    int filmId = 1;
    int typeId = 1;

    double deltaElevation = double(1);                  // Шаг развёртки

    vector<double> x;
    vector<double> y;

    double x_elevation;
    double y_elevation;
    

    // Так как есть ограничения зоны по высоте, то диапазон углов развертки зоны поражения для Dmax и Dmin будет не в диапазоне [myMinElevation, myMaxElevation], а в более узком
    // Возможны 9 ситуаций:
    // 1) Ограничения по высоте никак не затрагивают развёртку по углам
    // 2) Ограничение по высоте сверху затрагивают развёртку для Dmax
    // 3) Ограничение по высоте сверху затрагивают развёртку как для Dmax, так и для Dmin
    // 4) Ограничение по высоте снизу затрагивают развёртку для Dmax
    // 5) Ограничение по высоте снизу затрагивают развёртку как для Dmax, так и для Dmin
    // 6) Ограничение по высоте как сверху, так и снизу затрагивают развёртку для Dmax
    // 7) Ограничение по высоте как сверху, так и снизу затрагивают развёртку как для Dmax, так и для Dmin
    // 8) Ограничение по высоте сверху затрагивают развёртку как для Dmax, так и для Dmin ограничение снизу затрагивает развёртку для Dmax
    // 9) Ограничение по высоте снизу затрагивают развёртку как для Dmax, так и для Dmin ограничение свурху затрагивает развёртку для Dmax

    // для случаев 2, 4, 6, 8, 9 необходима промежуточная точка(и) между развёртками по Dmax и Dmin, для корректного отображения зоны поражения

    double minElevationDmax = myMinElevation;           // Минимальный угол развёртки для Dmax
    double maxElevationDmax = myMaxElevation;           // Максимальный угол развёртки для Dmax
    double minElevationDmin = myMinElevation;           // Минимальный угол развёртки для Dmin
    double maxElevationDmin = myMaxElevation;           // Максимальный угол развёртки для Dmin 

    // Секция определения углов развёртки
    y_elevation = sin(deg2rad(myMinElevation)) * myMaxRange;
    if ( y_elevation < myMinHeight )
        minElevationDmax = rad2deg( asin(myMinHeight / myMaxRange) );

    y_elevation = sin(deg2rad(myMaxElevation)) * myMaxRange;
    if (y_elevation > myMaxHeight)
        maxElevationDmax = rad2deg( asin(myMaxHeight / myMaxRange) );

    // Секция определения существования промежуточной верхней точки и промежуточной нижней точки

    bool topPointExist = false;
    double x_topPoint;
    double y_topPoint;

    double timeData_1 = sin(deg2rad(myMaxElevation)) * myMaxRange;
    double timeData_2 = sin(deg2rad(myMaxElevation)) * myMinRange;

    if (checkGE(myMaxElevation, 0.))
    {
        if ((sin(deg2rad(myMaxElevation)) * myMaxRange > myMaxHeight) && (sin(deg2rad(myMaxElevation)) * myMinRange < myMaxHeight))
        {
            topPointExist = true;
            y_topPoint = myMaxHeight;
            x_topPoint = y_topPoint / tan(deg2rad(myMaxElevation));
        }
    }
    else
    {
        if ((sin(deg2rad(myMaxElevation)) * myMaxRange < myMaxHeight) && (sin(deg2rad(myMaxElevation)) * myMinRange > myMaxHeight))
        {
            topPointExist = true;
            y_topPoint = myMaxHeight;
            x_topPoint = y_topPoint / tan(deg2rad(myMaxElevation));
        }
    }

    bool botPointExist = false;
    double x_botPoint;
    double y_botPoint;

    double timeData_3 = sin(deg2rad(myMinElevation)) * myMaxRange;
    double timeData_4 = sin(deg2rad(myMinElevation)) * myMinRange;

    if (checkLT(myMinElevation, 0.))
    {
        if ((sin(deg2rad(myMinElevation)) * myMaxRange < myMinHeight) && (sin(deg2rad(myMinElevation)) * myMinRange > myMinHeight))
        {
            botPointExist = true;
            y_botPoint = myMinHeight;
            x_botPoint = y_botPoint / tan(deg2rad(myMinElevation));
        }
    }
    else
    {
        if ((sin(deg2rad(myMinElevation)) * myMaxRange > myMinHeight) && (sin(deg2rad(myMinElevation)) * myMinRange < myMinHeight))
        {
            botPointExist = true;
            y_botPoint = myMinHeight;
            x_botPoint = y_botPoint / tan(deg2rad(myMinElevation));
        }
    }

    // Начало обхода

    // Фиксация верхней промежуточной точки
    if (topPointExist)
    {
        x.push_back(x_topPoint);
        y.push_back(y_topPoint);
    }

    // Развёртка по Dmax по часовой стрелки 
    for (double fElevaton = maxElevationDmax; fElevaton >= minElevationDmax; fElevaton -= deltaElevation)
    {
        y_elevation = sin(deg2rad(fElevaton)) * myMaxRange;
        x_elevation = cos(deg2rad(fElevaton)) * myMaxRange;

        x.push_back(x_elevation);
        y.push_back(y_elevation);
    }
	
    //Фиксация нижней промежуточной точки
    if (botPointExist)
    {
        x.push_back(x_botPoint);
        y.push_back(y_botPoint);
    }

    // Развёртка по Dmin против часовой стрелки 
    if (myMinRange>0)
    {
        y_elevation = sin(deg2rad(myMinElevation)) * myMinRange;
        if (y_elevation < myMinHeight)
            minElevationDmin = rad2deg( asin(myMinHeight / myMinRange) );

        y_elevation = sin(deg2rad(myMaxElevation)) * myMinRange;
        if (y_elevation > myMaxHeight)
            maxElevationDmin = rad2deg( asin(myMaxHeight / myMinRange) );

        for (double fElevaton = minElevationDmin; fElevaton <= maxElevationDmin; fElevaton += deltaElevation)
        {
            y_elevation = sin(deg2rad(fElevaton)) * myMinRange;
            x_elevation = cos(deg2rad(fElevaton)) * myMinRange;

            x.push_back(x_elevation);
            y.push_back(y_elevation);
        }

    }
    else
    {
        x.push_back(0.);
        y.push_back(0.);
    }
	
    ostringstream out;
    string space = "  ";
    out << "KILL" << space
        << filmName << space
        << (myAzimuthSector == 0. ? double(360) : myAzimuthSector) << space
        << x.size()+1;

    for (size_t i = 0; i < x.size(); i++)
    {
        out << space << x[i] << space << y[i];
    }
    out << space << x[0] << space << y[0];

    out << endl;
    Film.writeText(out.str());

}

