#ifndef _600_OpticThermal_System_CSysOpticThermal_h
#define _600_OpticThermal_System_CSysOpticThermal_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CSysOpticThermalType.h"
#include "../01_Calc/10_DataStructs.h"
#include "../02_Environment/01_CEnvironment.h"
#include "../02_Environment/02_CGlobalEnvironment.h"
#include "../91_Utility/02_CFormat.h"


namespace SysOpticThermal
{
    //=====================================================================================================================
    // Перечисление "Режим работы оптико-электронной системы"
    enum ESysIRTargetState
    {
        Detection,
        Recognition,
    };

    //=====================================================================================================================
    // Структура с динамической вероятностью обнаружения цели
    struct TargetP
    {
        ESysIRTargetState state;
        double cur_prob;
        double threshold_prob;
    };


    //=====================================================================================================================
    // Класс новой системы
    class CSysOpticThermal: public CSys
    {
    public:
        //-----------------------------------------------------------------------------------------------------------------
        enum EMode
        {
            Wide,
            Narrow
        };

    private:
        //-----------------------------------------------------------------------------------------------------------------
        static CEnvironment MyEnv;              // Параметры среды для оптико-электронной системы

    private:
        //-----------------------------------------------------------------------------------------------------------------
        bool myIsSwitchOn;                      // Признак включения оптико-электронной системы
        map<size_t, TargetP> myInRangeMap;      // Перечень объектов, попавших в зону оптико-электронной системы, но еще не обнаруженных, с их вероятностями обнаружения
        list<size_t> myTargetList;              // Перечень обнаруженных оьъектов
        size_t myTargetNum;

        mutable double myMinDistance;           // Ближняя граница сектора оптико-электронной системы
        mutable double myMaxDistance;           // Дальняя граница сектора оптико-электронной системы

        mutable double myMinElevation;          // Мин. сектора по углу места
        mutable double myMaxElevation;          // Макс. сектора по углу места

        mutable double myZoneElevation;
        mutable double myElevationRange;

        mutable double myScanElevation;         // Значение угла места при использовании продольного сканирования

        InputDataStruct myInputDataStruct;      // Входные данные для расчета вероятности
        EMode myOperationMode;                  // Режим работы оптико-электронной системы
        double myLastUpdateTime;                // Время последнего обновления информации о целях

        mutable bool myIsZoneRegistered = false;

    public:
        //-----------------------------------------------------------------------------------------------------------------
        // Конструктор класса
        CSysOpticThermal(const CSysType& sysType, CObject& object);


        //-----------------------------------------------------------------------------------------------------------------
        // Деструктор класса
        virtual ~CSysOpticThermal();


    public:
        //-----------------------------------------------------------------------------------------------------------------
        // Доступ к типу системы
        virtual const CSysOpticThermalType& getType() const;


    public:
        //-----------------------------------------------------------------------------------------------------------------
        // Сменные алгоритмы действий
        virtual void onAppear(double time);
        virtual void onDestroy(double time);
        virtual void onActivate(double time);
        virtual void onDisappear(double time);

        virtual void onTaskCmd(const CTaskCmd& taskCmd);
        virtual void onTaskPoint(const CTaskPoint& taskPoint);
        virtual void onImpact(double time, const CImpact& impact);
        virtual bool onRequest(double time, CRequest& request);


        //-----------------------------------------------------------------------------------------------------------------
        // Управление списком объектов, находящихся на максимальной дальности оптико-электронной системы
        double getMinDistance() const { return myMinDistance; }
        double getMaxDistance() const { return myMaxDistance; }


        //-----------------------------------------------------------------------------------------------------------------
        // Доступ к признаку включения оптико-электронной системы
        bool isSwitchedOn() const { return myIsSwitchOn; }


    private:
        //-----------------------------------------------------------------------------------------------------------------
        // Методы обработки событий
        // Обработка события включения оптико-электронной системы
        void _processEventSwitchOn(double curTime);
       
        // Обработка события выключения оптико-электронной системы
        void _processEventSwitchOff(double curTime);
        
        // Обработка события смены режима работы оптико-электронной системы
        bool _processEventSwitchMode(double curTime, size_t targetNum);
        
        // Обработка события завязки оптической трассы
        void _processEventTargetNew(double curTime, size_t targetNum);
       
        // Обработка события потери трассы оптико-электронной системой
        void _processEventTargetLost(double curTime, size_t targetNum);
        
        // Обработка события работы оптико-электронной системы
        void _processEventThink(double curTime);
        
        // Обработка события создания широкой зоны оптико-электронной системы
        void _processEventCreateDetZone();


        //-----------------------------------------------------------------------------------------------------------------
        // Обработка воздействия "Появление нового объекта"

        // Обработка воздействия "Включение оптико-электронной системы"
        void _processImpactOpticalSwitchOn(double curTime, const CImpact& impact);

        // Обработка воздействия "Выключение оптико-электронной системы"
        void _processImpactOpticalSwitchOff(double curTime, const CImpact& impact);

        // Обработка воздействия "Шаг работы оптико-электронной системы"
        void _processImpactThink(double curTime, const CImpact& impact);

        // Обработка воздействия "Гибель объекта"
        void _processImpactDie(double curTime, const CImpact& impact);


        //-----------------------------------------------------------------------------------------------------------------
        // Проверка попадания цели в зону оптико-электронной системы
        bool _checkInOpticalZone(double curTime, const CObject& target) const;


        //-----------------------------------------------------------------------------------------------------------------
        // Обновление структуры с параметрами для расчета вероятности
        void _updateInputData(double curTime, const CObject& target, ESysIRTargetState state);


        //-----------------------------------------------------------------------------------------------------------------
        // Расчет вероятности
        ResultDataStruct _computeStaticP() const;
        void _computeDynamicP(double static_p, map<size_t, TargetP>::iterator itTarget);

        //-----------------------------------------------------------------------------------------------------------------
        // Расчет высоты носителя оптико-электронной системы над землей (в км)
        double _getCarrierHeight(double curTime) const;

        // Расчет высоты цели над землей (в км)
        double _getTargetHeight(double curTime, const CObject& target) const;

        
        // Расчет расстояния до цели (в км)
        double _getTargetDistance(double curTime, const CObject& target) const;

        // Расчет координат фона
        CPointGeo _getBgGeodPoint(double curTime, double height,
            double distance, const CObject& carrier, const CObject& target) const;


        //-----------------------------------------------------------------------------------------------------------------
        // Управление списком объектов, находящихся в зоне действия оптико-электронной системы
        map<size_t, TargetP>::iterator _addInRange(size_t targetNum) 
        {
            TargetP tp;
            tp.state = ESysIRTargetState::Detection;
            tp.cur_prob = 0.0;
            tp.threshold_prob = real(rand()) / real(RAND_MAX);
            return myInRangeMap.insert(make_pair(targetNum, tp)).first;
        }
        void _removeInRange(map<size_t, TargetP>::iterator it) { myInRangeMap.erase(it); }
        map<size_t, TargetP>::iterator _findInRange(size_t targetNum) { return myInRangeMap.find(targetNum); }


        //-----------------------------------------------------------------------------------------------------------------
        // Управление списком тактических единиц, обнаруженных оптико-электронной системой
        bool _checkTarget(size_t targetNum) const { return find(myTargetList.begin(), myTargetList.end(), targetNum) != myTargetList.end(); }


        //-----------------------------------------------------------------------------------------------------------------
        // Создание зон
        void _createDetectionZone(double time) const;
       

        //-----------------------------------------------------------------------------------------------------------------
        // Получение описания поверхности по координатам
        string _getSurfaceString(const CPointGeo& geoPoint) const;


        //-----------------------------------------------------------------------------------------------------------------
        // Вывод в протокол информацию при обнаружении/распознавании
        void _printTargetNewMessage(double curTime, const CObject& target, const ESysIRTargetState state);


    private:
        //-----------------------------------------------------------------------------------------------------------------
        // Обработка запроса перечня зон
        void _makeRequestZoneList(CRequest &request) const;


        //-----------------------------------------------------------------------------------------------------------------
        // Тестовая запись в лога фильм
        template<typename ...Args>
        void WriteFilmLog(double time, const char* format, Args... args) const;

        
        //-----------------------------------------------------------------------------------------------------------------
    };

    template <typename ... Args>
    void CSysOpticThermal::WriteFilmLog(double time, const char* format, Args... args) const
    {
        const std::string msg = Utility::format("%s: %s", getType().getName().c_str(), Utility::format(format, args...).c_str());
        Film.writeLog(getObject().getFilmId(), time, msg);
    }


    //=====================================================================================================================
}


//=====================================================================================================================
#endif