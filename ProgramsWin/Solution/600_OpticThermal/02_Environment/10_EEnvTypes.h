#ifndef _600_OpticThermal_Environment_EEnvTypes_h
#define _600_OpticThermal_Environment_EEnvTypes_h


enum FirstCloudType
{
    firstCloudType_none = 0,			//Нет              
    firstCloudType_cirrus,			    //Перистые   	
    firstCloudType_cirrostratus,        //Перисто-слоистые 
    firstCloudType_highCumulus,			//Высоко-кучевые
    firstCloudType_highLayered,			//Высоко-слоистые  
    firstCloudType_layeredCumulus,		//Слоисто-кучевые  
    firstCloudType_layered,				//Слоистые   
    firstCloudType_layeredRain,			//Слоисто-дождевые 
    firstCloudType_cumulonimus			//Кучево-дождевые  

};
enum SecondCloudType
{
    SecondCloudType_none,			//Нет   
    SecondCloudType_cloudless,		//Безоблачно
    SecondCloudType_highCirrus,		//Высокая перистая
    SecondCloudType_avarageCumulus,	//Средняя кучевая
    SecondCloudType_lowLayered		//Низкая слоистая  

};
enum gidrometeorEnum
{
    GIDROMETEOR_NONE = 0,	   //Нет  
    GIDROMETEOR_MOROS,		   //Морось     
    GIDROMETEOR_RAIN,		   //Дождь          
    GIDROMETEOR_SNOW,		   //Снег        
    GIDROMETEOR_FOG_ADVECT,	   //Туман адекватный   
    GIDROMETEOR_FOG_RADIAT	   //Туман радиационный 
};

#endif
