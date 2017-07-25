#include "DevisePerifHandler.h"


OnBoardParams boardParams;
GpsInfoType GpsInfo;
AxelInfoType AxelInfo;
OneWireInfoType OneWireInfo;

int tmp = 0;


void On_akkum_volt (unsigned char* DataBuf)
{
tmp = ((DataBuf[V_IN_POS] << 8) | (DataBuf[V_IN_POS+1]));	boardParams.Akkum_V = (float)tmp/100;
tmp = ((DataBuf[A_IN_POS] << 8) | (DataBuf[A_IN_POS+1]));	boardParams.Akkum_C = (float)tmp/100;

tmp = ((DataBuf[V_140_POS] << 8) | (DataBuf[V_140_POS+1])); boardParams.Board_140V = (float)tmp/100;
tmp = ((DataBuf[V_5_POS] << 8) | (DataBuf[V_5_POS+1]));		boardParams.Board_5V = (float)tmp/100;
tmp = ((DataBuf[V_3_3_POS] << 8) | (DataBuf[V_3_3_POS+1])); boardParams.Board_3_3V = (float)tmp/100;
tmp = ((DataBuf[V_1_8_POS] << 8) | (DataBuf[V_1_8_POS+1])); boardParams.Board_1_8V = (float)tmp/100;


//"f" specifier not work in DEBUGMSG
//DEBUGMSG(TRUE, (TEXT("SPI_DLL: V_in = %f \r\n"),  volt_input));
/*
printf("SPI_DLL: V_in = %2.2fv \r\n",  volt_input);
printf("SPI_DLL: volt_140 = %2.2fv \r\n",  volt_140);
printf("SPI_DLL: volt_5 = %2.2fv \r\n",  volt_5);
printf("SPI_DLL: volt_3_3 = %2.2fv \r\n",  volt_3_3);
printf("SPI_DLL: volt_1_8 = %2.2fv \r\n",  volt_1_8);
printf("SPI_DLL: current_input = %2.2fmA \r\n",  current_input);
//*/
}



BOOL actual;

int TimeZone = 3; //GMT+3

void On_GPS (unsigned char* DataBuf)
{
union {
		float fd;
		char id[4];
}df;

	SYSTEMTIME st;
	unsigned char state = 0;

	GpsInfo.state = DataBuf[0];

	df.id[0] = DataBuf[1];
	df.id[1] = DataBuf[2];
	df.id[2] = DataBuf[3];
	df.id[3] = DataBuf[4];
	GpsInfo.lat = df.fd;
	
	df.id[0] = DataBuf[5];
	df.id[1] = DataBuf[6];
	df.id[2] = DataBuf[7];
	df.id[3] = DataBuf[8];
	GpsInfo.lon = df.fd;
//46.4712 30.7361

	GpsInfo.speed = DataBuf[9]; //m/s
	GpsInfo.course = ((DataBuf[10]<<8) | DataBuf[11]);

	GpsInfo.time.sec = DataBuf[12];
	GpsInfo.time.min = DataBuf[13];
	GpsInfo.time.hour = DataBuf[14];

	GpsInfo.date.year = DataBuf[15];
	GpsInfo.date.month = DataBuf[16];
	GpsInfo.date.day = DataBuf[17];

	if(GpsInfo.state&DATA_VALID)
	{
	/*printf("%s Lat: %4.4f, %s Lon: %4.4f Date: %d:%d:%d Time: %d:%d:%d\r", ((GpsInfo.state&WEST_EAST) ? "W":"E"), GpsInfo.lat, \
											   ((GpsInfo.state&NORTH_SOUTH) ? "N":"S"), GpsInfo.lon, \
											   GpsInfo.date.day, GpsInfo.date.month, GpsInfo.date.year, \
											   GpsInfo.time.hour+3, GpsInfo.time.min, GpsInfo.time.sec );
 */

	//printf("Speed %d, course %d, satellite cnt %d \r", GpsInfo.speed, GpsInfo.course, (GpsInfo.state&SATELITE_CNT_MASK));
	DEBUGMSG(TRUE, (TEXT("SPI_DLL: GPS data valid\r\n")));
	}
	else
	{
	//printf("Data not valid \r;");
	DEBUGMSG(TRUE, (TEXT("SPI_DLL: GPS data NOT valid\r\n")));
	}
		GetLocalTime(&st);
		if(abs(st.wMinute - GpsInfo.time.min)>5) //System time is different from GPS data
		{
			if(st.wHour != GpsInfo.time.hour + TimeZone) //System time is different from GPS data
			{
				DEBUGMSG(TRUE, (TEXT("SPI_DLL: device date-time actualised! Timezone = GMT %d\r\n", TimeZone)));
			ZeroMemory(&st, sizeof(st));
			st.wDay = GpsInfo.date.day;
			st.wMonth = GpsInfo.date.month;
			st.wYear = 2000 + GpsInfo.date.year;
			st.wHour = TimeZone + GpsInfo.time.hour;
			st.wMinute = GpsInfo.time.min;
			BOOL result = SetLocalTime(&st);
			}
		}

	/*if((GpsInfo.state&DATA_VALID) && (!actual))
	{
		DEBUGMSG(TRUE, (TEXT("SPI_DLL device date-time actualised! Timezone = GMT %d\r\n", TimeZone)));
		ZeroMemory(&st, sizeof(st));
		st.wDay = GpsInfo.date.day;
		st.wMonth = GpsInfo.date.month;
		st.wYear = 2000 + GpsInfo.date.year;
		st.wHour = TimeZone + GpsInfo.time.hour;
		st.wMinute = GpsInfo.time.min;
		BOOL result = SetLocalTime(&st);

		actual = 1;
		//printf("\r\n Sys time updated!");
	}*/
}


void On_Axel_Temp (unsigned char* DataBuf)
{
	
	AxelInfo.temp = ((DataBuf[9]<<8) | DataBuf[10]);
	AxelInfo.X = (int((DataBuf[0]<<8) | DataBuf[1]));
	AxelInfo.Y = (int((DataBuf[2]<<8) | DataBuf[3]));
	AxelInfo.Z = (int((DataBuf[4]<<8) | DataBuf[5]));
	
	if((AxelInfo.X == 0)/* && (AxelInfo.Y == 0)*/ && (AxelInfo.Y == 128))// axel not present
	{
	//for test without axel
	SYSTEMTIME st;
	GetLocalTime(&st);

	AxelInfo.temp = st.wHour;
	AxelInfo.X = -st.wSecond*10;
	AxelInfo.Y = -st.wSecond*10 + 5;
	AxelInfo.Z = -st.wSecond*10 + 50;
	}

	boardParams.AxelTemp = AxelInfo.temp;
	boardParams.StmTemperature = ((DataBuf[7]<<8) | DataBuf[8]);
}

void On_1w_data(unsigned char* DataBuf)
{
	OneWireInfo.StateID_1 = (char)DataBuf[0];
	OneWireInfo.ChipID_1 = (DWORD((DataBuf[1]<<24) | (DataBuf[2]<<16) | (DataBuf[3]<<8) | (DataBuf[4]<<0)));
	OneWireInfo.StateID_2 = (char)DataBuf[29];
	OneWireInfo.ChipID_2 = (DWORD((DataBuf[30]<<24) | (DataBuf[31]<<16) | (DataBuf[32]<<8) | (DataBuf[33]<<0)));
}