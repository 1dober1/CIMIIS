// Файл 02_CTableFunction.cpp
// Таблично заданная функция
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CInterpolator.h"
#include "02_CTableFunction.h"




//=====================================================================================================================
// Конструктор класса
CTableFunction::CTableFunction()
{
    // Изначально данные не упорядочены
    myIsSorted = false;
}




//=====================================================================================================================
// Добавление новой опорной точки
void CTableFunction::addPoint(double x, double y)
{
    // Если значение в этой точке уже задано
    if (_findArg(x) == true)
        return;

    // Если значение в этой точке не задано
    myTableInfo.push_back(tXY(x, y));
    myIsSorted = false;
}




//=====================================================================================================================
// Доступ к опорной точке
double CTableFunction::getPointX(size_t index) const
{
    if (index >= myTableInfo.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return 0.0;
    }

    ErrorCode = ERR_OK;
    return myTableInfo[index].first;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к опорной точке
double CTableFunction::getPointY(size_t index) const
{
    if (index >= myTableInfo.size())
    {
        ErrorCode = ERR_BAD_INDEX;
        return 0.0;
    }

    ErrorCode = ERR_OK;
    return myTableInfo[index].second;
}




//=====================================================================================================================
// Доступ к количеству опорных точек
size_t CTableFunction::getPointCount() const
{
    return myTableInfo.size();
}




//=====================================================================================================================
// Удаление всех опорных точек
void CTableFunction::clear()
{
    myTableInfo.clear();
    myIsSorted = false;
}




//=====================================================================================================================
// Вычисление значения функции в точке
double CTableFunction::computeFuncAt(double x) const
{
    // Если функция не задана
    if (myTableInfo.empty() == true)
    {
        ErrorCode = ERR_NO_DATA;
        return 0.0;
    }

    // Упорядочивание значений аргумента функции
    _sort();

    // Если функция не определена в искомой точке
    if (checkLT(x, myTableInfo[0].first) == true   ||   checkGT(x, myTableInfo[myTableInfo.size()-1].first) == true)
    {
        ErrorCode = ERR_OUT_OF_RANGE;
        return 0.0;
    }

    // Поиск двух опорных точек, между которыми находится искомая
    size_t leftIndex = SIZE_MAX;
    for (size_t i = 1; i < myTableInfo.size(); ++i)
    {
        // Доступ к левой и правой границам рассматриваемого интервала
        const tXY &left = myTableInfo[i-1];
        const tXY &right = myTableInfo[i];

        // Если искомое значение совпадает с левой границей интервала
        if (checkEQ(x, left.first) == true)
            return left.second;

        // Если искомое значение совпадает с правой границей интервала
        if (checkEQ(x, right.first) == true)
            return right.second;

        // Если искомое значение лежит внутри интервала
        if (checkGT(x, left.first) == true   &&   checkLT(x, right.first) == true)
        {
            leftIndex = i - 1;
            break;
        }
    }

    // Если функция в искомой точке не определена
    if (leftIndex == SIZE_MAX)
    {
        ErrorCode = ERR_OUT_OF_RANGE;
        return 0.0;
    }

    // Вычисление значения в точке
    ErrorCode = ERR_OK;
    const tXY &left = myTableInfo[leftIndex];
    const tXY &right = myTableInfo[leftIndex + 1];
    CInterpolator inter(left.first, left.second, right.first, right.second);
    return inter.computeFuncAt(x);
}




//=====================================================================================================================
// Доступ к диапазону значений аргумента
double CTableFunction::getMinArg() const
{
    // Если функция не задана
    if (myTableInfo.empty() == true)
    {
        ErrorCode = ERR_NO_DATA;
        return 0.0;
    }

    // Упорядочивание значений аргумента функции
    _sort();

    // Доступ к минимальному значению аргумента
    ErrorCode = ERR_OK;
    return myTableInfo[0].first;
}




//-----------------------------------------------------------------------------------------------------------------
// Доступ к диапазону значений аргумента
double CTableFunction::getMaxArg() const
{
    // Если функция не задана
    if (myTableInfo.empty() == true)
    {
        ErrorCode = ERR_NO_DATA;
        return 0.0;
    }

    // Упорядочивание значений аргумента функции
    _sort();

    // Доступ к минимальному значению аргумента
    ErrorCode = ERR_OK;
    return myTableInfo[myTableInfo.size()-1].first;
}




//=====================================================================================================================
// Вспомогательные функции
void CTableFunction::_sort() const
{
    // Если данные уже упорядочены
    if (myIsSorted == true)
        return;

    // Если данные не упорядчены, то упорядочить их
    sort(myTableInfo.begin(), myTableInfo.end());
    myIsSorted = true;
}




//-----------------------------------------------------------------------------------------------------------------
// Вспомогательные функции
bool CTableFunction::_findArg(double x) const
{
    // Поиск точки
    for (size_t i = 0; i < myTableInfo.size(); ++i)
        if (checkEQ(myTableInfo[i].first, x) == true)
            return true;

    // Если точка не найдена
    return false;
}

