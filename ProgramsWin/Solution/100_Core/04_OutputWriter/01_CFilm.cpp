// Файл 01_CFilm.cpp
// Генератор файла фильма
//
//=====================================================================================================================
// Заголовочные файлы
#include "01_CFilm.h"



//=====================================================================================================================
// Единый на всю программу глобальный генератор фильма
CFilm CFilm::myGlobalFilm;



//=====================================================================================================================
// Конструктор класса
CFilm::CFilm()
{
    myObjectCounter = 2;
    myRedSceneId = 1;
    myBlueSceneId = 2;
}



//=====================================================================================================================
// Деструктор класса
CFilm::~CFilm()
{
    stop();
}



//=====================================================================================================================
// Начало записи фильма
bool CFilm::start(const string &fileName)
{
    // Завершение фильма, если он уже пишется
    stop();

    // Если имя файла фильма не указано
    if (fileName.empty() == true)
        return false;

    // Открытие файла фильма
    if (myFile.openForWriting(fileName) == false)
        return false;

    // Запись команды начала фильма
    myFile.writeLine("START  0.00");

    // Зарегистрировать сценарий за "Красных"
    ostringstream out;
    out << format(0.0, 7, 2)
        << "  OBJECT  "
        << myRedSceneId
        << "  VIRT  0  "
        << toUTF8("КРАСНЫЕ  -  0  ");
        //<< toUTF8("Красные");
    myFile.writeLine(out.str());

    out.str(string());
    out << format(0.0, 7, 2)
        << "  POINT  "
        << myRedSceneId << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 9, 6) << "  "
        << format(0.0, 9, 6) << "  "
        << format(0.0, 9, 6) << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 12, 2) << "  "
        << 0;
    myFile.writeLine(out.str());

    // Зарегистрировать сценарий за "Синих"
    out.str(string());
    out << format(0.0, 7, 2)
        << "  OBJECT  "
        << myBlueSceneId
        << "  VIRT  0  "
        << toUTF8("СИНИЕ  -  0  ");
        //<< toUTF8("Синие");
    myFile.writeLine(out.str());

    out.str(string());
    out << format(0.0, 7, 2)
        << "  POINT  "
        << myBlueSceneId << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 9, 6) << "  "
        << format(0.0, 9, 6) << "  "
        << format(0.0, 9, 6) << "  "
        << format(0.0, 12, 2) << "  "
        << format(0.0, 12, 2) << "  "
        << 0;
    myFile.writeLine(out.str());
    return true;
}



//=====================================================================================================================
// Конец записи фильма
void CFilm::stop()
{
    if (myFile.isOpen() == true)
    {
        myFile.writeLine("END");
        myFile.writeLine("\n\n");
        myFile.close();
    }
}



//=====================================================================================================================
// Доступ к идентификаторам сценариев
uint32 CFilm::getRedSceneId() const
{
    return myRedSceneId;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к идентификаторам сценариев
uint32 CFilm::getBlueSceneId() const
{
    return myBlueSceneId;
}



//=====================================================================================================================
// Регистрация объектов в фильме
uint32 CFilm::registerObject(size_t absNum, double time, const string &type, const string &name, uint32 parentId, kind objKind)
{
    // Если файл фильма не готов
    if (myFile.isOpen() == false)
        return uint32_max;

    // Записать команду регистрации объекта
    myObjectCounter++;
    ostringstream out;
    out << format(time, 7, 2)
        << "  OBJECT  "
        << myObjectCounter;

    size_t pos;
    string unspacedType = type;
    while (pos = unspacedType.find(" "), pos != string::npos)
        unspacedType[pos] = '_';

    switch (objKind)
    {
    case AIR:
        out << "  AIR  0  ";
        break;
    case GROUND:
        out << "  GROUND  0  ";
        break;
    default:
        out << "  VIRT  0  ";
    }

    out << toUTF8(unspacedType)
        << "  SINGLE  "
        << parentId << "  "
        << toUTF8(type)
        << " ["
        << toUTF8(name)
        << "]";
    myFile.writeLine(out.str());

    // Сохранить соответствие между номером в фильме и модельным номером
    myFilmIdAbsNumMap[myObjectCounter] = absNum;
    return myObjectCounter;
}



//=====================================================================================================================
// Запись координат объекта в фильм
void CFilm::writePoint(uint32 objId, double time, double x, double y, double z, double vx, double vy, double vz, double roll)
{
    // Если файл фильма не готов
    if (myFile.isOpen() == false)
        return;

    // Если передан некорректный номер объекта
    if (objId == 0)
        return;
    if (objId > myObjectCounter)
        return;

    // Вычислить длину вектора скорости
    CVector3D velocity(vx, vy, vz);

    // Если вектор скорости нулевой
    double cosx, cosy, cosz;
    if (checkEQ(velocity.getLength(), 0.0) == true)
    {
        cosx = 0.0;
        cosy = 0.0;
        cosz = 0.0;
    }

    // Если вектор скорости ненулевой
    else
    {
        cosx = velocity.getCosX();
        cosy = velocity.getCosY();
        cosz = velocity.getCosZ();
    }

    // Записать точку траектории
    ostringstream out;
    out << format(time, 7, 2)
        << "  POINT  "
        << objId << "  "
        << format(x, 12, 2) << "  "
        << format(y, 12, 2) << "  "
        << format(z, 12, 2) << "  "
        << format(cosx, 9, 6) << "  "
        << format(cosy, 9, 6) << "  "
        << format(cosz, 9, 6) << "  "
        << format(velocity.getLength(), 12, 2) << "  "
        << format(roll, 12, 2) << "  "
        << 0;
    myFile.writeLine(out.str());
}



//---------------------------------------------------------------------------------------------------------------------
// Запись координат объекта в фильм
void CFilm::writePoint(uint32 objId, const CPointGlobal &point)
{
    writePoint(objId, point.getTime(), point.getPosition().getX(), point.getPosition().getY(), point.getPosition().getZ(),
        point.getVelocity().getX(), point.getVelocity().getY(), point.getVelocity().getZ(), point.getRoll());
}



//=====================================================================================================================
// Запись состояния объекта в фильм
void CFilm::writeState(uint32 objId, double time, state objState)
{
    // Если файл фильма не готов
    if (myFile.isOpen() == false)
        return;

    // Если передан некорректный номер объекта
    if (objId == 0)
        return;
    if (objId > myObjectCounter)
        return;

    // Записать состояние объекта
    ostringstream out;
    out << format(time, 7, 2)
        << "  STATE  "
        << objId << "  ";

    switch (objState)
    {
    case ALIVE:
        out << 1;
        break;
    case KILLED:
        out << 0;       //  15 - Перечёркивает крестом; 0 - убирает с экрана. Пока что будем всё убирать
        break;    
    case DESTROY:
        out << 15;
        break;
    case DEAD:
    default:
        out << 0;
    }

    myFile.writeLine(out.str());
}



//=====================================================================================================================
// Запись сообщения по объекту в фильм
void CFilm::writeLog(uint32 objId, double time, const string &text)
{
    static int dopN = 0;
    // Если файл фильма не готов
    if (myFile.isOpen() == false)
        return;

    // Если передан некорректный номер объекта
    if (objId == 0)
        return;
    if (objId > myObjectCounter)
        return;
    if (objId == 8)
        dopN++;
    // Записать состояние объекта
    ostringstream out;
    out << format(time, 7, 2)
        << "  LOG  "
        << objId << "  "
        << toUTF8(text);
    myFile.writeLine(out.str());

}



//=============================================================================
// Запись текстовых данных в файл
void CFilm::writeText(const string& text)
{
    // Если файл фильма не готов
    if (myFile.isOpen() == false)
        return;

    // Записать текст
    myFile.writeLine(toUTF8(text));
}



//=====================================================================================================================
// Запись огня
void CFilm::writeFire(uint32 objId, double time, bool showFire)
{
    // Если генератор фильма не готов к работе
    if (myFile.isOpen() == false)
        return;

    // Если передан некорректный номер объекта
    if (objId == 0)
        return;
    if (objId > myObjectCounter)
        return;

    ////////////////////////////////////////
    // Записать состояние объекта
    ostringstream out;
    out << format(time, 7, 2)
        << "  FIRE  "
        << objId;
    if (showFire == true)
    {
        out << "  1";
    }
    else
    {
        out << "  0";
    }
    myFile.writeLine(out.str());

    ////////////////////////////////////////

}



