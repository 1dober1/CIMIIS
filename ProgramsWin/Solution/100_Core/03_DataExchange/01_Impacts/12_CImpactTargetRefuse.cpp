// Файл 12_CImpactTargetRefuse.cpp
// Воздействие "Отказ от целеуказания"
//

// Заголовочные файлы
#include "12_CImpactTargetRefuse.h"




//=====================================================================================================================
// Конструктор класса
CImpactTargetRefuse::CImpactTargetRefuse(size_t targetAbsNum)
{
    myTargetAbsNum = targetAbsNum;
}




//=====================================================================================================================
// Деструктор класса
CImpactTargetRefuse::~CImpactTargetRefuse()
{
}




//=====================================================================================================================
// Доступ к параметрам воздействия
size_t CImpactTargetRefuse::getAbsNum() const
{
    return myTargetAbsNum;
}


