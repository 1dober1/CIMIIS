//=============================================================================
// Заголовочные файлы
#include "02_CSysOpticThermal.h"
#include "../01_Calc/01_AtmosphereCalc.h"
#include "../01_Calc/02_InfraRedContrastCalc.h"
#include "../01_Calc/03_DistanceCalc.h"
#include "../01_Calc/03_FieldOfVisionCalc.h"
#include "../01_Calc/04_StaticDetectionProbability.h"
#include "../01_Calc/11_EnumClasses.h"
#include "../01_Calc/99_EffectiveIrradianceCalc.h"
#include "../91_Utility/02_CFormat.h"


namespace optic = SysOpticThermal;
namespace util = SysOpticThermal::Utility;

optic::CEnvironment optic::CSysOpticThermal::MyEnv;


//=============================================================================
// Конструктор класса
optic::CSysOpticThermal::CSysOpticThermal(const CSysType& sysType, CObject& object)
    : CSys(sysType, object)
    , myIsSwitchOn(false)
    , myMinDistance(0.0), myMaxDistance(0.0)
    , myOperationMode(Wide)
    , myTargetNum(SIZE_MAX)
    , myLastUpdateTime(0.0)
{
    
    const CSysOpticThermalType& sysTTH = dynamic_cast<const CSysOpticThermalType&>(sysType);

    AtmosphereStruct& atm = myInputDataStruct.atm;
    atm.sun_height = MyEnv.getSunAngle();
    
    CarrierStruct& carrier = myInputDataStruct.carrier;
    carrier.height      = 0;
    carrier.johnson_det = sysTTH.getJohnosonDetIdle();
    carrier.johnson_rec = sysTTH.getJohnosonRec();
    carrier.op_skill    = 1;
    carrier.light       = 100;
    carrier.is_det      = true;

    SystemStruct& sys = myInputDataStruct.sys;
    sys.spectrum_range = sysTTH.getSpectrumRange() == CSysOpticThermalType::WaveLen_3_5;
    sys.azimuth       = sysTTH.getDetectionAzimuth() * Pi / 180.0;
    sys.elevation     = sysTTH.getDetectionElevation() * Pi / 180.0;
    sys.zoom          = 1;
    sys.lens_diff     = sysTTH.getLensDiff();
    sys.fill          = sysTTH.getFillCoef();
    sys.frame_freq    = sysTTH.getFps();
    sys.noise         = sysTTH.getNoiseTemp();
    sys.scan          = 1;
    sys.width         = static_cast<int>(sysTTH.getResolution().first);
    sys.height        = static_cast<int>(sysTTH.getResolution().second);

    BearingStruct& bearing = myInputDataStruct.bearing;
    bearing.det_p      = 0.5;
    bearing.rec_p      = 0.95;
    bearing.det_ir_val = 1;
    bearing.rec_ir_va  = sys.spectrum_range ? 1 : 1.5;
}



//=============================================================================
// Деструктор класса
optic::CSysOpticThermal::~CSysOpticThermal()
{
}


//=============================================================================
// Реакция на внешнее воздействие
void optic::CSysOpticThermal::onImpact(const double time, const CImpact &impact)
{
    if(typeid(impact) == typeid(CImpactAppear))
        _processImpactThink(time, impact);
}


//===============================================================================================
// Обработка внешних запросов к функциональной системе
bool optic::CSysOpticThermal::onRequest(const double time, CRequest &request)
{
    return false;
}

//=====================================================================================================================
// Доступ к типу системы
const optic:: CSysOpticThermalType& optic::CSysOpticThermal::getType() const
{
    return dynamic_cast<const CSysOpticThermalType&>(CSys::getType());
}



//=====================================================================================================================
// Сменные алгоритмы действий
void optic::CSysOpticThermal::onAppear(double time)
{
    
}



//---------------------------------------------------------------------------------------------------------------------
void optic::CSysOpticThermal::onDestroy(double time)
{
    _processEventSwitchOff(time);
}



//---------------------------------------------------------------------------------------------------------------------
void optic::CSysOpticThermal::onActivate(double time)
{
    if (myIsSwitchOn)
        _processEventThink(time);
}



//---------------------------------------------------------------------------------------------------------------------
void optic::CSysOpticThermal::onDisappear(double time)
{
}



//---------------------------------------------------------------------------------------------------------------------
void optic::CSysOpticThermal::onTaskCmd(const CTaskCmd& cmd)
{
    if (cmd.getName() == "Включить.ОЭС")
        _processEventSwitchOn(cmd.getTime());
    else if (cmd.getName() == "Выключить.ОЭС")
        _processEventSwitchOff(cmd.getTime());
}



//---------------------------------------------------------------------------------------------------------------------
void optic::CSysOpticThermal::onTaskPoint(const CTaskPoint& point)
{
}


//===============================================================================================
// Обработка события включения оптико-электронной системы
void optic::CSysOpticThermal::_processEventSwitchOn(const double curTime)
{
    //-------------------------------------------------------------------------------
    // Установка признака включения оптико-электронной системы
    myIsSwitchOn = true;

    //-------------------------------------------------------------------------------
    // Планирование события начала работы оптико-электронной системы
    _processEventThink(curTime);
       
    _createDetectionZone(curTime);

    ostringstream out;
    string space = "  ";
  
    // Время  ZONESTATE  номер_носителя_в_фильме  идентификатор_зоны  состояние  режим_отображения  направление_по_азимуту  направление_по_углу_места  0
    out << curTime << space
        << "ZONESTATE" << space
        << getObject().getFilmId() << space
        << 456 << space
        << 1 << space
        << 7 << space
        << 0 << space
        << myZoneElevation << endl;
    Film.writeText(out.str());

    WriteFilmLog(curTime, "Режим продольного сканирования: %s",
        getType().isScanEnabled() ? "включен" : "выключен");

    WriteFilmLog(curTime, "Минимальная дальность: %.2f м", myMinDistance);
    WriteFilmLog(curTime, "Максимальная дальность: %.2f м", myMaxDistance);

}


//===============================================================================================
// Обработка события выключения оптико-электронной системы
void optic::CSysOpticThermal::_processEventSwitchOff(const double curTime)
{
    
    // Последовательный обход всех каналов
    while(!myTargetList.empty())
    {
        // Доступ к конкретному каналу
        const size_t targetAbsNum = myTargetList.front();

        // Удаление информации об объекте из списка
        _processEventTargetLost(curTime, targetAbsNum);

        myTargetList.pop_front();
    }

    myInRangeMap.clear();

    //-------------------------------------------------------------------------------
    // Установка признака работы оптико-электронной системы в режим "Выключена"
    myIsSwitchOn = false;

    // Формируем строку для описания возникшей ситуации
    string str = "Выключение ОЭС";

    ostringstream out;
    string space = "  ";

    // время  ZONESTATE  номер_носителя_в_фильме  идентификатор_зоны  состояние  режим_отображения  направление_по_азимуту  направление_по_углу_места  0
    out << curTime << space
        << "ZONESTATE" << space
        << getObject().getFilmId() << space
        << 456 << space
        << 0 << space
        << 7 << space
        << 0 << space
        << myZoneElevation << endl;
    Film.writeText(out.str());
}

bool optic::CSysOpticThermal::_processEventSwitchMode(const double curTime, size_t targetNum)
{
    if (targetNum != SIZE_MAX)
    {
        const CObject& target = ObjectList.getObject(targetNum);

        if (target.isExist()
            && target.isRed() != getObject().isRed()
            && _checkInOpticalZone(curTime, target))
        {
            myTargetNum = targetNum;
            myOperationMode = Narrow;

            ostringstream out;
            string space = "  ";

            // время  ZONESTATE  номер_носителя_в_фильме  идентификатор_зоны  состояние  режим_отображения  направление_по_азимуту  направление_по_углу_места  0
            out << curTime << space
                << "ZONESTATE" << space
                << getObject().getFilmId() << space
                << 456 << space
                << 1 << space
                << 3 << space
                << 0 << space
                << 0 << space
                << target.getFilmId() << endl;
            Film.writeText(out.str());

            return true;
        }
    }


    myOperationMode = Wide;
    myTargetNum = SIZE_MAX;

    ostringstream out;
    string space = "  ";
    
    // время  ZONESTATE  номер_носителя_в_фильме  идентификатор_зоны  состояние  режим_отображения  направление_по_азимуту  направление_по_углу_места  0
    out << curTime << space
        << "ZONESTATE" << space
        << getObject().getFilmId() << space
        << 456 << space
        << 1 << space
        << 7 << space
        << 0 << space
        << myZoneElevation << endl;
    Film.writeText(out.str());
        
    return true;
}


//===============================================================================================
// Обработка события обнаружения цели
void optic::CSysOpticThermal::_processEventTargetNew(const double curTime, const size_t targetNum)
{
    //-------------------------------------------------------------------------
    // Доступ к объекту-цели
    const CObject& target = ObjectList.getObject(targetNum);

    //-------------------------------------------------------------------------
    // Запомнить цель
    myTargetList.push_back(targetNum);

    // Уведомить себя о завязке оптической трассе
    const CImpactTargetNew newImpact(targetNum);
    getObject().processImpact(curTime, newImpact);
    
    ESysIRTargetState state;
    if (getType().getOperationMode() == CSysOpticThermalType::Recognition)
        state = Recognition;
    else
        state = Detection;
    _printTargetNewMessage(curTime, target, state);
}


//===============================================================================================
// Обработка события потери цели
void optic::CSysOpticThermal::_processEventTargetLost(const double curTime, const size_t targetNum)
{
    //-------------------------------------------------------------------------
    // Удаление информации о цели
    myInRangeMap.erase(targetNum);
    myTargetList.remove(targetNum);
    if (targetNum == myTargetNum)
    {
        size_t targetNew = SIZE_MAX;
        for (std::list<size_t>::const_iterator it = myTargetList.begin(); it != myTargetList.end(); ++it)
        {
            if (ObjectList.getObject(*it).isExist())
            {
                targetNew = *it;
                break;
            }
        }
        _processEventSwitchMode(curTime, targetNew);
    }

    const CObject& target = ObjectList.getObject(targetNum);
    WriteFilmLog(curTime, "Объект '%s [%04d]' был потерян", target.getName().c_str(), target.getAbsNum());

    //-------------------------------------------------------------------------
    // Уведомить себя о сбросе цели
    const CImpactTargetLost newImpact(targetNum);
    getObject().processImpact(curTime, newImpact);
}


//===============================================================================================
// Обработка события работы оптико-электронной системы
void optic::CSysOpticThermal::_processEventThink(const double curTime)
{
    const size_t carrierNum = getObject().getAbsNum();

    for (size_t i = 1; i <= ObjectList.getObjectCount(); ++i)
    {
        // Пропуск носителя оптико-электронной системы
        if ( i == carrierNum )
            continue;

        // Доступ к цели
        const CObject& target = ObjectList.getObject(i);
        if (!target.isExist())
        {
            if (target.getAbsNum() == myTargetNum)
                _processEventTargetLost(curTime, myTargetNum);
            else
                continue;
        }
              
        bool isInZone = false;

        // Проверка, сохранена ли цель в перечне объектов, которые попали в зону, но еще не были обнаружены
        map<size_t, TargetP>::iterator itInRange = _findInRange(i);

        // Проверка, попадает ли цель в зону
        isInZone = _checkInOpticalZone(curTime, target);

        // Проверка, была ли цель уже обнаружена
        const bool isDetected = _checkTarget(i);

        // Если цель не в зоне
        if ( ! isInZone )
        {
            // Если цель была сохранена в перечне объектов, которые попали в зону, но еще не были обнаружены, то ее надо оттуда удалить
            if (itInRange != myInRangeMap.end())
            {
                myInRangeMap.erase(itInRange);
            }
            // Если цель была обнаружена, то надо запланировать события потери цели
            if ( isDetected )
            {
                _processEventTargetLost(curTime, i);
            }
        }

        // Если цель была обнаружена, то надо обновить информацию о ней
        else if ( isDetected )
        {
            //// Доступ к текущему положению цели
            CTaskPoint targetCurPos = target.getSysMotion().computePoint(curTime);

            if (curTime - myLastUpdateTime >= getType().getMinUpdateInterval())
            {
                myLastUpdateTime = curTime;

            }
        }

        // Иначе цель уже вошла в зону, но еще не была обнаружена
        else
        {
            // Если цель вошла в зону впервые, то надо добавить ее в перечень и начатать отслеживать
            if ( itInRange == myInRangeMap.end() )
            {
                itInRange = _addInRange(i);
                const CPointGeo targetPoint = target.getSysMotion().computePoint(curTime).getPointGeo();
            }

            // Обновление данных и подсчет вероятности
            _updateInputData(curTime, target, itInRange->second.state);
            ResultDataStruct res = _computeStaticP();
            _computeDynamicP(res.p.static_probability, itInRange);

            if (getType().isDebugMsg())
            {
                const double vel = target.getSysMotion().computePoint(curTime).getPointGlobal().getVelocity().getLength();
                WriteFilmLog(curTime, "'%s [%04d]' > Скорость: %.2f | Стат. вер-ть: %f | Вер-ть: %f | Порог: %f", 
                    target.getName().c_str(), target.getAbsNum(), 
                    vel, res.p.static_probability, itInRange->second.cur_prob, itInRange->second.threshold_prob);
            }

            // Если вероятность стала больше разыгранной, значит цель была обнаружена
            if ( itInRange->second.threshold_prob <= itInRange->second.cur_prob )
            {
                if (itInRange->second.state == Detection && getType().getOperationMode() != CSysOpticThermalType::Detection)
                {
                    if (getType().getOperationMode() == CSysOpticThermalType::Recognition)
                    {
                        itInRange->second.state = Recognition;
                        itInRange->second.cur_prob = res.p.static_probability;
                        itInRange->second.threshold_prob = rand() / static_cast<double>(RAND_MAX);
                        if (getType().isDebugMsg())
                            _printTargetNewMessage(curTime, target, Detection);
                    }

                    if (myTargetNum == SIZE_MAX)
                    {
                        double delta_time;
                        if (getType().getOperationMode() == CSysOpticThermalType::Recognition)
                            delta_time = 3.0;
                        else
                            delta_time = 0.0;
                        _processEventSwitchMode(curTime, itInRange->first);
                    }

                    if (getType().getOperationMode() != CSysOpticThermalType::Recognition)
                    {
                        myInRangeMap.erase(itInRange);
                        _processEventTargetNew(curTime, i);
                    }
                }
                else
                {
                    myInRangeMap.erase(itInRange);
                    _processEventTargetNew(curTime, i);
                }
            }
        }
    }
}

void optic::CSysOpticThermal::_processEventCreateDetZone()
{
    
}


//===============================================================================================
// Расчет вероятностей
optic::ResultDataStruct optic::CSysOpticThermal::_computeStaticP() const
{
    const InputDataStruct& data = myInputDataStruct;
    ResultDataStruct res;
    res.atm = Calc::Atm::calculateAtmosphereValue(data.atm, data.sys, data.carrier, data.target);
    res.ir  = Calc::IRContrast::calculateInfraRedContrast(res.atm, data.sun_rad, data.atm, data.sys, data.carrier, data.target, data.outline);
    res.fov = Calc::FOV::calculateFOV(data.sys);
    res.p   = Calc::Static::calculateStaticProbability(data.sys, data.carrier, data.target, res.atm, res.ir, res.fov);

    return res;
}


//-----------------------------------------------------------------------------------------------
void optic::CSysOpticThermal::_computeDynamicP(const double static_p, map<size_t, TargetP>::iterator itTarget)
{
    const double tau = 3.0 - 2.2 * static_p;
    const double delta_p = getType().getDeltaTime() / tau * (static_p - itTarget->second.cur_prob);

    if (delta_p > 0.0)
        itTarget->second.cur_prob += delta_p;
}


//===============================================================================================
// Проверка попадания цели в зону оптико-электронной системы
void optic::CSysOpticThermal::_updateInputData(const double curTime, const CObject& target, ESysIRTargetState state)
{
    // Вспомогательные переменные
    const CGlobalEnvironment& env = CSysOpticThermalType::MyGlobalEnv;
    const CObject& carrier = getObject();

    // Признак подвижности цели
    const bool isTargetIdle = target.getSysMotion().getLastFixedPoint().getPointGlobal().getVelocity().getLength() == 0.0;

    // Координаты носителя и цели в геодезической системе координат
    const CPointGeo carrierPoint = carrier.getSysMotion().computePoint(curTime).getPointGeo();
    const CPointGeo targetPoint  = target.getSysMotion().computePoint(curTime).getPointGeo();

    // Высота рельефа в точке носителя и цели (пока не используется)
    const double unitRelief   = env.getHeight(carrierPoint.getLatitude(), carrierPoint.getLongitude());
    const double targetRelief = env.getHeight(targetPoint.getLatitude(), targetPoint.getLongitude());

    // Высоты носителя и цели над землей (в км)
    const double unitAltitude   = carrierPoint.getAltitude() / 1000.0;
    const double targetAltitude = targetPoint.getAltitude() / 1000.0;
    const double targetDistance = _getTargetDistance(curTime, target);

    // Координаты фона в геодезической системе координат
    const CPointGeo bgPoint = _getBgGeodPoint(curTime, (unitAltitude - targetAltitude) * 1000.0, targetDistance * 1000.0,
        carrier, target);

    // Переменные среды
    const double atmTemp = env.getTemperature(carrierPoint.getLatitude(),
                                            carrierPoint.getLongitude(),
                                            carrierPoint.getAltitude());
    const double hum = env.getRelativeHumidity();
    const TypeSurface targetSurface = env.getSurfaceType(targetPoint.getLatitude(), targetPoint.getLongitude());
    const TypeSurface bgSurface = env.getSurfaceType(bgPoint.getLatitude(), bgPoint.getLongitude());
    const SecondCloudType clouds = env.getSecondTypeOfCloud();

    // Обновление данных для модели
    InputDataStruct& data = myInputDataStruct;
    SpectrumRange::type srange = data.sys.spectrum_range ? SpectrumRange::WaveLen_3_5 : SpectrumRange::WaveLen_8_14;
    data.outline.clear();

    data.atm.mdv = env.getMeteoVisibility();
    data.atm.abs_hum = CEnvironment::getAbsHumidity(hum, atmTemp - 273.5);
    data.atm.temp = atmTemp;
    data.atm.prec = CEnvironment::getPrecipitationType(env.getGidrometeor());
    data.atm.is_night = env.getIsNight(carrierPoint.getLatitude(), carrierPoint.getLongitude());

    data.carrier.height = unitAltitude;
    data.carrier.johnson_det = isTargetIdle ? getType().getJohnosonDetIdle() : getType().getJohnosonDetMoving();

    TargetStruct& ts = data.target;
    ts.height   = targetAltitude;
    ts.distance = targetDistance;

    Fragment outline_f;
    TargetFragment &tf     = outline_f.first;
    BackgroundFragment &bf = outline_f.second;

    bf.temperature = env.getSurfaceTemperature(bgPoint.getLatitude(), bgPoint.getLongitude());
    bf.rad_coef =  env.getSurfaceEmissivity(bgPoint.getLatitude(), bgPoint.getLongitude());
    bf.table_coef = MyEnv.getBgTblCoef(srange, bgSurface, clouds);
    bf.sun_pos = MyEnv.getBgSunPos();
    bf.surface = MyEnv.getBgSurface();
    bf.angle = MyEnv.getBgAngle() * Pi / 180.0;

    tf.crit_size   = target.getType().getHeight() / 1000.0;
    tf.temperature = bf.temperature + MyEnv.getObjTempDelta();
    tf.rad_coef    = data.sys.spectrum_range ? MyEnv.getObjRadCoef3_5() : MyEnv.getObjRadCoef8_14();
    tf.table_coef  = MyEnv.getObjTblCoef(srange, targetSurface, clouds);
    tf.sun_pos     = MyEnv.getObjSunPos();
    tf.surface     = MyEnv.getObjSurface();

    data.outline.push_back(outline_f);

    ts.max_crit_size = 0;
    for (std::list<Fragment>::iterator it = data.outline.begin(); it != data.outline.end(); ++it)
    {
        if (it->first.crit_size > ts.max_crit_size)
            ts.max_crit_size = it->first.crit_size;
    }

    data.sun_rad = Calc::SunRad::calculateSunRadiation(data.atm, data.outline.front());

    data.sys.height = static_cast<int>(getType().getResolution().second);
    if (myOperationMode == Wide)
    {
        data.sys.elevation = getType().getDetectionElevation() * Pi / 180.0;
        if (getType().isScanEnabled())
        {
            // Пересчитываем высоту матрицы
            data.sys.height *= static_cast<int>(myScanElevation / data.sys.elevation);
            data.sys.elevation = myScanElevation;
        }
    }
    else
    {
        data.sys.azimuth = getType().getRecognitionAzimuth() * Pi / 180.0;
        data.sys.elevation = getType().getRecognitionElevation() * Pi / 180.0;
    }
    data.carrier.is_det = (state == Detection);

    if (getType().isDebugMsg())
    {
        WriteFilmLog(curTime, "Исследование с помощью ОЭС объекта '%s [%04d]'. Фон: %s",
            target.getName().c_str(), target.getAbsNum(), _getSurfaceString(bgPoint).c_str());
    }
}

//===============================================================================================
// Проверка попадания цели в зону оптико-электронной системы
bool optic::CSysOpticThermal::_checkInOpticalZone(const double curTime, const CObject& target) const
{
    if (!target.isExist())
        return false;

    const size_t targetNum = target.getAbsNum();

    // Доступ к объекту-носителю
    const CObject& carrier = getObject();

    // Вычислить текущие координаты объекта-носителя
    const CTaskPoint carPos = carrier.getSysMotion().computePoint(curTime);

    // Вычислить угол поворота объекта-носителя в азимутальной плоскости относительно севера
    CPointLocal carPosLocal = carPos.getPointGlobal().toPointLocal(carPos.getPointGlobal().getPosition());
    const double selfAzimuth = carPosLocal.computeRadianSelfAzimuth();

    // Повернуть плоскость xOy (азимутальную) под нашим носителем так, чтобы направление "вперёд" носителя совпадало с направлением оси Oy
    carPosLocal.rotatePlaneXY(-selfAzimuth);

    // Вычислить текущие координаты объекта-цели
    const CTaskPoint objPos = target.getSysMotion().computePoint(curTime);

    // Пропустить объекты-цели, которые по дальности не попадают в зону сканирования
    const double distance = carPos.getPointGlobal().getPosition().computeDistanceTo(objPos.getPointGlobal().getPosition());

    // Пересчитать координаты объекта-цели в местную декартову СК,
    // связанную с нашим носителем
    CPointLocal objPosLocal = objPos.getPointGlobal().toPointLocal(carPos.getPointGlobal().getPosition());

    // Повернуть плоскость xOy (горизонтальную) под объектом-целью таким образом, чтобы ось Oy совпадала с направлением "вперёд" носителя
    objPosLocal.rotatePlaneXY(-selfAzimuth);

    // Вычислить углы по азимуту и углу места между направлением "вперёд" нашего носителя и направлением на объект-цель
    const double azimuth = carPosLocal.computeRadianAzimuthOf(objPosLocal.getPosition());
    const double elevation = carPosLocal.computeRadianElevationOf(objPosLocal.getPosition());

    // Перевести углы цели в градусы
    const double azimuthDegrees = rad2deg(azimuth);
    const double elevationDegrees = rad2deg(elevation);

    // Перевести азимут цели в диапазон от -180 до +180
    const double azimuthDegrees180 = normalizeTo180(azimuthDegrees);
    const double elevationDegrees180 = normalizeTo180(elevationDegrees);

    if (myOperationMode == Wide)
    {
        const double halfAzimuth = getType().getDetectionAzimuth() / 2.0;
        return  elevationDegrees180 <= myMaxElevation && elevationDegrees180 >= myMinElevation &&
                fabs(azimuthDegrees180) <= halfAzimuth;
    }

    if (target.getAbsNum() == myTargetNum && fabs(azimuthDegrees180) <= getType().getTurnAngle())
        return myMaxDistance > distance;

    if (myTargetNum <= ObjectList.getObjectCount() && target.getAbsNum() != myTargetNum)
    {
        double azimuthHalfSector;
        double elevationHalfSector;
        if (getType().getOperationMode() == CSysOpticThermalType::Detection_Follow_Wide)
        {
            azimuthHalfSector = getType().getDetectionAzimuth() / 2.0;
            elevationHalfSector = getType().getDetectionElevation() / 2.0;
        }
        else
        {
            azimuthHalfSector = getType().getRecognitionAzimuth() / 2.0;
            elevationHalfSector = getType().getRecognitionElevation() / 2.0;
        }

        const CObject& center = ObjectList.getObject(myTargetNum);
       
        // Вычислить текущие координаты объекта-цели
        const CTaskPoint centerPos = center.getSysMotion().computePoint(curTime);

        // Пересчитать координаты объекта-цели в местную декартову СК, связанную с нашим носителем
        CPointLocal centerPosLocal = centerPos.getPointGlobal().toPointLocal(carPos.getPointGlobal().getPosition());

        // Повернуть плоскость xOy (горизонтальную) под объектом-целью таким образом, чтобы ось Oy совпадала с направлением "вперёд" носителя
        centerPosLocal.rotatePlaneXY(-selfAzimuth);

        // Вычислить углы по азимуту и углу места между направлением "вперёд" нашего носителя и направлением на объект-цель
        const double centerAzimuth = carPosLocal.computeRadianAzimuthOf(centerPosLocal.getPosition());
        const double centerEelevation = carPosLocal.computeRadianElevationOf(centerPosLocal.getPosition());

        // Перевести углы цели в градусы
        const double centerAzimuthDegrees = rad2deg(centerAzimuth);
        const double centerElevationDegrees = rad2deg(centerEelevation);

        return  elevationDegrees <= centerElevationDegrees + elevationHalfSector &&
            elevationDegrees >= centerElevationDegrees - elevationHalfSector &&
            azimuthDegrees <= centerAzimuthDegrees + azimuthHalfSector &&
            azimuthDegrees >= centerAzimuthDegrees - azimuthHalfSector;
    }
    return false;
}


//===============================================================================================
// Расчет высоты носителя над землей (в км)
double optic::CSysOpticThermal::_getCarrierHeight(const double curTime) const
{
    return getObject().getSysMotion().computePoint(curTime).getPointGeo().getAltitude() / 1000.0;
}



//===============================================================================================
// Расчет высоты цели над землей (в км)
double optic::CSysOpticThermal::_getTargetHeight(const double curTime, const CObject& target) const
{
    return target.getSysMotion().computePoint(curTime).getPointGeo().getAltitude() / 1000.0;
}


//===============================================================================================
// Расчет расстояния до цели (в км)
double optic::CSysOpticThermal::_getTargetDistance(const double curTime, const CObject& target) const
{

    const CPoint3D unitPoint   = getObject().getSysMotion().computePoint(curTime).getPointGlobal().getPosition();
    const CPoint3D targetPoint = target.getSysMotion().computePoint(curTime).getPointGlobal().getPosition();

    return unitPoint.computeDistanceTo(targetPoint) / 1000.0;
}


//===============================================================================================
// Расчет координат фона
CPointGeo optic::CSysOpticThermal::_getBgGeodPoint(const double curTime, const double height, const double distance,
    const CObject& carrier, const CObject& target) const
{
    CPoint3D p1 = carrier.getSysMotion().computePoint(curTime).getPointGlobal().getPosition();
    CPoint3D p2 = target.getSysMotion().computePoint(curTime).getPointGlobal().getPosition();

    const double critSize = target.getType().getHeight();
    const double bgDistance = (height + critSize) * distance / height;

    CVector3D targetVector(p2.getX() - p1.getX(), p2.getY() - p1.getY(), p2.getZ() - p1.getZ());
    CVector3D bgVector(bgDistance, targetVector.getCosX(), targetVector.getCosY(), targetVector.getCosZ());
    CPoint3D p3(bgVector.getX() + p1.getX(), bgVector.getY() + p1.getY(), bgVector.getZ() + p1.getZ());
    CPointGlobal bgGlobal(0.0, p3);

    return CTaskPoint(bgGlobal).getPointGeo();
}


//===============================================================================================
// Обработка воздействия "Включение оптико-электронной системы"
void optic::CSysOpticThermal::_processImpactOpticalSwitchOn(const double curTime, const CImpact &impact)
{
    //----------------------------------------------------------------------
    // Планирование события "Включение оптико-электронной системы"
    _processEventSwitchOn(curTime);
}

//===============================================================================================
// Обработка воздействия "Выключение оптико-электронной системы"
void optic::CSysOpticThermal::_processImpactOpticalSwitchOff(const double curTime, const CImpact &impact)
{
    _processEventSwitchOff(curTime);
}


//=============================================================================
// Обработка воздействия "Появление нового объекта"
void optic::CSysOpticThermal::_processImpactThink(const double curTime, const CImpact &impact)
{
    
}

void optic::CSysOpticThermal::_processImpactDie(const double curTime, const CImpact& impact)
{
    
}

void optic::CSysOpticThermal::_createDetectionZone(double time) const
{
   
    CTaskPoint curPoint = getObject().getSysMotion().computePoint(time);
    const CPointGeo& geoPoint = curPoint.getPointGeo();
    const double unitAltitude = geoPoint.getAltitude() / 1000.0;

    InputDataStruct data = myInputDataStruct;
    data.outline.clear();

    if (getType().getOrientationHeight() == 0.0)
        data.carrier.height = unitAltitude;
    else
        data.carrier.height = getType().getOrientationHeight();

    TargetStruct& target = data.target;
    target.height = 0;
    target.distance = 0;
    target.hor_distance = 0;
    target.max_crit_size = 3.0 / 1000.0;

    const CGlobalEnvironment& env = CSysOpticThermalType::MyGlobalEnv;

    AtmosphereStruct& atm = data.atm;
    atm.mdv = env.getMeteoVisibility();
    atm.temp = env.getTemperature(geoPoint.getLatitude(), geoPoint.getLongitude(), geoPoint.getAltitude());
    atm.abs_hum = CEnvironment::getAbsHumidity(env.getRelativeHumidity(), atm.temp - 273.5);
    atm.prec = CEnvironment::getPrecipitationType(env.getGidrometeor());
    atm.is_night = env.getIsNight(geoPoint.getLatitude(), geoPoint.getLongitude());

    ResultDataStruct res;
    res.atm = Calc::Atm::calculateAtmosphereValue(data.atm, data.sys, data.carrier, data.target);
    res.fov = Calc::FOV::calculateFOV(data.sys);

    DistanceResult d_res = Calc::Distance::calculateDistances(data.bearing, data.carrier, data.sys, data.target, res.atm, res.fov);

    if (getType().isScanEnabled())
    {
        // Нахождение нового угла
        d_res.min_distance = sqrt(pow(getType().getMinScanDistance(), 2) + pow(data.carrier.height, 2));
        if (getType().getMaxScanDistance() != 0.0)
            d_res.max_distance = sqrt(pow(getType().getMaxScanDistance(), 2) + pow(data.carrier.height, 2));;
        myScanElevation = max(
            atan(sqrt(pow(d_res.max_distance, 2) / pow(data.carrier.height, 2) - 1)) -
            atan(sqrt(pow(d_res.min_distance, 2) / pow(data.carrier.height, 2) - 1)),
            data.sys.elevation);

        // Пересчитываем высоту матрицы
        data.sys.height *= static_cast<int>(myScanElevation / data.sys.elevation);
        data.sys.elevation = myScanElevation;

    }
    else
    {
        if (getType().getOrientationMaxD() != 0.0)
        {
            d_res.max_distance = sqrt(pow(getType().getOrientationMaxD(), 2) + pow(data.carrier.height, 2));
            d_res.min_distance = data.carrier.height / cos(acos(data.carrier.height / d_res.max_distance) - data.sys.elevation);
        }
        else if (getType().getOrientationMinD() != 0.0)
        {
            d_res.min_distance = sqrt(pow(getType().getOrientationMinD(), 2) + pow(data.carrier.height, 2));
            d_res.max_distance = data.carrier.height / cos(acos(data.carrier.height / d_res.min_distance) + data.sys.elevation);
        }
    }

    myMinDistance = d_res.min_distance * 1000.0;
    myMaxDistance = d_res.max_distance * 1000.0;

    myMinElevation = -(90.0 - std::acos(data.carrier.height / d_res.min_distance) * 180.0 / Pi);
    myMaxElevation = -(90.0 - std::acos(data.carrier.height / d_res.max_distance) * 180.0 / Pi);

    myElevationRange = myMaxElevation - myMinElevation;
    const double elPart = myElevationRange / 2.0;

    myZoneElevation = myMaxElevation - elPart;
        
    if (myIsZoneRegistered)
        return;

    ostringstream out;
    string space = "  ";
    out << "DETECT" << space
        << getObject().getType().getName() << space
        << 0.0 << space
        << 20000.0 << space
        << getType().getDetectionAzimuth() << space
        << -elPart << space
        << elPart << space
        << 0.0 << space
        << 0 << space
        << 190 << space
        << 0 << space
        << 456;

    Film.writeText(out.str());
    myIsZoneRegistered = true;
}

 
//=============================================================================
// Получение описания поверхноcти по координатам
string optic::CSysOpticThermal::_getSurfaceString(const CPointGeo& geoPoint) const
{
    const CGlobalEnvironment& GlobalEnv = CSysOpticThermalType::MyGlobalEnv;

    string surfaceStr;
    const TypeSurface surface = GlobalEnv.getSurfaceType(geoPoint.getLatitude(), geoPoint.getLongitude());
    switch (surface)
    {
    case surface_city:
        surfaceStr = "город";
        break;
    case surface_road:
        surfaceStr = "дорога";
        break;
    case surface_railroad:
        surfaceStr = "железная дорога";
        break;
    case surface_water:
        surfaceStr = "вода";
        break;
    case surface_forest:
        surfaceStr = "лес";
        break;
    case surface_fields:
        surfaceStr = "поле";
        break;
    case surface_snow:
        surfaceStr = "снег";
        break;
    case surface_desertAndRock:
        surfaceStr = "пустыня";
        break;
    case surface_industrialArea:
        surfaceStr = "промзона";
        break;
    default:
        surfaceStr = "не определено";
    }

    return surfaceStr;
}


//=============================================================================
// Вывод в протокол информацию при обнаружении/распознавании
void optic::CSysOpticThermal::_printTargetNewMessage(const double curTime, const CObject& target, const ESysIRTargetState state)
{
    // Координаты носителя и цели в геодезической системе координат
    const CPointGeo carrierGeodPoint = getObject().getSysMotion().computePoint(curTime).getPointGeo();
    const CPointGeo targetGeodPoint = target.getSysMotion().computePoint(curTime).getPointGeo();
    const double unitAltitude = carrierGeodPoint.getAltitude();
    const double targetAltitude = targetGeodPoint.getAltitude();
    const double targetDistance = _getTargetDistance(curTime, target);

    // Координаты фона в геодезической системе координат
    const CPointGeo bgGeodPoint = _getBgGeodPoint(curTime, unitAltitude - targetAltitude, targetDistance, getObject(), target);

    WriteFilmLog(curTime, "%s с помощью ОЭС цель '%s [%04d]'", 
        state == Recognition ? "Распознал" : "Обнаружил", target.getName().c_str(), target.getAbsNum());
    if (getType().isDebugMsg())
    {
        WriteFilmLog(curTime, "> Местность: %s, Фон: %s",
            _getSurfaceString(targetGeodPoint), _getSurfaceString(bgGeodPoint));
    }
}


//=============================================================================
// Обработка запроса перечня зон
void optic::CSysOpticThermal::_makeRequestZoneList(CRequest &request) const
{
   
}