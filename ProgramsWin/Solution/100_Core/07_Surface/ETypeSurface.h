// Файл ETypeSurface.h
// Типы поверхности
//

#ifndef Core_Surface_ETypeSurface_h
#define Core_Surface_ETypeSurface_h


//=====================================================================================================================
// Заголовочные файлы
#include "../Settings.h"


//=====================================================================================================================
// Типы поверхности
enum TypeSurface
{
    surface_error = -1,             // Внутренняя ошибка
    surface_city = 0,               // Городская зона
    surface_road = 1,               // Дорога
    surface_railroad = 2,           // Железная дорога
    surface_water = 3,              // Вода
    surface_forest = 4,             // Лес
    surface_fields = 5,             // Поле
    surface_snow = 6,               // Заснеженная поверхность
    surface_desertAndRock = 7,      // Пустыня и камни
    surface_industrialArea = 8,     // Индустриальная зона
    surface_undefined = 9,          // Не определено
    surface_highway = 10,           // Автострада
    surface_building = 11,          // Здание
    surface_ground = 12,            // Поверхность

    OUTOFRANGE = 99                 // Ошибка: по запрашиваемым координатам отсутствуют данные.

};


//=====================================================================================================================
#endif


