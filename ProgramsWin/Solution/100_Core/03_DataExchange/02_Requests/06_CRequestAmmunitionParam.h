// Файл 06_CRequestAmmunitionParam.h
// Запрос параметров вооружения
//

#ifndef Core_DataExchange_CRequestAmmunitionParam_h
#define Core_DataExchange_CRequestAmmunitionParam_h


//=====================================================================================================================
// Заголовочные файлы
#include "../../Settings.h"
#include "../02_CRequest.h"


//=====================================================================================================================
// Запрос параметров вооружения
class CRequestAmmunitionParam: public CRequest
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    string myWeaponName;        // Название оружия (результат запроса)

    double myMissile_DpmMax;        // Дальность пуска АСП (макс.)
    double myMissile_sekt;          // Сектор пуска АСП (град.)
    double myR_os;                  // Радиус разлета осколков АСП


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Конструктор класса
    CRequestAmmunitionParam();

    // Деструктор класса
    virtual ~CRequestAmmunitionParam();


public:
    //-----------------------------------------------------------------------------------------------------------------
    // Управление результатами запроса
    const string& getWeaponName() const;
    double getMissile_DpmMax() const;
    double getMissile_sekt() const;
    double getR_os() const;
    void setWeaponName(const string& weaponName);
    void setMissile_DpmMax(double inMissile_Dpm);
    void setMissile_sekt(double inMissile_sekt);
    void setR_os(double inRos);


    //-----------------------------------------------------------------------------------------------------------------
};


//=====================================================================================================================
#endif


