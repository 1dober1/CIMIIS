
//=====================================================================================================================
// Заголовочные файлы

#include "02_CSysMotionNavedenieNPC.h"




//=====================================================================================================================
// Конструктор класса

CSysMotionNavedenieNPC::CSysMotionNavedenieNPC(const CSysType &sysType, CObject &object)
    : CSysMotion(sysType, object), fl(plane)
{
    // Указание начальных значений параметров
	fl.plane.ttx_bpla = getType().TX_bpla;
}



//=====================================================================================================================
// Деструктор класса

CSysMotionNavedenieNPC::~CSysMotionNavedenieNPC()
{
}



//=====================================================================================================================
// Доступ к типу системы

const CSysMotionNavedenieNPCType & CSysMotionNavedenieNPC::getType() const
{
    return (const CSysMotionNavedenieNPCType &) CSys::getType();
}



//=====================================================================================================================
// Сменные алгоритмы действий

void CSysMotionNavedenieNPC::onAppear(double time)
{
	fl.dt = getType().getTimeStep();
	
	fl.plane.object.myObjNum = this->getObject().getFilmId();
	fl.plane.object.myAbsNum = this->getObject().getAbsNum();

	const vector<CTaskPoint>& route = getInitialRoute();

	for (CTaskPoint elem : route)
	{
		fl.plane.object.myRoute.push_back(elem.getPointGlobal());
		fl.plane.object.myRouteGeo.push_back(elem.getPointGeo());
	}

	// Задание начальной точки и параметров движения

	CPointGlobal p1 = plane.object.getPoint(0);                 // Расчет координат БпЛА в геоцентрической СК в 1 точке

	CPointGeo p1g = p1.toPointGeo();                            // Расчет координат БпЛА  со скорректированной высотой и временем в геоцентрической СК в 1 точке

	double B1 = p1g.getLatitude();                              // Считываем широту в точке 1 в геодезической СК
	double L1 = p1g.getLongitude();                             // Считываем долготу в точке 1 в геодезической СК
	double H1 = p1g.getAltitude();                              // Считываем высоту в точке 1 в геодезической СК

	// Корректировка назначаемой высоты

	if (H1 != 0.0)
	{
		H1 = 0.0;

		p1g = CPointGeo(p1.getTime(), B1, L1, H1);
		p1 = p1g.toPointGlobal();
	}

	//
	
	double x1 = p1.getPosition().getX();                        // Определение координаты x БпЛА в геоцентрической СК в 1 точке
	double y1 = p1.getPosition().getY();                        // Определение координаты y БпЛА в геоцентрической СК в 1 точке
	double z1 = p1.getPosition().getZ();                        // Определение координаты z БпЛА в геоцентрической СК в 1 точке

	CPointGlobal p2 = plane.object.getPoint(1);                 // Расчет координат БпЛА в геоцентрической СК во 2 точке

	double x2 = p2.getPosition().getX();                        // Определение координаты x БпЛА в геоцентрической СК в 1 точке
	double y2 = p2.getPosition().getY();                        // Определение координаты y БпЛА в геоцентрической СК в 1 точке
	double z2 = p2.getPosition().getZ();                        // Определение координаты z БпЛА в геоцентрической СК в 1 точке

	CPoint3D zentr1(x1, y1, z1);                                // Создание центра местной СК в 1 точке
	CPointLocal result1 = p1.toPointLocal(zentr1);              // Перевод точки p1 из геоцентрической СК в местную СК
	CPointLocal result2 = p2.toPointLocal(zentr1);              // Перевод точки p2 из геоцентрической СК в местную СК

	double xg1 = result1.getPosition().getX();                  // Определение координаты xg БпЛА в местной СК в 1 точке
	double xg2 = result2.getPosition().getX();                  // Определение координаты xg БпЛА в местной СК во 2 точке

	double yg1 = result1.getPosition().getY();                  // Определение координаты yg БпЛА в местной СК в 1 точке
	double yg2 = result2.getPosition().getY();                  // Определение координаты yg БпЛА в местной СК во 2 точке

	double zg1 = result1.getPosition().getZ();                  // Определение координаты zg БпЛА в местной СК в 1 точке
	double zg2 = result2.getPosition().getZ();                  // Определение координаты zg БпЛА в местной СК во 2 точке

	double kurs = atan((xg2 - xg1) / (yg2 - yg1));              // Расчет значения угла визирования в горизонтальной плоскости 2 точки из 1 точки

	// Пересчет значения угла визирования в горизонтальной плоскости 2 точки из 1 точки с учетом его знака относительно оси OgYg в местной СК //

	if (((xg2 - xg1) >= 0.0) && ((yg2 - yg1) >= 0.0))
	{
		kurs = kurs;
	}
	else
		if (((xg2 - xg1) >= 0.0) && ((yg2 - yg1) < 0.0))
		{
			kurs = kurs + Pi;
		}
		else
			if (((xg2 - xg1) < 0.0) && ((yg2 - yg1) >= 0.0))
			{
				kurs = kurs + 2 * Pi;
			}
			else
				if (((xg2 - xg1) < 0.0) && ((yg2 - yg1) < 0.0))
				{
					kurs = kurs + Pi;
				}

	double V1 = p1.getVelocity().getLength();                          // Скорость БпЛА в 1 точке
		
	double Vx1 = V1 * p1.getVelocity().getCosX();

	double Vy1 = V1 * p1.getVelocity().getCosY();

	double Vz1 = V1 * p1.getVelocity().getCosZ();

	double gam1 = 0.0;

	fl.F_LastTime = p1.getTime();

	// Запись параметров движения БпЛА в начальный (нулевой) момент времени

	plane.curPos.time = p1.getTime();
	plane.curPos.x = x1;
	plane.curPos.y = y1;
	plane.curPos.z = z1;
	plane.curPos.vx = Vx1;
	plane.curPos.vy = Vy1;
	plane.curPos.vz = Vz1;
	plane.curPos.fi = kurs;
	plane.curPos.teta = 0.0;
	plane.curPos.gamma = gam1;

	plane.Trajectory.push_back(plane.curPos);                          // Запись начальной точки в вектор

	plane.kursvz = kurs * 180.0 / Pi;

	if (getObject().getName() == "БпЛА")
	{
		fl.PrTarg = 0;
	}
	else
	{
		fl.PrTarg = 1;
	}

	plane.ttx_bpla.kren_max = atan(sqrt(plane.ttx_bpla.ny_max * plane.ttx_bpla.ny_max - 1)) * 180.0 / Pi;     // Максимально допустимый крен
	
	if (plane.ttx_bpla.kren_max > 30.0)
	{
		plane.ttx_bpla.kren_max = 30.0;
	}
}



//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNavedenieNPC::onDestroy(double time)
{
}



//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNavedenieNPC::onActivate(double time)
{
	if (!this->getObject().isExist())
	{
		return;
	}
	if (fl.PrTarg == 1)                                     // Если данный объект является НПЦ, а не БПЛА, то задаем его движение
	{

		int ParN = this->getObject().getParentNum();
		
		if (ParN > 0)
		{
			// Вариант 1. Для детей. Отработка всех детей в колонне со второго (дети родителя)

			set<size_t> CList = ObjectList.getObject(ParN).getChildrenList();
			
			for (set<size_t> ::iterator i = CList.begin(); i != CList.end(); ++i )
			{
				size_t num = *i;
				if(!ObjectList.getObject(num).isExist() && fl.FindDead(num) == true)
				{
					fl.init_StopObj = true;
					fl.Time_Stop = time;
					fl.StopNumObject = num;
					fl.OldStopNumbers.push_back(num);
					fl.FindDead(num);
				}
			}

			// Вариант 2. Для родителя. Отработка поражения первого в колонне (родитель для всех в колонне)
			
			if (!ObjectList.getObject(ParN).isExist() && fl.FindDead(ParN) == true)
			{
				fl.init_StopObj = true;
				fl.Time_Stop = time;
				fl.StopNumObject = ParN;
				fl.OldStopNumbers.push_back(ParN);
				fl.FindDead(ParN);
			}
		}
		else
		{
			//Вариант 3. Сам_родитель. Для родителя, проверяем сущестование детей

			set<size_t> CList = this->getObject().getChildrenList();

			for (set<size_t> ::iterator i = CList.begin(); i != CList.end(); ++i)
			{
				size_t num = *i;
				if (!ObjectList.getObject(num).isExist() && fl.FindDead(num) == true)
				{
					fl.init_StopObj = true;
					fl.Time_Stop = time;
					fl.StopNumObject = num;
					fl.OldStopNumbers.push_back(num);
				}
			}
		}

		if (time >= fl.Time_Stop + 60.0 && fl.init_StopObj == true)
		{
			fl.init_StopObj = false;	
			if(fl.StopNumObject > 0)	
			{
				int numFilm = ObjectList.getObject(fl.StopNumObject).getFilmId();
				Film.writeFire(ObjectList.getObject(fl.StopNumObject).getFilmId(), time, false);
				Film.writeState(numFilm, time, CFilm::DEAD);	
			}
		}

		if (fl.res11 != 1)
		{
			fl.res11 = fl.Dvijenie_po_marshrutu();          // Моделирование движения НПЦ

			CTaskPoint CTP_RR(CPointGlobal(plane.curPos.time, CPoint3D(plane.curPos.x, plane.curPos.y, plane.curPos.z), CVector3D(plane.curPos.vx, plane.curPos.vy, plane.curPos.vz), plane.curPos.gamma));
			this->saveLastFixedPoint(CTP_RR);
		}
		if (fl.res11 == 1)
		{
			this->getObject().disappear(time);            // Уничтожение АК
		}
					          
		return;
	}
	
	if (fl.init0 == true)
	{
		
		CObject& target = ObjectList.getObject(fl.NumTarg);	

		// Обход всех систем
		
		CObject & carrier = getObject();
		for (size_t j = 0; j < carrier.getSysCount(); ++j)
		{
			// Если система ничего не знает о боекомплекте, то пропускаем
			
			CSys& sys = carrier.getSys(j);
			if (&sys == this)
				continue;

			CRequestAmmunition requestAmmo;
			if (sys.onRequest(time, requestAmmo) == false)
				continue;

			// Если у системы нет боекомплекта, то пропускаем
			
			const size_t ammunition = requestAmmo.getAmmunition();
			if (ammunition == 0)
				continue;

			CRequestServiceTime requestTime(fl.NumTarg);
			if (sys.onRequest(time, requestTime) == false)
				continue;
		}


		fl.init0 = false;
	}

	
	fl.maxT = time;

	// Моделирование тактической ситуации наведения БпЛА на НПЦ

	// Моделирование взлета БпЛА

	if (fl.res1 != 1)
	{
		fl.res1 = fl.Vzlet(plane.kursvz);                                                        // Производится взлет БпЛА
	}

	//

	// Моделирование набора высоты БпЛА

	if (fl.res1 == 1 && fl.res2 != 1)
	{
		fl.res2 = fl.Nabor_vysoty(plane.ttx_bpla.H_kres, plane.ttx_bpla.teta_nab, plane.ttx_bpla.V_kres);       // Производится набор высоты БпЛА
	}

	//
		
	// Моделирование вывода БпЛА в район НПЦ

	if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 != 1)
	{
		fl.res3 = fl.Vyvod_v_boevoe_soprikosnovenie();                                            // Производится вывод БпЛА в район НПЦ

		if (fl.res3 == -1)
		{
			fl.res3 = 1;
			fl.res4 = 1;
			fl.res5 = 1;
			fl.res6 = 1;
			fl.res7 = 1;
		}
	}
		
	//

	// Моделирование наведения БпЛА на НПЦ

	if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 == 1 && fl.res4 != 1)
	{
		fl.res4 = fl.Navedenie();                                                                 // Производится наведение БпЛА на НПЦ
		
													 
		if (fl.res4 == 1)                                                                         // Если наведение завершено, то производим пуск УР
		{
			CImpactLaunchWeapon impactWeaponLaunchSimple(fl.NumTarg);                             // Формирование объекта-воздействия
			this->getObject().processImpact(fl.F_LastTime, impactWeaponLaunchSimple);
		}

		if (fl.res4 == -1)
		{
			fl.res4 = 1;
			fl.res5 = 1;
			fl.res6 = 1;
			fl.res7 = 1;
		}
	}

	//

	// Моделирование отворота после атаки НПЦ с разгоном до максимальной скорости

	double az = 0;                                                                                // Начальное значение азимута НПЦ относительно БпЛА
	double psi = 0;                                                                               // Начальное значение относительного пеленга в горизонтальной плоскости НПЦ относительно БпЛА

	if (fl.res4 == 1 && fl.init_otv == true)                                                      // Если наведение БпЛА выпонено (БпЛА вышел в точку атаки) и признБпЛА расчета параметров для вычисления угла отворота true
	{
		MyTrajectoryBpLA curPosNPC = fl.getCurPosNPC(time);                                       // Параметры движения в текущий момент времени

		fl.xt_TA = curPosNPC.x;                                                                   // Координата x НПЦ в момент атаки
		fl.yt_TA = curPosNPC.y;                                                                   // Координата y НПЦ в момент атаки
		fl.zt_TA = curPosNPC.z;                                                                   // Координата z НПЦ в момент атаки
		fl.fit_TA = curPosNPC.fi;                                                                 // Курс НПЦ в момент атаки
		fl.timet_TA = curPosNPC.time;                                                             // Время существования НПЦ в момент атаки
		
		fl.init_otv = false;                                                                      // Признак БпЛА расчета параметров для вычисления угла отворота (false - расчет разрешен)
	}

	CPointGlobal pBpLA = CPointGlobal(fl.plane.curPos.time, CPoint3D(fl.x_TA, fl.y_TA, fl.z_TA)); // Создание точки БпЛА в геоцентрической СК

	CPoint3D zentrnm1(fl.x_TA, fl.y_TA, fl.z_TA);                                                 // Создание центра местной СК в точке БпЛА 

	CPointGlobal pNPT = CPointGlobal(fl.timet_TA, CPoint3D(fl.xt_TA, fl.yt_TA, fl.zt_TA));        // Создание точки НПЦ в геоцентрической СК

	CPointLocal result1 = pBpLA.toPointLocal(zentrnm1);                                           // Перевод точки pBpLA из геоцентрической СК в местную СК 
	CPointLocal result2 = pNPT.toPointLocal(zentrnm1);                                            // Перевод точки pNPT из геоцентрической СК в местную СК 


	double xgBpLA = result1.getPosition().getX();                                                 // Определение координаты xg БпЛА в местной СК 
	double xgNPT = result2.getPosition().getX();                                                  // Определение координаты xg НПЦ в местной СК 

	double ygBpLA = result1.getPosition().getY();                                                 // Определение координаты yg БпЛА в местной СК
	double ygNPT = result2.getPosition().getY();                                                  // Определение координаты yg НПЦ в местной СК

	double zgBpLA = result1.getPosition().getZ();                                                 // Определение координаты zg БпЛА в местной СК
	double zgNPT = result2.getPosition().getZ();                                                  // Определение координаты zg НПЦ в местной СК


	if (xgNPT >= xgBpLA)                                                                          // Если НПЦ справа от БпЛА
	{
		az = acos((ygNPT - ygBpLA) / (sqrt((ygNPT - ygBpLA) * (ygNPT - ygBpLA) + (xgNPT - xgBpLA) * (xgNPT - xgBpLA))));            // Расчет азимута НПЦ относительно БпЛА
	}
	else                                                                                                                            // Иначе
	{
		az = 2 * Pi - acos((ygNPT - ygBpLA) / (sqrt((ygNPT - ygBpLA) * (ygNPT - ygBpLA) + (xgNPT - xgBpLA) * (xgNPT - xgBpLA)))); // Расчет азимута НПЦ относительно БпЛА
	}

	psi = abs(az - fl.fi_TA);                                               // Расчет относительного пеленга в горизонтальной плоскости НПЦ относительно БпЛА

	if (fl.Pasp == 0)                                                       // Если применяется УР
	{
		if (((ygNPT >= ygBpLA) && (fl.fit_TA > 0) && (fl.fit_TA < Pi)) || ((ygNPT < ygBpLA) && (fl.fit_TA >= Pi) && (fl.fit_TA <= 2 * Pi)))  // Если НПЦ впереди БпЛА и курс НПЦ лежит в пределах от 0 до пи или НПЦ сзади БпЛА и курс НПЦ лежит в пределах от пи до 2 пи
		{
			fl.ugolotv = Pi / 2 - psi;                                      // Расчет угла отворота БпЛА
		}
		else                                                                // Иначе
		{
			fl.ugolotv = psi - Pi / 2;                                      // Расчет угла отворота БпЛА
		}
	}

	if (fl.Pasp == 1)                                                       // Если применяется АБ или НАР
	{
        double R = plane.ttx_bpla.V_ataki * plane.ttx_bpla.V_ataki / (Settings.getEarthGravity() * plane.ttx_bpla.kren_max * Pi / 180.0);

		if ((fl.Pnpt == 1) || (fl.Pnpt == 2))                               // Если атакуется головная машина в колонне или машина, не являющаяся головной или замыкающей
		{
			double L1 = abs(fl.fit_TA - fl.fi_TA) * R;
			double L2 = (2 * Pi - abs(fl.fit_TA - fl.fi_TA)) * R;

			if (L1 >= L2)
			{
				fl.ugolotv = -(2 * Pi - (fl.fit_TA - fl.fi_TA));
			}
			else
			{
				fl.ugolotv = (fl.fit_TA - fl.fi_TA);
			}
		}

		if (fl.Pnpt == 0)                                                   // Если атакуется замыкающая машина в колонне
		{
			double prot_kurs = fl.fit_TA + Pi;

			// Прведение угла finf к пределам от 0 до 2 пи

			if (prot_kurs >= 2 * Pi)
			{
				prot_kurs = prot_kurs - 2 * Pi;
			}

			double L1 = abs(prot_kurs - fl.fi_TA) * R;
			double L2 = (2 * Pi - abs(prot_kurs - fl.fi_TA)) * R;

			if (L1 >= L2)
			{
				if ((prot_kurs - fl.fi_TA) >= 0.0)
				{
					fl.ugolotv = -(2 * Pi - (prot_kurs - fl.fi_TA));    // Задание значения шага изменения угла поворота траектории БпЛА при правом развороте
				}
				else
				{
					fl.ugolotv = (prot_kurs - fl.fi_TA);
				}
			}
			else
			{
				if ((prot_kurs - fl.fi_TA) >= 0.0)
				{
					fl.ugolotv = (prot_kurs - fl.fi_TA);   // Задание значения шага изменения угла поворота траектории БпЛА при левом развороте
				}
				else
				{
					fl.ugolotv = -(2 * Pi - (prot_kurs - fl.fi_TA));
				}
			}
		}
	}

	//

	if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 == 1 && fl.res4 == 1 && fl.res5 != 1)
	{
		if (fl.Pasp == 0)                                                   // Если применяется УР
		{
			fl.res5 = fl.Razvorot(fl.ugolotv * 180.0 / Pi, plane.ttx_bpla.H_kres, plane.ttx_bpla.V_kres);      // Производится отворот БпЛА после атаки УР
		}
		else                                                                 // Иначе
		{
			fl.res5 = fl.Razvorot(fl.ugolotv * 180.0 / Pi, 5.0 * fl.R_os, plane.ttx_bpla.V_max);   // Производится отворот БпЛА после атаки АБ или НАР
		}
	}

	//

	// Моделирование второго отворота БпЛА на 90 градусов (после атаки УР)

	if (fl.Pasp == 0)                                                      // Если применяется УР
	{
		if (((ygNPT >= ygBpLA) && (fl.fit_TA > 0) && (fl.fit_TA < Pi)) || ((ygNPT < ygBpLA) && (fl.fit_TA >= Pi) && (fl.fit_TA <= 2 * Pi)))   // Если НПЦ впереди БпЛА и курс НПЦ лежит в пределах от 0 до пи или НПЦ сзади БпЛА и курс НПЦ лежит в пределах от пи до 2пи
		{
			fl.ugolotv = Pi / 2;                                           // Расчет угла второго отворота БпЛА
		}
		else                                                               // Иначе
		{
			fl.ugolotv = -Pi / 2;                                          // Расчет угла второго отворота БпЛА
		}

		if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 == 1 && fl.res4 == 1 && fl.res5 == 1 && fl.res6 != 1)
		{
			fl.res6 = fl.Razvorot(fl.ugolotv * 180.0 / Pi, plane.ttx_bpla.H_kres, plane.ttx_bpla.V_max);     // Производится второй отворот БпЛА на 90 градусов
		}
	}
	else                                                                   // Иначе
	{
		fl.res6 = 1;
	}                                                                      // Второй отворот БпЛА не производится

	//

	// Моделирование прямолинейного полета БпЛА

	double Tpol = fl.Rt / plane.ttx_bpla.V_max;                                                            // Определение времени прямолинейного полета БпЛА

	if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 == 1 && fl.res4 == 1 && fl.res5 == 1 && fl.res6 == 1 && fl.res7 != 1)
	{
		fl.res7 = fl.Pryam_polet(plane.ttx_bpla.V_max, Tpol);                                              // Производится прямолинейный полет БпЛА
	}

	//

	// Моделирование вывода БпЛА в район аэродрома

	if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 == 1 && fl.res4 == 1 && fl.res5 == 1 && fl.res6 == 1 && fl.res7 == 1 && fl.res8 != 1)
	{
		fl.res8 = fl.Vyvod_v_rayon_aerodroma(plane.ttx_bpla.H_kres, plane.ttx_bpla.V_kres);                     // Производится вывод БпЛА в район аэродрома
	}

	//

	// Моделирование захода БпЛА на посадку

	if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 == 1 && fl.res4 == 1 && fl.res5 == 1 && fl.res6 == 1 && fl.res7 == 1 && fl.res8 == 1 && fl.res9 != 1)
	{
		fl.res9 = fl.Zahod_na_posadku();                                                              // Производится заход БпЛА на посадку
	}

	//

	// Моделирование посадки БпЛА

	if (fl.res1 == 1 && fl.res2 == 1 && fl.res3 == 1 && fl.res4 == 1 && fl.res5 == 1 && fl.res6 == 1 && fl.res7 == 1 && fl.res8 == 1 && fl.res9 == 1 && fl.res10 != 1)
	{
		fl.res10 = fl.Posadka();                                                                      // Производится посадка БпЛА
	}

	if (fl.res10 == 1 )
	{
		this->getObject().disappear(time);                                                            // Уничтожение БпЛА
	}
	
	CTaskPoint CTP_RR(CPointGlobal(plane.curPos.time, CPoint3D(plane.curPos.x, plane.curPos.y, plane.curPos.z), CVector3D(plane.curPos.vx, plane.curPos.vy, plane.curPos.vz), plane.curPos.gamma));
	this->saveLastFixedPoint(CTP_RR);
}


//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNavedenieNPC::onDisappear(double time)
{
}


//---------------------------------------------------------------------------------------------------------------------
bool CSysMotionNavedenieNPC::onRouteEnd(double time)
{
    return false;        // Подтверждаем окончание маршрута
	
    // Замечание 1
    // После подтверждения окончания маршрута движения моделируемый объект автоматически исчезает

    // Замечание 2
    // Если требуется, чтобы объект продолжал существовать после окончания начального маршрута
    // (например, если движение объекта дальше зависит от событий, происходящих во время моделирования), 
    // то необходимо отклонить окончание маршрута движения (написать "return false;") и не забыть
    // при вычислении новых состояний системы в нужный момент вызвать функцию disappear() у объекта-носителя.
}


//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNavedenieNPC::onTaskCmd(const CTaskCmd &cmd)
{
}


//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNavedenieNPC::onTaskPoint(const CTaskPoint &point)
{
}


//---------------------------------------------------------------------------------------------------------------------
void CSysMotionNavedenieNPC::onImpact(double time, const CImpact &impact)
{
	// Если кто-то появился в виртуальном мире		


    if (fl.PrTarg == 0)                               // Если данный объект является БпЛА
    {
		CObject& carrier = getObject();

        for (size_t j = 0; j < carrier.getSysCount(); ++j)
        {
            // Если система ничего не знает о боекомплекте, то пропускаем

            CSys& sys = carrier.getSys(j);
            if (&sys == this)
                continue;

            // Если у системы нет боекомплекта, то пропускаем

            CRequestAmmunitionParam requestAmmoParam;
            if (sys.onRequest(time, requestAmmoParam) == false)
                continue;

            if (!requestAmmoParam.isCorrect())
                continue;

            fl.missile_Dpm = requestAmmoParam.getMissile_DpmMax();
            fl.missile_sekt = requestAmmoParam.getMissile_sekt();
            fl.R_os = requestAmmoParam.getR_os();
        }

        fl.Rt = plane.ttx_bpla.Rt;                                      // Максимальный радиус поражения огневого средства в составе колонны

        fl.NumTarg = plane.ttx_bpla.NumTarg;                            // Номер цели по списку
        fl.Pnpt = plane.ttx_bpla.Pnpt;                                  // Признак расположения атакуемой НПЦ (0 - замыкающая в колонне, 1 - головная в колонне, 2 - не головная и не замыкающая)

        fl.Prpo = plane.ttx_bpla.Prpo;                                  // Признак ракурса применения АСП (0 - всеракурсное, 1 - невсеракурсное)
        fl.Pasp = plane.ttx_bpla.Pasp;                                  // Признак типа применяемого АСП (0 - УР, 1 - АБ или НАР)

        if (fl.missile_Dpm < 1.5 * plane.ttx_bpla.H_kres)               // Если дальность пуска АСП меньше 1000 м
        {
            fl.Pasp = 1;                              // Значение признака типа применяемого АСП (1 - АБ или НАР)
        }
        else                                          // Иначе
        {
            fl.Pasp = 0;                              // Значение признака типа применяемого АСП (0 - УР)
        }
    }
				
	if (typeid(impact) == typeid(CImpactTargetNew))
	{
		const CImpactTargetNew& impTarg = static_cast<const CImpactTargetNew&>(impact);
		int TarN = impTarg.getAbsNum();

		if (TarN == fl.NumTarg)
		{
			fl.NumTarg = TarN;
			CTaskPoint CP = ObjectList.getObject(fl.NumTarg).getSysMotion().computePoint(time);
			double x_Tar = CP.getPointGlobal().getPosition().getX();
			double y_Tar = CP.getPointGlobal().getPosition().getY();
			double z_Tar = CP.getPointGlobal().getPosition().getZ();
			plane.D_bs = sqrt((plane.curPos.x - x_Tar) * (plane.curPos.x - x_Tar) + (plane.curPos.y - y_Tar) * (plane.curPos.y - y_Tar) + (plane.curPos.z - z_Tar) * (plane.curPos.z - z_Tar));
		}
	}

	if (typeid(impact) == typeid(CImpactTargetHit))
	{
		const CImpactTargetHit& impTarg = static_cast<const CImpactTargetHit&>(impact);
		int TarN = impTarg.getAbsNum();

		if (TarN == fl.NumTarg)
		{
			Film.writeFire(ObjectList.getObject(TarN).getFilmId(), time, true);
		}
	}
		
}


//---------------------------------------------------------------------------------------------------------------------
bool CSysMotionNavedenieNPC::onRequest(double time, CRequest &request)
{
    // Обработать запрос боекомплекта
   
	if (typeid(request) == typeid(CRequestAmmunition))
    {
        // Доступ к запросу
      
		CRequestAmmunition &r = (CRequestAmmunition &)request;

        // Сформировать ответ на запрос
     
		r.setWeaponName("Ракета");
        r.setAmmunition(100);
        r.acknowledge();    // Подтвердить корректность ответа на запрос
        return true;        // Подтвердить, что запрос понят
    }

    // Запросы других видов не поддерживаются
    
	return false;
}




