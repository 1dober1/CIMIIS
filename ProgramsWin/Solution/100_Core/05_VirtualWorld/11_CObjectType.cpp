// Файл 11_CObjectType.cpp
// Описания типа моделируемого объекта
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_CSysTypeNull.h"
#include "03_CSysTypeList.h"
#include "11_CObjectType.h"



//=====================================================================================================================
// Единственный на всю программу технологический экземпляр пустого типа объектов
CObjectType CObjectType::myGlobalObjectTypeNull;

// Единственный на всю программу технологический экземпляр пустого варианта оснащения
CObjectType::equipment CObjectType::myEmptyEquipment;  // Пустой вариант оснащения



//=====================================================================================================================
// Конструктор класса
CObjectType::CObjectType()
{
    myKind = GROUND;
    myIsWeapon = false;
    myHeight = 1.0;
}



//=====================================================================================================================
// Деструктор класса
CObjectType::~CObjectType()
{
    mySysTypeIndexList.clear();
}



//=====================================================================================================================
// Признак того, что это технологический пустой тип объектов
bool CObjectType::isNull() const
{
    if (this == &ObjectTypeNull)
        return true;
    return false;
}



//=====================================================================================================================
// Загрузка информации о типе объектов из текстового файла
result CObjectType::load(const CDataMap &data, const string &fileName)
{
    // Признак наличия предупреждений по содержимому файла
    bool wereWarnings = false;

    // Выделение основного имени типа объектов из имени файла
    myName = extractPureFileName(fileName);
    if (myName.empty() == true)
    {
        Log << "    Ошибка в " << fileName << ": имя файла включает только расширение!" << EndL;
        return RESULT_ERROR;
    }

    // Считывание имени типа объектов для представления пользователю-человеку
    if (extractString(data, fileName, "/show_name", myShowName, false,
        "поле \"show_name\" не найдено!",
        "в поле \"show_name\" не указано значение!",
        "в поле \"show_name\" указано пустое значение!") == false)
        return RESULT_ERROR;

   
    // Считывание природы объектов данного типа
    string kind;
    if (extractString(data, fileName, "/kind", kind, false,
        "поле \"kind\" не найдено!",
        "в поле \"kind\" не указано значение!",
        "в поле \"kind\" указано пустое значение!") == false)
        return RESULT_ERROR;
    kind = toLower(kind);
    if (kind == "воздушный")
    {
        myKind = AIR;
        myIsWeapon = false;
    }
    else if (kind == "поверхность-воздух"   ||   kind == "воздух-воздух"   ||   kind == "воздух-поверхность"   ||   kind == "поверхность-поверхность")
    {
        myKind = AIR;
        myIsWeapon = true;
    }
    else
    {
        myKind = GROUND;
        myIsWeapon = false;
    }   
   

    // Считывание списка функциональных систем
    vector<string> sysFileList;
    if (extractStringColumn(data, fileName, "/systems", sysFileList, false,
        "поле \"systems\" не найдено!",
        "в поле \"systems\" нет ни одной строки!",
        "в поле \"systems\" все значения пустые!") == false)
        return RESULT_ERROR;

    // Анализ списка функциональных систем
    vector<size_t> sysIndexList;
    size_t sysMotionIndex = INDEX_NOT_FOUND;
    for (size_t i = 0; i < sysFileList.size(); ++i)
    {
        // Выделение основного имени функциональной системы
        const string &sysFileName = sysFileList[i];
        string sysName = extractPureFileName(sysFileName);
        if (sysName.empty() == true)
            continue;

        // Поиск функциональной системы в базе систем
        size_t sysIndex = SysTypeList.find(sysName);
        if (sysIndex == INDEX_NOT_FOUND)
        {
            Log << "    Предупреждение в " << fileName << ": поле \"systems\" содержит ссылку на неизвестную систему! <" << sysFileName << ">" << EndL;
            wereWarnings = true;
            continue;
        }

        // Если это система движения
        const CSysType &sysType = SysTypeList.get(sysIndex);
        const CSysMotionType *sysMotion = dynamic_cast<const CSysMotionType *>(&sysType);
        if (sysMotion != NULL)
        {
            // Если уже есть система движения
            if (sysMotionIndex != INDEX_NOT_FOUND)
            {
                Log << "    Предупреждение в " << fileName << ": поле \"systems\" содержит лишнюю ссылку на систему движения! <" << sysFileName << ">" << EndL;
                wereWarnings = true;
            }

            // Если ещё нет системы движения
            else
            {
                // Запись индекса системы движения
                sysMotionIndex = sysIndex;
            }
        }

        // Если это не система движения
        else
        {

            // Запись индекса системы
            sysIndexList.push_back(sysIndex);
        }
    }

    // Если в составе типа объекта не найдено ни одной системы движения
    if (sysMotionIndex == INDEX_NOT_FOUND)
    {
        Log << "    Ошибка в " << fileName << ": поле \"systems\" не содержит ни одной ссылки на систему движения!" << EndL;
        return RESULT_ERROR;
    }

    // Система движения запоминается первой
    mySysTypeIndexList.clear();
    mySysTypeIndexList.push_back(sysMotionIndex);

    // Остальные системы запоминаются после неё
    for (size_t i = 0; i < sysIndexList.size(); ++i)
        mySysTypeIndexList.push_back(sysIndexList[i]);

   // Ключевые поля прочитаны
    if (wereWarnings == true)
        return RESULT_WARNING;
    else
        return RESULT_OK;
}



//=====================================================================================================================
// Создание экземпляра объекта данного типа
CObject CObjectType::createObject(size_t absNum, bool isRed) const
{
    return CObject(*this, absNum, isRed);
}



//=====================================================================================================================
// Доступ к характеристикам типа объектов
const string & CObjectType::getName() const
{
    return myName;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа объектов
const string & CObjectType::getShowName() const
{
    return myShowName;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа объектов
CObjectType::kind CObjectType::getKind() const
{
    return myKind;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа объектов
bool CObjectType::isWeapon() const
{
    return myIsWeapon;
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к характеристикам типа объектов
double CObjectType::getHeight() const
{
    return myHeight;
}



//=====================================================================================================================
// Доступ к типам систем объектов данного типа
size_t CObjectType::getSysTypeCount() const
{
    return mySysTypeIndexList.size();
}



//---------------------------------------------------------------------------------------------------------------------
// Доступ к типам систем объектов данного типа
const CSysType & CObjectType::getSysType(size_t index) const
{
    // Если индекс некорректный
    if (index >= mySysTypeIndexList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return SysTypeNull;
    }

    // Доступ к типу системы
    const CSysType &sysType = SysTypeList.get(mySysTypeIndexList[index]);
    if (ErrorCode != ERR_OK)
        return SysTypeNull;

    // Всё хорошо
    ErrorCode = ERR_OK;
    return sysType;
}


