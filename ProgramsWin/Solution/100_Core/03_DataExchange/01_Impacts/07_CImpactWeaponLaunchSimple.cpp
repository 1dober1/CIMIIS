// Файл 07_CImpactWeaponLaunchSimple.cpp
// Воздействие "Команда на пуск оружия по цели", которая передаётся от Системы пуска АСП в АСП (Система пуска АСП->АСП)
//
// Заголовочные файлы
#include "07_CImpactWeaponLaunchSimple.h"




//=====================================================================================================================
// Конструктор класса
CImpactWeaponLaunchSimple::CImpactWeaponLaunchSimple(size_t targetAbsNum)
{
    myTargetAbsNum = targetAbsNum;
}




//=====================================================================================================================
// Деструктор класса
CImpactWeaponLaunchSimple::~CImpactWeaponLaunchSimple()
{
}




//=====================================================================================================================
// Доступ к параметрам воздействия
size_t CImpactWeaponLaunchSimple::getAbsNum() const
{
    return myTargetAbsNum;
}


