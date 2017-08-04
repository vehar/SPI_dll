
#include "Communication_DM_STM.h"

 unsigned char Communication::Packed_START	= 0;
 unsigned char Communication::Packed_CMD	= 0;
 unsigned char Communication::Packed_SIZE	= 0;
 unsigned char Communication::Packed_data	= 0;

 unsigned char Communication::Packed_XOR	= 0;
 unsigned char Communication::Real_XOR		= 0;
 unsigned char Communication::Packed_stop	= 0;

 unsigned char Communication::Rx_buf[SPI_BUFF_SIZE];
 unsigned char Communication::Tx_buf[SPI_BUFF_SIZE]; 

Communication::Communication()
{
	RxBufClear();
	TxBufClear();
}
 
Communication::~Communication(){}

unsigned char Communication::XorCalc (unsigned char *Arr, int start, int stop)
{
unsigned char result = 0;
	for(int i = start; i<=stop; i++){result ^=Arr[i];} 
return result;
}

void Communication::CmdPack(int cmd)// fill tx buf with cmd and other data
{
TxBufClear();
	Tx_buf[START_POS] = START_BYTE;
	Tx_buf[COMAND_POS] = cmd;//KBD_DATA
	Tx_buf[PACKED_SIZE_POS] = 4;
	Tx_buf[DATA_1_POS] = 0; //CR Управляющее слово
	Tx_buf[DATA_1_POS+1] = 0; //STrac
	Tx_buf[DATA_1_POS+2] = 0; //ESetH
	Tx_buf[DATA_1_POS+3] = 0; //ESetL
	Tx_buf[DATA_1_POS+4] = XorCalc(Tx_buf,COMAND_POS, DATA_1_POS+3);
	Tx_buf[DATA_1_POS+5] = STOP_1_BYTE;
}

void Communication::CmdPack(int cmd, int data_cnt, char* data)// fill tx buf with cmd & cpy-ed data buf
{
TxBufClear();
	Tx_buf[START_POS] = START_BYTE;
	Tx_buf[COMAND_POS] = cmd;//KBD_DATA
	Tx_buf[PACKED_SIZE_POS] = data_cnt;

	memcpy(Tx_buf+DATA_1_POS, data, data_cnt);//cpy data buff into Tx_packed buff

	Tx_buf[XOR_POS+data_cnt]	= XorCalc(Tx_buf,COMAND_POS, PACKED_SIZE_POS+data_cnt);//Код комманды ^ Кол-во байт ^ Данные = XOR
	Tx_buf[STOP_1_POS+data_cnt] = STOP_1_BYTE;
	Tx_buf[STOP_2_POS+data_cnt] = STOP_2_BYTE;
}

int Communication::PackedCorrect(unsigned char* Packed)
{
	static int Header_fail_cnt = 0;
	static int XOR_fail_cnt = 0;

	static int success_cnt = 0;
	static int fail_cnt = 0;

	int result = 0;

	Packed_START	= Packed[START_POS];
	Packed_CMD		= Packed[COMAND_POS];
	Packed_SIZE		= Packed[PACKED_SIZE_POS];
	Packed_XOR		= Packed[DATA_1_POS + Packed_SIZE];

	//printf("St = %X, Cmd:%d, Size:%d, XOR:%d \n",Packed_START, Packed_CMD, Packed_SIZE, Packed_XOR );
	if(Packed_START == START_BYTE 
	&& Packed[DATA_1_POS + Packed_SIZE +1] == STOP_1_BYTE 
	/*&&  Packed[DATA_1_POS + Packed_SIZE +2] == STOP_2_BYTE*/
	) 
	{//if packed valid - check XOR
		Real_XOR		= XorCalc(Packed,COMAND_POS,Packed_SIZE+2);//Код комманды ^ Кол-во байт ^ Данные = XOR
		if(Real_XOR != Packed_XOR)
		{
			XOR_fail_cnt++;
			return 0;
		}
		success_cnt++;
		if(success_cnt == 100) //debug
		{
			while(0){Sleep(0);};
		}
		result = 1;
	}
	else
	{
		//printf("SPI_invalid start %d\n", SPI_Rx_buf[0] ); 
		 RxBufClear();
		 Header_fail_cnt++;
		 if(Header_fail_cnt == 100) //debug
		{
			while(0){Sleep(0);};
		} 
		 if((Header_fail_cnt%2) == 0)
		 {
			 fail_cnt = XOR_fail_cnt + Header_fail_cnt;
			 DEBUGMSG(TRUE, (TEXT("SPI_DLL: PACK_ERR! FAILED = %u SUCCESSED = %u \r\n"),  fail_cnt, success_cnt));
		 }
		 result = 0;
	}
return result;
}

void Communication::DataExchange()
{
	SPI_exchange(Rx_buf, Tx_buf, SPI_BUFF_SIZE);//SpiDriver::
}

void Communication::RxBufClear(void)
{
	memset(Rx_buf,0,SPI_BUFF_SIZE);
}
void Communication::TxBufClear(void)
{
	memset(Tx_buf,0,SPI_BUFF_SIZE);
}

