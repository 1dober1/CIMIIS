// Файл 50_CTask.cpp
// Описание задачи моделирования
//
//=====================================================================================================================
// Заголовочные файлы
#include "50_CTask.h"




//=====================================================================================================================
// Единое на всю программу глобальное описание задачи моделирования
CTask CTask::myGlobalTask;




//=====================================================================================================================
// Конструктор класса
CTask::CTask()
    : myEmptyObject(string(), string(), vector<CTaskPoint>(1), vector<CTaskCmd>(), true),
      myEmptyRouteFlight(string(), vector<CTaskPoint>(1), true),
      myEmptyZonePatrol(string(), vector<CTaskPoint>(1), true)
{
}




//=====================================================================================================================
// Загрузка описания задачи моделирования
bool CTask::load(const string &taskFileName)
{
    // Извлечение имени задачи моделирования
    myName = extractPureFileName(taskFileName);

    // Загрузка файла задачи моделирования
    string text;
    string fileName = extractFileName(taskFileName);
    Console << "  Загрузка задачи моделирования..." << EndL;
    Log     << "  Загрузка задачи моделирования..." << EndL;
    Console << "    файл задачи: \"" << taskFileName << "\"" << EndL;
    Log     << "    файл задачи: \"" << taskFileName << "\"" << EndL;
    if (CTextFile::readFile(taskFileName, text) == false)
    {
        Log << "    Ошибка в " << fileName << ": нет доступа к файлу!" << EndL;
        return false;
    }

    // Синтаксический разбор файла задачи моделирования
    Parser.parse(text);
    if (Parser.wasError() == true)
    {
        Log << "    Ошибка в " << fileName << Parser.getErrorMessage() << "  [" << Parser.getScanResult() << "]" << EndL;
        return false;
    }


    // Сбор информации о файлах сценариев за "Красных"
    const CDataMap &data = Parser.getData();
    vector<string> redSceneFileList = _extractSceneFileList(data, true);
    if(redSceneFileList.empty() == true)
        Log << "    Заметка в " << fileName << ": не указано ни одного сценария за \"Красных\"" << EndL;
    
    // Сбор информации о файлах сценариев за "Синих"
    vector<string> blueSceneFileList = _extractSceneFileList(data, false);
    if (blueSceneFileList.empty() == true)
        Log << "    Заметка в " << fileName << ": не указано ни одного сценария за \"Синих\"" << EndL;


    // Загрузка сценариев за "Красных"
    bool wereErrors = false;
    bool wereWarnings = false;
    for (size_t i = 0; i < redSceneFileList.size(); ++i)
    {
        result r = _loadScene(redSceneFileList[i], true);
        if (r == RESULT_ERROR)
            wereErrors = true;
        if (r == RESULT_WARNING)
            wereWarnings = true;
    }

    // Загрузка сценариев за "Синих"
    for (size_t i = 0; i < blueSceneFileList.size(); ++i)
    {
        result r = _loadScene(blueSceneFileList[i], false);
        if (r == RESULT_ERROR)
            wereErrors = true;
        if (r == RESULT_WARNING)
            wereWarnings = true;
    }

    // Если были предупреждения
    if (wereWarnings == true)
        Console << "    Внимание! Были предупреждения при загрузке сценариев!" << EndL;

    // Если были ошибки
    if (wereErrors == true)
    {
        Console << "    Внимание! Были ошибки при загрузке сценариев!" << EndL;
        return false;
    }

    // Упорядочивание очереди сообщений
    myMsgQueue.sort();

    Console << EndL;
    Log     << EndL;
    return true;
}




//=====================================================================================================================
// Доступ к имени задачи моделирования
const string & CTask::getName() const
{
    return myName;
}




//=====================================================================================================================
// Доступ к описанию моделируемых объектов
size_t CTask::getObjectCount() const
{
    return myObjectList.size();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к описанию моделируемых объектов
const CTaskObject & CTask::getObject(size_t index) const
{
    // Если индекс некорректный
    if (index >= myObjectList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return myEmptyObject;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return myObjectList[index];
}




//=====================================================================================================================
// Доступ к описанию маршрутов полёта
size_t CTask::getRouteFlightCount() const
{
    return myRouteFlightList.size();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к описанию маршрутов полёта
const CTaskRouteFlight & CTask::getRouteFlight(size_t index) const
{
    // Если индекс некорректный
    if (index >= myRouteFlightList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return myEmptyRouteFlight;
    }

    // Выдача результата
    ErrorCode = ERR_OK;
    return myRouteFlightList[index];
}



//=====================================================================================================================
// Доступ к очереди сообщений
const CTaskMsg & CTask::peekMsg() const
{
    return myMsgQueue.peekMsg();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к очереди сообщений
const CTaskMsg & CTask::popMsg()
{
    return myMsgQueue.popMsg();
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к очереди сообщений
bool CTask::isQueueEmpty() const
{
    return myMsgQueue.isEmpty();
}




//=====================================================================================================================
// Вспомогательные методы
vector<string> CTask::_extractSceneFileList(const CDataMap &data, bool isRed)
{
    ostringstream out;
    string sceneFileName;
    vector<string> sceneFileList;
    CDataMap::const_iterator itr;
    for (size_t i = 1; i < 100; ++i)
    {
        // Конструирование имени искомого элемента
        out.str(string());
        if (isRed == true)
            out << "/RedScene." << i;
        else
            out << "/BlueScene." << i;

        // Поиск ссылки на очередной сценарий
        itr = data.find(out.str());
        if (itr == data.end())
            break;

        // Если значение поля не указано
        if (itr->second.getRowCount() == 0 || itr->second.getColumnCount() == 0)
            continue;

        // Если указано пустое значение
        sceneFileName = itr->second.getValue(0, 0);
        if (sceneFileName.empty() == true)
            continue;

        // Всё в порядке
        sceneFileList.push_back(sceneFileName);
    }

    // Выдача результата
    return sceneFileList;
}




//---------------------------------------------------------------------------------------------------------------------
// Вспомогательные методы
result CTask::_loadScene(const string &fileName, bool isRed)
{
    bool wereWarnings = false;

    // Извлечение имени сценария
    string sceneName = extractPureFileName(fileName);

    // Конструирование полного пути к файлу сценария
    string sceneFileName;
    if (isRed == true)
        sceneFileName = Settings.getRedScenesDir() + fileName;
    else
        sceneFileName = Settings.getBlueScenesDir() + fileName;

    // Загрузка файла сценария
    string text;
    Log << "    Загрузка сценария \"" << fileName << "\" за \"" << (isRed ? "Красных" : "Синих") << "\"..." << EndL;
    if (CTextFile::readFile(sceneFileName, text) == false)
    {
        Log << "      Ошибка в " << fileName << ": нет доступа к файлу!" << EndL;
        return RESULT_ERROR;
    }

    // Синтаксический разбор файла сценария
    Parser.parse(text);
    if (Parser.wasError() == true)
    {
        Log << "      Ошибка в " << fileName << Parser.getErrorMessage() << "  [" << Parser.getScanResult() << "]" << EndL;
        return RESULT_ERROR;
    }

    // Чтение типа сценария
    string sceneType;
    const CDataMap &data = Parser.getData();
  
    // Последовательное считывание информации о моделируемых объектах
    ostringstream out;
    CDataMap::const_iterator itr;
    for (uint32 i = 1; i < 100000; ++i)
    {
        // Конструирование имени очередного элемента, содержащего описание моделируемого объекта
        out.str(string());
        out << "/tu." << i;

        // Поиск ссылки на тип объекта
        itr = data.find(out.str() + "/tu_type");
        if (itr == data.end())
            break;

        // Считывание типа объекта
        string typeFileName;
        if (extractString(data, fileName, out.str() + "/tu_type", typeFileName, false,
            "поле \"" + out.str() + "/tu_type\" не найдено!",
            "в поле \"" + out.str() + "/tu_type\" не указано значение!",
            "в поле \"" + out.str() + "/tu_type\" указано пустое значение!") == false)
            return RESULT_ERROR;

        // Считывание имени объекта
        string objectName;
        if (extractString(data, fileName, out.str() + "/name", objectName, true,
            "поле \"" + out.str() + "/name\" не найдено!",
            "в поле \"" + out.str() + "/name\" не указано значение!",
            "в поле \"" + out.str() + "/name\" указано пустое значение!") == false)
            wereWarnings = true;

        // Считывание номера объекта, которому подчинён данный
        uint32 parentNum;
        if (extractUIntAny(data, fileName, out.str() + "/cp", parentNum, true,
            "поле \"" + out.str() + "/cp\" не найдено!",
            "в поле \"" + out.str() + "/cp\" не указано значение!") == false)
            wereWarnings = true;
        if (parentNum > i)
        {
            Log << "      Предупреждение в " << fileName << ": нижестоящий объект " << i << " определён раньше вышестоящего объекта " << parentNum << "!" << EndL;
            parentNum = 0;
            wereWarnings = true;
        }
        size_t parentIndex = SIZE_MAX;
        for (size_t i = 0; i < myObjectList.size(); ++i)
        {
            if (myObjectList[i].getSceneName() == sceneName && myObjectList[i].getSceneNum() == parentNum)
            {
                parentIndex = i;
                break;
            }
        }

        // Поиск программы поведения объекта
        CDataMap::const_iterator itr = data.find(out.str() + "/program");
        if (itr == data.end())
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" не найдена!" << EndL;
            return RESULT_ERROR;
        }

        // Если программа поведения пустая
        if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" не содержит ни одной строки!" << EndL;
            return RESULT_ERROR;
        }
        if (itr->second.getColumnCount() < 7)
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" содержит менее 7 столбцов!" << EndL;
            return RESULT_ERROR;
        }

        // Считывание программы поведения объекта
        vector<CTaskCmd> cmdList;
        vector<CTaskPoint> pointList;
        for (size_t j = 0; j < itr->second.getRowCount(); ++j)
        {
            // Считывание момента времени
            double time = atof(itr->second.getValue(j, 0).c_str());
            if (time < 0.0)
            {
                Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" содержит отрицательное время!" << EndL;
                return RESULT_ERROR;
            }

            // Считывание имени команды
            const string &name = itr->second.getValue(j, 1);

            // Если это точка маршрута
            if (toLower(name) == "point")
            {
                // Чтение координат точки
                double latitude = atof(itr->second.getValue(j, 2).c_str());     // широта
                double longitude = atof(itr->second.getValue(j, 3).c_str());    // долгота
                double altitude = atof(itr->second.getValue(j, 5).c_str());     // высота над ОЗЭ

                // Добавление точки в маршрут движения
                CTaskPoint newPoint(CPointGeo(time, latitude, longitude, altitude));
                pointList.push_back(newPoint);
            }

            // Если это другая команда
            else
            {
                // Чтение параметров команды
                vector<string> paramList;
                for (size_t k = 2; k < itr->second.getColumnCount(); ++k)
                {
                    const string &param = itr->second.getValue(j, k);
                    if (param.empty() == true)
                        continue;
                    paramList.push_back(param);
                }

                // Добавление команды в список команд
                CTaskCmd newCmd(time, name, paramList);
                cmdList.push_back(newCmd);
            }
        }

        // В маршруте движения объекта должно быть не менее двух точек
        if (pointList.size() < 2)
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" содержит менее двух точек!" << EndL;
            return RESULT_ERROR;
        }

        // Доступ к первой и последней точкам маршрута
        stable_sort(cmdList.begin(), cmdList.end());       // Упорядочиваем команды (формально, чтобы были по порядку)
        stable_sort(pointList.begin(), pointList.end());
        const CTaskPoint &firstPoint = pointList[0];
        const CTaskPoint &lastPoint = pointList[pointList.size()-1];

        // Первая и последняя точки маршрута должны различаться по времени
        if (checkEQ(firstPoint.getPointGlobal().getTime(), lastPoint.getPointGlobal().getTime()) == true)
        {
            Log << "    Ошибка в " << fileName << ": в таблице \"" + out.str() + "/program\" первая и последняя точка совпадают по времени!" << EndL;
            return RESULT_ERROR;
        }

        // Коррекция векторов скорости в точках
        for (size_t i = 0; i < pointList.size()-1; ++i)
        {
            CPointGlobal &curPoint = pointList[i].getPointGlobal();
            const CPointGlobal &nextPoint = pointList[i+1].getPointGlobal();
            CVector3D velocity(curPoint.getPosition(), nextPoint.getPosition());
            double dt = nextPoint.getTime() - curPoint.getTime();
            if (checkEQ(dt, 0.0) == false)
                velocity.setLength(velocity.getLength() / dt);
            curPoint.setVelocity(velocity);
        }
        const CPointGlobal &plp = pointList[pointList.size()-2].getPointGlobal();
        CPointGlobal &lp = pointList[pointList.size()-1].getPointGlobal();
        lp.setVelocity(plp.getVelocity());

        // Добавление нового объекта в задачу моделирования
        string typeName = extractPureFileName(typeFileName);
        CTaskObject newObject(sceneName, i, parentIndex, typeName, objectName, pointList, cmdList, isRed);
        myObjectList.push_back(newObject);

        // Формирование сообщения о появлении объекта
        CTaskMsg appearMsg(firstPoint.getPointGlobal().getTime(), CTaskMsg::MSG_OBJECT_APPEAR, myObjectList.size()-1, 0);
        myMsgQueue.pushMsg(appearMsg);

        // Формирование сообщения об исчезновении объекта
        CTaskMsg disappearMsg(lastPoint.getPointGlobal().getTime(), CTaskMsg::MSG_OBJECT_ROUTE_END, myObjectList.size()-1, pointList.size()-1);
        myMsgQueue.pushMsg(disappearMsg);

        // Формирование сообщений о промежуточных точках маршрута
        for (size_t k = 1; k < pointList.size()-1; ++k)
        {
            CTaskMsg newMsg(pointList[k].getPointGlobal().getTime(), CTaskMsg::MSG_OBJECT_POINT, myObjectList.size()-1, k);
            myMsgQueue.pushMsg(newMsg);
        }

        // Формирование сообщений о командах объекту
        for (size_t k = 0; k < cmdList.size(); ++k)
        {
            CTaskMsg newMsg(cmdList[k].getTime(), CTaskMsg::MSG_OBJECT_CMD, myObjectList.size()-1, k);
            myMsgQueue.pushMsg(newMsg);
        }
    }


    // Последовательное считывание информации об объектах
    for (uint32 i = 1; i < 100000; ++i)
    {
        // Конструирование имени очередного элемента, содержащего описание моделируемого объекта
        out.str(string());
        out << "/te." << i;

        // Поиск ссылки на тип элемента
        itr = data.find(out.str() + "/te_type");
        if (itr == data.end())
            break;

        // Считывание типа элемента
        string typeName;
        if (extractString(data, fileName, out.str() + "/te_type", typeName, false,
            "поле \"" + out.str() + "/te_type\" не найдено!",
            "в поле \"" + out.str() + "/te_type\" не указано значение!",
            "в поле \"" + out.str() + "/te_type\" указано пустое значение!") == false)
            return RESULT_ERROR;

        // Проверка корректности типа элемента
        string typeNameLower = toLower(typeName);
        if (typeNameLower != "зона_барражирования" && typeNameLower != "маршрут_полёта" && typeNameLower != "зона_поиска")
        {
            Log << "    Предупреждение в " << fileName << ": в поле \"" << out.str() << "\" указано неизвестное значение! <" << typeName << ">" << EndL;
            wereWarnings = true;

            // Пропуск неизвестных тактических элементов
            continue;
        }

        // Считывание обозначения элемента
        string elementName;
        if (extractString(data, fileName, out.str() + "/name", elementName, true,
            "поле \"" + out.str() + "/name\" не найдено!",
            "в поле \"" + out.str() + "/name\" не указано значение!",
            "в поле \"" + out.str() + "/name\" указано пустое значение!") == false)
            wereWarnings = true;

        // Поиск программы поведения элемента
        CDataMap::const_iterator itr = data.find(out.str() + "/program");
        if (itr == data.end())
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" не найдена!" << EndL;
            return RESULT_ERROR;
        }

        // Если программа поведения пустая
        if (itr->second.getRowCount() == 0   ||   itr->second.getColumnCount() == 0)
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" не содержит ни одной строки!" << EndL;
            return RESULT_ERROR;
        }
        if (itr->second.getColumnCount() < 7)
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" содержит менее 7 столбцов!" << EndL;
            return RESULT_ERROR;
        }

        // Считывание программы поведения элемента
        vector<CTaskPoint> pointList;
        for (size_t j = 0; j < itr->second.getRowCount(); ++j)
        {
            // Считывание момента времени
            double time = atof(itr->second.getValue(j, 0).c_str());
            if (time < 0.0)
            {
                Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" содержит отрицательное время!" << EndL;
                return RESULT_ERROR;
            }

            // Считывание имени команды
            const string &name = itr->second.getValue(j, 1);

            // Если это точка маршрута
            if (toLower(name) == "point")
            {
                // Чтение координат точки
                double latitude = atof(itr->second.getValue(j, 2).c_str());     // широта
                double longitude = atof(itr->second.getValue(j, 3).c_str());    // долгота
                double altitude = atof(itr->second.getValue(j, 5).c_str());     // высота над ОЗЭ

                // Добавление точки в маршрут движения
                CTaskPoint newPoint(CPointGeo(time, latitude, longitude, altitude));
                pointList.push_back(newPoint);
            }

            // Другие команды в объектах не поддерживаются
        }

        // В маршруте движения элемента должно быть не менее двух точек
        if (pointList.size() < 2)
        {
            Log << "    Ошибка в " << fileName << ": таблица \"" + out.str() + "/program\" содержит менее двух точек!" << EndL;
            return RESULT_ERROR;
        }

        // Доступ к первой и последней точкам маршрута
        stable_sort(pointList.begin(), pointList.end());
        const CTaskPoint &firstPoint = pointList[0];
        const CTaskPoint &lastPoint = pointList[pointList.size()-1];

        // Первая и последняя точки маршрута должны различаться по времени
        if (checkEQ(firstPoint.getPointGlobal().getTime(), lastPoint.getPointGlobal().getTime()) == true)
        {
            Log << "    Ошибка в " << fileName << ": в таблице \"" + out.str() + "/program\" первая и последняя точка совпадают по времени!" << EndL;
            return RESULT_ERROR;
        }

        // Коррекция векторов скорости в точках
        for (size_t i = 0; i < pointList.size()-1; ++i)
        {
            CPointGlobal &curPoint = pointList[i].getPointGlobal();
            const CPointGlobal &nextPoint = pointList[i+1].getPointGlobal();
            CVector3D velocity(curPoint.getPosition(), nextPoint.getPosition());
            double dt = nextPoint.getTime() - curPoint.getTime();
            if (checkEQ(dt, 0.0) == false)
                velocity.setLength(velocity.getLength() / dt);
            curPoint.setVelocity(velocity);
        }
        const CPointGlobal &plp = pointList[pointList.size()-2].getPointGlobal();
        CPointGlobal &lp = pointList[pointList.size()-1].getPointGlobal();
        lp.setVelocity(plp.getVelocity());

        // Добавление нового маршрута полёта в задачу моделирования
        if (typeNameLower == "маршрут_полёта")
        {
            CTaskRouteFlight newRouteFlight(sceneName, i, elementName, pointList, isRed);
            myRouteFlightList.push_back(newRouteFlight);
        }

    }

    return RESULT_OK;
}

