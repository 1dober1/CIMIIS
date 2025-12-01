// Файл 03_CSysTypeList.cpp
// Массив возможных типов систем, загруженных из текстовых файлов
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_CSysTypeNull.h"
#include "03_CSysTypeList.h"



//=====================================================================================================================
// Единый на всю программу глобальный массив возможных типов систем (база типов систем)
CSysTypeList CSysTypeList::myGlobalSysTypeList;



//=====================================================================================================================
// Стандартная функция создания типа систем по ключу, которая ничего не делает
static CSysType * CreateSysDefault(const string &)
{
    return NULL;
}



//=====================================================================================================================
// Конструктор класса
CSysTypeList::CSysTypeList()
{
    myCreateSysFunc = CreateSysDefault;
}



//=====================================================================================================================
// Деструктор класса
CSysTypeList::~CSysTypeList()
{
    // Последовательно удаление данных о типа систем
    for (size_t i = 0; i < mySysTypeList.size(); ++i)
    {
        CSysType *sysType = mySysTypeList[i];
        if (sysType == NULL)
            continue;
        delete sysType;
    }

    // Очистка массива указателей
    mySysTypeList.clear();
}



//=====================================================================================================================
// Поиск типа систем по основному имени
size_t CSysTypeList::find(const string &name) const
{
    // Последовательный обход загруженных типов систем
    string lowerName = toLower(name);
    for (size_t i = 0; i < mySysTypeList.size(); ++i)
    {
        // Доступ к очередному типу систем
        CSysType *sysType = mySysTypeList[i];
        if(sysType == NULL)
            continue;

        // Проверка совпадения по основному имени
        if (lowerName == toLower(sysType->getName()))
            return i;
    }

    // Если тип систем не найден
    return INDEX_NOT_FOUND;
}



//=====================================================================================================================
// Доступ к типу системы по индексу
const CSysType & CSysTypeList::get(size_t index) const
{
    // Если индекс некорректный
    if (index >= mySysTypeList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return SysTypeNull;
    }

    // Если тип систем не загружен
    if (mySysTypeList[index] == NULL)
    {
        ErrorCode = ERR_NULL_POINTER;
        return SysTypeNull;
    }

    // Всё нормально
    ErrorCode = ERR_OK;
    return *mySysTypeList[index];
}



//=====================================================================================================================
// Загрузка информации о типах систем из текстовых файлов
bool CSysTypeList::load(const string &systemsDir)
{
    size_t succeed = 0;       // Количество успешно загруженных файлов
    size_t warnings = 0;      // Из них загружено с предупреждениями
    size_t errors = 0;        // Количество некорректных файлов


    // Получение перечня файлов с характеристиками функциональных систем
    Console << "  Загрузка характеристик функциональных систем... " << EndL;
    Log     << "  Загрузка характеристик функциональных систем... " << EndL;
    Console << "    каталог: \"" << systemsDir << "\"" << EndL;
    Log     << "    каталог: \"" << systemsDir << "\"" << EndL;
    vector<CFileInfo> fileList = CFileInfo::listDirectory(systemsDir, "*.sys");
    if (fileList.empty() == true)
    {
        Console << Error << "    Файлы с характеристиками функциональных систем не найдены!" << EndL;
        Log     << Error << "    Файлы с характеристиками функциональных систем не найдены!" << EndL;
        return false;
    }

       

    // Последовательная обработка файлов с характеристиками функциональных систем
    for (size_t i = 0; i < fileList.size(); ++i)
    {
        // Открытие файла
        string text;
        const string &fileName = fileList[i].getFileName();
        string fullFileName = fileList[i].getFilePath() + string("/") + fileList[i].getFileName();
        if (CTextFile::readFile(fullFileName, text) == false)
        {
            Log << "    Ошибка в " << fileName << ": нет доступа к файлу!" << EndL;
            errors++;
            continue;
        }

        // Синтаксический разбор файла
        Parser.parse(text);
        if (Parser.wasError() == true)
        {
            Log << "    Ошибка в " << fileName << Parser.getErrorMessage() << "  [" << Parser.getScanResult() << "]" << EndL;
            errors++;
            continue;
        }

        // Чтение ключевого слова типа системы
        string sysKey;
        const CDataMap &data = Parser.getData();
        if (extractString(data, fileName, "/Sys_Type", sysKey, false,
            "поле \"Sys_Type\" не найдено!",
            "в поле \"Sys_Type\" не указано значение!",
            "в поле \"Sys_Type\" указано пустое значение!") == false)
        {
            errors++;
            continue;
        }

        // Создание формуляра для типа систем
        CSysType *newType = myCreateSysFunc(toLower(sysKey));
        if (newType == NULL)
        {
            Log << "    Ошибка в " << fileName << ": в поле \"Sys_Type\" указан неизвестный тип системы! <" << sysKey << ">" << EndL;
            errors++;
            continue;
        }

        // Загрузка данных в формуляр типа систем
        result r = newType->load(data, fileName);
        if (r == RESULT_ERROR)
        {
            errors++;
            delete newType;
            continue;
        }
        else if (r == RESULT_WARNING)
        {
            warnings++;
        }

        // Сохранение характеристик системы в массиве
        mySysTypeList.push_back(newType);

        // Файл успешно загружен
        succeed++;
    }


    // Вывод результата
    Console << Success << "    Загружено файлов: " << succeed;
    Log     << Success << "    Загружено файлов: " << succeed;
    if (warnings != 0)
    {
        Console << Regular << ", из них " << Warning << "с предупреждениями: " << warnings;
        Log     << Regular << ", из них " << Warning << "с предупреждениями: " << warnings;
    }
    Console << EndL;
    Log     << EndL;
    if(errors != 0)
    {
        Console << Error << "    Проигнорировано некорректных файлов: " << errors << EndL;
        Log     << Error << "    Проигнорировано некорректных файлов: " << errors << EndL;
    }


    // Проверка, что хотя бы что-то загрузилось
    if (mySysTypeList.empty() == true)
    {
        Console << Error << "    Файлы с характеристиками функциональных систем некорректны!" << EndL;
        Log     << Error << "    Файлы с характеристиками функциональных систем некорректны!" << EndL;
        return false;
    }
    Console << EndL;
    Log     << EndL;
    return true;
}



//=====================================================================================================================
// Указание функции создания типа систем по ключу
void CSysTypeList::setCreateSysType(CSysTypeList::createSysTypeFunc csf)
{
    myCreateSysFunc = csf;
}



