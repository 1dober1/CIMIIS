// Файл 02_CEquationCubic.cpp
// Обыкновенное кубическое уравнение
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CEquationQuadratic.h"
#include "02_CEquationCubic.h"




//=====================================================================================================================
// Конструктор класса
CEquationCubic::CEquationCubic(double a, double b, double c, double d)
{
    myA = a;
    myB = b;
    myC = c;
    myD = d;
}




//=====================================================================================================================
// Признак вырожденного уравнения
bool CEquationCubic::isConfluent() const
{
    // Уравнение вырождено, если коэффициенты при старшем, квадратичном
    // и линейном членах все равны нулю
    if (checkEQ(myA, 0.0) == true   &&   checkEQ(myB, 0.0) == true   &&   checkEQ(myC, 0.0) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Вычисление корней уравнения
vector<double> CEquationCubic::solve() const
{
    vector<double> rootList;    // Будущий список корней уравнения


    // Если уравнение вырожденное, то список корней пуст
    if (isConfluent() == true)
        return rootList;


    // Если уравнение квадратное, то использовать алгоритм
    // решения квадратных уравнений
    if (checkEQ(myA, 0.0) == true)
    {
        CEquationQuadratic e(myB, myC, myD);
        rootList = e.solve();
        return rootList;
    }


    // Разделить обе части уравнения на коэффициент при старшем члене
    // (получится уравнение x^3 + a*x^2 + b*x + c = 0)
    double a = myB / myA;
    double b = myC / myA;
    double c = myD / myA;


    // Решение методом Виета-Кардано. Вычисление вспомогательных величин
    //         a*a - 3*b             2*a*a*a - 9*a*b + 27*c
    //    Q = -----------;      R = ------------------------
    //             9                         54
    double Q = (a*a - 3.0*b) / 9.0;
    double R = (2.0*a*a*a - 9.0*a*b + 27.0*c) / 54.0;


    // Если R*R < Q*Q*Q, то уравнение имеет три корня
    //         1            R                                      a
    //    t = --- arccos-------------;   x1 = -2*sqrt(Q)*cos(t) - ---;
    //         3         sqrt(Q*Q*Q)                               3
    //
    //                             2        a                               2        a
    //    x2 = -2*sqrt(Q)*cos(t - ---pi) - ---;    x2 = -2*sqrt(Q)*cos(t + ---pi) - ---
    //                             3        3                               3        3
    if (checkLT(R*R, Q*Q*Q) == true)
    {
        double t = 1.0/3.0 * acos(R / sqrt(Q*Q*Q));
        double x1 = -2.0*sqrt(Q)*cos(t) - a/3.0;
        double x2 = -2.0*sqrt(Q)*cos(t-2.0/3.0*Pi) - a/3.0;
        double x3 = -2.0*sqrt(Q)*cos(t+2.0/3.0*Pi) - a/3.0;
        rootList.push_back(x1);
        rootList.push_back(x2);
        rootList.push_back(x3);
        return rootList;
    }


    // Если R*R >= Q*Q*Q, то уравнение имеет либо один корень (два другие -
    // комплексные), либо один и два совпадающие, либо три совпадающих
    //            R    3 /------------------------
    //     A = - --- * \/ |R| + sqrt(R*R - Q*Q*Q)   ;
    //           |R|    
    //
    //     Если A != 0, то B = Q/A
    //     Если A == 0, то B = 0
    //
    //     x1 = A + B - a/3
    //
    //     Если A == B, то x2 = x3 = -A - a/3
    else
    {
        double A = -R/fabs(R) * pow(fabs(R) + sqrt(R*R - Q*Q*Q), 1.0/3.0);
        double B = 0.0;
        if (checkEQ(A, 0.0) == false)
            B = Q / A;
        double x1 = A + B - a/3.0;
        rootList.push_back(x1);

        if (checkEQ(A, B) == true)
        {
            double x23 = -A - a/3.0;
            if (checkEQ(x23, x1) == false)
                rootList.push_back(x23);
        }
        return rootList;
    }
}

