// Файл 08_CImpactWeaponExplosion.cpp
// Воздействие "Подрыв БЧ АСП", которое отсылает АСП в систему пуска АСП 
//

// Заголовочные файлы
#include "08_CImpactWeaponExplosion.h"




//=====================================================================================================================
// Конструктор класса
CImpactWeaponExplosion::CImpactWeaponExplosion(size_t weaponAbsNum, const CTaskPoint& explosionPoint)
{
    myWeaponAbsNum = weaponAbsNum;
    myExplosionPoint = explosionPoint;
}




//=====================================================================================================================
// Деструктор класса
CImpactWeaponExplosion::~CImpactWeaponExplosion()
{
}




//=====================================================================================================================
// Доступ к параметрам воздействия
size_t CImpactWeaponExplosion::getAbsNum() const
{
    return myWeaponAbsNum;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к параметрам воздействия
const CTaskPoint & CImpactWeaponExplosion::getExplosionPoint() const
{
    return myExplosionPoint;
}


