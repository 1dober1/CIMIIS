// Файл UnityBuild.100.cpp
// Средство модульной сборки блока 100_Core
//

//=============================================================================
// Общий файл для модульной сборки (Unity Build)
//
// Данный файл НЕЛЬЗЯ собирать вместе с остальными cpp-файлами!
// Включать в сборку надо ЛИБО только этот файл, ЛИБО все остальные cpp-файлы!
//
#include "01_CoordSystems/01_CPointLocal.cpp"
#include "01_CoordSystems/02_CPointGlobal.cpp"
#include "01_CoordSystems/03_CPointGeo.cpp"
#include "02_TaskReader/01_CTaskPoint.cpp"
#include "02_TaskReader/02_CTaskCmd.cpp"
#include "02_TaskReader/03_CTaskObject.cpp"
#include "02_TaskReader/20_CTaskRouteFlight.cpp"
#include "02_TaskReader/30_CTaskZonePatrol.cpp"
#include "02_TaskReader/40_CTaskMsg.cpp"
#include "02_TaskReader/41_CTaskMsgQueue.cpp"
#include "02_TaskReader/50_CTask.cpp"

#include "03_DataExchange/01_CImpact.cpp"
#include "03_DataExchange/01_Impacts/01_CImpactAppear.cpp"
#include "03_DataExchange/01_Impacts/02_CImpactDestroy.cpp"
#include "03_DataExchange/01_Impacts/03_CImpactDisappear.cpp"
#include "03_DataExchange/01_Impacts/04_CImpactTargetNew.cpp"
#include "03_DataExchange/01_Impacts/05_CImpactTargetLost.cpp"
#include "03_DataExchange/01_Impacts/06_CImpactLaunchWeapon.cpp"
#include "03_DataExchange/01_Impacts/07_CImpactWeaponLaunchSimple.cpp"
#include "03_DataExchange/01_Impacts/08_CImpactWeaponExplosion.cpp"
#include "03_DataExchange/01_Impacts/09_CImpactTargetHit.cpp"
#include "03_DataExchange/01_Impacts/10_CImpactFireDelay.cpp"
#include "03_DataExchange/01_Impacts/11_CImpactTargetMiss.cpp"
#include "03_DataExchange/01_Impacts/12_CImpactTargetRefuse.cpp"
#include "03_DataExchange/01_Impacts/15_CImpactAttackAllarm.cpp"

#include "03_DataExchange/02_CRequest.cpp"
#include "03_DataExchange/02_Requests/01_CRequestAmmunition.cpp"
#include "03_DataExchange/02_Requests/02_CRequestTimeToMeetWeapon.cpp"
#include "03_DataExchange/02_Requests/03_CRequestServiceTime.cpp"
#include "03_DataExchange/02_Requests/06_CRequestAmmunitionParam.cpp"

#include "04_OutputWriter/01_CFilm.cpp"

#include "05_VirtualWorld/01_CSysType.cpp"
#include "05_VirtualWorld/02_CSysTypeNull.cpp"
#include "05_VirtualWorld/03_CSysTypeList.cpp"
#include "05_VirtualWorld/04_CSys.cpp"
#include "05_VirtualWorld/05_CSysNull.cpp"
#include "05_VirtualWorld/06_CSysMotionType.cpp"
#include "05_VirtualWorld/07_CSysMotion.cpp"
#include "05_VirtualWorld/10_CObject.cpp"
#include "05_VirtualWorld/11_CObjectType.cpp"
#include "05_VirtualWorld/12_CObjectTypeList.cpp"
#include "05_VirtualWorld/13_CObjectList.cpp"

#include "07_Surface/CSharedMemory.cpp"
#include "07_Surface/CSurface.cpp"
#include "07_Surface/CSurfaceBase.cpp"




