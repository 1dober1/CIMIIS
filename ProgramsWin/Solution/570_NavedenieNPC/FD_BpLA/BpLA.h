//=====================================================================================================================
// АК в n-й момент времени 
//
#ifndef BpLA_h
#define BpLA_h

//=====================================================================================================================
// Заголовочные файлы
#include "TTX_BpLA.h"
#include "Trajectory_BpLA.h"
#include "NObject.h"
#include <vector>
#include <string>


struct MyBpLAStruct
{
	double kursvz;                        // Курс взлета БпЛА (или начальный курс движения НПЦ)
	
	std::string name;                     // Имя 

	vector <MyTrajectoryBpLA> Trajectory; // Вектор параметров движения (struct Trajectory_plane)
			
	MyTrajectoryBpLA curPos;              // Параметры движения в текущий момент времени
		
	NObject object;                        // АК (class NObject)
	
	TTX_BpLA ttx_bpla;                     // ТТХ (struct TTX)

	double D_bs = -1;						// Дальность действия бортовых средств		
		
};

//=====================================================================================================================
#endif
