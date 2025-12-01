// Файл 12_CObjectTypeList.cpp
// Массив возможных типов объектов, загруженных из текстовых файлов
//

//=====================================================================================================================
// Заголовочные файлы
#include "11_CObjectType.h"
#include "12_CObjectTypeList.h"



//=====================================================================================================================
// Единый на всю программу глобальный массив возможных типов объектов (база типов объектов)
CObjectTypeList CObjectTypeList::myGlobalObjectTypeList;



//=====================================================================================================================
// Конструктор класса
CObjectTypeList::CObjectTypeList()
{
}



//=====================================================================================================================
// Поиск типа объектов по основному имени
size_t CObjectTypeList::find(const string &name) const
{
    // Последовательный обход загруженных типов объектов
    string lowerName = toLower(name);
    for (size_t i = 0; i < myObjectTypeList.size(); ++i)
    {
        // Доступ к очередному типу объектов
        const CObjectType &objectType = myObjectTypeList[i];

        // Проверка совпадения по основному имени
        if (lowerName == toLower(objectType.getName()))
            return i;
    }

    // Если тип объектов не найден
    return INDEX_NOT_FOUND;
}



//=====================================================================================================================
// Доступ к типу объектов по индексу
const CObjectType & CObjectTypeList::get(size_t index) const
{
    // Если индекс некорректный
    if (index >= myObjectTypeList.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return ObjectTypeNull;
    }

    // Всё нормально
    ErrorCode = ERR_OK;
    return myObjectTypeList[index];
}



//=====================================================================================================================
// Доступ к количеству типов объектов
size_t CObjectTypeList::count() const
{
    return myObjectTypeList.size();
}



//=====================================================================================================================
// Загрузка информации о типах объектов из текстовых файлов
bool CObjectTypeList::load(const string &objectsDir)
{
    size_t succeed = 0;       // Количество успешно загруженных файлов
    size_t warnings = 0;      // Из них загружено с предупреждениями
    size_t errors = 0;        // Количество некорректных файлов


    // Получение перечня файлов с характеристиками моделируемых объектов
    Console << "  Загрузка характеристик моделируемых объектов... " << EndL;
    Log     << "  Загрузка характеристик моделируемых объектов... " << EndL;
    Console << "    каталог: \"" << objectsDir << "\"" << EndL;
    Log     << "    каталог: \"" << objectsDir << "\"" << EndL;
    vector<CFileInfo> fileList = CFileInfo::listDirectory(objectsDir, "*.tth");
    if (fileList.empty() == true)
    {
        Console << Error << "    Файлы с характеристиками моделируемых объектов не найдены!" << EndL;
        Log     << Error << "    Файлы с характеристиками моделируемых объектов не найдены!" << EndL;
        return false;
    }


    // Последовательная обработка файлов с характеристиками моделируемых объектов
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

        // Загрузка данных в формуляр типа объектов
        CObjectType newType;
        const CDataMap &data = Parser.getData();
        result r = newType.load(data, fileName);
        if (r == RESULT_ERROR)
        {
            errors++;
            continue;
        }
        else if (r == RESULT_WARNING)
        {
            warnings++;
        }

        // Сохранение характеристик объектов в массиве
        myObjectTypeList.push_back(newType);

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
    if (myObjectTypeList.empty() == true)
    {
        Console << Error << "    Файлы с характеристиками моделируемых объектов некорректны!" << EndL;
        Log     << Error << "    Файлы с характеристиками моделируемых объектов некорректны!" << EndL;
        return false;
    }
        

    Console << EndL;
    Log     << EndL;
    return true;
}



