// Файл 01_CInterpolator.cpp
// Линейный интерполятор
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CInterpolator.h"




//=====================================================================================================================
// Конструктор класса
CInterpolator::CInterpolator(double leftArg, double leftFunc, double rightArg, double rightFunc)
{
    // Если границы совпадают, то нет интерполяции берём просто левую точку
    if (checkEQ(leftArg, rightArg) == true)
    {
        myLeftArg = leftArg;
        myLeftFunc = leftFunc;

        myRightArg = leftArg;
        myRightFunc = leftFunc;

        myK = 0.0;
        myB = leftFunc;
        return;
    }

    // Значение аргумента на левой границе отрезка должно быть строго меньше
    // значения аргумента на правой границе отрезка. Если это не так, то меняем местами
    // левую и правую границу и значения в них
    if (checkLT(leftArg, rightArg) == false)
    {
        double temp = leftArg;
        leftArg = rightArg;
        rightArg = temp;

        temp = leftFunc;
        leftFunc = rightFunc;
        rightFunc = temp;
    }

    // Сохранение информации об отрезке интерполяции
    myLeftArg = leftArg;
    myLeftFunc = leftFunc;

    myRightArg = rightArg;
    myRightFunc = rightFunc;

    // Вычисление коэффициентов интерполирующей прямой
    myK = (myRightFunc - myLeftFunc) / (myRightArg - myLeftArg);
    myB = myLeftFunc - myK*myLeftArg;
}




//=====================================================================================================================
// Интерполяция значения функции в точке
double CInterpolator::computeFuncAt(double x) const
{
    return myK*x + myB;
}




//=====================================================================================================================
// Доступ к параметрам интерполятора
double CInterpolator::getLeftArg() const
{
    return myLeftArg;
}




//-----------------------------------------------------------------------------------------------------------------
// Доступ к параметрам интерполятора
double CInterpolator::getLeftFunc() const
{
    return myLeftFunc;
}




//-----------------------------------------------------------------------------------------------------------------
// Доступ к параметрам интерполятора
double CInterpolator::getRightArg() const
{
    return myRightArg;
}




//-----------------------------------------------------------------------------------------------------------------
// Доступ к параметрам интерполятора
double CInterpolator::getRightFunc() const
{
    return myRightFunc;
}




//-----------------------------------------------------------------------------------------------------------------
// Доступ к параметрам интерполятора
double CInterpolator::getK() const
{
    return myK;
}




//-----------------------------------------------------------------------------------------------------------------
// Доступ к параметрам интерполятора
double CInterpolator::getB() const
{
    return myB;
}



