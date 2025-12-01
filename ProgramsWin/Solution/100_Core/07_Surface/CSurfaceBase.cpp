// Файл CSurfaceBase.cpp
// Описание поверхности
//

//=====================================================================================================================
// Заголовочные файлы
#include "CSurfaceBase.h"
#include "CSurface.h"




//=====================================================================================================================
// Общие функции
std::shared_ptr<CSurfaceBase> CSurfaceBase::getSurface(const std::vector<std::string> name)
{
    std::shared_ptr<CSurfaceBase> surfacePtr;
    if (name[0] == "FILE")
        surfacePtr = std::make_shared<CSurface>();
    //else if (name[0] == "PARAMS")
    //  surfacePtr = std::make_shared<CSurfaceParams>();
    //else if (name[0] == "PLANAR")
    //  surfacePtr = std::make_shared<CSurfacePlat>();
    else
        return nullptr;

    if (surfacePtr.get()->_init(name))
        return surfacePtr;
    else
        return nullptr;
}




//---------------------------------------------------------------------------------------------------------------------
// Общие функции
double CSurfaceBase::getDefaultSpeed(TypeSurface type) const
{
    if (type != OUTOFRANGE && type != surface_undefined)
    {
        return dataSurfaceDefauldSpeed.at(static_cast<size_t>(type));
    }
    return 0.0;
}


