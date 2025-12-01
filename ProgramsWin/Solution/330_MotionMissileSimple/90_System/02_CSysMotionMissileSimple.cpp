//=====================================================================================================================
// Заголовочные файлы
#include "02_CSysMotionMissileSimple.h"


//=====================================================================================================================
// Конструктор класса
CSysMotionMissileSimple::CSysMotionMissileSimple(const CSysType &sysType, CObject &object)
    : CSysMotion(sysType, object)
{
    // Указание начальных значений параметров
    myMeetTime = 10000;    
}


//=====================================================================================================================
// Деструктор класса
CSysMotionMissileSimple::~CSysMotionMissileSimple()
{
}


//=====================================================================================================================
// Доступ к типу системы
const CSysMotionMissileSimpleType & CSysMotionMissileSimple::getType() const
{
    return (const CSysMotionMissileSimpleType &) CSys::getType();
}


//=====================================================================================================================
// Сменные алгоритмы действий
void CSysMotionMissileSimple::onAppear(double time)
{
    const vector<CTaskPoint>& myRoute = this->getInitialRoute();
    CPointGlobal myStartPointGlobal = myRoute.at(0).getPointGlobal();
    CPointGlobal myMeetPointGlobal = myRoute.at(myRoute.size() - 1).getPointGlobal();
    CPointGeo myMeetPointGeo = myRoute.at(myRoute.size() - 1).getPointGeo();
    CVector3D myVelocityVector(myStartPointGlobal.getPosition(), myMeetPointGlobal.getPosition());
    myVelocityVector.setLength(this->getType().getVelocity());
    myStartPointGlobal.setVelocity(myVelocityVector);
    this->saveLastFixedPoint(CTaskPoint(myStartPointGlobal));
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysMotionMissileSimple::onDestroy(double time)
{
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysMotionMissileSimple::onActivate(double time)
{
    double dS = this->getType().getVelocity() * this->getType().getTimeStep();

    CObjectList& fObjectList = ObjectList;

    // Блок доводки
    {
        CPointGlobal currentMissilePointGlobal = this->computePoint(time).getPointGlobal();
        CPointGlobal nextTargetPointGlobal = ObjectList.getObject(myTargetAbsNum).getSysMotion().computePoint(this->getNextTime()).getPointGlobal();
        double curDistance = currentMissilePointGlobal.getPosition().computeDistanceTo(ObjectList.getObject(myTargetAbsNum).getSysMotion().computePoint(time).getPointGlobal().getPosition());
        double nextDistance = currentMissilePointGlobal.getPosition().computeDistanceTo(nextTargetPointGlobal.getPosition());

        CVector3D missileVelocityVector(currentMissilePointGlobal.getPosition(), nextTargetPointGlobal.getPosition());
       
        currentMissilePointGlobal.setVelocity(missileVelocityVector);
        this->saveLastFixedPoint(CTaskPoint(currentMissilePointGlobal));

        Film.writePoint(this->getObject().getFilmId(), currentMissilePointGlobal);
    }
    // Конец блока доводки

    CTaskPoint currentPoint = this->computePoint(time);
    CTaskPoint targetPoint = ObjectList.getObject(myTargetAbsNum).getSysMotion().computePoint(time);

    CPointGlobal currentPointGlobal = currentPoint.getPointGlobal();
    CPointGlobal targentPointGlobal = targetPoint.getPointGlobal();

    double curDistance = currentPoint.getPointGlobal().getPosition().computeDistanceTo(targetPoint.getPointGlobal().getPosition());

    if (curDistance < getType().getExplosionRadius())
    {
        int weaponAbsNum = this->getObject().getAbsNum();
        // Оповещаем системы о промахе
        CImpactWeaponExplosion myImpactTargetMiss(this->getObject().getAbsNum(), currentPoint);
        ObjectList.getObject( this->getObject().getParentNum() ).processImpact(time, myImpactTargetMiss);
        Console << "  " << format(time, 7, 2) <<" "<< getType().getMissileName() << " взорвалась" << EndL;

        Film.writePoint(this->getObject().getFilmId(), currentPoint.getPointGlobal());

        this->getObject().disappear(time);
    }

}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysMotionMissileSimple::onDisappear(double time)
{
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
bool CSysMotionMissileSimple::onRouteEnd(double time)
{
    return true;        // Подтверждаем окончание маршрута

    // Замечание 1
    // После подтверждения окончания маршрута движения моделируемый объект автоматически исчезает

    // Замечание 2
    // Если требуется, чтобы объект продолжал существовать после окончания начального маршрута
    // (например, если движение объекта дальше зависит от событий, происходящих во время моделирования), 
    // то необходимо отклонить окончание маршрута движения (написать "return false;") и не забыть
    // при вычислении новых состояний системы в нужный момент вызвать функцию disappear() у объекта-носителя.
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysMotionMissileSimple::onTaskCmd(const CTaskCmd &cmd)
{
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysMotionMissileSimple::onTaskPoint(const CTaskPoint &point)
{
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
void CSysMotionMissileSimple::onImpact(double time, const CImpact &impact)
{

    if (typeid(impact) == typeid(CImpactWeaponLaunchSimple))
    {
        Console << "  " << format(time, 7, 2) << "  система пуска получила внешнее воздействие" << EndL;
        Log << "  " << format(time, 7, 2) << "  система пуска получила внешнее воздействие" << EndL;
        CImpactWeaponLaunchSimple myImpactWeaponLaunchSimple = static_cast<const CImpactWeaponLaunchSimple&>(impact);
        myTargetAbsNum = myImpactWeaponLaunchSimple.getAbsNum();
    }
}


//---------------------------------------------------------------------------------------------------------------------
// Сменные алгоритмы действий
bool CSysMotionMissileSimple::onRequest(double time, CRequest &request)
{
    // Обработать запрос боекомплекта
    if (typeid(request) == typeid(CRequestAmmunition))
    {
        // Доступ к запросу
        CRequestAmmunition &r = (CRequestAmmunition &)request;

        // Сформировать ответ на запрос
        r.setWeaponName("Ракета");
        r.setAmmunition(100);
        r.acknowledge();    // Подтвердить корректность ответа на запрос
        return true;        // Подтвердить, что запрос понят
    }

    // Запросы других видов не поддерживаются
    return false;
}



