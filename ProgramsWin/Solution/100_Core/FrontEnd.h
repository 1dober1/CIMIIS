// ‘айл FrontEnd.h
// “очка подключени€ всех возможностей блока 100_Core
//

#ifndef Core_FrontEnd_h
#define Core_FrontEnd_h


//=====================================================================================================================
// —истемы координат положени€ объекта
#include "01_CoordSystems/01_CPointLocal.h"
#include "01_CoordSystems/02_CPointGlobal.h"
#include "01_CoordSystems/03_CPointGeo.h"


//=====================================================================================================================
// Ёлементы блока чтени€ задачи моделировани€
#include "02_TaskReader/01_CTaskPoint.h"
#include "02_TaskReader/02_CTaskCmd.h"
#include "02_TaskReader/03_CTaskObject.h"
#include "02_TaskReader/20_CTaskRouteFlight.h"
#include "02_TaskReader/30_CTaskZonePatrol.h"
#include "02_TaskReader/40_CTaskMsg.h"
#include "02_TaskReader/41_CTaskMsgQueue.h"
#include "02_TaskReader/50_CTask.h"


//=====================================================================================================================
// Ёлементы дл€ обмена данными между системами
#include "03_DataExchange/01_CImpact.h"
#include "03_DataExchange/01_Impacts/01_CImpactAppear.h"
#include "03_DataExchange/01_Impacts/02_CImpactDestroy.h"
#include "03_DataExchange/01_Impacts/03_CImpactDisappear.h"
#include "03_DataExchange/01_Impacts/04_CImpactTargetNew.h"
#include "03_DataExchange/01_Impacts/05_CImpactTargetLost.h"
#include "03_DataExchange/01_Impacts/06_CImpactLaunchWeapon.h"
#include "03_DataExchange/01_Impacts/07_CImpactWeaponLaunchSimple.h"
#include "03_DataExchange/01_Impacts/08_CImpactWeaponExplosion.h"
#include "03_DataExchange/01_Impacts/09_CImpactTargetHit.h"
#include "03_DataExchange/01_Impacts/10_CImpactFireDelay.h"
#include "03_DataExchange/01_Impacts/11_CImpactTargetMiss.h"
#include "03_DataExchange/01_Impacts/12_CImpactTargetRefuse.h"
#include "03_DataExchange/01_Impacts/15_CImpactAttackAllarm.h"

#include "03_DataExchange/02_CRequest.h"
#include "03_DataExchange/02_Requests/01_CRequestAmmunition.h"
#include "03_DataExchange/02_Requests/02_CRequestTimeToMeetWeapon.h"
#include "03_DataExchange/02_Requests/03_CRequestServiceTime.h"
#include "03_DataExchange/02_Requests/06_CRequestAmmunitionParam.h"


//=====================================================================================================================
// Ёлементы генерации фильма
#include "04_OutputWriter/01_CFilm.h"


//=====================================================================================================================
// Ёлементы виртуального модельного ћира
#include "05_VirtualWorld/01_CSysType.h"
#include "05_VirtualWorld/02_CSysTypeNull.h"
#include "05_VirtualWorld/03_CSysTypeList.h"
#include "05_VirtualWorld/04_CSys.h"
#include "05_VirtualWorld/05_CSysNull.h"
#include "05_VirtualWorld/06_CSysMotionType.h"
#include "05_VirtualWorld/07_CSysMotion.h"
#include "05_VirtualWorld/10_CObject.h"
#include "05_VirtualWorld/11_CObjectType.h"
#include "05_VirtualWorld/12_CObjectTypeList.h"
#include "05_VirtualWorld/13_CObjectList.h"


//=====================================================================================================================
// Ёлементы дл€ работы с поверхностью «емли
#include "07_Surface/CSurface.h"
#include "07_Surface/ETypeSurface.h"


//=====================================================================================================================
// ѕодключение файла статической библиотеки при компил€ции
#if !defined(USED_CMAKE_BUILD_TOOL)   &&   defined(COMPILER_MICROSOFT)
__pragma(comment(lib, "100_Core"))
#endif


//=====================================================================================================================
#endif
