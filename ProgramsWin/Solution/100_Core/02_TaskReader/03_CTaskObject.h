// Файл 03_CTaskObject.h
// Данные о моделируемом объекте в сценарии действий
//

#ifndef Core_TaskReader_CTaskObject_h
#define Core_TaskReader_CTaskObject_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"
#include "01_CTaskPoint.h"
#include "02_CTaskCmd.h"


//=====================================================================================================================
// Данные о моделируемом объекте в сценарии боевых действий
class CTaskObject
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    static CTaskPoint myEmptyPoint;     // Единственный на всю программу технологический экземпляр пустой точки
    static CTaskCmd myEmptyCmd;         // Единственный на всю программу технологический экземпляр пустой команды


private:
    //-----------------------------------------------------------------------------------------------------------------
    string mySceneName;         // Имя сценария, откуда взят объект
    uint32 mySceneNum;          // Номер объекта в сценарии
    size_t myParentIndex;       // Индекс объекта в задаче(!), которому подчиняется данный объект
    string myTypeName;          // Имя типа объектов, экземпляром которого должен быть данный объект
    string myName;              // Основное имя объекта в виртуальном мире, по которому на него можно ссылаться
    bool myIsRed;               // Признак принадлежности к "Красным"


private:
    //-----------------------------------------------------------------------------------------------------------------
    mutable size_t myAbsNum;    // Номер объекта, который был создан по данному описанию в ходе моделирования


private:
    //-----------------------------------------------------------------------------------------------------------------
    vector<CTaskPoint> myPointList;     // Список опорных точек маршрута движения объекта
    vector<CTaskCmd> myCmdList;         // Список команд объекту


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CTaskObject(const string &typeName, const string &name, const vector<CTaskPoint> &pointList, const vector<CTaskCmd> &cmdList, bool isRed);
    CTaskObject(const string &sceneName, uint32 sceneNum, const string &typeName, const string &name, const vector<CTaskPoint> &pointList, const vector<CTaskCmd> &cmdList, bool isRed);
    CTaskObject(const string &sceneName, uint32 sceneNum, size_t parentIndex, const string &typeName, const string &name, const vector<CTaskPoint> &pointList, const vector<CTaskCmd> &cmdList, bool isRed);


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ информации об объекте
    const string & getSceneName() const;
    uint32 getSceneNum() const;
    size_t getParentIndex() const;
    const string & getTypeName() const;
    const string & getName() const;
    bool isRed() const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Управление номером созданного модельного объекта
    size_t getAbsNum() const;
    void saveAbsNum(size_t absNum) const;


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Доступ к маршруту движения объекта
    size_t getPointCount() const;
    const CTaskPoint & getPoint(size_t index) const;

    // Доступ к командам объекту
    size_t getCmdCount() const;
    const CTaskCmd & getCmd(size_t index) const;


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


