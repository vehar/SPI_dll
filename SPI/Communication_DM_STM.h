#ifndef COMMUNICATION_DM_STM_H
#define COMMUNICATION_DM_STM_H

#pragma once
#include "SpiDriver.h"

#define SPI_BUFF_SIZE 64 //rx-tx primary buff
#define CMD_BUFF_SIZE 64 //rx data

#define START_BYTE  0xAA
#define STOP_1_BYTE 0x55
#define STOP_2_BYTE 0xFF

#define START_POS			0
#define COMAND_POS			1
#define PACKED_SIZE_POS		2
#define DATA_1_POS			3
#define DATA_2_POS			4

#define XOR_POS		(PACKED_SIZE_POS+1)
#define STOP_1_POS	(PACKED_SIZE_POS+2)
#define STOP_2_POS	(PACKED_SIZE_POS+3)

class Communication : public SpiDriver
{
public:
	Communication();
	~Communication();

//private:
static unsigned char Rx_buf[SPI_BUFF_SIZE];
static unsigned char Tx_buf[SPI_BUFF_SIZE];
unsigned char DataBuf[CMD_BUFF_SIZE];

//++++++++++++++++++++++++++++++++++++++++
//+++SPI_cust_protocol++++++++++++++++++++
static unsigned char Packed_START;
static unsigned char Packed_CMD;
static unsigned char Packed_SIZE;
static unsigned char Packed_data;

static unsigned char Packed_XOR;
static unsigned char Real_XOR;
static unsigned char Packed_stop;
//+++SPI_cust_protocol++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++

static void CmdPack(int cmd);// fill tx buf with cmd and other data
static void CmdPack(int cmd, int data_cnt, char* data);// fill tx buf with cmd & cpy-ed data buf

static unsigned char  XorCalc (unsigned char *Arr, int start, int stop);
static int  PackedCorrect(unsigned char* Packed);
static void SPI_Handling(void);
static void DataExchange();

static void RxBufClear(void);
static void TxBufClear(void);
};

#endif COMMUNICATION_DM_STM_H