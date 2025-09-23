
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "stm32l4xx.h"

#define MAX_TX_BUFF_SIZE		128
#define MAX_RX_RING_BUFF_SIZE	256
#define MAX_RX_BUFF_SIZE		128

#define CMD_BIT_MANUAL_START	0x00
#define CMD_MEASUER_START		0x01
#define CMD_MEASUER_STOP		0x02
#define CMD_MEASUER_READ_VAL	0x03

#define CMD_START_FAN_CLEAN		0x04
#define CMD_GET_AT_CLEAN_SEC	0x05
#define CMD_AT_CLEAN_INTVAL		0x06

#define	CMD_BOUNDARY_VOLT		0x07

#define CMD_PD_TEST_MODE		0x08
#define CMD_SIPM_TEST_MODE		0x09
#define CMD_SLEEP_MODE			0x0A
#define CMD_WAKE_UP				0x0B

#define CMD_DEVICE_INFO			0x0C
#define CMD_DEVICE_VER			0x0D
#define CMD_DEVICE_STATE		0x0E
#define CMD_SOFT_RESET			0x0F

#define CMD_SET_CORRECTION_VAL	0x10

#define CMD_SET_FIRMWARE_DOWNLOAD	0xF0

#define CMD_IDLE				0xFF


struct _tMSG_PACKET
{
	uint8_t RxBuff[MAX_RX_BUFF_SIZE];
	uint8_t Txbuff[MAX_TX_BUFF_SIZE];

	uint8_t FlagTxPktOn;
	uint8_t FlagRxPktOn;

	uint8_t TxMsgCnt;
	uint8_t RxMsgCnt;

	uint8_t RxErrCnt;

	uint8_t Cmd;
	uint8_t	Set;
	uint8_t Addr;
	uint8_t Leng;

	uint8_t Chksum;

	uint8_t MsgState;
} ;

struct _tUART_MSG
{
	uint8_t RingRxTemp[MAX_RX_RING_BUFF_SIZE];
	uint8_t RingTxTemp[MAX_RX_RING_BUFF_SIZE];
	uint8_t RxTemp[MAX_RX_BUFF_SIZE];
	uint8_t RxStxCnt;
	uint8_t RxTempCnt;
	uint8_t TxTempCnt;

	uint8_t uOldPos;
	uint8_t uNewPos;

	uint8_t FlagMsgErr;

	uint8_t FlagMsgRcvOn;
	uint8_t FlagPktFindOn;
	uint8_t FlagMsgRcvRdy;
} ;


extern struct _tUART_MSG 		tUartMsg;
extern struct _tMSG_PACKET		tMsgPkt;

extern void UART_Init(void);

extern void UART_RxInit();
extern void UART_TxInit();
extern void UART_StuffAssambly(void);
extern uint8_t UART_Chksum(uint8_t* pCh, uint8_t nCnt);
extern void UART_MsgProc(void);

