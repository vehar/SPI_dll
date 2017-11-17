#pragma once

//-------------------1 WIRE IDs--------------------------
#define DEV_PARAM_CNT 10
#define ROM_SZ 8

typedef struct ow_device_t
{
		char ROM_NO[ROM_SZ]; // id- ���������� ������
		char tipe; //��� �������������� ����
		WORD serialNumber; //��������� �����
		char ddOfManufacture; //���� ������������
		char mmOfManufacture; //����� ������������
		char yyOfManufacture; //��� ������������
		char expirationMonths; //���-�� ������� ��������
		char ddOfCommissioning; //���� ����� � ������������
		char mmOfCommissioning; //����� ����� � ������������
		char yyOfCommissioning; //��� ����� � ������������
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
	char state; //1(������ �������������).
	float lat;
	float lon;
	int course;
	int speed;
	GpsTimeType time;
	GpsDateType date;
	char _3D_fix; //3D-Fix- ������������� �����������, 1-��� �����������, 2- ��������� ������������� �����������, 3-����������.
	//TODO:	PDOP, HDOP, VDOP 
	
	/*
		������, �������, PDOP, HDOP, VDOP � ������� (float), ��� �������� 4 �����. 
		�������� �������� (�/���) (word 2�). ����������� ��������, � �������� �� ������(0-359) 0-�����.
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
	float StmTemperature; //����������� STM-a
	float AxelTemp; //����������� �� �������������
	float Akkum_C; //��� ������
	float Akkum_V;	//���������� ������
	float Board_1_8V;
	float Board_3_3V;
	float Board_5V;
	float Board_140V; 
	int InRestMinute; //����� ��� �������� � ���
}; 

extern OnBoardParams boardParams;

//-------------------FUNCs--------------------------
//������� ����������������� ������������� �����
void Program_1w_data(OneWireInfoType* OW_info);