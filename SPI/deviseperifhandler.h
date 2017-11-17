#pragma once

//-------------------1 WIRE IDs--------------------------
#define DEV_PARAM_CNT 10
#define ROM_SZ 8

typedef struct ow_device_t
{
		char ROM_NO[ROM_SZ]; // id- микросхемы памяти
		char tipe; //тип аккустического узла
		WORD serialNumber; //заводской номер
		char ddOfManufacture; //день производства
		char mmOfManufacture; //месяц производства
		char yyOfManufacture; //год производства
		char expirationMonths; //кол-во месяцев гарантии
		char ddOfCommissioning; //день ввода в эксплуатацию
		char mmOfCommissioning; //месяц ввода в эксплуатацию
		char yyOfCommissioning; //год ввода в эксплуатацию
		char parameter[DEV_PARAM_CNT];
		WORD CRC16;
}ow_device_t;


typedef struct {
	char SB; //StateBit
	union{
				char da[sizeof(ow_device_t)];
				ow_device_t ow_device;	
			}owd;
}OneWireInfoType;


typedef struct {
	char SB;
	union{
				char da[sizeof(ow_device_t)];
				ow_device_t ow_device;	
			}owd;
}BPIdInfoType;

///////////////////////////////////////////////////

//-------------------GPS_STATE_BITS--------------------------
#define DATA_VALID (1<<7)
#define WEST_EAST (1<<6)
#define NORTH_SOUTH (1<<5)
#define SATELITE_CNT_MASK (0x0F)


typedef struct struct_GpsTime
{
	char sec;
	char min;
	char hour;
}GpsTimeType;

typedef struct struct_GpsDate
{
	char year;
	char month;
	char day;
}GpsDateType;

typedef struct struct_GpsInfo
{	
	char state; //1(данные действительны).
	float lat;
	float lon;
	int course;
	int speed;
	GpsTimeType time;
	GpsDateType date;
	char _3D_fix; //3D-Fix- навигационное определение, 1-нет определения, 2- двумерное навигационное определение, 3-трехмерное.
	//TODO:	PDOP, HDOP, VDOP 
	
	/*
		Широта, долгота, PDOP, HDOP, VDOP в формате (float), что занимает 4 байта. 
		Скорость движения (м/мин) (word 2б). Направление движения, в градусах от севера(0-359) 0-север.
	*/
} GpsInfoType;

extern GpsInfoType GpsInfo;

//-------------------AXELEROMETER--------------------------
typedef struct struct_Axelerometer
{
	int X;
	int Y;
	int Z;
	int temp; //Axel Temperature
}AxelInfoType;

extern AxelInfoType AxelInfo;


// All important params
struct OnBoardParams     
{    
	float StmTemperature; //температура STM-a
	float AxelTemp; //температура на акселерометре
	float Akkum_C; //Ток аккума
	float Akkum_V;	//Напряжение аккума
	float Board_1_8V;
	float Board_3_3V;
	float Board_5V;
	float Board_140V; 
	int InRestMinute; //Время без движения в мин
}; 

extern OnBoardParams boardParams;

//-------------------FUNCs--------------------------
//Команда «Программирование акустического блока»
void Program_1w_data(OneWireInfoType* OW_info);