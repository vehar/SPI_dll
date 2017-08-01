#pragma once
#include <windows.h>
#include "Communication_Defines.h"


#define DEV_PARAM_CNT 10
#define ROM_SZ 8

typedef struct ow_device_t
{
		char ROM_NO[ROM_SZ]; // id- микросхемы пам€ти
		char tipe; //тип аккустического узла
		WORD serialNumber; //заводской номер
		char ddOfManufacture; //день производства
		char mmOfManufacture; //мес€ц производства
		char yyOfManufacture; //год производства
		char expirationMonths; //кол-во мес€цев гарантии
		char ddOfCommissioning; //день ввода в эксплуатацию
		char mmOfCommissioning; //мес€ц ввода в эксплуатацию
		char yyOfCommissioning; //год ввода в эксплуатацию
		char parameter[DEV_PARAM_CNT];
		WORD CRC16;
}ow_device_t;


typedef struct {
	char SB;
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