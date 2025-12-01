// Файл 10_CObject.cpp
// Описание моделируемого объекта
//

//=====================================================================================================================
// Заголовочные файлы
#include "05_CSysNull.h"
#include "10_CObject.h"
#include "11_CObjectType.h"
#include "12_CObjectTypeList.h"
#include "13_CObjectList.h"
#include "../03_DataExchange/01_Impacts/01_CImpactAppear.h"
#include "../03_DataExchange/01_Impacts/02_CImpactDestroy.h"
#include "../03_DataExchange/01_Impacts/03_CImpactDisappear.h"
#include "../04_OutputWriter/01_CFilm.h"



//=====================================================================================================================
// Единственный на всю программу технологический экземпляр пустого объекта
CObject CObject::myGlobalObjectNull(ObjectTypeNull, 0, true);



//=====================================================================================================================
// Конструктор класса
CObject::CObject(const CObjectType &objectType, size_t absNum, bool isRed)
{
    myTypePtr = &objectType;
    myAbsNum = absNum;
    myIsExist = false;
    myIsRed = isRed;
    myFilmId = uint32_max;

    // Создание функциональных систем моделируемого объекта
    for (size_t i = 0; i < objectType.getSysTypeCount(); ++i)
    {
        // Создание экземпляра очередной системы
        CSys *newSys = objectType.getSysType(i).createSystem(*this);
        if(newSys == NULL)
            continue;

        // Добавление системы в объект
        mySysList.push_back(newSys);
    }

    // Вышестоящего объекта нет
    myParentNum = SIZE_MAX;
}



//---------------------------------------------------------------------------------------------------------------------
// Конструктор класса
CObject::CObject(const CObjectType &objectType, size_t absNum, size_t parentNum, bool isRed)
{
    myTypePtr = &objectType;
    myAbsNum = absNum;
    myIsExist = false;
    myIsRed = isRed;
    myFilmId = uint32_max;

    // Создание функциональных систем моделируемого объекта
    for (size_t i = 0; i < objectType.getSysTypeCount(); ++i)
    {
        // Создание экземпляра очередной системы
        CSys *newSys = objectType.getSysType(i).createSystem(*this);
        if (newSys == NULL)
            continue;

        // Добавление системы в объект
        mySysList.push_back(newSys);
    }

    // Организация связи с вышестоящим по иерархии объектом
    myParentNum = SIZE_MAX;
    CObject &parentObject = ObjectList.getObject(parentNum);
    if (ErrorCode == ERR_OK)
    {
        myParentNum = parentNum;
        parentObject.myChildrenList.insert(myAbsNum);
    }
}



//=====================================================================================================================
// Деструктор класса
CObject::~CObject()
{
    // Удаление систем из состава объекта
    for (size_t i = 0; i < mySysList.size(); ++i)
    {
        CSys *sys = mySysList[i];
        if (sys == NULL)
            continue;
        delete sys;
    }

    // Очистка списка систем
    mySysList.clear();
}



//=====================================================================================================================
// Признак того, что это технологический пустой объект
bool CObject::isNull() const
{
    if (this == &ObjectNull)
        return true;
    return false;
}



//=====================================================================================================================
// Доступ к типу системы
const CObjectType & CObject::getType() const
{
    return *myTypePtr;
}



//=====================================================================================================================
// Доступ к системам объекта
size_t CObject::getSysCount() const
{
    return mySysList.size();
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к системам объекта
const CSys & CObject::getSys(size_t index) const
{
    // Если индекс некорректный
    if (index >= mySysList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return SysNull;
    }

    // Если тип систем не загружен
    if (mySysList[index] == NULL)
    {
        ErrorCode = ERR_NULL_POINTER;
        return SysNull;
    }

    // Всё нормально
    ErrorCode = ERR_OK;
    return *mySysList[index];
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к системам объекта
CSys & CObject::getSys(size_t index)
{
    // Если индекс некорректный
    if (index >= mySysList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return SysNull;
    }

    // Если тип систем не загружен
    if (mySysList[index] == NULL)
    {
        ErrorCode = ERR_NULL_POINTER;
        return SysNull;
    }

    // Всё нормально
    ErrorCode = ERR_OK;
    return *mySysList[index];
}



//=====================================================================================================================
// Доступ к системе движения
const CSysMotion & CObject::getSysMotion() const
{
    // Система движения есть всегда
    return static_cast<const CSysMotion &>(*mySysList[0]);
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к системе движения
CSysMotion & CObject::getSysMotion()
{
    // Система движения есть всегда
    return static_cast<CSysMotion &>(*mySysList[0]);
}



//=====================================================================================================================
// Доступ к параметрам объекта
size_t CObject::getAbsNum() const
{
    return myAbsNum;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам объекта
bool CObject::isExist() const
{
    return myIsExist;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам объекта
bool CObject::isRed() const
{
    return myIsRed;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам объекта
size_t CObject::getParentNum() const
{
    return myParentNum;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам объекта
const set<size_t> & CObject::getChildrenList() const
{
    return myChildrenList;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам объекта
uint32 CObject::getFilmId() const
{
    return myFilmId;
}



//=====================================================================================================================
// Управление именем объекта в виртуальном мире
const string & CObject::getName() const
{
    return myName;
}



//---------------------------------------------------------------------------------------------------------------------
// Управление именем объекта в виртуальном мире
void CObject::setName(const string &name)
{
    myName = name;
}



//=====================================================================================================================
// Управление существованием объекта
void CObject::appear(double time)
{
    // Если объект уже существует
    if (myIsExist == true)
        return;

    // Зарегистрировать объект в фильме
    uint32 parentId = (myIsRed ? Film.getRedSceneId() : Film.getBlueSceneId());
    if (myParentNum != SIZE_MAX)
    {
        const CObject &parentObject = ObjectList.getObject(myParentNum);
        if (ErrorCode == ERR_OK)
            parentId = parentObject.getFilmId();
    }
    bool isAir = (getType().getKind() == CObjectType::AIR) ? true : false;
    bool isWeapon = getType().isWeapon();
    string typeName = getType().getName();
   
    myFilmId = Film.registerObject(myAbsNum, time, typeName, format((uint32)myAbsNum, 4, fillZero), parentId, ((isAir == true) ? CFilm::AIR : CFilm::GROUND));
    Film.writePoint(myFilmId, getSysMotion().getLastFixedPoint().getPointGlobal());
    Film.writeState(myFilmId, time, CFilm::ALIVE);

    // Вывести информацию об объекте
    Film.writeLog(myFilmId, time, string("Тип объекта: ") + getType().getShowName() + string(" (") + getType().getName() + string(")"));
    Film.writeLog(myFilmId, time, string("Имя объекта: ") + getName());
    Film.writeLog(myFilmId, time, string("Номер объекта: ") + format((uint32)getAbsNum()));
    if (getParentNum() == SIZE_MAX)
        Film.writeLog(myFilmId, time, "Номер вышестоящего объекта: нет");
    else
        Film.writeLog(myFilmId, time, string("Номер вышестоящего объекта: ") + format((uint32)getParentNum()));
    Film.writeLog(myFilmId, time, "Список функциональных систем объекта:");
    for (size_t i = 0; i < getSysCount(); i++)
    {
        const CSysType &sys = getSys(i).getType();
        Film.writeLog(myFilmId, time, string("  * ") + sys.getShowName() + string(" (") + sys.getName() + string(")"));
    }

    // Сделать запись в протокол
    Console << "  " << format(time, 7, 2) << "  появился объект " << format((uint32)myAbsNum, 4, fillZero) << " типа " << getType().getName() << EndL;
    Log     << "  " << format(time, 7, 2) << "  появился объект " << format((uint32)myAbsNum, 4, fillZero) << " типа " << getType().getName() << EndL;

    // Выставить признак существования объекта
    myIsExist = true;

    // Уведомить системы объекта о его появлении
    for (size_t i = 0; i < mySysList.size(); ++i)
        mySysList[i]->appear(time);

   
    // Разослать воздействие всем моделируемым объектам
    CImpactAppear impact(myAbsNum);
    ObjectList.processImpact(myAbsNum, time, impact);
}



//---------------------------------------------------------------------------------------------------------------------
// Управление существованием объекта
void CObject::destroy(double time)
{
    // Если объект уже не существует
    if (myIsExist == false)
        return;

    // Уведомить все системы об уничтожении объекта
    for (size_t i = 0; i < mySysList.size(); ++i)
        mySysList[i]->destroy(time);

    // Разослать воздействие всем моделируемым объектам
    CImpactDestroy impact(myAbsNum);
    ObjectList.processImpact(myAbsNum, time, impact);

    // Выставить признак уничтожения объекта
    myIsExist = false;

    // Выставить состояние в фильме
    CPointGlobal lastPoint = getSysMotion().getLastFixedPoint().getPointGlobal();
    double dt = time - lastPoint.getTime();
    if (checkEQ(dt, 0.0) == false)
    {
        CPoint3D point = lastPoint.getPosition();
        point.moveOn(lastPoint.getVelocity() * dt);
        lastPoint.setPosition(point);
		lastPoint.setTime(time);
		lastPoint.setVelocity(CVector3D(0, 0, 0));
    }
    Film.writePoint(myFilmId, lastPoint);
    Film.writeState(myFilmId, time, CFilm::DESTROY);

   
    // Сделать запись в протокол
    Console << "  " << format(time, 7, 2) << "  уничтожен объект " << format((uint32)myAbsNum, 4, fillZero) << " типа " << getType().getName() << EndL;
    Log     << "  " << format(time, 7, 2) << "  уничтожен объект " << format((uint32)myAbsNum, 4, fillZero) << " типа " << getType().getName() << EndL;
}



//---------------------------------------------------------------------------------------------------------------------
// Управление существованием объекта
bool CObject::routeEnd(double time)
{
    return getSysMotion().routeEnd(time);
}



//---------------------------------------------------------------------------------------------------------------------
// Управление существованием объекта
void CObject::disappear(double time)
{
    // Если объект уже не существует
    if (myIsExist == false)
        return;

    // Уведомить все системы об исчезновении объекта
    for (size_t i = 0; i < mySysList.size(); ++i)
        mySysList[i]->disappear(time);

    // Разослать воздействие всем моделируемым объектам
    CImpactDisappear impact(myAbsNum);
    ObjectList.processImpact(myAbsNum, time, impact);

    // Выставить признак исчезновения объекта
    myIsExist = false;

    // Выставить состояние в фильме
    CPointGlobal lastPoint = getSysMotion().getLastFixedPoint().getPointGlobal();
    double dt = time - lastPoint.getTime();
    if (checkEQ(dt, 0.0) == false)
    {
        CPoint3D point = lastPoint.getPosition();
        point.moveOn(lastPoint.getVelocity() * dt);
        lastPoint.setPosition(point);
        lastPoint.setTime(time);
    }
    Film.writePoint(myFilmId, lastPoint);
    Film.writeState(myFilmId, time, CFilm::DEAD);
       

    // Сделать запись в протокол
    Console << "  " << format(time, 7, 2) << "  исчез объект " << format((uint32)myAbsNum, 4, fillZero) << " типа " << getType().getName() << EndL;
    Log     << "  " << format(time, 7, 2) << "  исчез объект " << format((uint32)myAbsNum, 4, fillZero) << " типа " << getType().getName() << EndL;
}



//=====================================================================================================================
// Управление передачей информации
void CObject::processTaskCmd(const CTaskCmd &taskCmd)
{
    // Активизировать обработку команды в системах
    for (size_t i = 0; i < mySysList.size(); ++i)
        mySysList[i]->onTaskCmd(taskCmd);
}



//---------------------------------------------------------------------------------------------------------------------
// Управление передачей информации
void CObject::processTaskPoint(const CTaskPoint &taskPoint)
{
    // Активизировать обработку точки в системах
    for (size_t i = 0; i < mySysList.size(); ++i)
        mySysList[i]->onTaskPoint(taskPoint);
}



//---------------------------------------------------------------------------------------------------------------------
// Управление передачей информации
void CObject::processImpact(double time, const CImpact &impact)
{
    // Активизировать обработку воздействия в системах
    for (size_t i = 0; i < mySysList.size(); ++i)
        mySysList[i]->onImpact(time, impact);
}


