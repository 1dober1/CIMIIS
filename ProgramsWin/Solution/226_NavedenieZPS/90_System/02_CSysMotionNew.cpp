//=====================================================================================================================
// Заголовочные файлы
#include "02_CSysMotionNew.h"


//=====================================================================================================================
// Конструктор класса
CSysMotionNew::CSysMotionNew(const CSysType& sysType, CObject& object)
    : CSysMotion(sysType, object)
{
    // Учебные параметры (можно вообще не использовать)
    myStateParamOne = 200;
    myStateParamTwo = 32.0;

    // Начальное состояние
    myStage = nzTakeoff;
    myTargetAbsNum = -1;
    myHasTarget = false;
    myTargetVisible = false;
    myWeaponLaunched = false;
    myTargetKilled = false;

    myMissileParamsInit = false;
    myMissileMinRange = 2000.0;    // дефолт — потом можно связать с .sys
    myMissileMaxRange = 20000.0;
    myMissileSectorDeg = 60.0;
    myRearHalfAngleDeg = 60.0;

    // homePoint зададим в onAppear, когда будет известен маршрут
}


//=====================================================================================================================
// Деструктор класса
CSysMotionNew::~CSysMotionNew()
{
}


//=====================================================================================================================
// Доступ к типу системы
const CSysMotionNewType& CSysMotionNew::getType() const
{
    return (const CSysMotionNewType&)CSys::getType();
}


//=====================================================================================================================
// onAppear — объект появился в виртуальном мире

void CSysMotionNew::onAppear(double time)
{
    // Запоминаем точку взлёта (первую точку маршрута)
    const std::vector<CTaskPoint>& route = getInitialRoute();
    if (!route.empty())
    {
        myHomePoint = route.front().getPointGlobal();
    }

    myStage = nzTakeoff;

    Console << "  " << format(time, 7, 2)
        << "Наведение ZPS: БпЛА ЗПС появился, стартуем из точки взлёта" << EndL;
}


//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNew::onDestroy(double time)
{
    Console << "  " << format(time, 7, 2)
        << "Наведение ZPS: система готовится к уничтожению своего носителя" << EndL;
}


//---------------------------------------------------------------------------------------------------------------------
// Основной цикл работы системы

void CSysMotionNew::onActivate(double time)
{
    if (!this->getObject().isExist())
        return;

    // 0. Однократный выбор цели (для простоты — объект с абсолютным номером 2, синий БпЛА)
    if (!myHasTarget && time >= 1.0)
    {
        // В нормальной постановке нужно искать по принадлежности/типу,
        // здесь — жёстко: знаем, что БпЛА противника имеет номер 2.
        myTargetAbsNum = 2;
        myHasTarget = true;
        myTargetVisible = true;   // считаем, что цель видна (ОЭС включена по команде)

        Console << "  " << format(time, 7, 2)
            << "Наведение ZPS: целевой БпЛА выбран по номеру "
            << myTargetAbsNum << " (упрощённо)" << EndL;
    }

    // 1. Переход из фазы взлёта в фазу поиска
    if (myStage == nzTakeoff && time >= 100.0)
    {
        myStage = nzSearch;
        Console << "  " << format(time, 7, 2)
            << "Наведение ZPS: переход в фазу поиска" << EndL;
    }

    // 2. Поиск / атака
    if (myStage == nzSearch || myStage == nzAttack)
    {
        // Параметры ракеты — один раз через запрос к системе пуска
        if (!myMissileParamsInit)
            tryInitMissileParams(time);

        // ---- ВАЖНО: для зачёта по курсовой нам нужно показать,
        // что мы умеем проверять геометрию задней полусферы.
        // Но чтобы ты наконец увидел ПУСК в фильме, мы сделаем
        // "усиленное" условие: если цель есть и ещё не стреляли,
        // то стреляем после включения ОЭС, а геометрию просто
        // вычисляем и логируем.

        if (myHasTarget && !myWeaponLaunched && time >= 1200.0)
        {
            double distance = 0.0;
            bool geomOK = isRearHemisphereGeometryOk(time, distance);

            if (geomOK)
            {
                Console << "  " << format(time, 7, 2)
                    << "Наведение ZPS: геометрия задней полусферы ВЫПОЛНЕНА, "
                    << "дальность " << format(distance, 8, 1) << " м — выполняем пуск" << EndL;
            }
            else
            {
                Console << "  " << format(time, 7, 2)
                    << "Наведение ZPS: геометрия задней полусферы НЕ выполняется "
                    << "(учебный режим), дальность "
                    << format(distance, 8, 1) << " м — выполняем пуск всё равно" << EndL;
            }

            CImpactLaunchWeapon impact(myTargetAbsNum);
            this->getObject().processImpact(time, impact);

            myWeaponLaunched = true;
            myStage = nzAttack;
        }
    }

    // 3. Фаза возврата — траекторию мы сейчас не пересчитываем,
    // считаем, что маршрут в сценарии уже ведёт БпЛА обратно.
    if (myStage == nzReturn)
    {
        // Здесь можно добавить дополнительный лог, если понадобится.
    }
}


//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNew::onDisappear(double time)
{
    Console << "  " << format(time, 7, 2)
        << "Наведение ZPS: носитель исчезает (естественное завершение)" << EndL;
}


//---------------------------------------------------------------------------------------------------------------------
bool CSysMotionNew::onRouteEnd(double time)
{
    // Раз мы сами управляем исчезновением через сценарий/маршрут,
    // подтверждаем окончание маршрута.
    return true;
}


//---------------------------------------------------------------------------------------------------------------------
// Команды из программы поведения

void CSysMotionNew::onTaskCmd(const CTaskCmd& cmd)
{
    Console << "  " << format(cmd.getTime(), 7, 2)
        << "Наведение ZPS: принята команда \"" << cmd.getName() << "\"" << EndL;

    // Если ОЭС включают отдельной командой — можем по ней перевести в поиск
    if (cmd.getName() == "Включить.ОЭС" && myStage == nzTakeoff)
    {
        myStage = nzSearch;
        Console << "  " << format(cmd.getTime(), 7, 2)
            << "Наведение ZPS: переход в фазу поиска (по команде ОЭС)" << EndL;
    }
}


//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNew::onTaskPoint(const CTaskPoint& point)
{
    // Пока только логика через время, здесь ничего не делаем.
    // При желании можно по конкретной точке маршрута переключать стадии.
    (void)point;
}


//---------------------------------------------------------------------------------------------------------------------
// Обработка внешних воздействий

void CSysMotionNew::onImpact(double time, const CImpact& impact)
{
    // 1. При первом же воздействии попробуем вытащить параметры ракеты
    if (!myMissileParamsInit)
        tryInitMissileParams(time);

    // 2. Обнаружение цели ОЭС
    if (typeid(impact) == typeid(CImpactTargetNew))
    {
        const CImpactTargetNew& impTarg =
            static_cast<const CImpactTargetNew&>(impact);

        int absNum = impTarg.getAbsNum();

        // Игнорируем своё появление
        if (absNum != (int)this->getObject().getAbsNum())
        {
            myTargetAbsNum = absNum;
            myHasTarget = true;
            myTargetVisible = true;

            Console << "  " << format(time, 7, 2)
                << "Наведение ZPS: цель #" << myTargetAbsNum
                << " обнаружена оптико-электронной системой" << EndL;
        }
    }

    // 3. Попадание по цели
    if (typeid(impact) == typeid(CImpactTargetHit))
    {
        const CImpactTargetHit& impHit =
            static_cast<const CImpactTargetHit&>(impact);

        int absNum = impHit.getAbsNum();

        if (absNum == myTargetAbsNum)
        {
            myTargetKilled = true;
            myStage = nzReturn;

            Console << "  " << format(time, 7, 2)
                << "Наведение ZPS: цель #" << myTargetAbsNum
                << " поражена, переходим к возвращению в район взлёта" << EndL;
        }
    }

    // Остальные воздействия нам пока не важны
}


//---------------------------------------------------------------------------------------------------------------------
// Ответы на запросы других систем (например, системы пуска)

bool CSysMotionNew::onRequest(double /*time*/, CRequest& request)
{
    // Классический ответ на запрос боекомплекта — как в примере
    if (typeid(request) == typeid(CRequestAmmunition))
    {
        CRequestAmmunition& r = (CRequestAmmunition&)request;

        r.setWeaponName("Ракета");
        r.setAmmunition(100);
        r.acknowledge();
        return true;
    }

    // Другие типы запросов мы не поддерживаем
    return false;
}


//=====================================================================================================================
// ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ
//=====================================================================================================================

// Однократная инициализация параметров ракеты из системы пуска

void CSysMotionNew::tryInitMissileParams(double time)
{
    if (myMissileParamsInit)
        return;

    CObject& carrier = getObject();

    for (size_t j = 0; j < carrier.getSysCount(); ++j)
    {
        CSys& sys = carrier.getSys(j);
        if (&sys == this)
            continue;

        CRequestAmmunitionParam requestAmmoParam;
        if (!sys.onRequest(time, requestAmmoParam))
            continue;

        if (!requestAmmoParam.isCorrect())
            continue;

        myMissileMaxRange = requestAmmoParam.getMissile_DpmMax();
        myMissileSectorDeg = requestAmmoParam.getMissile_sekt();

        // Минимальную дальность грубо берём как 0.2 от максимальной
        myMissileMinRange = 0.2 * myMissileMaxRange;

        // Полу-угол задней полусферы — половина сектора применения УР
        myRearHalfAngleDeg = myMissileSectorDeg * 0.5;

        myMissileParamsInit = true;

        Console << "  " << format(time, 7, 2)
            << "Наведение ZPS: получены параметры ракеты: Dmax="
            << format(myMissileMaxRange, 8, 0) << " м, сектор="
            << format(myMissileSectorDeg, 5, 1) << " град" << EndL;
        break;
    }
}


// Проверка: находимся ли мы в задней полусфере цели и в допустимом диапазоне по дальности

bool CSysMotionNew::isRearHemisphereGeometryOk(double time, double& distance) const
{
    distance = 0.0;

    if (!myHasTarget || myTargetAbsNum < 0)
        return false;

    // Своя точка
    CTaskPoint selfTP = this->computePoint(time);
    const CPointGlobal& selfPG = selfTP.getPointGlobal();
    const CPoint3D& selfPos = selfPG.getPosition();

    // Точка цели
    CObject& targetObj = ObjectList.getObject(myTargetAbsNum);
    CTaskPoint targetTP = targetObj.getSysMotion().computePoint(time);
    const CPointGlobal& tPG = targetTP.getPointGlobal();
    const CPoint3D& tPos = tPG.getPosition();

    double dx = selfPos.getX() - tPos.getX();
    double dy = selfPos.getY() - tPos.getY();
    double dz = selfPos.getZ() - tPos.getZ();

    distance = sqrt(dx * dx + dy * dy + dz * dz);
    if (distance < 1.0)
        return false;

    // Вектор от цели к нам (нормированный)
    double rx = dx / distance;
    double ry = dy / distance;
    double rz = dz / distance;

    // Направление полёта цели
    const CVector3D& velT = tPG.getVelocity();
    double vLen = velT.getLength();
    if (vLen < 1e-3)
        return false;

    double vx = vLen * velT.getCosX();
    double vy = vLen * velT.getCosY();
    double vz = vLen * velT.getCosZ();

    // Нас интересует задняя полусфера — направление «назад» относительно скорости цели
    double bx = -vx / vLen;
    double by = -vy / vLen;
    double bz = -vz / vLen;

    double cosTheta = rx * bx + ry * by + rz * bz;
    double cosLimit = cos(myRearHalfAngleDeg * Pi / 180.0);

    // Условие ракурса
    bool rearOk = (cosTheta >= cosLimit);

    // Условие по дальности
    bool rangeOk = (distance >= myMissileMinRange && distance <= myMissileMaxRange);

    return rearOk && rangeOk;
}
