#ifndef STM_CPU_PROTOCOL_DEFS_H
#define STM_CPU_PROTOCOL_DEFS_H

//STM-CPU_Protocol_defs.h

/*
Accepted notation:
D_ - data (from stm to cpu. OR request from CPU to stm)
C_ - comand (from CPU to stm)
A_ - alarm data (from STM to cpu)
*/

//COMMANDS

//A_ means Alarm/Asynchronous packets
#define A_CURRENT_OVR_YELLOW
#define A_CURRENT_OVR_RED
#define A_STM_ADC
#define A_FREE_FALL
#define A_BATT_LOW_YELLOW
#define A_BATT_LOW_RED
#define A_OVERHEAT_YELLOW	//ПЕРЕГРЕВ ПРИБОРА! 
#define A_OVERHEAT_RED		//ПЕРЕГРЕВ ПРИБОРА! 
#define A_PEP_CONNECTED
#define A_DEVICE_PUT_DOWN
#define A_DEVICE_TAKE_UP

//Comands to/from STM
#define KBD_DATA		(0x00) //
#define VOLTAGE_DATA	(0x01) //
#define GPS_DATA		(0x02) //
#define AXEL_TEMP_DATA	(0x03) //

#define ALL_INFO_DATA	(0x08) //
#define ONE_WIRE_DATA	(0x10) //
#define ONE_WIRE_WRITE	(0x90) //


#define MOUSE_DATA		0x06
#define RAW_DATA		0x07
#define ENCODER_DATA    0x80
#define BTN_CNT_RST     0xC1 //??Очистить счётчик нажатий клавиши( В байте Данных указать номер клавиши, 0х51 - 0х00, 0х52 - 0х01 … 0х61 - 0х06 и тд)

#define V_IN_POS (0)
#define V_140_POS (2)
#define V_5_POS (4)
#define V_3_3_POS (6)
#define V_1_8_POS (8)
#define A_IN_POS (10)

#define AXEL_TEMPERATURE     0xB3
#define STM_TEMPERATURE 0xB4 

#define D_PEP_ID
#define D_GPS_DATA 
#define D_STM_ADC //internal adc: format - STM_ADC.num_of_bytes=3.Channel_code.data[HI].data[Lo]

//------AXELEROMETER------------------------------
#define	D_AXEL //XX.YY.ZZ
//R/W RANDOM INTERNAL REGISTERS
#define C_AXEL_R_REG //USE VERY CAREFULLY!!!
#define C_AXEL_W_REG //USE VERY CAREFULLY!!!


//------SERVICE_CMD------------------------------
#define C_SERVICE// service comand - USE VERY CAREFULLY!!!// POW_ON; POW_OFF; POW_RESET; STM_RESET; RESERVED
	#define POW_ON		1
	#define POW_OFF		2
	#define POW_RESET	4
	#define STM_RESET	8
	#define RESERVED	16




//------------R/W STM RANDOM INTERNAL REGISTERS---------------------
#define C_STM_R_REG //USE VERY CAREFULLY!!!
#define C_STM_W_REG //USE VERY CAREFULLY!!!

//------LED_CMD------------------------------
#define C_LED //LED_ON; LED_OFF; LED_TOGGLE;
enum C_Led_State{LED_ON,LED_OFF,LED_TOGGLE};

//-------------------------------------------
//Key codes

#define BTN_RELEASE (0)
#define BTN_CLICK (0x0F)
#define BTN_PRESS (0x3F)

#define KEY_F1		 0
#define KEY_F2		 1
#define KEY_F3		 2
#define KEY_F4		 3
#define KEY_F5		 4
#define KEY_Joint 	 5 //styk
#define KEY_EncSw	 6
#define KEY_EncUp	 7
#define KEY_EncDn	 8



//////////////////////////////////////////////
extern int EncState;
extern int EncCNT;

extern unsigned short rawVolt; //uint16_t
extern unsigned short rawCurrent; //uint16_t
extern unsigned short StmTemperature; //uint16_t

extern unsigned short Board_5V;
extern unsigned short Board_140V;


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
}; 

extern OnBoardParams boardParams;


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
	char state;
	float lat;
	float lon;
	int course;
	int speed;
	GpsTimeType time;
	GpsDateType date;
} GpsInfoType;

extern GpsInfoType GpsInfo;


typedef struct struct_Axelerometer
{
	int X;
	int Y;
	int Z;
	int temp; //Axel Temperature
}AxelInfoType;

extern AxelInfoType AxelInfo;

typedef struct struct_OneWire
{
	char StateID_1;
	DWORD ChipID_1;

	char StateID_2;
	DWORD ChipID_2;

}OneWireInfoType;

extern OneWireInfoType OneWireInfo;

#endif //STM_CPU_PROTOCOL_DEFS_H