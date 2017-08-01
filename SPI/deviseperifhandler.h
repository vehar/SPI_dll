#pragma once
#include <windows.h>
#include "Communication_Defines.h"


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