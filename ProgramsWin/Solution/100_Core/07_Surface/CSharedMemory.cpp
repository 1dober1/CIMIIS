// Файл CSharedMemory.cpp
// Разделяемая память
//

//=====================================================================================================================
// Заголовочные файлы
#include "CSharedMemory.h"




//=====================================================================================================================
// Конструктор
CSharedMemory::CSharedMemory() {
    m_Size = 0;
}




//=====================================================================================================================
// Деструктор
CSharedMemory::~CSharedMemory() {
    delete[]m_Memory;
}




//=====================================================================================================================
//Попытка подключения к существующему в памяти файлу. Если не удастся - файл запишется в новый QSharedMemory
bool CSharedMemory::readFile(std::string Key) {
    std::streampos size;
    std::ifstream file;
    file.open(Key, std::ios::binary | std::ios::in | std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        m_Memory = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(m_Memory, size);
        file.close();
        m_Size = size;
        return true;
    }
    else
    {
        return false;
    }



    /*QFile File(Key);
    if (!File.open(QIODevice::ReadOnly)){
        std::cout << "\nCan't find this file : " << Key.toStdString() << std::endl;
        return false;
    }
    m_Size = File.size();
    m_Memory.setKey(Key);
    if (!m_Memory.attach()){
        if (!m_Memory.create(m_Size)){
            std::cout << Key.toStdString() << ", cant create" << std::endl;
            return false;
        }
        if (m_Memory.lock()){
            char* data = (char*)m_Memory.data();
            QByteArray byteArray = File.readAll();
            memcpy(data, byteArray, m_Size);
            m_Memory.unlock();
            File.close();
            return true;
        }
        else{
            std::cout << "Cant write into QSharedMemory - memory was locked" << Key.toStdString() << " - please try again" << std::endl;
            return false;
        }
    }
    else{
        std::cout << "File was read from memory" << std::endl;
        return true;
    }*/
}




//=====================================================================================================================
//возвращает значение типа double, которое находится в QSharedMemory на позиции pos. Необходимо учитывать смещение относительно шапки
char CSharedMemory::getCharValue(long long int num) const {
    if (num > m_Size || num < 0) {
        return -1;
    }
    return m_Memory[num];
}




//=====================================================================================================================
//возвращает значение типа int, которое находится в QSharedMemory на позиции pos
int CSharedMemory::getIntValue(long long int pos) {
    if (pos > m_Size || pos < 0) {
        //std::cout << "Out of range" << std::endl;
        return -1;
    }

    int* Value = (int*)(m_Memory + pos);
    int newValue = *Value;
    return newValue;
}




//=====================================================================================================================
//возвращает значение типа double, которое находится в QSharedMemory на позиции pos
double CSharedMemory::getDoubleValue(int pos) {
    if (pos > m_Size || pos < 0) {
        //std::cout << "Out of range" << std::endl;
        return -1;
    }
    double* Value = (double*)(m_Memory + pos);
    double newValue = *Value;
    return newValue;
}




//=====================================================================================================================
//возвращает значение типа short int, которое находится в QSharedMemory на позиции pos
short int CSharedMemory::getShortIntValue(long long int pos) const {
    if (pos > m_Size || pos < 0) {
        //std::cout << "Out of range" << std::endl;
        return -1;
    }
    short int* value = (short int*)(m_Memory + pos);
    short int newValue = *value;
    return newValue;
}
