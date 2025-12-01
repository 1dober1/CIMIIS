// Файл 04_CSystemLog.cpp
// Протокол работы программы
//

//=====================================================================================================================
// Заголовочные файлы
#include "04_CSystemLog.h"
#include "../03_Encoding/UTF8.h"




//=====================================================================================================================
// Единственный на всю программу экземпляр протокола работы
CSystemLog CSystemLog::myGlobalLog;




//=====================================================================================================================
// Конструктор
CSystemLog::CSystemLog()
{
}




//=====================================================================================================================
// Деструктор
CSystemLog::~CSystemLog()
{
    stop();
}




//=====================================================================================================================
// Запуск протоколирования
bool CSystemLog::start(const string &fileName)
{
    // Остановка протоколирования, если оно было включено ранее
    stop();

    // Если имя файла протокола не указано
    if (fileName.empty() == true)
        return false;

    // Открытие файла протокола
    if (myFile.openForWriting(fileName) == false)
        return false;

    return true;
}




//=====================================================================================================================
// Прекращение протоколирования
void CSystemLog::stop()
{
    if (myFile.isOpen() == true)
        myFile.close();
}




//=====================================================================================================================
// Вывод стандартных строк в протокол
CSystemLog & CSystemLog::operator <<(const string & str)
{
    if (myFile.isOpen() == true)
        myFile.write(toUTF8(str));
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод стандартных строк в протокол
CSystemLog & CSystemLog::operator <<(const char *str)
{
    if (myFile.isOpen() == true)
        myFile.write(toUTF8(string(str)));
    return *this;
}




//=====================================================================================================================
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(double val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(size_t val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(uint32 val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(uint16 val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << (uint32)val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(uint8 val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << (uint32)val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(int32 val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(int16 val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << (int32)val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(int8 val)
{
    if (myFile.isOpen() == true)
    {
        ostringstream out;
        out << (int32)val;
        myFile.write(out.str());
    }
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(const CControlChar & cmd)
{
    if (myFile.isOpen() == false)
        return *this;

    if (&cmd == &Tab)
        myFile.write("\t");
    else if (&cmd == &Line)
        myFile.writeLine(string());
    else if (&cmd == &Space)
        myFile.write(" ");
    return *this;
}




//---------------------------------------------------------------------------------------------------------------------
// Вывод числовых значений в протокол
CSystemLog & CSystemLog::operator <<(const CTextColor &)
{
    // Ничего не делать
    return *this;
}




