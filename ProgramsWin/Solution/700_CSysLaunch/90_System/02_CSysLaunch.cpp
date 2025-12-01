// Файл 02_CSysLaunch.cpp
// Описание функциональной системы пуска ракеты
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_CSysLaunch.h"


//=====================================================================================================================
// Конструктор класса
CSysLaunch::CSysLaunch(const CSysType &sysType, CObject &object)
    : CSys(sysType, object)
{
    // Указание начальных значений параметров
    myIsSysWorking = false;

    myIsWeaponEmpty = true;

    myStartDelayBlockTime = 0;

    myFreeWeaponCount = 0;
    myWeaponLaunchCount = 0;
}


//=====================================================================================================================
// Деструктор класса
CSysLaunch::~CSysLaunch()
{
}


//=====================================================================================================================
// Доступ к типу системы
const CSysLaunchType & CSysLaunch::getType() const
{
    return (const CSysLaunchType &) CSys::getType();
}


//=====================================================================================================================
// Сменные алгоритмы действий
void CSysLaunch::onAppear(double time)
{
    switch (this->getType().getInfoLevel())
    {
    case SInfoLevel::lvl_0:
    break;
    case SInfoLevel::lvl_2:
    break;
    case SInfoLevel::lvl_1:
    default:
        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " системе пуска задано начальное состояние" << EndL;
            Log     << "  " << format(time, 7, 2) << " системе пуска задано начальное состояние" << EndL;
        }
        break;
    }
 
    myIsSysWorking = true;

    myStartDelayBlockTime = 0;

    if (getType().getDisplayZone() == true)
    {
        getType().registerZone(getObject().getType().getName());

        ostringstream out;
        string space = "  ";

        // Время  ZONESTATE  номер_носителя_в_фильме  идентификатор_зоны  состояние  режим_отображения  направление_по_азимуту  направление_по_углу_места  0
        out << time << space
            << "ZONESTATE" << space
            << getObject().getFilmId() << space
            << 1 << space
            << 1 << space
            << 1 << space
            << 0 << space
            << 0 << space
            << 0 << endl;

        Film.writeText(out.str());
    }

}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysLaunch::onDestroy(double time)
{
    // Console << "  " << format(time, 7, 2) << " система пуска подготовилась к уничтожению (поражению) своего носителя" << EndL;

    myIsSysWorking = false;
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysLaunch::onActivate(double time)
{
    //Блок "Проверка системы пуска"
    if (_moduleSystemCheck(time) == false)
        return;

    std::multimap<size_t, targetParam>::iterator targetsMapIterator;
    for (targetsMapIterator = myTargetsMap.begin(); targetsMapIterator != myTargetsMap.end();)
    {
        if (targetsMapIterator->second.targetState == tarEmpty)
            continue;

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
        if (targetsMapIterator->second.targetState == tarLaunchPrepare)
            _moduleTarLaunchPrepare(time, targetsMapIterator);

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
        if (targetsMapIterator->second.targetState == tarReadyForAttack)
            _moduleTarReadyForAttack(time, targetsMapIterator);

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
        if (targetsMapIterator->second.targetState == tarWeaponPrepare)
            _moduleTarWeaponPrepare(time, targetsMapIterator);

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
        if (targetsMapIterator->second.targetState == tarWeaponStart)
            _moduleTarWeaponStart(time, targetsMapIterator);

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
        if (targetsMapIterator->second.targetState == tarWeaponAppearance)
            _moduleTarWeaponAppearance(time, targetsMapIterator);

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
        if (targetsMapIterator->second.targetState == tarWeaponFlight)
            _moduleTarWeaponFlight(time, targetsMapIterator);

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
		if (targetsMapIterator->second.targetState == tarTargetLost)
			_moduleTarTargetLost(time, targetsMapIterator);

        // Блок, котороый должен работать с периодом активации системы пуска    |   Вызов в onActivate
		if (targetsMapIterator->second.targetState == tarTargetRefuse)
			_moduleTarTargetRefuse(time, targetsMapIterator);
		
        // Дальше возможно единичное/множественное закрытие заявки по цели
        // В любом случае необходимо изменение итератора
        switch (targetsMapIterator->second.targetState)
        {
        case tarTargetSingleEnd:
            _moduleTarTargetSingleEnd(time, targetsMapIterator);

            targetsMapIterator = myTargetsMap.erase(targetsMapIterator);
            break;

        case tarTargetGlobalEnd:
            if (_moduleTarTargetGlobalEnd(time, targetsMapIterator) == true) // Если глобальный обход завершился нормально (нет запускаемых ракет, то можно удалить итератор с состояние "Глобальное завершение")
                targetsMapIterator = myTargetsMap.erase(targetsMapIterator);
            else
                targetsMapIterator++;
            break;
        default:
            targetsMapIterator++;
            break;
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysLaunch::onDisappear(double time)
{
    // Console << "  " << format(time, 7, 2) << " система пуска подготовилась к исчезновению (естественным путём) своего носителя" << EndL;

    myIsSysWorking = false;
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysLaunch::onTaskCmd(const CTaskCmd& cmd)
{
 
    if (cmd.getName() == "Установить.Боекомплект")
        if (cmd.getParam(0) == getType().getWeaponName())
        {
            double myTime = cmd.getTime();
            if (Settings.inDebugMode() == true)
                Console << "  " << format(cmd.getTime(), 7, 2) << " система пуска приняла команду \"" << cmd.getName() << "\" из программы поведения своего носителя" << EndL;
            Film.writeLog(this->getObject().getFilmId(), cmd.getTime(), getType().getShowName() + ": " + cmd.getName());

            myFreeWeaponCount = atoi(cmd.getParam(1).c_str());
            if (myFreeWeaponCount != 0)
                myIsWeaponEmpty = false;
        }

    if (cmd.getName() == "БАБАХ.ЗАПРОС")
    {
        double fTime = cmd.getTime();
        const CObjectList& fObjectList = ObjectList;
        size_t fFilmTargetNum = atoi(cmd.getParam(0).c_str());
        size_t fTargetNum = 0;
        for (int i = 1; i < ObjectList.getObjectCount() + 1; i++)
        {
            size_t fObjectFilmId = ObjectList.getObject(i).getFilmId();
            if (fObjectFilmId == fFilmTargetNum)
            {
                fTargetNum = i;

                CRequestServiceTime fRequestTargetNew(fTargetNum);

                if (this->onRequest(fTime, fRequestTargetNew) == true)
                {
                    if (fRequestTargetNew.isCorrect())
                    {
                        if (Settings.inDebugMode() == true)
                            Console << "  " << format(fTime, 7, 2) << " система пуска. Запрос корректен" << EndL;
                    }
                    else
                    {
                        if (Settings.inDebugMode() == true)
                            Console << "  " << format(fTime, 7, 2) << " система пуска. Запрос некорректен" << EndL;
                    }
                }
            }
        }
    }

    if (cmd.getName() == "БАБАХ")
    {
        double fTime = cmd.getTime();
        const CObjectList& fObjectList = ObjectList;
        size_t fFilmTargetNum = atoi(cmd.getParam(0).c_str());
        size_t fTargetNum = 0;
        for (int i = 1; i < ObjectList.getObjectCount() + 1; i++)
        {
            size_t fObjectFilmId = ObjectList.getObject(i).getFilmId();
            if (fObjectFilmId == fFilmTargetNum)
            {
                fTargetNum = i;

                CImpactLaunchWeapon fImpactAttackTheTarget(fTargetNum);
                this->getObject().processImpact(fTime, fImpactAttackTheTarget);
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysLaunch::onTaskPoint(const CTaskPoint &point)
{
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysLaunch::onImpact(double time, const CImpact &impact)
{
    if (typeid(impact) == typeid(CImpactLaunchWeapon))
        _processImpactLaunchWeapon(time, impact);
    
    if (typeid(impact) == typeid(CImpactWeaponExplosion))
        _processImpactWeaponExplosion(time, impact);

    if (typeid(impact) == typeid(CImpactDisappear))
        _processImpactDisappear(time, impact);

    if (typeid(impact) == typeid(CImpactDestroy))
        _processImpactDestroy(time, impact);

    if (typeid(impact) == typeid(CImpactTargetLost))
        _processImpactTargetLost(time, impact);

    if (typeid(impact) == typeid(CImpactFireDelay))
        _processImpactFireDelay(time, impact);
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
bool CSysLaunch::onRequest(double time, CRequest &request)
{
    // Обработать запрос боекомплекта
    if (typeid(request) == typeid(CRequestAmmunition))
    {
        switch (this->getType().getInfoLevel())
        {
        case SInfoLevel::lvl_0:
            break;
        case SInfoLevel::lvl_2:
            break;
        case SInfoLevel::lvl_1:
        default:
            break;
        }
      
        _processRequestAmmunition(time, request);

        return true;        // Подтвердить, что запрос понят
    }


    if (typeid(request) == typeid(CRequestAmmunitionParam))
    {
        
        _processRequestAmmunitionParam(time, request);

        return true;        // Подтвердить, что запрос понят
    }


    if (typeid(request) == typeid(CRequestServiceTime))
    {
       
        _processRequestServiceTime(time, request);

        return true;        // Подтвердить, что запрос понят
    }

    // Запросы других видов не поддерживаются
    return false;
}


//=====================================================================================================================
// Блок "Проверка систем"
bool CSysLaunch::_moduleSystemCheck(double time)
{
    if (myIsSysWorking == false)
    {
        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " не работает " << EndL;
            Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " не работает " << EndL;
        }

        return false;
    }
    else
        return true;
}


//=====================================================================================================================
// Блок "Подготовка оружия к пуску"
bool CSysLaunch::_moduleTarLaunchPrepare(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // Блокировка всей системы для реализации задержки между пусками
    if (checkLT(time, myStartDelayBlockTime))
        return false;

    if (checkLT(time, targetsMapIterator->second.stateActivationTime))
        return false;

    targetParam newTarget = targetsMapIterator->second;
    CObject& object = ObjectList.getObject(newTarget.targetAbsNum);

    // Проверка цели на наличие в диспетчере моделируемых объектов
    if (ErrorCode != ERR_OK)
    {
        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""Объекта с номером # " << newTarget.targetAbsNum << " не существует""" << EndL;
            Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""Объекта с номером # " << newTarget.targetAbsNum << " не существует""" << EndL;
        }

        targetsMapIterator->second.targetState = tarTargetRefuse;
        return false;
    }

    // Проверка цели на существование
    if (object.isExist() == false)
    {
        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""Объекта с номером # " << newTarget.targetAbsNum << " уже не существует""" << EndL;
            Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""Объекта с номером # " << newTarget.targetAbsNum << " уже не существует""" << EndL;
        }

        targetsMapIterator->second.targetState = tarTargetRefuse;
        return false;
    }

    // Проверка на существование типа вооружения
    size_t typeIndex = ObjectTypeList.find(this->getType().getWeaponName());
    if (typeIndex == INDEX_NOT_FOUND)
    {

        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""не существует тип" << this->getType().getWeaponName() << """" << EndL;
            Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""не существует тип" << this->getType().getWeaponName() << """" << EndL;
        }
        
        targetsMapIterator->second.targetState = tarTargetRefuse;
        return false; // Ошибка, тип myWeaponType не существует!
    }

    // Проверка наличия вооружения
    if ((myIsWeaponEmpty == true)||(this->getType().getWeaponName().empty()==true))
    {
        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""отсутствует " << this->getType().getWeaponName() << """" << EndL;
            Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " отказ в работе : ""отсутствует " << this->getType().getWeaponName() << """" << EndL;
        }

        targetsMapIterator->second.targetState = tarTargetRefuse;
        return false;
    }

        
	//  Резервируем вооружение: выставляем в формуляре цели состояния для вооружения
    if (myFreeWeaponCount == 0)
        return false;
	
    // Все проверки пройдены успешно - возможен переход к следующей стадии
    targetsMapIterator->second.weaponState = wpnReserved;
    myFreeWeaponCount--;

    targetsMapIterator->second.targetState = tarReadyForAttack;
    targetsMapIterator->second.stateActivationTime = 0.;

    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " зарезервировала ракету для цели #" << newTarget.targetAbsNum << EndL;
        Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " зарезервировала ракету для цели #" << newTarget.targetAbsNum << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": " + "Зарезервирована ракета для цели [" + format((uint32)newTarget.targetAbsNum, 4, fillZero) + "]");

    return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Блок "Подготовка ракеты к пуску"
bool CSysLaunch::_moduleTarReadyForAttack(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // Блокировка всей системы для реализации задержки между пусками
    if (checkLT(time, myStartDelayBlockTime))
        return false;

    if (checkLT(time, targetsMapIterator->second.stateActivationTime))
        return false;

    // Можно вставить кусок кода на ожидание внешней команды на заупск, после получения которой формируется время на подготовку и состояние цели

    // Закладываем время на подготовку и меняем состояние цели "Цель подготавливается" 
    targetsMapIterator->second.targetState = tarWeaponPrepare;
    targetsMapIterator->second.stateActivationTime = time;

    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " переведена в состояние подготовки к пуску ракеты по цели #" << targetsMapIterator->second.targetAbsNum << EndL;
        Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " переведена в состояние подготовки к пуску ракеты по цели #" << targetsMapIterator->second.targetAbsNum << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Подготовка к пуску ракеты по цели [" + format((uint32)targetsMapIterator->second.targetAbsNum, 4, fillZero) + "]");

    return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Блок "Подготовка ракеты к пуску"
bool CSysLaunch::_moduleTarWeaponPrepare(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // Блокировка всей системы для реализации задержки между пусками
    if (checkLT(time, myStartDelayBlockTime))
        return false;
    
    // Задержка перед пуском
    if (checkLT(time, targetsMapIterator->second.stateActivationTime))
        return false;

    CObject& jetObject = this->getObject();

    double deltaTimeToMeet = 0.;
    double cumulativeTimeToMeet = double_max;
    double meetTime = 0.;
    CTaskPoint meetPoint;
   
    // Если флаг на запрос времени == true, то запрашиваем время у ракеты
    // Иначе
    // Рассчитываем сами

    double prepareDelay = this->getType().getPrepareDelay();

    CTaskPoint startPoint = jetObject.getSysMotion().computePoint(time + prepareDelay);
    CTaskPoint targetPoint = ObjectList.getObject(targetsMapIterator->second.targetAbsNum).getSysMotion().computePoint(time + prepareDelay);

    // Если взведён флаг на запрос времени встречи цели и ракеты у системы пуска
    if (this->getType().getTimeToMeetRequest() == true)
    {

        // Повторная проверка на существование типа вооружения
        size_t typeIndex = ObjectTypeList.find(this->getType().getWeaponName());
        if (typeIndex == INDEX_NOT_FOUND)
        {
            return false;
        }
        else
        {
            const CObjectType& type = ObjectTypeList.get(typeIndex);
            CObject temporaryObject = type.createObject(0, this->getObject().isRed());
            CRequestTimeToMeetWeapon requestTimeToMeetWeapon;
            requestTimeToMeetWeapon.setStartWeaponPoint(startPoint);
            requestTimeToMeetWeapon.setTargetPoint(targetPoint);

            if (temporaryObject.getSysMotion().onRequest(time, requestTimeToMeetWeapon) == false)
            {
                if (Settings.inDebugMode() == true)
                {
                    Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи не обработан системой движения ракеты" << EndL;
                    Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи не обработан системой движения ракеты" << EndL;
                }

                return false;
            }

            // Проверка корректности полученного ответа
            if (requestTimeToMeetWeapon.isCorrect() == false)
            {
                if (Settings.inDebugMode() == true)
                {
                    Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи обработан системой движения ракеты, но некорректен" << EndL;
                    Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи обработан системой движения ракеты, но некорректен" << EndL;
                }

                return false;
            }

            // Получили время встречи по запросу
            deltaTimeToMeet = requestTimeToMeetWeapon.getDeltaTimeToMeet();
        }
    }
    // Если флаг не взведён, то рассчитываем время до встречи ракеты и цели сами
    else
    {
          //     T Vцt              Vц          Ц
          //Vpt  o------------------o----------o                            
          //      \                         @ /
          //       \                         /
          //        \                       /
          //         \                     /
          //          \                   /
          //           \                 /
          //            \               /
          //             \             /  D
          //              \           /
          //           Vp  o         /
          //                \       /
          //                 \     /
          //                  \   /
          //                   \ /
          //                    o
          //                    C  
    //Допущения:
    //    1) Прямолинейное движение цели и ракеты;
    //    2) Равномерное движение цели и ракеты.
    // 
    //Обозначения:
    //    C  - положение носителя в момент пуска ракеты;
    //    Ц  - положение цели в момент пуска ракеты
    //    T  - точка встречи ракеты и цели
    //    D (вектор ЦС) - дальность от цели до носителя ракеты в момент пуска
    //    Vp - вектор средней скорости ракеты
    //    Vц - вектор скорости цели
    //    Vрt- положение ракеты в момент встречи
    //    Vцt - положение цели в момент встречи
    //    @ - угол между вектором Vц и D(ЦС)

    //        Из теоремы косинусов:
    //    (Vpt) ^ 2 = (Vцt) ^ 2 + D ^ 2 - 2 * (Vцt)*D * cos(@)
    // 
    //        Из скалярного умножение векторов Vц и D можеп получить cos(@):
    //        cos(@) =  Vц D      = VцX*DX + VцY*DY + VцZ*DZ = VцX * CX + VцY * CY + VцZ * CZ
    //                ---------     ------------------------   ------------------------------
    //                |Vц| |D|             |Vц| |D|                      |Vц| |D|

    //        Получаем :
    //        (Vц ^ 2 - Vр ^ 2) * t ^ 2 + (-2 * Vц * D * cos(@)) * t + D ^ 2
    //        ax ^ 2 + bx + c = 0
    //        a = (Vц ^ 2 - Vр ^ 2);
    //    b = (-2 * Vц * D * cos(@));
    //    c = D ^ 2
    //    Дискр = b^2 - 4*a*c
    //                  ______
    //    x1,x2 = -b+-\|Дискр
    //           -------------
    //                 2*a
    
        // Получаем точки в локальной системе координа, расположенной в месте положения цели
        CPointLocal targetPointLocal = targetPoint.getPointGlobal().toPointLocal(targetPoint.getPointGlobal().getPosition());
        CPointLocal startPointLocal = startPoint.getPointGlobal().toPointLocal(targetPoint.getPointGlobal().getPosition());

        CVector3D targetVector = targetPointLocal.getVelocity();
        CVector3D startVector(targetPointLocal.getPosition(), startPointLocal.getPosition());

        // Вычисляем cos(@)
        double cos_a = cos(targetVector.computeAngleWith(startVector));

        // Вычисляем |Vц|, |Vр|, |D|
        double vTarget = targetPoint.getPointGlobal().getVelocity().getLength();
        double vWeapon = this->getType().getVelocity();
        double targetDistance = startPoint.getPointGlobal().getPosition().computeDistanceTo(targetPoint.getPointGlobal().getPosition()); 
        
        // Вычисляем коэффициенты a,b,c
        double a = (pow(vTarget, 2) - pow(vWeapon, 2));
        double b = -(2 * vTarget * targetDistance * cos_a);
        double c = pow(targetDistance, 2);
        double funcDiscriminant = pow(b, 2) - 4 * a * c;

        if (checkLT(funcDiscriminant,0.) == true)
            return false;

        double t1 = (-b - sqrt(funcDiscriminant)) / (2 * a);
        double t2 = (-b + sqrt(funcDiscriminant)) / (2 * a);

        if ((checkGE(t1, 0.) == false) && (checkGE(t2, 0.) == false))
            return false;

        if ((checkGE(t1, 0.) == false) && (checkGE(t2, 0.) == true))
            deltaTimeToMeet = t2;

        if ((checkGE(t1, 0.) == true) && (checkGE(t2, 0.) == false))
            deltaTimeToMeet = t1;

        if ((checkGE(t1, 0.) == true) && (checkGE(t2, 0.) == true))
            deltaTimeToMeet = min(t1, t2);
    }

    cumulativeTimeToMeet = deltaTimeToMeet + prepareDelay;
    meetTime = time + cumulativeTimeToMeet;

    // время встречи должно быть строго больше чем текущее время
    if ( checkLE(meetTime, time) )
        return false;

    const CSysLaunchType& sysLaunchType = this->getType();

    //Вычисление точки встречи
    meetPoint = ObjectList.getObject(targetsMapIterator->second.targetAbsNum).getSysMotion().computePoint(meetTime);

    // Вычисляем точку встречу в местной декартовой СК, котороя привязана к точке старта
    CPointLocal meetPointLocal = meetPoint.getPointGlobal().toPointLocal(startPoint.getPointGlobal().getPosition());

    // Проверка на попадание тчк встречи в зону поражения
    double meetDistance = meetPoint.getPointGlobal().getPosition().computeDistanceTo(startPoint.getPointGlobal().getPosition());

    // Проверка ограничения по дальности границ зоны поражения
    if (checkLT(meetDistance, sysLaunchType.getMinRange()))
        return false;

    if (checkGT(meetDistance, sysLaunchType.getMaxRange()))
        return false;

    // Проверка ограничения по высоте границ зоны поражения
    double meetHeight = meetPointLocal.getPosition().getZ();

    if (checkLT(meetHeight, sysLaunchType.getMinHeight()))
        return false;

    if (checkGT(meetHeight, sysLaunchType.getMaxHeight()))
        return false;

    // Проверка ограничений по углам
    CPointLocal startPointLocal = startPoint.getPointGlobal().toPointLocal(startPoint.getPointGlobal().getPosition());

    // Вычисление направления в азимутальной плоскости направления своего вектора скорости
    double selfRadAzimuth = startPointLocal.computeRadianSelfAzimuth();

    // Поворот плоскости xOy (вид сверху) так, чтобы ось Ox была направлена вдоль проекции вектора скорости на xOy
    startPointLocal.rotatePlaneXY(-selfRadAzimuth);
    meetPointLocal.rotatePlaneXY(-selfRadAzimuth);

    // Вычисление направления в угломестной плоскости направления вектора скорости носителя
    double selfRadElevation = startPointLocal.computeRadianSelfElevation();

    // Поворот плоскости yOz (вид сбоку) так, чтобы ось Oy была направлена "вперёд"
    startPointLocal.rotatePlaneYZ(selfRadElevation);
    meetPointLocal.rotatePlaneYZ(selfRadElevation);

    // Вычисление крена носителя ракеты
    double selfRadRoll = deg2rad(startPointLocal.getRoll());

    // Поворот плоскости zOx (вид сзади) так, чтобы ось Oz была направлена "вверх"
    startPointLocal.rotatePlaneZX(selfRadRoll);
    meetPointLocal.rotatePlaneZX(selfRadRoll);
	
    // Вычисление азимута цели
    double targetDegAzimuth = rad2deg(startPointLocal.computeRadianAzimuthOf(meetPointLocal.getPosition()));
    if (checkLT(targetDegAzimuth, -sysLaunchType.getAzimuthSector()))
        return false;

    if (checkGT(targetDegAzimuth, sysLaunchType.getAzimuthSector()))
        return false;

    // Вычисление угла места цели
    double targetDegElevation = rad2deg(startPointLocal.computeRadianElevationOf(meetPointLocal.getPosition()));
    targetDegElevation -= rad2deg(selfRadElevation);
    if (checkLT(targetDegAzimuth, sysLaunchType.getMinElevation()))
        return false;

    if (checkGT(targetDegAzimuth, sysLaunchType.getMaxElevation()))
        return false;

    targetsMapIterator->second.targetState = tarWeaponStart;
    targetsMapIterator->second.weaponDeltaTimeToMeet = deltaTimeToMeet;
    targetsMapIterator->second.stateActivationTime = time + prepareDelay;

    // блок согласование времени стрельбы
    // носитель не может стрелять залпом со всех орудий, поэтому необходимо определить момент времени, в который будет произведён выстрел, и от него вести расчёт
    CImpactFireDelay impFireDelay(time + prepareDelay + this->getType().getFireDelay());
    this->getObject().processImpact(time, impFireDelay);

    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " переведена в состояние применения ракеты по цели #" << targetsMapIterator->second.targetAbsNum << EndL;
        Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " переведена в состояние применения ракеты по цели #" << targetsMapIterator->second.targetAbsNum << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Примение ракеты по цели [" + format((uint32)targetsMapIterator->second.targetAbsNum, 4, fillZero) + "]");

    return true;
}


//=====================================================================================================================
// Сход оружия
bool CSysLaunch::_moduleTarWeaponStart(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // Если оружие не сошло, то ждём следующий момент времени
    if (checkLT(time, targetsMapIterator->second.stateActivationTime))
        return false;
    
    // Если оружие сошло, то меняем состояние заявки по цели на "Появилось оружие"
    targetsMapIterator->second.targetState = tarWeaponAppearance;
    targetsMapIterator->second.stateActivationTime = time;

    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": ракета, применяемая по цели #" << targetsMapIterator->second.targetAbsNum << ", сход с пилона. Система переведена в состояние появление ракеты" << EndL;
        Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": ракета, применяемая по цели #" << targetsMapIterator->second.targetAbsNum << ", сход с пилона. Система переведена в состояние появление ракеты" << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Сход ракеты с пилона для применения по цели [" + format((uint32)targetsMapIterator->second.targetAbsNum, 4, fillZero) + "]");

    return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Сход ракеты
bool CSysLaunch::_moduleTarWeaponAppearance(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // Если ракета не сошла, то ждём следующий момент времени
    if (checkLT(time, targetsMapIterator->second.stateActivationTime))
        return false;

    // Определяем тип вооружения
    size_t typeIndex = ObjectTypeList.find(getType().getWeaponName());
    const CObjectType& type = ObjectTypeList.get(typeIndex);

    // Создаём ракету и регистрируем ее в системе с указанием родительского объекта и признаком принадлежности
    size_t absNumWeapon = ObjectList.createObject(type,
        this->getObject().getAbsNum(),         // Подчиняем носителю системы
        this->getObject().isRed());            // Принадлежность как у носителя системы 

    // Увеличиваем счётчик пусков
    myWeaponLaunchCount++;

    // В заявке по цели указываем номер ракеты
    targetsMapIterator->second.weaponAbsNum = absNumWeapon;

    // Меняем в заявке по цели состояние ракеты
    targetsMapIterator->second.weaponState = wpnApplied;
	   
    //Формируем маршрут для зарегестрированной ракеты
    // Вычислить текущее положение своего носителя
    CTaskPoint startPos = this->getObject().getSysMotion().computePoint(time);

    // Вычислить положение цели в момент предпологаемого времени встречи
    CTaskPoint targetPos = ObjectList.getObject(targetsMapIterator->second.targetAbsNum).getSysMotion().computePoint(time+targetsMapIterator->second.weaponDeltaTimeToMeet);

    // Сформировать начальный маршрут движения для ракеты
    vector<CTaskPoint> route;
    route.push_back(startPos);
    route.push_back(targetPos);

    // Присваивание начального маршрута новому объекту
    CObject& weaponObject = ObjectList.getObject(absNumWeapon);
    weaponObject.getSysMotion().setInitialRoute(route);
	   
    // Формируем воздействие на ракету, в котором указываем цель, на котороую надо наводиться ракете
    // Формирование объекта-воздействия
    CImpactWeaponLaunchSimple impactWeaponLaunchSimple(targetsMapIterator->second.targetAbsNum);
    weaponObject.processImpact(time, impactWeaponLaunchSimple);

    // Появление ракеты
    weaponObject.appear(time);

    // Оповещаем цель о применияемом оружие и о том, кто применяет оружие
    // Формирование объекта-воздействия
    CImpactAttackAllarm impactAttackAllarm(absNumWeapon, this->getObject().getAbsNum());
    ObjectList.getObject(targetsMapIterator->second.targetAbsNum).processImpact(time, impactAttackAllarm);

    // Меняем формуляр по цели на "Полёт ракеты"
    targetsMapIterator->second.targetState = tarWeaponFlight;
    targetsMapIterator->second.stateActivationTime = time + this->getType().getMaxLifeTime();

    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": переведена в состояние ожидания поражения цели #" << targetsMapIterator->second.targetAbsNum << EndL;
        Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": переведена в состояние ожидания поражения цели #" << targetsMapIterator->second.targetAbsNum << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Появление ракеты [" + format((uint32)targetsMapIterator->second.weaponAbsNum, 4, fillZero) + "] для применения по цели [" + format((uint32)targetsMapIterator->second.targetAbsNum, 4, fillZero) + "]");
	    	
    return true;
}


//=====================================================================================================================
// Полёт ракеты
bool CSysLaunch::_moduleTarWeaponFlight(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // В данном случае время активации состояния играет другую роль: мы проверяем не превышено ли время жизни ракеты, если превышено, то прекращаем работу по цели
    if (checkLT(time, targetsMapIterator->second.stateActivationTime))
        return false;

    // Время жизни ракеты превышено и никаких воздействий меняющих состояние цели не поступила, следовательно, ракета не поразила цель
    ObjectList.getObject(targetsMapIterator->second.weaponAbsNum).disappear(time);

    //Формируем воздействие, оповещающие о промахе по цели и передайм объекту-носителю
    CImpactTargetMiss impactTargetMiss(targetsMapIterator->second.targetAbsNum);
    this->getObject().processImpact(time, impactTargetMiss);

    targetsMapIterator->second.targetState = tarTargetSingleEnd;
    targetsMapIterator->second.weaponState = wpnDissapear;
    targetsMapIterator->second.stateActivationTime = 0.;

    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Превышено максимальное время полёта ракеты #" << targetsMapIterator->second.weaponAbsNum << EndL;
        Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Превышено максимальное время полёта ракеты #" << targetsMapIterator->second.weaponAbsNum << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Превышено максимальное время полёта ракеты [" + format((uint32)targetsMapIterator->second.weaponAbsNum, 4, fillZero) + "]");

    return true;
}


//=====================================================================================================================
// Потеря цели
bool CSysLaunch::_moduleTarTargetLost(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // Планируем исчезновение ракеты
    ObjectList.getObject(targetsMapIterator->second.weaponAbsNum).disappear(time);

    //Формируем воздействие, оповещающие о промахе по цели и передаём объекту-носителю
    CImpactTargetMiss impactTargetMiss(targetsMapIterator->second.targetAbsNum);
    this->getObject().processImpact(time, impactTargetMiss);

    targetsMapIterator->second.targetState = tarTargetSingleEnd;
    targetsMapIterator->second.weaponState = wpnDissapear;
    targetsMapIterator->second.stateActivationTime = 0.;

    return false;
}


//=====================================================================================================================
// Отказ от цели
bool CSysLaunch::_moduleTarTargetRefuse(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
    // Формирование объекта-воздействия
    CImpactTargetRefuse impact(targetsMapIterator->second.targetAbsNum);

    // Передача воздействия объекту-носителю
    getObject().processImpact(time, impact);

    targetsMapIterator->second.targetState = tarTargetSingleEnd;
    targetsMapIterator->second.stateActivationTime = 0.;

    return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Потеря цели
bool CSysLaunch::_moduleTarTargetSingleEnd(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
	CObject& usedWeapon = ObjectList.getObject(targetsMapIterator->second.weaponAbsNum);

	switch (targetsMapIterator->second.weaponState)
	{
	case wpnReserved:
		myFreeWeaponCount++;
		break;
	case wpnApplied:
		if (ErrorCode != ERR_OK)
		{
			targetsMapIterator->second.weaponAbsNum = 0;
			targetsMapIterator->second.weaponState = wpnEmpty;

			return true;
		}

		if (usedWeapon.isExist())
			usedWeapon.disappear(time);

        targetsMapIterator->second.weaponState = wpnDissapear;
		break;
	case wpnDissapear:
	case wpnDestroy:
	case wpnExplode:
		break;
	}

	return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Потеря цели
bool CSysLaunch::_moduleTarTargetGlobalEnd(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
	bool moduleResult = true;

	typedef std::multimap<size_t, targetParam>::iterator multiMapIteratorType;
	pair<multiMapIteratorType, multiMapIteratorType>mapBeginEnd = myTargetsMap.equal_range(targetsMapIterator->second.targetAbsNum);

	multiMapIteratorType multiMapIterator;
	for (multiMapIterator = mapBeginEnd.first; multiMapIterator != mapBeginEnd.second; multiMapIterator++)
	{
		switch (multiMapIterator->second.targetState)
		{
		case tarEmpty:
		case tarLaunchPrepare:
			multiMapIterator->second.targetState = tarTargetSingleEnd;
			multiMapIterator->second.stateActivationTime = 0.;
			break;
		case tarReadyForAttack:
		case tarWeaponPrepare:
			multiMapIterator->second.targetState = tarTargetSingleEnd;
			multiMapIterator->second.stateActivationTime = 0.;
			break;

		// Когда ракета сходит с пилона или появляется, то нам необходимо отложить глобальное завершение, чтобы корректно завершить работу ракет
		case tarWeaponStart:
		case tarWeaponAppearance:
			moduleResult = false;
			break;
		case tarWeaponFlight:
			multiMapIterator->second.targetState = tarTargetSingleEnd;
			multiMapIterator->second.stateActivationTime = 0.;
			break;
		case tarWeaponExplode:
		case tarTargetLost:
		case tarTargetSingleEnd:
		case tarTargetGlobalEnd:
		case tarTargetRefuse:
			break;
		default:
			break;
		}
	}

	return moduleResult;
}


//=====================================================================================================================
// Подрыв ракеты
bool CSysLaunch::_moduleImpTarWeaponExplode(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
	CObject& attackedTarget = ObjectList.getObject(targetsMapIterator->second.targetAbsNum);
	if (ErrorCode != ERR_OK)
	{
		if (Settings.inDebugMode() == true)
		{
			Console << "  " << format(time, 7, 2) << "номер цели №" << targetsMapIterator->second.targetAbsNum << " оказался некорректным до подрыва ракеты" << EndL;
			Log << "  " << format(time, 7, 2) << "номер цели №" << targetsMapIterator->second.targetAbsNum << " оказался некорректным до подрыва ракеты" << EndL;
		}
		return false;
	}

    CObject& usedWeapon = ObjectList.getObject(targetsMapIterator->second.weaponAbsNum);
	if (ErrorCode != ERR_OK)
	{
		if (Settings.inDebugMode() == true)
		{
			Console << "  " << format(time, 7, 2) << "номер ракеты №" << targetsMapIterator->second.targetAbsNum << " оказался некорректным до подрыва ракеты" << EndL;
			Log << "  " << format(time, 7, 2) << "номер ракеты №" << targetsMapIterator->second.targetAbsNum << " оказался некорректным до подрыва ракеты" << EndL;
		}
		return false;
	}
	
    // Цель в момент подрыва не существует
	if (attackedTarget.isExist() == false)
	{
		if (Settings.inDebugMode() == true)
			Console << "цель №" << targetsMapIterator->second.targetAbsNum << "не существует" << EndL;

		// Оповещаем системы, что цель не существует
		CImpactTargetRefuse myImpactTargetRefuse(targetsMapIterator->second.targetAbsNum);
		this->getObject().processImpact(time, myImpactTargetRefuse);

        // Убираем ракету из моделирования в момент получения импакта myImpactTargetRefuse
		if (usedWeapon.isExist())
			usedWeapon.disappear(time);

        // Планируем глобальное закрытие заявок по несуществующей цели
		targetsMapIterator->second.targetState = tarTargetGlobalEnd;
		targetsMapIterator->second.weaponState = wpnExplode;
        targetsMapIterator->second.stateActivationTime = 0.;

		return false;
	}
	
	// Ракеты в момент подрыва не существует
	if (usedWeapon.isExist() == false)
	{
		if (Settings.inDebugMode() == true)
			Console << "ракета №" << targetsMapIterator->second.targetAbsNum << "не существует" << EndL;

		// Оповещаем системы о промахе
		CImpactTargetMiss impactTargetMiss(targetsMapIterator->second.targetAbsNum);
		this->getObject().processImpact(time, impactTargetMiss);

		// Планируем закрытие одиночной заявки по цели
		targetsMapIterator->second.targetState = tarTargetSingleEnd;
		targetsMapIterator->second.weaponState = wpnExplode;
		targetsMapIterator->second.stateActivationTime = 0.;

		return false;
	}


    // --------------------------------------------------------------------------------------------------------------
    // Цель и ракета в момент подрыва существуют, поэтому производим расчёт дальностей и вероятности поражения

	CPointGlobal targetPointGlobal = attackedTarget.getSysMotion().computePoint(time).getPointGlobal();
	CPointGlobal weaponExlodePointGlobal = usedWeapon.getSysMotion().computePoint(time).getPointGlobal();

	// Если ракета взорвалась слишком далеко от цели
	if (checkLT(getType().getHitRadius(), weaponExlodePointGlobal.getPosition().computeDistanceTo(targetPointGlobal.getPosition())))
	{
		// Убираем ракету из моделирования в момент получения импакта
		if (usedWeapon.isExist())
			usedWeapon.disappear(time);

		// Оповещаем системы о промахе
		CImpactTargetMiss impactTargetMiss(targetsMapIterator->second.targetAbsNum);
		this->getObject().processImpact(time, impactTargetMiss);

        // Планируем закрытие одиночной заявки по цели
		targetsMapIterator->second.targetState = tarTargetSingleEnd;
		targetsMapIterator->second.weaponState = wpnExplode;
		targetsMapIterator->second.stateActivationTime = 0.;

		if (Settings.inDebugMode() == true)
		{
			Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " подрыв ракеты #" << targetsMapIterator->second.weaponAbsNum << " вне радиуса поражения" << EndL;
			Log << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " подрыв ракеты #" << targetsMapIterator->second.weaponAbsNum << " вне радиуса поражения" << EndL;
		}
		Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Подрыв ракеты [" + format((uint32)targetsMapIterator->second.weaponAbsNum, 4, fillZero) + "] вне радиуса поражения" + " <" + getType().getName() + ">");
				
		return false;
	}
	
	// Ракета взорвалась достаточно близко, разыгрываем поражение цели

	double generatedProbability = double(rand()) / double(RAND_MAX);

	// Полученная вероятность поражения оказалась выше, чем необходимая вероятность поражения, поэтому промах
	if (checkGT(generatedProbability, getType().getHitProbability()))
	{
		// Убираем ракету из моделирования в момент получения импакта
		if (usedWeapon.isExist())
			usedWeapon.disappear(time);

		// Оповещаем системы о промахе
		CImpactTargetMiss impactTargetMiss(targetsMapIterator->second.targetAbsNum);
		this->getObject().processImpact(time, impactTargetMiss);

        // Планируем закрытие одиночной заявки по цели
		targetsMapIterator->second.targetState = tarTargetSingleEnd;
		targetsMapIterator->second.weaponState = wpnExplode;
		targetsMapIterator->second.stateActivationTime = 0.;

		if (Settings.inDebugMode() == true)
		{
			Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " ракета #" << targetsMapIterator->second.weaponAbsNum << " промах по цели" << EndL;
			Log << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " ракета #" << targetsMapIterator->second.weaponAbsNum << " промах по цели" << EndL;
		}
		Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Ракета [" + format((uint32)targetsMapIterator->second.weaponAbsNum, 4, fillZero) + "] промахнулась по цели [" + format((uint32)targetsMapIterator->second.targetAbsNum, 4, fillZero) + "]: rand = " + format(generatedProbability) + ", порог = " + format(getType().getHitProbability()));
				
		return false;
	}

	// Если цель в зоне поражения ракеты и полученная вероятность поражения меньше либо равна необходимой вероятности, то
	// поразили цель
	CImpactTargetHit impactTargetHit(targetsMapIterator->second.targetAbsNum);
	this->getObject().processImpact(time, impactTargetHit);
	attackedTarget.destroy(time);

	// Убираем ракету из моделирования в момент получения импакта
	if (usedWeapon.isExist())
		usedWeapon.disappear(time);

    // Планируем глобальное закрытие заявок по несуществующей цели
	targetsMapIterator->second.targetState = tarTargetGlobalEnd;
	targetsMapIterator->second.weaponState = wpnExplode;
	targetsMapIterator->second.stateActivationTime = 0.;

	Console << "  " << format(time, 7, 2) << "  система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " ракета #" << targetsMapIterator->second.weaponAbsNum << " Поражение цели #" << targetsMapIterator->second.targetAbsNum << EndL;
	Log << "  " << format(time, 7, 2) << "  система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " ракета #" << targetsMapIterator->second.weaponAbsNum << " Поражение цели #" << targetsMapIterator->second.targetAbsNum << EndL;
	Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Ракета [" + format((uint32)targetsMapIterator->second.weaponAbsNum, 4, fillZero) + "] поразила цель [" + format((uint32)targetsMapIterator->second.targetAbsNum, 4, fillZero) + "]: rand = " + format(generatedProbability) + ", порог = " + format(getType().getHitProbability()));

	return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Подрыв ракеты
bool CSysLaunch::_moduleImpTarWeaponDisapear(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
	targetsMapIterator->second.targetState = tarTargetSingleEnd;
	targetsMapIterator->second.weaponState = wpnExplode;
	targetsMapIterator->second.stateActivationTime = 0.;

	return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Подрыв ракеты
bool CSysLaunch::_moduleImpTarWeaponDestroy(double time, std::multimap<size_t, targetParam>::iterator targetsMapIterator)
{
	// Оповещаем системы о промахе
	CImpactTargetMiss impactTargetMiss(targetsMapIterator->second.targetAbsNum);
	this->getObject().processImpact(time, impactTargetMiss);

	targetsMapIterator->second.targetState = tarTargetSingleEnd;
	targetsMapIterator->second.weaponState = wpnExplode;
	targetsMapIterator->second.stateActivationTime = 0.;

	return true;
}


////=====================================================================================================================
//// Обработка воздействия "Команда на пуск ракеты по цели" от средства обнаружения
void CSysLaunch::_processImpactLaunchWeapon(double time, const CImpact& impact)
{
    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие на пуск ракеты" << EndL;
        Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие на пуск ракеты" << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Применение ракеты");

    const CImpactLaunchWeapon& impTargetNew = static_cast<const CImpactLaunchWeapon&>(impact);

    targetParam newTarget;
    newTarget.targetAbsNum = impTargetNew.getTargetNum();
    
    if (getType().getMultipleTargerService() == true)
    {
        newTarget.targetState = tarLaunchPrepare;
        myTargetsMap.insert({ newTarget.targetAbsNum, newTarget });
    }
    else
    {
        std::multimap<size_t, targetParam>::iterator targetsMapIterator;
        for (targetsMapIterator = myTargetsMap.begin(); targetsMapIterator != myTargetsMap.end(); targetsMapIterator++)
            if (targetsMapIterator->second.targetAbsNum == newTarget.targetAbsNum)
                return;

        newTarget.targetState = tarLaunchPrepare;
        myTargetsMap.insert({ newTarget.targetAbsNum, newTarget });
    }

}


//=====================================================================================================================
// Реакция на уведомление о потере цели
void CSysLaunch::_processImpactTargetLost(double time, const CImpact& impact)
{
    const CImpactTargetLost& impTargetLost = static_cast<const CImpactTargetLost&>(impact);

    if (!this->getType().getFireAndForget())
    {
        typedef std::multimap<size_t, targetParam>::iterator myMultiMapIterator;
        pair<myMultiMapIterator, myMultiMapIterator>myBeginEnd = myTargetsMap.equal_range(impTargetLost.getAbsNum());

        myMultiMapIterator multiMapIterator;
        for (multiMapIterator = myBeginEnd.first; multiMapIterator != myBeginEnd.second; multiMapIterator++)
            multiMapIterator->second.targetState = tarTargetLost;

        Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Потеря объекта [" + format((uint32)impTargetLost.getAbsNum(), 4, fillZero) + "]. \"Выстрелил и забыл\" - нет");
        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Потеря объекта"". \"Выстрелил и забыл\" - нет" << EndL;
            Log << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Потеря объекта"". \"Выстрелил и забыл\" - нет" << EndL;
        }
    }
    else
    {
        Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Потеря объекта [" + format((uint32)impTargetLost.getAbsNum(), 4, fillZero) + "]. \"Выстрелил и забыл\" - да");
        if (Settings.inDebugMode() == true)
        {
            Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Потеря объекта"". \"Выстрелил и забыл\" - да" << EndL;
            Log << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Потеря объекта"". \"Выстрелил и забыл\" - да" << EndL;
        }
    }       
}


//=====================================================================================================================
// Реакция на уведомление о подрыве ракеты
void CSysLaunch::_processImpactWeaponExplosion(double time, const CImpact& impact)
{   
    const CImpactWeaponExplosion& impWeaponExploison = static_cast<const CImpactWeaponExplosion&>(impact);

    size_t weaponAbsNum = impWeaponExploison.getAbsNum();
    std::multimap<size_t, targetParam>::iterator targetsMapIterator;
    for (targetsMapIterator = myTargetsMap.begin(); targetsMapIterator != myTargetsMap.end(); targetsMapIterator++)
        if (targetsMapIterator->second.weaponAbsNum == weaponAbsNum)
        {
            if (Settings.inDebugMode() == true)
            {
                Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Подрыв ракеты"" от объекта # " << weaponAbsNum << EndL;
                Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Подрыв ракеты"" от объекта # " << weaponAbsNum << EndL;
            }
            Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Подрыв ракеты [" + format((uint32)weaponAbsNum, 4, fillZero) + "]");

            targetsMapIterator->second.targetState = tarWeaponExplode;
            targetsMapIterator->second.weaponState = wpnExplode;

            _moduleImpTarWeaponExplode(time, targetsMapIterator);
        }
}


//=====================================================================================================================
// Реакция на уведомление об исчезновении кого-нибудь
void CSysLaunch::_processImpactDisappear(double time, const CImpact& impact)
{
    const CImpactDisappear& impDisappear = static_cast<const CImpactDisappear&>(impact);

    size_t objAbsNum = impDisappear.getAbsNum();
    std::multimap<size_t, targetParam>::iterator targetsMapIterator;

    // Проходим по списку заявок и ищем, что прекратило существование: ракета или цель
    for (targetsMapIterator = myTargetsMap.begin(); targetsMapIterator != myTargetsMap.end(); targetsMapIterator++)
    {
        if (targetsMapIterator->second.weaponAbsNum == objAbsNum)
        {
            if (Settings.inDebugMode() == true)
            {
                Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Исчезновение моделируемого объекта""  от объекта # " << objAbsNum << EndL;
                Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Исчезновение моделируемого объекта"" от объекта # " << objAbsNum << EndL;
            }
            Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Исчезновение ракеты [" + format((uint32)objAbsNum, 4, fillZero) + "]");

            _moduleImpTarWeaponDisapear(time, targetsMapIterator);

            return;
        }

        if (targetsMapIterator->second.targetAbsNum == objAbsNum)
        {
            targetsMapIterator->second.targetState = tarTargetLost;

            if (Settings.inDebugMode() == true)
            {
                Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Исчезновение моделируемого объекта""  от объекта # " << objAbsNum << EndL;
                Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Исчезновение моделируемого объекта"" от объекта # " << objAbsNum << EndL;
            }
            Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Исчезновение цели [" + format((uint32)objAbsNum, 4, fillZero) + "]");

            return;
        }
    }
}


//=====================================================================================================================
// Реакция на уведомление о поражении кого-нибудь
void CSysLaunch::_processImpactDestroy(double time, const CImpact& impact)
{
    const CImpactDestroy& impDestroy = static_cast<const CImpactDestroy&>(impact);

    // Проверяем, уничтожена ли применяемая ракета
    size_t objAbsNum = impDestroy.getAbsNum();
    std::multimap<size_t, targetParam>::iterator targetsMapIterator;
    for (targetsMapIterator = myTargetsMap.begin(); targetsMapIterator != myTargetsMap.end(); targetsMapIterator++)
    {
        if (targetsMapIterator->second.weaponAbsNum == objAbsNum)
        {
            if (Settings.inDebugMode() == true)
            {
                Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Уничтожение моделируемого объекта""  от объекта # " << objAbsNum << EndL;
                Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Уничтожение моделируемого объекта"" от объекта # " << objAbsNum << EndL;
            }
            Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Уничтожение ракеты [" + format((uint32)objAbsNum, 4, fillZero) + "]");

            _moduleImpTarWeaponDestroy(time, targetsMapIterator);
        }

        if (targetsMapIterator->second.targetAbsNum == objAbsNum)
        {

            targetsMapIterator->second.targetState = tarTargetGlobalEnd;

            if (Settings.inDebugMode() == true)
            {
                Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Исчезновение моделируемого объекта""  от объекта # " << objAbsNum << EndL;
                Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие ""Исчезновение моделируемого объекта"" от объекта # " << objAbsNum << EndL;
            }
            Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Уничтожение цели [" + format((uint32)objAbsNum, 4, fillZero) + "]");

            return;
        }
    }
}


//=====================================================================================================================
// Реакция на уведомление о задержке пуска ракеты
void CSysLaunch::_processImpactFireDelay(double time, const CImpact& impact)
{
    const CImpactFireDelay& impFireDelay = static_cast<const CImpactFireDelay&>(impact);
    myStartDelayBlockTime = impFireDelay.getDelayTime() - this->getType().getPrepareDelay();

    if (Settings.inDebugMode() == true)
    {
        Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие \"Задержка между пусками\"" << EndL;
        Log << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << " получила воздействие \"Задержка между пусками\"" << EndL;
    }
    Film.writeLog(this->getObject().getFilmId(), time, getType().getShowName() + ": Задержка между пусками");
}


//=====================================================================================================================
// Реакция на запрос боекомплекта
void CSysLaunch::_processRequestAmmunition(double time, CRequest& request)
{
    // Доступ к запросу
    CRequestAmmunition& r = (CRequestAmmunition&)request;

    if (myIsSysWorking == false)
    {
        r.discard();
        return;
    }
    else
    {
        r.setWeaponName(this->getType().getWeaponName());
        r.setAmmunition(myFreeWeaponCount);

        r.acknowledge();
        return;
    }


}


//--------------------------------------------------------------------------------------------------------------------- 
// Реакция на запрос параметров боекомплекта
void CSysLaunch::_processRequestAmmunitionParam(double time, CRequest& request)
{
    // Доступ к запросу
    CRequestAmmunitionParam& r = (CRequestAmmunitionParam&)request;

    if (myIsSysWorking == false)
    {
        r.discard();
        return;
    }
    else
    {
        r.setWeaponName(this->getType().getWeaponName());

        r.setMissile_DpmMax(this->getType().getMaxRange());
        r.setMissile_sekt(this->getType().getAzimuthSector());
        r.setR_os(this->getType().getHitRadius());

        r.acknowledge();
        return;
    }
}


//=====================================================================================================================
// Реакция на запрос предполагаемого времени обслуживания цели
void CSysLaunch::_processRequestServiceTime(double time, CRequest& request)
{
    // Доступ к запросу
    CRequestServiceTime& r = (CRequestServiceTime&)request;

    if (myIsSysWorking == false)
    {
        r.discard();
        return;
    }
	    
    double timeForAttack = max(time, myStartDelayBlockTime);

    size_t targetAbsNum = r.getTargetNum();

    CObject& objTarget = ObjectList.getObject(targetAbsNum);

    // Проверка цели на наличие в диспетчере моделируемых объектов
    if (ErrorCode != ERR_OK)
    {
        r.setServiceTime(double_max);
        return;
    }
        
    // Проверка цели на существование
    if (objTarget.isExist() == false)
    {
        r.setServiceTime(double_max);
        return;
    }

    // Проверка на существование типа вооружения
    size_t typeIndex = ObjectTypeList.find(this->getType().getWeaponName());
    if (typeIndex == INDEX_NOT_FOUND)
    {
        r.discard();
        return;
    }

    if (this->getType().getWeaponName().empty() == true)
    {
        r.discard();
        return;
    }

    // Проверка наличия вооружения
    if (myIsWeaponEmpty == true)
    {
        r.setServiceTime(double_max);
        return;
    }

    	
    //  Резервируем ракету: выставляем в формуляре цели состояния для ракеты
    if (myFreeWeaponCount == 0)
    {
        r.setServiceTime(double_max);
        return;
    }


    // Пуск ракеты возможен
    // ___________________________________________
    //  Рассчёт точки встречи

    double deltaTimeToMeet = 0.;
    double cumulativeTimeToMeet = double_max;
    double meetTime = 0.;
    CTaskPoint meetPoint;
    // Если флаг на запрос времени == true, то запрашиваем время у ракеты
    // иначе
    // рассчитываем сами

    CObject& myPlane = this->getObject();

    double prepareDelay = this->getType().getPrepareDelay();

    CTaskPoint startPoint = myPlane.getSysMotion().computePoint(timeForAttack + prepareDelay);
    CTaskPoint targetPoint = ObjectList.getObject(targetAbsNum).getSysMotion().computePoint(timeForAttack + prepareDelay);

    // Если взведён флаг на запрос времени встречи цели и ракеты у системы пуска
    if (this->getType().getTimeToMeetRequest() == true)
    {
        // Повторная проверка на существование типа вооружения
        size_t typeIndex = ObjectTypeList.find(this->getType().getWeaponName());
        if (typeIndex == INDEX_NOT_FOUND)
        {
            r.discard();
            return;
        }
        else
        {
            const CObjectType& type = ObjectTypeList.get(typeIndex);
            CObject temporaryObject = type.createObject(0, this->getObject().isRed());
            CRequestTimeToMeetWeapon requestTimeToMeetWeapon;
            requestTimeToMeetWeapon.setStartWeaponPoint(startPoint);
            requestTimeToMeetWeapon.setTargetPoint(targetPoint);

            if (temporaryObject.getSysMotion().onRequest(timeForAttack, requestTimeToMeetWeapon) == false)
            {
                if (Settings.inDebugMode() == true)
                {
                    Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи не обработан системой движения ракеты" << EndL;
                    Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи не обработан системой движения ракеты" << EndL;
                }

                r.discard();
                return;
            }

            // Проверка корректности полученного ответа
            if (requestTimeToMeetWeapon.isCorrect() == false)
            {
                if (Settings.inDebugMode() == true)
                {
                    Console << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи обработан системой движения ракеты, но некорректен" << EndL;
                    Log     << "  " << format(time, 7, 2) << " система пуска " << this->getType().getWeaponName() << " объекта # " << this->getObject().getAbsNum() << ": Запрос на время встречи обработан системой движения ракеты, но некорректен" << EndL;
                }

                r.setServiceTime(double_max);
                return;
            }

            // Получили время встречи по запросу
            deltaTimeToMeet = requestTimeToMeetWeapon.getDeltaTimeToMeet();
        }
    }
    // Если флаг не взведён, то рассчитываем время до встречи ракеты и цели сами
    else
    {
        //     T Vцt              Vц          Ц
        //Vpt  o------------------o----------o                            
        //      \                         @ /
        //       \                         /
        //        \                       /
        //         \                     /
        //          \                   /
        //           \                 /
        //            \               /
        //             \             /  D
        //              \           /
        //           Vp  o         /
        //                \       /
        //                 \     /
        //                  \   /
        //                   \ /
        //                    o
        //                    C  
  //Допущения:
  //    1) Прямолинейное движение цели и ракеты;
  //    2) Равномерное движение цели и ракеты.
  // 
  //Обозначения:
  //    C  - положение носителя в момент пуска ракеты;
  //    Ц  - положение цели в момент пуска ракеты
  //    T  - точка встречи ракеты и цели
  //    D(вектор ЦС) - дальность от цели до носителя в момент пуска
  //    Vp - вектор средней скорости ракеты
  //    Vц - вектор скорости цели
  //    Vрt- положение ракеты в момент встречи
  //    Vцt - положение цели в момент встречи
  //    @ - угол между вектором Vц и D(ЦС)

  //        Из теоремы косинусов:
  //    (Vpt) ^ 2 = (Vцt) ^ 2 + D ^ 2 - 2 * (Vцt)*D * cos(@)
  // 
  //        Из скалярного умножение векторов Vц и D можеп получить cos(@):
  //        cos(@) =  Vц D      = VцX*DX + VцY*DY + VцZ*DZ = VцX * CX + VцY * CY + VцZ * CZ
  //                ---------     ------------------------   ------------------------------
  //                |Vц| |D|             |Vц| |D|                      |Vц| |D|

  //        Получаем :
  //        (Vц ^ 2 - Vр ^ 2) * t ^ 2 + (-2 * Vц * D * cos(@)) * t + D ^ 2
  //        ax ^ 2 + bx + c = 0
  //        a = (Vц ^ 2 - Vр ^ 2);
  //    b = (-2 * Vц * D * cos(@));
  //    c = D ^ 2
  //    Дискр = b^2 - 4*a*c
  //                  ______
  //    x1,x2 = -b+-\|Дискр
  //           -------------
  //                 2*a

      // Получаем точки в локальной системе координат, расположенной в месте положения цели
        CPointLocal targetPointLocal = targetPoint.getPointGlobal().toPointLocal(targetPoint.getPointGlobal().getPosition());
        CPointLocal startPointLocal = startPoint.getPointGlobal().toPointLocal(targetPoint.getPointGlobal().getPosition());

        CVector3D targetVector = targetPointLocal.getVelocity();
        CVector3D startVector(targetPointLocal.getPosition(), startPointLocal.getPosition());

        // Вычисляем cos(@)
        double cos_a = cos(targetVector.computeAngleWith(startVector));

        // Вычисляем |Vц|, |Vр|, |D|
        double vTarget = targetPoint.getPointGlobal().getVelocity().getLength();
        double vWeapon = this->getType().getVelocity();
        double targetDistance = startPoint.getPointGlobal().getPosition().computeDistanceTo(targetPoint.getPointGlobal().getPosition());

        // Вычисляем коэффициенты a,b,c
        double a = (pow(vTarget, 2) - pow(vWeapon, 2));
        double b = -(2 * vTarget * targetDistance * cos_a);
        double c = pow(targetDistance, 2);
        double funcDiscriminant = pow(b, 2) - 4 * a * c;

        if (checkLT(funcDiscriminant, 0.) == true)
        {
            r.setServiceTime(double_max);
            return;
        }

        double t1 = (-b - sqrt(funcDiscriminant)) / (2 * a);
        double t2 = (-b + sqrt(funcDiscriminant)) / (2 * a);

        if ((checkGE(t1, 0.) == false) && (checkGE(t2, 0.) == false))
        {
            r.setServiceTime(double_max);
            return;
        }

        if ((checkGE(t1, 0.) == false) && (checkGE(t2, 0.) == true))
            deltaTimeToMeet = t2;

        if ((checkGE(t1, 0.) == true) && (checkGE(t2, 0.) == false))
            deltaTimeToMeet = t1;

        if ((checkGE(t1, 0.) == true) && (checkGE(t2, 0.) == true))
            deltaTimeToMeet = min(t1, t2);
    }

    cumulativeTimeToMeet = deltaTimeToMeet + prepareDelay;
    meetTime = timeForAttack + cumulativeTimeToMeet;

    // Время встречи должно быть больше чем время пуска
    if (checkLE(meetTime, timeForAttack))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }
	
    const CSysLaunchType& myType = this->getType();

    //Вычисление точки встречи
    meetPoint = ObjectList.getObject(targetAbsNum).getSysMotion().computePoint(meetTime);

    // Вычисляем точку встречу в местной декартовой СК, котороя привязана к точке старта
    CPointLocal meetPointLocal = meetPoint.getPointGlobal().toPointLocal(startPoint.getPointGlobal().getPosition());

    // Проверка на попадание точки встречи в зону поражения
    double meetDistance = meetPoint.getPointGlobal().getPosition().computeDistanceTo(startPoint.getPointGlobal().getPosition());

    // Проверка ограничения по дальности границ зоны поражения
    if (checkLT(meetDistance, myType.getMinRange()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }

    if (checkGT(meetDistance, myType.getMaxRange()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }

    // Проверка ограничения по высоте границ зоны поражения
    double meetHeight = meetPointLocal.getPosition().getZ();

    if (checkLT(meetHeight, myType.getMinHeight()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }

    if (checkGT(meetHeight, myType.getMaxHeight()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }
	
    // Проверка ограничений по углам
    CPointLocal startPointLocal = startPoint.getPointGlobal().toPointLocal(startPoint.getPointGlobal().getPosition());

    // Вычисление направления в азимутальной плоскости направления своего вектора скорости
    double selfRadAzimuth = startPointLocal.computeRadianSelfAzimuth();

    // Поворот плоскости xOy (вид сверху) так, чтобы ось Ox была направлена вдоль проекции вектора скорости на xOy
    startPointLocal.rotatePlaneXY(-selfRadAzimuth);
    meetPointLocal.rotatePlaneXY(-selfRadAzimuth);

    // Вычисление направления в угломестной плоскости направления вектора скорости носителя
    double selfRadElevation = startPointLocal.computeRadianSelfElevation();

    // Поворот плоскости yOz (вид сбоку) так, чтобы ось Oy была направлена "вперёд"
    startPointLocal.rotatePlaneYZ(selfRadElevation);
    meetPointLocal.rotatePlaneYZ(selfRadElevation);

    // Вычисление крена носителя
    double selfRadRoll = deg2rad(startPointLocal.getRoll());

    // Поворот плоскости zOx (вид сзади) так, чтобы ось Oz была направлена "вверх"
    startPointLocal.rotatePlaneZX(selfRadRoll);
    meetPointLocal.rotatePlaneZX(selfRadRoll);
	
    // Вычисление азимута цели
    double targetDegAzimuth = rad2deg(startPointLocal.computeRadianAzimuthOf(meetPointLocal.getPosition()));
    if (checkLT(targetDegAzimuth, -myType.getAzimuthSector()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }

    if (checkGT(targetDegAzimuth, myType.getAzimuthSector()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }

    // Вычисление угла места цели
    double targetDegElevation = rad2deg(startPointLocal.computeRadianElevationOf(meetPointLocal.getPosition()));
    targetDegElevation -= rad2deg(selfRadElevation);
    if (checkLT(targetDegAzimuth, myType.getMinElevation()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }

    if (checkGT(targetDegAzimuth, myType.getMaxElevation()))
    {
        r.setServiceTime(cumulativeTimeToMeet);
        return;
    }

    r.setServiceTime(cumulativeTimeToMeet);

    // Сформировать ответ на запрос
    r.acknowledge();    // Подтвердить корректность ответа на запрос
    return;
}


//=====================================================================================================================
// Вычисления времени до встречи
double CSysLaunch::_computeTimeToMeetWeapon(CPoint3D inStartPoint, CPoint3D inTargetPoint)
{
    return 0.;
}


//=====================================================================================================================
// Доступ к характеристикам систем
double CSysLaunch::getStartDelayBlockTime() const
{
    return myStartDelayBlockTime;
}


//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам систем
void   CSysLaunch::setDelayBlockTime(double time)
{
    // time - момент активации, поэтому myStartDelayBlockTime не может быть меньше time
    myStartDelayBlockTime = time + (this->getType().getFireDelay() - this->getType().getPrepareDelay());
}


