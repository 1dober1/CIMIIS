#include "NObject.h"

//=====================================================================================================================
// Глобальные объекты
CPointGlobal NObject::myEmptyPoint(0.0, CPoint3D(0.0, 0.0, 0.0), 0.0);
CPointGeo NObject::myEmptyPointGeo(0.0, 0.0, 0.0, 0.0);
//=====================================================================================================================
// Доступ к номеру объекта
uint32 NObject::getObjNum() const
{
	return myObjNum;
}


//=====================================================================================================================
// Доступ к точкам маршрута в геоцентрической декартовой СК
size_t NObject::getPointCount() const
{
	return myRoute.size();
}

//---------------------------------------------------------------------------------------------------------------------
const CPointGlobal& NObject::getPoint(size_t index) const
{
	if (index >= myRoute.size())
		return myEmptyPoint;
	return myRoute[index];
}

//=====================================================================================================================
// Доступ к точкам маршрута в геодезической сферической СК
size_t NObject::getPointGeoCount() const
{
	return myRouteGeo.size();
}

//---------------------------------------------------------------------------------------------------------------------
const CPointGeo& NObject::getPointGeo(size_t index) const
{
	if (index >= myRouteGeo.size())
		return myEmptyPointGeo;
	return myRouteGeo[index];
}
