// Файл 03_CEquationQuartic.cpp
// Обыкновенное уравнение четвёртой степени
//

//=====================================================================================================================
// Заголовочные файлы
#include "01_CEquationQuadratic.h"
#include "02_CEquationCubic.h"
#include "03_CEquationQuartic.h"




//=====================================================================================================================
// Конструктор класса
CEquationQuartic::CEquationQuartic(double a, double b, double c, double d, double e)
{
    myA = a;
    myB = b;
    myC = c;
    myD = d;
    myE = e;
}




//=====================================================================================================================
// Признак вырожденного уравнения
bool CEquationQuartic::isConfluent() const
{
    // Уравнение вырождено, если коэффициенты при старшем, кубичном,
    // квадратичном и линейном членах все равны нулю
    if (checkEQ(myA, 0.0) == true   &&   checkEQ(myB, 0.0) == true   &&   checkEQ(myC, 0.0) == true   &&   checkEQ(myD, 0.0) == true)
        return true;
    return false;
}




//=====================================================================================================================
// Вычисление корней уравнения
vector<double> CEquationQuartic::solve() const
{
    vector<double> rootList;    // Будущий список корней уравнения


    // Если уравнение вырожденное, то список корней пуст
    if (isConfluent() == true)
        return rootList;


    // Если уравнение кубичное, то использовать алгоритм
    // решения кубичных уравнений
    if (checkEQ(myA, 0.0) == true)
    {
        CEquationCubic e(myB, myC, myD, myE);
        rootList = e.solve();
        return rootList;
    }


    // Разделить обе части уравнения на коэффициент при старшем члене
    // (получится уравнение x^4 + a*x^3 + b*x^2 + c*x + d = 0)
    double a = myB / myA;
    double b = myC / myA;
    double c = myD / myA;
    double d = myE / myA;


    // Решение методом Феррари. Вычисление корней резольвенты
    //    y^3  -  b*y^2  +  (a*c - 4*d)*y  - a*a*d + 4*b*d - c*c = 0
    CEquationCubic resolvent(1.0, -b, a*c - 4.0*d, -a*a*d + 4.0*b*d - c*c);
    vector<double> resolventRootList = resolvent.solve();
    if (resolventRootList.empty() == true)
        return rootList;

    // Для дальнейшего решения используется первый корень (он всегда должен быть)
    double y = resolventRootList[0];

    // Корни исходного уравнения вычисляются как корни уравнений
    //    x*x  + a*x/2 + y/2 =   sqrt( (a*a/4 - b + y)*x*x  +  (a*y/2 - c)*x  +  y*y/4 - d  ),
    //    x*x  + a*x/2 + y/2 = - sqrt( (a*a/4 - b + y)*x*x  +  (a*y/2 - c)*x  +  y*y/4 - d  ),
    // причём подкоренное выражение в правой части является полным квадратом.
    //
    // Соответственно, подкоренное выражение можно представить как (k*x + m)^2 = k*k*x*x* + 2*k*m*x + m*m.
    // Следовательно, k*k = (a*a/4 - b + y), m*m = y*y/4 - d и 2*k*m = a*y/2 - c.
    // Отсюда   k = sqrt(k*k)   и   m =  sqrt(m*m) при a*y/2 - c >= 0
    //   или    k = sqrt(k*k)   и   m = -sqrt(m*m) при a*y/2 - c < 0
    //
    // Тогда уравнения принимают вид:
    //    x*x  + (a/2 + k)*x + y/2 + m = 0,
    //    x*x  + (a/2 - k)*x + y/2 - m = 0.
    double kk = a*a/4.0 - b + y;
    double mm = y*y/4.0 - d;
    if (kk < 0.0   ||   mm < 0.0)
        return rootList;


    // Вычисление вспомогательных коэффициентов k и m
    double k = sqrt(kk);
    double m = sqrt(mm);
    if (a*y/2.0 - c < 0.0)
        m = -m;


    // Решение вспомогательных квадратных уравнений
    CEquationQuadratic extraEquation1(1.0, a/2.0 + k, y/2.0 + m);
    rootList = extraEquation1.solve();
    CEquationQuadratic extraEquation2(1.0, a/2.0 - k, y/2.0 - m);
    vector<double> rootExtraList = extraEquation2.solve();

    // Добавление корней второго уравнения к корням первого
    if (_checkEQ(rootList, rootExtraList) == false)
    {
        for (size_t i = 0; i < rootExtraList.size(); ++i)
        {
            // Поиск очередного корня второго уравнения среди корней первого
            bool newRoot = true;
            for (size_t j = 0; j < rootList.size(); ++j)
            {
                if (checkEQ(rootList[j], rootExtraList[i]) == true)
                {
                    newRoot = false;
                    break;
                }
            }

            // Если очередной корень второго уравнения новый, то добавить его в
            // список корней уравнения четвёртой степени
            if (newRoot == true)
                rootList.push_back(rootExtraList[i]);
        }
    }


    // Выдача списка корней уравнения четвёртой степени
    return rootList;
}




//=====================================================================================================================
// Вспомогательные функции
bool CEquationQuartic::_checkEQ(const vector<double> &left, const vector<double> &right)
{
    // Если размеры не совпадают, то векторы не равны
    if(left.size() != right.size())
        return false;

    // Если хотя бы одна пара не совпадает, то векторы не равны
    for (size_t i = 0; i < left.size(); ++i)
        if (checkEQ(left[i], right[i]) == false)
            return false;

    // Если всё совпадает, то векторы равны
    return true;
}


