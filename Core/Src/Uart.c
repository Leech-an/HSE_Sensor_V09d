
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <Const.h>
#include <Config.h>
#include <G_Value.h>
#include <Parameter.h>
#include <Ext_IO.h>
#include <Uart.h>


#define MSG_STAT_NO_ERR					0x00
#define MSG_STAT_WRONG_DATA_LENG		0x01	//Wrong data length for this command (too much or little data)
#define	MSG_STAT_UNKNOWN_CMD			0x02	//Unknown command
#define MSG_STAT_NO_ACC_CMD				0x03	//No Access right for command // MODE Check.
#define MSG_STAT_OUT_OF_PARAMETER		0x04	//Illegal command parameter or parameter out of allowed range
#define MSG_STAT_INTER_FUNC_OVR_RANGE 	0x05	//Internal function argument out of range (?)
#define MSG_STAT_CMD_NOT_ALLOW_DEVICE	0x06	//Command not allowed in current state 	(Busy?)


#define SUB_GET_BIT_STATE				0x01
#define SUB_MEASURE_START 				0x02
#define SUB_MEASURE_STOP				0x00
#define SUB_READ_MEASURE				0x00
#define LENG_READ_MEASURE				0x34		//52

#define SUB_FAN_CLEAN_START				0x00

#define SUB_AT_CLEAN_GET_SEC			0x01		// read 1 byte  -> Set 2bye? or AT CLEAN ON/OFF ----> 4Byte Interval sec  Add FAN HI -> 10Sec (Flag Check)
#define SUB_AT_CLEAN_SET_SEC			0x02
#define LENG_AT_CLEAN_SEC				0x04		// Current, Set Sec 4byte

#define SUB_AT_CLEAN_GET_INTVAL			0x01
#define SUB_AT_CLEAN_SET_INTVAL			0x05
#define LENG_AT_CLEAN_INTERVAL			0x06		// Current, Set Sec 4byte


#define SUB_GET_BOUNDARY_VOLT			0x02
#define SUB_SET_BOUNDARY_VOLT			0x06
#define LENG_BOUNDARY_VOLT				0x06

#define SUB_PD_TEST_ON_MODE				0x02
#define SUB_PD_TEST_OFF_MODE			0x00
#define SUB_SiPM_TEST_OFF_MODE			0x00
#define SUB_SiPM_TEST_ON_MODE			0x02
#define LENG_ADC_TEST_OFF_MODE			0x00
#define LENG_ADC_TEST_ON_MODE			0x02

#define SUB_SLEEP						0x00
#define SUB_WAKE_UP						0x00

#define SUB_GET_DEVICE_PROC_TYPE		0x00
#define SUB_GET_DEVICE_SERIAL_NUM		0x03

#define LENG_DEVICE_PROC_TYPE			0x08
#define LENG_DEVICE_SERIAL_NUM			0x10		//16

#define SUB_GET_DEVICE_VER				0x00
#define LENG_DEVICE_VER					0x04

#define SUB_GET_DEVICE_STATE			0x01
#define LENG_DEVICE_STATE				0x01

#define SUB_SOFT_RESET					0x00
#define LENG_DEVICE_MAINT				0x00


#define SUB_MSG_ERROR					0xEE


#define LENG_REQ_RETURN					0x00

struct _tUART_MSG 		tUartMsg;
struct _tMSG_PACKET		tMsgPkt;


void UART_RxInit()
{
	tMsgPkt.RxMsgCnt = 0;
	tUartMsg.RxTempCnt = 0;
	//tUartMsg.uNewPos = 0;
	//tUartMsg.uOldPos = 0;
	tUartMsg.FlagMsgRcvOn = OFF;

	//memset(tUartMsg.RingRxTemp, 0, sizeof(tUartMsg.RingRxTemp));
	memset(tMsgPkt.RxBuff, 0, sizeof(tMsgPkt.RxBuff));
}

void UART_TxInit()
{
	tMsgPkt.TxMsgCnt = 0;
	tMsgPkt.FlagTxPktOn = OFF;

	tMsgPkt.Cmd = 0;
	tMsgPkt.Chksum = 0;
	tMsgPkt.Set = 0;
	tMsgPkt.Leng = 0;

	tUartMsg.TxTempCnt = 0;
	tMsgPkt.MsgState = MSG_STAT_NO_ERR;
	memset(tMsgPkt.Txbuff, 0, sizeof(tMsgPkt.Txbuff));
	memset(tUartMsg.RingTxTemp, 0, sizeof(tUartMsg.RingTxTemp));
}

uint8_t UART_CMD_CHK(uint8_t nCmd, uint8_t nLeng)
{
	uint8_t Result = FALSE;

	switch (nCmd)
	{
		case CMD_BIT_MANUAL_START : 	{ if (0 == nLeng) { Result = TRUE; } } break;
		case CMD_MEASUER_START : 		{ if (2 == nLeng) { Result = TRUE; } } break;
		case CMD_MEASUER_STOP : 		{ if (0 == nLeng) { Result = TRUE; } } break;
		case CMD_MEASUER_READ_VAL : 	{ if (0 == nLeng) { Result = TRUE; } } break;
		case CMD_START_FAN_CLEAN : 		{ if (0 == nLeng) { Result = TRUE; } } break;
		case CMD_GET_AT_CLEAN_SEC : 	{ if ((1 == nLeng) || (2 == nLeng)) { Result = TRUE; } } break;
		case CMD_AT_CLEAN_INTVAL : 		{ if ((1 == nLeng) || (5 == nLeng)) { Result = TRUE; } } break;
		case CMD_BOUNDARY_VOLT : 		{ if ((2 == nLeng) || (4 == nLeng)) { Result = TRUE; } } break;
		case CMD_PD_TEST_MODE : 		{ if (2 == nLeng) { Result = TRUE; } } break;
		case CMD_SIPM_TEST_MODE : 		{ if (2 == nLeng) { Result = TRUE; } } break;
		case CMD_SLEEP_MODE	:			{ if (0 == nLeng) { Result = TRUE; } } break;
		case CMD_WAKE_UP	:			{ if (0 == nLeng) { Result = TRUE; } } break;
		case CMD_DEVICE_INFO	:		{ if ((1 == nLeng) || (8 == nLeng) || (16 == nLeng)) { Result = TRUE; } } break;
		case CMD_DEVICE_VER	:			{ if ((1 == nLeng) || (5 == nLeng)) 				 { Result = TRUE; } } break;
		case CMD_DEVICE_STATE	:		{ if (1 == nLeng) { Result = TRUE; } } break;
		case CMD_SOFT_RESET	:			{ if (0 == nLeng) { Result = TRUE; } } break;
		case CMD_SET_CORRECTION_VAL :	{ if ((2 == nLeng) || (6 == nLeng)) { Result = TRUE; }  } break;
		case CMD_SET_FIRMWARE_DOWNLOAD:	{ if (0 == nLeng) { Result = TRUE; }  } break;
	}

	return Result;

}

uint8_t FindRcvMsgToPacket(uint8_t* nRcvMsg, uint8_t nRcvCnt)
{
	uint8_t i = 0;
	uint8_t ChkSum = 0;
	uint8_t ChksCnt = 0;

	uint8_t MsgCnt = 0;
	uint8_t flagPacktOn = OFF;

	uint8_t Result = TRUE;
	uint8_t FlagstuffOn = 0;
	uint8_t stuffSkipCnt = 0;

	tMsgPkt.MsgState = 0x00;

	for (i = 0; i < nRcvCnt; i++)
	{
		if ((FALSE==flagPacktOn) && (STX == nRcvMsg[i]))
		{
			tMsgPkt.RxBuff[MsgCnt] = nRcvMsg[i];
			flagPacktOn = ON;
			ChkSum = ChkSum + tMsgPkt.RxBuff[MsgCnt];
			ChksCnt = i;
		}

		if ( (ON == flagPacktOn ) && (i >= ChksCnt + 1) )
		{
			MsgCnt++;

			 	  if( (0xDB == nRcvMsg[MsgCnt]) && (0xDC == nRcvMsg[MsgCnt+1]) ) { tMsgPkt.RxBuff[MsgCnt] = 0xC0; FlagstuffOn = ON; }
			 else if( (0xDB == nRcvMsg[MsgCnt]) && (0xDD == nRcvMsg[MsgCnt+1]) ) { tMsgPkt.RxBuff[MsgCnt] = 0xDB; FlagstuffOn = ON; }
			 else if( (0xDB == nRcvMsg[MsgCnt]) && (0xDE == nRcvMsg[MsgCnt+1]) ) { tMsgPkt.RxBuff[MsgCnt] = 0xC2; FlagstuffOn = ON; }

			if(OFF == FlagstuffOn)
			{
				tMsgPkt.RxBuff[MsgCnt] = nRcvMsg[i];

				if(1 == MsgCnt) { tMsgPkt.Addr = tMsgPkt.RxBuff[MsgCnt];  }
				if(2 == MsgCnt) { tMsgPkt.Cmd  = tMsgPkt.RxBuff[MsgCnt];  }
				if(3 == MsgCnt) { tMsgPkt.Leng = tMsgPkt.RxBuff[MsgCnt];  }

				if(i < (nRcvCnt-2)) {
					ChkSum = (uint8_t)(ChkSum + tMsgPkt.RxBuff[MsgCnt]);
				}

				if(ETX == nRcvMsg[i])
				{
					ChkSum = (uint8_t)(ChkSum + tMsgPkt.RxBuff[MsgCnt]);
					break;
				}
			}

			else
			{
				stuffSkipCnt++;

				if(2 <= stuffSkipCnt) { stuffSkipCnt = 0; FlagstuffOn = OFF;}
				else { MsgCnt--; }
			}
		}
	}

	tMsgPkt.Chksum = tMsgPkt.RxBuff[MsgCnt-1];


	//Protocol Error Check
	//=============================================================================================================================================================================================
	//CheckSum CHK!
	if(ChkSum != tMsgPkt.Chksum) {tMsgPkt.MsgState  = MSG_STAT_WRONG_DATA_LENG; }
	//STX ERROR
	if(STX != tMsgPkt.RxBuff[0]) {tMsgPkt.MsgState = MSG_STAT_WRONG_DATA_LENG; }
	// Unknown CMD
	if( FALSE == UART_CMD_CHK(tMsgPkt.Cmd, tMsgPkt.Leng) ) {tMsgPkt.MsgState = MSG_STAT_UNKNOWN_CMD;}

	// Operation Mode Allow case Command : Measure Stop, Read Value
	if( (ON == tSysState.FlagOperModeOn) && !((CMD_MEASUER_STOP == tMsgPkt.Cmd)||(CMD_MEASUER_READ_VAL == tMsgPkt.Cmd))  ) { tMsgPkt.MsgState = MSG_STAT_NO_ACC_CMD; }
	if( ON == tSysState.FlagBitChkOn)																					   { tMsgPkt.MsgState = MSG_STAT_NO_ACC_CMD; }

	// Sleep Mode
	if( (ON == tSysState.FlagSleepModeOn) && (CMD_WAKE_UP != tMsgPkt.Cmd) ) { tMsgPkt.MsgState = MSG_STAT_CMD_NOT_ALLOW_DEVICE; }
	//=============================================================================================================================================================================================


	if(0x00 != tMsgPkt.MsgState) { Result = FALSE; }

	return Result;

}


void RcvMsgErrAssambly(uint8_t nCmd)
{
	uint8_t i = 0;

	uint8_t ChkSum = 0;

	tMsgPkt.TxMsgCnt = 0;
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = STX;
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ADDR;
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = nCmd;


	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tMsgPkt.MsgState;
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = LENG_REQ_RETURN;

	for(i = 0; i < tMsgPkt.TxMsgCnt; i++)
	{
		ChkSum |= tMsgPkt.Txbuff[i];
	}

	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ChkSum;
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ETX;

	tMsgPkt.FlagTxPktOn = ON;
}



void UART_MsgProc(void)
{
	if(ON == tUartMsg.FlagMsgRcvOn)
	{
		if(FALSE == FindRcvMsgToPacket(tUartMsg.RxTemp, tUartMsg.RxTempCnt))
		{
			RcvMsgErrAssambly(tMsgPkt.Cmd);
			memset(tUartMsg.RxTemp, 0, sizeof(tUartMsg.RxTemp));
			tMsgPkt.MsgState = MSG_STAT_NO_ERR;
			UART_RxInit();
			return;
		}

		tMsgPkt.FlagRxPktOn = ON;
	}
}


void UART_Init(void)
{
	memset(tUartMsg.RingRxTemp, 0, 	sizeof(tUartMsg.RingRxTemp) );

	memset(tMsgPkt.Txbuff, 	 0, 	sizeof(tMsgPkt.Txbuff) );
	memset(tMsgPkt.RxBuff, 	 0, 	sizeof(tMsgPkt.RxBuff) );
	memset(&tMsgPkt, 0, 			sizeof(struct _tMSG_PACKET) );

	tUartMsg.FlagMsgErr  	= FALSE;
	tUartMsg.FlagMsgRcvOn 	= FALSE;
	tUartMsg.FlagPktFindOn 	= FALSE;
	tUartMsg.RxStxCnt  = 0;
	tUartMsg.RxTempCnt = 0;
	tUartMsg.TxTempCnt = 0;

	tMsgPkt.FlagTxPktOn	= OFF;

	tMsgPkt.RxMsgCnt = 0;
	tMsgPkt.TxMsgCnt = 0;
}

void UART_StuffAssambly(void)
{
	uint8_t i = 0;
	memset(tUartMsg.RingRxTemp, 0, sizeof(tUartMsg.RingTxTemp));
	tUartMsg.TxTempCnt = 0;

	tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = STX;

	for(i = 1; i < tMsgPkt.TxMsgCnt; i++)
	{
			 if(0xC0 == tMsgPkt.Txbuff[i]) { tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = 0xDB; tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = 0xDC; }
		else if(0xDB == tMsgPkt.Txbuff[i]) { tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = 0xDB; tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = 0xDD; }
		else if(0xC2 == tMsgPkt.Txbuff[i]) { tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = 0xDB; tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = 0xDE; }
		else tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = tMsgPkt.Txbuff[i];
	}

	tUartMsg.RingTxTemp[tUartMsg.TxTempCnt++] = ETX;

}

uint8_t UART_Chksum(uint8_t* pCh, uint8_t nCnt)
{
	uint8_t i = 0;
	uint8_t Result = 0;

	for( i = 0; i < nCnt; i++)
	{
		Result = (uint8_t)(Result + pCh[i]);
	}

	Result = (uint8_t)(Result + ETX);
	return Result;
}
