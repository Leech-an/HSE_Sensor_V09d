
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <stm32l4xx_hal.h>
#include <Const.h>
#include <G_Value.h>

#include "Uart.h"



#include "Process.h"
#include "Parameter.h"
#include "Config.h"

#include "Ext_IO.h"
#include "Fan.h"

#include <Measure.h>

#define AIRCR_VECTKEY_MASK  0x05FA0000




// BIT SEQ Check time Const
//=====================================================================================================================
#define CHK_TIME_OFF					99
#define EEP_EEPROM_CHK_TIME				1			//EEPROM(Config) read checking time (5sec)
#define EEP_PARAM_READ_CHK_TIME			3			//EEPROM(Param) read checking time (5sec)

#define FAN_CON_ON_TIME					3
#define FAN_MEASURE_TIME				1			//(100msec * 10) = Sec

#define LD_CTRL_ON_TIME					3			//Laser diode control On time (3sec)
#define PD_CTRL_ON_TIME					3
#define HV_CTRL_ON_TIME					3
#define SiPM_CTRL_ON_TIME				3


#define LD_ADC_MEASURE_TIME				10			//Laser diode control On ADC checking time (100msec * 10) = Sec
#define HV_ADC_MEASURE_TIME				50			//Ext. High voltage control On ADC checking time (100msec * 10) = Sec
#define PD_ADC_MEASURE_TIME				30			//Laser diode control On Photo diode ADC checking time (3Sec)
#define SiPM_ADC_MEASURE_TIME			50			//Laser diode control On, Ext. High voltage On,

#define UART_MSG_SEND_ADC_TIME			5			//UART client send loop time (100msec)
#define UART_MSG_RESP_TIME				100			//UART client Read Value time (1sec)
#define UART_MSG_MANUAL_FAN_CLE_TIME	10			//UART client Read Value time (10sec)
//======================================================================================================================


// Built in test Sequence (Define)
//======================================================================================================================
#define BIT_SEQ_IDLE						0x00
#define BIT_SEQ_START						0x10
#define BIT_SEQ_EEPROM_WRITE_READ			0x20
#define BIT_SEQ_EEPROM_WRITE_TIME_CHK		0x22
#define BIT_SEQ_EEPROM_WRITE_VERIFY_CHK		0x24

#define BIT_SEQ_FAN_CON_LO_SPD_CTRL_ON		0x30
#define BIT_SEQ_FAN_CON_LO_SPD_STAB_TIME	0x32
#define BIT_SEQ_FAN_CON_LO_SPD_CHK			0x34

#define BIT_SEQ_FAN_CON_HI_SPD_CTRL_ON		0x35
#define BIT_SEQ_FAN_CON_HI_SPD_STAB_TIME	0x37
#define BIT_SEQ_FAN_CON_HI_SPD_CHK			0x39

#define BIT_SEQ_LD_MON_ADC_CTRL_ON			0x40
#define BIT_SEQ_LD_MON_ADC_STAB_TIME		0x42
#define BIT_SEQ_LD_MON_ADC_CHK_ON			0x44

#define BIT_SEQ_HV_CHK_ADC_CTRL_ON			0x50
#define BIT_SEQ_HV_CHK_ADC_STAB_TIME		0x52
#define BIT_SEQ_HV_CHK_ADC_CHK_ON			0x54

#define BIT_SEQ_PD_SIG_ADC_CTRL_ON			0x60
#define BIT_SEQ_PD_SIG_ADC_STAB_TIME		0x62
#define BIT_SEQ_PD_SIG_ADC_CHK_ON			0x64

#define BIT_SEQ_SiPM_SIG_ADC_CTRL_ON		0x70
#define BIT_SEQ_SiPM_SIG_ADC_STAB_TIME		0x72
#define BIT_SEQ_SiPM_SIG_ADC_CHK_ON			0x74

#define BIT_SEQ_COMPLETE					0x90
#define BIT_SEQ_ABMOMAL_COMPLETE			0x9F

#define BIT_SEQ_RETRY						0x80

#define BIT_SEQ_ERROR						0xE0
#define BIT_SEQ_ABORT						0xEE
#define BIT_SEQ_FAIL						0xEF
//======================================================================================================================


// System Sequence (Define)
//======================================================================================================================
#define SYS_SEQ_IDLE					0x00
#define SYS_SEQ_READY					0x05
#define SYS_SEQ_INIT					0x10

#define SYS_SEQ_GET_CONFIG				0x20
#define SYS_SEQ_GET_PARAM				0x25

#define SYS_SEQ_BIT_START				0x30
#define SYS_SEQ_BIT_WAIT				0x35

#define SYS_SEQ_STANDBY_MODE			0x40
#define SYS_SEQ_OPRATION_MODE			0x50

#define SYS_SEQ_POWER_DOWN_MODE			0x60
#define SYS_SEQ_WAKE_UP_MODE			0x62

#define SYS_SEQ_MANUAL_FAN_CLEAN_MODE	0x64

#define SYS_SEQ_GET_ADC_TEST_MODE		0x70

#define SYS_SEQ_STOP					0x80
#define SYS_SEQ_RETRY					0x90
#define SYS_SEQ_RESET					0xA0

#define SYS_SEQ_ERROR_REPORT			0xE0
#define SYS_SEQ_FAULT					0xEE
//======================================================================================================================


// Device State (Define)
//======================================================================================================================
#define DEVICE_BIT_ADDR_SPD_H			0
#define DEVICE_BIT_ADDR_SPD_N			1
#define DEVICE_BIT_ADDR_FAN_OK			2
#define DEVICE_BIT_ADDR_LD_OK			3
#define DEVICE_BIT_ADDR_EEPROM_OK		4
#define DEVICE_BIT_ADDR_HV_OK			5
#define DEVICE_BIT_ADDR_PD_OK			6
#define DEVICE_BIT_ADDR_SiPM_OK			7

#define DEVICE_BIT_ADDR_CLEANING_ON		8
#define DEVICE_BIT_ADDR_TEMP_OVR_ON		9
#define DEVICE_BIT_ADDR_RESRV_1			10
#define DEVICE_BIT_ADDR_RESRV_2			11

#define DEVICE_BIT_ADDR_RESRV_3			12
#define DEVICE_BIT_ADDR_RESRV_4			13
#define DEVICE_BIT_ADDR_RESRV_5			14
#define DEVICE_BIT_ADDR_RESRV_6			15
//======================================================================================================================


// Device InfoType
//======================================================================================================================
#define DEVICE_INFO_PRODUCT_TYPE	0
#define DEVICE_INFO_RESERVE1		1
#define DEVICE_INFO_RESERVE2		2
#define DEVICE_INFO_SERIAL_NUM		3
#define DEVICE_INFO_FIRMWARE		4
//======================================================================================================================


#define TICK_10mSEC			10  // 10mSec
#define TICK_100mSEC		10 // 100mSec
#define TICK_1SEC 			10 // 1Sec

struct _tPM_RESULT1	tPmResult1[PM_CH_MAX];
struct _tPM_RESULT2	tPmResult2;

struct _tSYS_STAT 	tSysState;
struct _tSYS_TIME	tSysTime;

uint8_t m_BITProcSeq = BIT_SEQ_IDLE;
uint8_t m_SysProcSeq = SYS_SEQ_IDLE;

uint16_t m_OldAdcVal  = 0;
uint16_t m_MaxPeckAdVal = 0;
uint16_t m_MaxPeckAdVal2 = 0;

uint16_t m_PeckPDAdVal = 0;
uint16_t m_PeckSiPMAdVal = 0;

uint8_t m_FlagDectPeckOn = 0;
uint8_t m_FlagDectPeckOn2 = 0;
uint8_t m_FlagAdcSendOn = 0;

uint8_t m_FlagDataGetkOn = 0;

uint8_t m_SelOldPdAdcRng = 0;
uint16_t m_OldSiPmAdc  = 0;
uint16_t m_CurrSiPmAdc = 0;

uint16_t m_CurrSiPmCnt = 0;

uint32_t m_DeviceCurrUseCount = 0;

static void SystemTimeCtrl(void);

void RngOfPdAdcSamplingVal(uint16_t nAdcVal);
void RngOfSiPmAdcSamplingVal(uint16_t nAdcVal);

void PROC_Init(void)
{
	memset( &tPmResult1, 0, sizeof(struct _tPM_RESULT1) );
	memset( &tPmResult2, 0, sizeof(struct _tPM_RESULT2) );
	memset( &tSysState,  0, sizeof(struct _tSYS_STAT) );

	m_BITProcSeq = BIT_SEQ_IDLE;
	m_SysProcSeq = SYS_SEQ_IDLE;
}

void WaitTime_Init(void)
{
	tSysTime.Sys10mSecTick = 0;
	tSysTime.Sys10mSecTick = 0;
	tSysTime.Sys1SecTick = 0;

	tSysTime.WaitSecTime = 0;
	tSysTime.WaitmSecTime = 0;

	uwTick = 0;
}

uint8_t WaitHoldTime_Sec(uint8_t ChkOn, uint32_t ChkTime)
{
	uint8_t result = FALSE;

	if (OFF == ChkOn) { WaitTime_Init(); tSysTime.FlagWaitSecTimeOn = OFF; return result; }
	else
	{
		tSysTime.FlagWaitSecTimeOn = ON;
	}

	if (tSysTime.WaitSecTime >= ChkTime) {
		result = TRUE;
		tSysTime.WaitSecTime = 0;
		tSysTime.FlagWaitSecTimeOn = OFF;
	}

	return result;
}

uint8_t WaitHoldTime_mSec(uint8_t ChkOn, uint32_t ChkTime)
{
	uint8_t result = FALSE;

	if (OFF == ChkOn) { WaitTime_Init(); tSysTime.FlagWaitmSecTimeOn = OFF; return result; }
	else
	{
		tSysTime.FlagWaitmSecTimeOn = ON;
	}

	if (tSysTime.WaitmSecTime >= ChkTime)
	{
		result = TRUE;
		tSysTime.WaitmSecTime = 0;
		tSysTime.FlagWaitmSecTimeOn = OFF;
	}

	return result;
}

uint16_t DeviceSetState(uint16_t Addr, uint8_t Flag)
{
	uint16_t result = 0;

	if(ON == Flag) 	{ result |= (1 << Addr);  }
	else 			{ result &= ~(1 << Addr); }

	return result;
}


uint8_t MCU_AlarmTempMonitor(uint16_t nTemp)
{
	uint8_t result = OFF;
	float cpu_temp = 0.0f;


	if( 0 != nTemp)
    {
		float temperature = ((float)nTemp / 4095) * 3300; // ADC 값을 전압으로 변환
		cpu_temp = ((temperature - 760.0) / 2.5); // 온도로 변환
		//cpu_temp = 65;
    }


    if (DEFAULT_TEMP_OVER_VAL < cpu_temp)
    {
    	tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_TEMP_OVR_ON, ON);
    }

    else
    {
    	tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_TEMP_OVR_ON, OFF);
    }

    return result;
}
/*
void floatToByteArray(float value, uint8_t *byteArray) {
    // Use memcpy to copy the float's memory representation to the byte array
    memcpy(byteArray, &value, sizeof(float));
}
*/
void ReadMeasuredVal(void)
{
	uint8_t ch = 0;
	uint8_t i = 0;

	float fTmpTps = 0.0f;
	float fTmpCon = 0.0f;

	uint8_t nTmpConv8[4] = {0,};


	if (OFF == tSysState.FlagLoopOn) { WaitTime_Init(); tSysState.FlagLoopOn = ON; }

	if ( TRUE == WaitHoldTime_mSec(ON, UART_MSG_RESP_TIME) )
	{

		Calc_GetSelNumConnect();
		Calc_GetNumberConcent();
		Calc_GetSelMassConnect();
		Calc_GetMassConcentration();
		Calc_GetTypicalPaticleSize();

		for (i = PM_CH1; i <= PM_CH5; i++)
		{
			fTmpTps += tPmResult1[i].fTps;
			fTmpCon += tPmResult1[i].fNum_Concet;
		}

		tPmResult2.fResultTps = (float)(fTmpTps / fTmpCon);

		tPmResult2.fCFU = (float)(tPmResult1[PM_SiPM].nTmpNumConcet * tParam[PM_SiPM].CfuCal);
		tPmResult2.fNumConcetBio = (float)(tPmResult1[PM_SiPM].nTmpNumConcet);
		//---------------------------------------------------------------------------------------------------------------------------------------
		if(ON == tSysState.FlagSendPmResultOn)
		{
			// HEADER
			tMsgPkt.TxMsgCnt = 0;

			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = STX;
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // ADDR
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = CMD_MEASUER_READ_VAL;
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // MSG STATE
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x34; // 52

			for (ch = 0; ch < 4; ch++) // MASS Concentration
			{
				memcpy( nTmpConv8, &tPmResult1[ch].fMass_Concet, sizeof(float));

				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[3];
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[2];
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[1];
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[0];

				memset(nTmpConv8, 0, sizeof(nTmpConv8));
			}

			for (ch = 0; ch < 5; ch++) // Number Concentration
			{
				memcpy( nTmpConv8, &tPmResult1[ch].fNum_Concet, sizeof(float));

				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[3];
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[2];
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[1];
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[0];

				memset(nTmpConv8, 0, sizeof(nTmpConv8));
			}


			memcpy( nTmpConv8, &tPmResult2.fResultTps, sizeof(float));
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[3];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[2];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[1];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[0];
			memset(nTmpConv8, 0, sizeof(nTmpConv8));

			memcpy( nTmpConv8, &tPmResult2.fCFU, sizeof(float));
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[3];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[2];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[1];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[0];
			memset(nTmpConv8, 0, sizeof(nTmpConv8));

			memcpy( nTmpConv8, &tPmResult2.fNumConcetBio, sizeof(float));
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[3];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[2];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[1];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[0];
			memset(nTmpConv8, 0, sizeof(nTmpConv8));

			memcpy( nTmpConv8, &tPmResult2.fReverved, sizeof(float));
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[3];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[2];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[1];
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t) nTmpConv8[0];
			memset(nTmpConv8, 0, sizeof(nTmpConv8));

			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = UART_Chksum(tMsgPkt.Txbuff, tMsgPkt.TxMsgCnt);
			UART_StuffAssambly();
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ETX;


			tPdCtrl.ChkCount = 0;
			tMsgPkt.FlagTxPktOn = ON;
			tSysState.FlagSendPmResultOn = OFF;
		}

		Calc_Init();
		WaitTime_Init();
		tSysState.FlagLoopOn = OFF;
		tPdCtrl.ChkCount = 0;
	}

	else
	{
		RngOfPdAdcSamplingVal(tPdCtrl.CurrAdcVal);
		RngOfSiPmAdcSamplingVal(tSiPmCtrl.CurrAdcVal);

	}
}

uint8_t FanCleanCtrl(void)
{

	uint8_t Result = FALSE;

	if (OFF == tSysState.FlagLoopOn) { WaitTime_Init(); tSysState.FlagLoopOn = ON; EXT_LD_Ctrl(OFF); EXT_HV_Ctrl(OFF); EXT_PD_Ctrl(OFF); FAN_Ctrl(HI, ON); }

	if ( TRUE == WaitHoldTime_Sec(ON, UART_MSG_MANUAL_FAN_CLE_TIME) )
	{
		//Check Get ADC

		// HEADER
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = STX;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // ADDR
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = CMD_START_FAN_CLEAN;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tMsgPkt.MsgState; // MSG STATE
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // Message Length

		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = UART_Chksum(tMsgPkt.Txbuff, tMsgPkt.TxMsgCnt);
		UART_StuffAssambly();
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ETX;

		if(ON == tSysState.FlagOperModeOn) 	{ FAN_Ctrl(LO, ON);  EXT_LD_Ctrl(ON); EXT_HV_Ctrl(ON); EXT_PD_Ctrl(ON); }
		else 								{ FAN_Ctrl(LO, OFF); }

		tFan.CleWorkTime = 0;
		SetCfg_CurrentFanUseTime(0);

		tMsgPkt.FlagTxPktOn = ON;
		tSysState.FlagStatClenaOn = OFF;

		WaitTime_Init();
		tSysState.FlagLoopOn = OFF;

		Result = TRUE;
	}

	return Result;
}


void Measure_Start(void)
{
	if(OFF == tLdCtrl.FlagCtrlOn) 	{ EXT_LD_Ctrl(ON);  tLdCtrl.CurrAdcVal = 0; tLdCtrl.ChkCount = 0;}
	if(OFF == tFan.FlagCtrlOn) 		{ FAN_Ctrl(LO, ON); tPdCtrl.CurrAdcVal = 0; tPdCtrl.ChkCount = 0;}
	if(OFF == tHvCtrl.FlagCtrlOn ) 	{ EXT_HV_Ctrl(ON);  tHvCtrl.CurrAdcVal = 0; tHvCtrl.ChkCount = 0;}
	tSysState.FlagGetAdcOn = ON;
}

void Measure_Stop(void)
{
	if(ON == tLdCtrl.FlagCtrlOn) 	{ EXT_LD_Ctrl(OFF);  tLdCtrl.CurrAdcVal = 0; tLdCtrl.ChkCount = 0;}
	if(ON == tFan.FlagCtrlOn) 		{ FAN_Ctrl(LO, OFF); tPdCtrl.CurrAdcVal = 0; tPdCtrl.ChkCount = 0;}
	if(ON == tHvCtrl.FlagCtrlOn ) 	{ EXT_HV_Ctrl(OFF);  tHvCtrl.CurrAdcVal = 0; tHvCtrl.ChkCount = 0;}
	tSysState.FlagGetAdcOn = OFF;
}

void AssmTxMsg_GetBoundaryVolt(uint8_t nCh)
{
	 if(0x00 == nCh)
	 {
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = nCh;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[nCh].BasePD >> 8) & 0xFF);
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[nCh].BasePD  ) & 0xFF);
	 }

	 else if((0x01 <= nCh)&&(0x05 >= nCh))
	 {
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = nCh;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[nCh-1].UB >> 8) & 0xFF);
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[nCh-1].UB  ) & 0xFF);
	 }

	 else if(0x06 == nCh)
	 {
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = nCh;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[PM_SiPM].LB >> 8) & 0xFF);
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[PM_SiPM].LB  ) & 0xFF);
	 }

	 else if(0x07 == nCh)
	 {
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = nCh;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[PM_SiPM].UB >> 8) & 0xFF);
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tParam[PM_SiPM].UB  ) & 0xFF);
	 }
}

void SetBoundaryVolt(uint8_t nCh)
{
	 if(0x00 == nCh)
	 {

		tParam[nCh].BasePD  = (uint16_t)(tMsgPkt.RxBuff[6] << 8);
		tParam[nCh].BasePD |= (uint16_t)(tMsgPkt.RxBuff[7]);

		SetParam_BoundaryVolt(nCh, tParam[nCh].BasePD);

		tParam[nCh].LB = tParam[PM_CH1].BasePD + 1;
	 }

	 else if((0x01 <= nCh)&&(0x05 >= nCh))
	 {
		tParam[nCh-1].UB  = (uint16_t)(tMsgPkt.RxBuff[6] << 8);
		tParam[nCh-1].UB |= (uint16_t)(tMsgPkt.RxBuff[7]);

		SetParam_BoundaryVolt(nCh, tParam[nCh-1].UB);
	 }

	 else if(0x06 == nCh)
	 {
		tParam[PM_SiPM].LB  = (uint16_t)(tMsgPkt.RxBuff[6] << 8);
		tParam[PM_SiPM].LB |= (uint16_t)(tMsgPkt.RxBuff[7]);

		SetParam_BoundaryVolt(nCh, tParam[PM_SiPM].LB);
	 }

	 else if(0x07 == nCh)
	 {
		tParam[PM_SiPM].UB  = (uint16_t)(tMsgPkt.RxBuff[6] << 8);
		tParam[PM_SiPM].UB |= (uint16_t)(tMsgPkt.RxBuff[7]);

		SetParam_BoundaryVolt(nCh, tParam[PM_SiPM].UB);
	 }
}

//EEROR
void AssmTxMsg_GetAdjustCal(uint8_t nCh)
{
	uint32_t nRelt = 0;

//		 if(0x00 == nCh)					{ nRelt = *((uint32_t*)&tParam[PM_CH1].den); 		}
//	else if((0x01 <= nCh)&&(0x05 >= nCh))	{ nRelt = *((uint32_t*)&tParam[PM_CH1-1].Cal); 	}
//	else if(0x06 == nCh)					{ nRelt = *((uint32_t*)&tParam[PM_SiPM].CfuCal); 	}

		 if(0x00 == nCh)					{ memcpy(&nRelt, &tParam[PM_CH1].den, sizeof nRelt); }
	else if((0x01 <= nCh)&&(0x05 >= nCh))	{ memcpy(&nRelt, &tParam[nCh-1].Cal, sizeof nRelt); 	}
	else if(0x06 == nCh)					{ memcpy(&nRelt, &tParam[PM_SiPM].CfuCal, sizeof nRelt); 	}

	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = nCh;
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((nRelt >> 24)  & 0xFF);
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((nRelt >> 16)  & 0xFF);
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((nRelt >>  8)  & 0xFF);
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((nRelt 	   )  & 0xFF);
}

void SetAdjustCal(uint8_t nCh)
{
	uint32_t result  = 0;

	result  = (uint32_t)(tMsgPkt.RxBuff[6] << 24);
	result |= (uint32_t)(tMsgPkt.RxBuff[7] << 16);
	result |= (uint32_t)(tMsgPkt.RxBuff[8] << 8);
	result |= (uint32_t)(tMsgPkt.RxBuff[9]);

	 if(0x00 == nCh)
	 {
		tParam[nCh].den = *((float*)&result);
		SetParam_AdjCal(nCh, tParam[nCh].den);
	 }

	 else if((0x01 <= nCh)&&(0x05 >= nCh))
	 {
		tParam[nCh-1].Cal  = *((float*)&result);
		SetParam_AdjCal(nCh, tParam[nCh-1].Cal);
	 }

	 else if(0x06 == nCh)
	 {
		tParam[PM_SiPM].CfuCal = *((float*)&result);
		SetParam_BoundaryVolt(nCh, tParam[PM_SiPM].LB);
	 }
}

void AssmTxMsg_DeviceInfo(uint8_t nMode)
{
	uint8_t i = 0;

	if(DEVICE_INFO_PRODUCT_TYPE == nMode){
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x08;
		for( i = 0; i < 8; i++)
		{ tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tConfig.prodtype[i]; }
	}

	else if(DEVICE_INFO_RESERVE1 == nMode)
	{
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x01;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
	}

	else if(DEVICE_INFO_RESERVE2 == nMode)
	{
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x01;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
	}


	else if(DEVICE_INFO_SERIAL_NUM == nMode)
	{
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x10;
		for( i = 0; i < 16; i++)
		{ tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tConfig.prodSerial[i]; }
	}
}

void DeviceInfo(uint8_t nMode)
{
	uint8_t i = 0;

	if(DEVICE_INFO_PRODUCT_TYPE == nMode){
		for( i = 0; i < 8; i++)
		{ tConfig.prodtype[i] = tMsgPkt.RxBuff[4+i]; }

		SetCfg_Device_ProcType(tConfig.prodtype);
	}

	else if(DEVICE_INFO_SERIAL_NUM == nMode)
	{
		for( i = 0; i < 16; i++)
		{ tConfig.prodSerial[i] = tMsgPkt.RxBuff[4+i]; }
		SetCfg_Device_ProcSerial(tConfig.prodSerial);
	}

}

void DeviceFirmwareVer(void)
{
	uint8_t i = 0;
	for( i = 0; i < 5; i++)
	{
			 if(0 == i) { tConfig.FirmwareVer[0] = DEFAULT_FIRMWARE_MAJ_VERSION; }
		else if(1 == i) { tConfig.FirmwareVer[1] = DEFAULT_FIRMWARE_MIN_VERSION; }
		else 			{ tConfig.FirmwareVer[i] = tMsgPkt.RxBuff[4+i]; }
	}
	SetCfg_Device_ProcVer(tConfig.FirmwareVer);
}

void RspAssamblyTxMsg(uint8_t nCMD)
{
	UART_TxInit();
	// HEADER
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = STX;	// STX
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; 	// ADDR
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = nCMD;
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tMsgPkt.MsgState; 	// MSG STATE

	switch (nCMD)
	{
		case CMD_BIT_MANUAL_START : { // LENGTH 2 Byte , Device State
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x02;

			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tSysState.DeviceState >> 8) & 0xFF);
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)(tSysState.DeviceState & 0xFF);
		} break;

		case CMD_MEASUER_START	  : { // LENGTH 0, Measurement Start
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		} break;

		case CMD_MEASUER_STOP	  : { // LENGTH 0, Measurement Stop
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		} break;

//		case CMD_MEASUER_READ_VAL : { // LENGTH 0, Read Measurement Value
//			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x34; //52
//			AssmTxMsg_PmMesaureResult();
//		} break;

		case CMD_START_FAN_CLEAN : {
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		}break;

		case CMD_GET_AT_CLEAN_SEC : {
			if (CLIENT_REQ_GET == tSysState.ClientReqType) { // LENGTH 4, Get Auto Cleaning time U32bit
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x04;
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleWorkTime >> 24) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleWorkTime >> 16) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleWorkTime >>  8) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleWorkTime      ) & 0xFF);
			}
			else if (CLIENT_REQ_SET == tSysState.ClientReqType) { // LENGTH 0, Read Measurement Value
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_AT_CLEAN_INTVAL : {
			if (CLIENT_REQ_GET == tSysState.ClientReqType) { // LENGTH 4, Get Auto Cleaning Interval U32bit
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x04;
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleInterval >> 24) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleInterval >> 16) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleInterval >>  8) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tFan.CleInterval      ) & 0xFF);
			}
			else if (CLIENT_REQ_SET == tSysState.ClientReqType) { // LENGTH 0, Read Measurement Value
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_BOUNDARY_VOLT : {
			if (CLIENT_REQ_GET == tSysState.ClientReqType) { // LENGTH 4, Get Boundary Voltage U16bit
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x04;
				AssmTxMsg_GetBoundaryVolt(tSysState.ClientReqSelCh);
			}
			else if (CLIENT_REQ_SET == tSysState.ClientReqType) { // LENGTH 0, Set Boundary Voltage U16bit
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_PD_TEST_MODE : {
			if (OFF == tSysState.FlagGetAdcOn) { // LENGTH 0, PD Test Mode OFF
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_SIPM_TEST_MODE : {
			if (OFF == tSysState.FlagGetAdcOn) { // LENGTH 0, SiPM Test Mode OFF
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_SLEEP_MODE : {
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // LENGTH 0, Sleep
		}break;

		case CMD_WAKE_UP : {
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // LENGTH 0, Wake up
		}break;

		case CMD_DEVICE_INFO : {
			if (CLIENT_REQ_GET == tSysState.ClientReqType) { // LENGTH 4, Get Auto Cleaning Interval U32bit
				AssmTxMsg_DeviceInfo(tSysState.ClientReqDevInfo);
			}
			else if (CLIENT_REQ_SET == tSysState.ClientReqType) { // LENGTH 0, Read Measurement Value
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_DEVICE_VER : {
			if (CLIENT_REQ_GET == tSysState.ClientReqType) { // LENGTH 4, Get Auto Cleaning Interval U32bit
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x05;
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = DEFAULT_FIRMWARE_MAJ_VERSION; //(uint8_t)(tConfig.FirmwareVer[0]);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = DEFAULT_FIRMWARE_MIN_VERSION; //(uint8_t)(tConfig.FirmwareVer[1]);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)(tConfig.FirmwareVer[2]);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)(tConfig.FirmwareVer[3]);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)(tConfig.FirmwareVer[4]);
			}
			else if (CLIENT_REQ_SET == tSysState.ClientReqType) { // LENGTH 0, Read Measurement Value
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_DEVICE_STATE : {
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x02;
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tSysState.DeviceState >>  8) & 0xFF);
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)((tSysState.DeviceState      ) & 0xFF);
		}break;

		case CMD_SET_CORRECTION_VAL :
		{
			if (CLIENT_REQ_GET == tSysState.ClientReqType) { // LENGTH 4, Get Boundary Voltage U16bit
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x05;

				AssmTxMsg_GetAdjustCal(tSysState.ClientReqSelCh);
			}
			else if (CLIENT_REQ_SET == tSysState.ClientReqType) { // LENGTH 0, Set Boundary Voltage U16bit
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
			}
		}break;

		case CMD_SOFT_RESET: {
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		}break;

		case CMD_SET_FIRMWARE_DOWNLOAD: {
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00;
		}break;
	}

	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = UART_Chksum((uint8_t *)tMsgPkt.Txbuff, tMsgPkt.TxMsgCnt); 	// CHECKSUM
	UART_StuffAssambly();
	tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ETX; 	// ETX

	tMsgPkt.FlagTxPktOn = ON;
}

void Manual_GetAdcMsg(void)
{
	if (ON == tSysState.FlagGetAdcOn)
	{
		if (OFF == tSysState.FlagLoopOn)
		{
			UART_TxInit();
			WaitTime_Init();
			tSysState.FlagLoopOn = ON;
		}

		//if ( TRUE == WaitHoldTime_mSec(ON, UART_MSG_SEND_ADC_TIME) )
		if(ON == m_FlagAdcSendOn)
		{
			//Check Get ADC

			// HEADER
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = STX;
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // ADDR

			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tSysState.SelGetAdcCh;

			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // MSG STATE
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x02; // Length

			if(ADC_GET_PD_CH == tSysState.SelGetAdcCh){
				//tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (tPdCtrl.CurrAdcVal >> 8) & 0xFF);
				//tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (tPdCtrl.CurrAdcVal 	) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (m_PeckPDAdVal >> 8) & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (m_PeckPDAdVal) 	 & 0xFF);
				m_FlagDectPeckOn = OFF;
			}
			else if(ADC_GET_SiPM_CH == tSysState.SelGetAdcCh){
				//tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (tSiPmCtrl.CurrAdcVal >> 8) & 0xFF);
				//tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (tSiPmCtrl.CurrAdcVal) 		& 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (m_PeckSiPMAdVal >> 8)  & 0xFF);
				tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = (uint8_t)( (m_PeckSiPMAdVal) 	 	& 0xFF);
				m_FlagDectPeckOn2 = OFF;
			}

			//FOOTER
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = UART_Chksum((uint8_t *)tMsgPkt.Txbuff, tMsgPkt.TxMsgCnt);
			UART_StuffAssambly();
			tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ETX;

			tMsgPkt.FlagTxPktOn = ON;
			tPdCtrl.ChkCount = 0;
			WaitTime_Init();
			Calc_Init();


			m_FlagAdcSendOn = OFF;
		}
		else
		{
			if(ADC_GET_PD_CH == tSysState.SelGetAdcCh){ RngOfPdAdcSamplingVal(tPdCtrl.CurrAdcVal); 	 	 }
			else 									  {	RngOfSiPmAdcSamplingVal(tSiPmCtrl.CurrAdcVal);	 }
		}
	}
	else
	{
		//Check Get ADC
		UART_TxInit();
		// HEADER
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = STX;
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // ADDR

		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tSysState.SelGetAdcCh;

		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = tMsgPkt.MsgState; // MSG STATE
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = 0x00; // Length

		//FOOTER
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = UART_Chksum((uint8_t *)tMsgPkt.Txbuff, tMsgPkt.TxMsgCnt);
		UART_StuffAssambly();
		tMsgPkt.Txbuff[tMsgPkt.TxMsgCnt++] = ETX;

		tMsgPkt.FlagTxPktOn = ON;
		WaitTime_Init();
		tPdCtrl.ChkCount = 0;
		m_SysProcSeq = SYS_SEQ_STANDBY_MODE;
	}
}

void PostMessage_Proc(void)
{
	if(ON == tMsgPkt.FlagRxPktOn)
	{
		switch(tMsgPkt.Cmd)
		{
			case CMD_BIT_MANUAL_START :
			{
				tSysState.FlagBitChkOn = ON;
				m_SysProcSeq = SYS_SEQ_IDLE;
			}break;

			case CMD_MEASUER_START :
			{
				RspAssamblyTxMsg(tMsgPkt.Cmd);
				Measure_Start();
				tSysState.FlagOperModeOn = ON;
				m_SysProcSeq = SYS_SEQ_OPRATION_MODE;
 			}break;

			case CMD_MEASUER_STOP :
			{
				RspAssamblyTxMsg(tMsgPkt.Cmd);
				Measure_Stop();
				tSysState.FlagOperModeOn = OFF;
				SetCfg_CurrentFanUseTime(tFan.CleWorkTime);
				m_SysProcSeq = SYS_SEQ_STANDBY_MODE;
 			}break;

			case CMD_MEASUER_READ_VAL :
			{
				Measure_Start();
				tSysState.FlagSendPmResultOn = ON;
				m_SysProcSeq = SYS_SEQ_OPRATION_MODE;
			}break;

			case CMD_START_FAN_CLEAN :
			{
				m_SysProcSeq = SYS_SEQ_MANUAL_FAN_CLEAN_MODE;
			}break;

			case CMD_GET_AT_CLEAN_SEC :
			{
				tSysState.ClientReqType = CLIENT_REQ_GET;
				RspAssamblyTxMsg(tMsgPkt.Cmd);
			}break;


			case CMD_AT_CLEAN_INTVAL :
			{
				if (0x01 == tMsgPkt.Leng) {tSysState.ClientReqType = CLIENT_REQ_GET;}
				if (0x05 == tMsgPkt.Leng)
				{
					tSysState.ClientReqType = CLIENT_REQ_SET;

					tFan.CleInterval  = (uint32_t)( tMsgPkt.RxBuff[5] << 24);
					tFan.CleInterval |= (uint32_t)( tMsgPkt.RxBuff[6] << 16);
					tFan.CleInterval |= (uint32_t)( tMsgPkt.RxBuff[7] << 8);
					tFan.CleInterval |= (uint32_t)( tMsgPkt.RxBuff[8] );

					SetCfg_CurrentFanInterValTime(tFan.CleInterval);
				}

				RspAssamblyTxMsg(tMsgPkt.Cmd);
			}break;

			case CMD_BOUNDARY_VOLT :
			{
				if (0x02 == tMsgPkt.Leng) { tSysState.ClientReqType = CLIENT_REQ_GET; tSysState.ClientReqSelCh = tMsgPkt.RxBuff[5]; }
				if (0x04 == tMsgPkt.Leng)
				{
					tSysState.ClientReqType = CLIENT_REQ_SET;
					tSysState.ClientReqSelCh = tMsgPkt.RxBuff[5];

					SetBoundaryVolt(tSysState.ClientReqSelCh);
				}

				RspAssamblyTxMsg(tMsgPkt.Cmd);
			}break;

			case CMD_PD_TEST_MODE :
			{
				if(SYS_SEQ_OPRATION_MODE != m_SysProcSeq)
				{
					if(ON == tMsgPkt.RxBuff[5]) 	{ Measure_Start(); FAN_Ctrl(LO, ON);  }
					else							{ Measure_Stop();  FAN_Ctrl(LO, OFF); }

					tSysState.SelGetAdcCh = ADC_GET_PD_CH;
					m_SysProcSeq = SYS_SEQ_GET_ADC_TEST_MODE;
				}
			}break;

			case CMD_SIPM_TEST_MODE :
			{
				if(SYS_SEQ_OPRATION_MODE != m_SysProcSeq)
				{
					if(ON == tMsgPkt.RxBuff[5])  	{ Measure_Start();  FAN_Ctrl(LO, ON);  }
					else							{ Measure_Stop();   FAN_Ctrl(LO, OFF); }

					tSysState.SelGetAdcCh = ADC_GET_SiPM_CH;
					m_SysProcSeq = SYS_SEQ_GET_ADC_TEST_MODE;
				}
			}break;


			case CMD_SLEEP_MODE :
			{
				RspAssamblyTxMsg(tMsgPkt.Cmd);
				m_SysProcSeq = SYS_SEQ_POWER_DOWN_MODE;
			}break;

			case CMD_WAKE_UP :
			{
				RspAssamblyTxMsg(tMsgPkt.Cmd);
				m_SysProcSeq = SYS_SEQ_STANDBY_MODE;
			}break;

			// Error
			case CMD_DEVICE_INFO :
			{
				if (0x01 == tMsgPkt.Leng) { tSysState.ClientReqDevInfo = tMsgPkt.RxBuff[4];  tSysState.ClientReqType = CLIENT_REQ_GET;}
				if (0x08 == tMsgPkt.Leng) { DeviceInfo(DEVICE_INFO_PRODUCT_TYPE); tSysState.ClientReqType = CLIENT_REQ_SET;}
				if (0x10 == tMsgPkt.Leng) { DeviceInfo(DEVICE_INFO_SERIAL_NUM); tSysState.ClientReqType = CLIENT_REQ_SET;}

				RspAssamblyTxMsg(tMsgPkt.Cmd);
			}break;

			case CMD_DEVICE_VER :
			{
				if (0x01 == tMsgPkt.Leng) { tSysState.ClientReqType = CLIENT_REQ_GET;}
				if (0x05 == tMsgPkt.Leng) { DeviceFirmwareVer(); tSysState.ClientReqType = CLIENT_REQ_SET;}

				RspAssamblyTxMsg(tMsgPkt.Cmd);
			}break;

			case CMD_DEVICE_STATE :
			{
				if (0x00 == tMsgPkt.Leng) { tSysState.ClientReqType = CLIENT_REQ_GET;}

				RspAssamblyTxMsg(tMsgPkt.Cmd);
			}break;


			case CMD_SET_CORRECTION_VAL :
			{
				if (0x02 == tMsgPkt.Leng) { tSysState.ClientReqType = CLIENT_REQ_GET; tSysState.ClientReqSelCh = tMsgPkt.RxBuff[5]; }
				if (0x06 == tMsgPkt.Leng)
				{
					tSysState.ClientReqType = CLIENT_REQ_SET;
					tSysState.ClientReqSelCh = tMsgPkt.RxBuff[5];

					SetAdjustCal(tSysState.ClientReqSelCh);
				}

				RspAssamblyTxMsg(tMsgPkt.Cmd);
			}break;

			case CMD_SET_FIRMWARE_DOWNLOAD :
			{
				//if (0x00 == tMsgPkt.Leng) { tSysState.ClientReqType = CLIENT_REQ_GET;}
				RspAssamblyTxMsg(tMsgPkt.Cmd);
				//SetCfg_Flash_Firmware_Write();
				//m_SysProcSeq = SYS_SEQ_RESET;
				FlagFirmwareWROn = ON;

			}break;

			case CMD_SOFT_RESET :
			{
				if (0x00 == tMsgPkt.Leng) { tSysState.ClientReqType = CLIENT_REQ_GET;}

				RspAssamblyTxMsg(tMsgPkt.Cmd);
				m_SysProcSeq = SYS_SEQ_RESET;
			}break;
		}

		UART_RxInit();
		tMsgPkt.FlagRxPktOn = OFF;
	}
}


void Operating_Process(void)
{
	if(OFF == tSysState.FlagStatClenaOn)
	{
		if(ON == tSysState.FlagOperModeOn)
		{
			EXT_HV_Ctrl(ON);
			EXT_PD_Ctrl(ON);
			EXT_LD_Ctrl(ON);

			ReadMeasuredVal();
		}
	}
	else
	{
		FanCleanCtrl();
	}
}


void RngOfPdAdcSamplingVal(uint16_t nAdcVal)
{
	//volatile uint16_t Tmp = nAdcVal;

	uint8_t m_SelCurrPdAdcRng = 99;
	uint8_t FlagGetOn = OFF;

//	if ( (250 < nAdcVal) && (m_MaxPeckAdVal < nAdcVal) )
	if ( (tParam[PM_CH1].LB < nAdcVal) && (m_MaxPeckAdVal < nAdcVal) )
	{
		m_MaxPeckAdVal = nAdcVal;
		m_FlagDectPeckOn = ON;
	}

	//if (0 >= nAdcVal) {m_FlagDectPeckOn = OFF; }

//	if  ( (250 > nAdcVal) && (ON == m_FlagDectPeckOn) )
	if  ( (tParam[PM_CH1].LB > nAdcVal) && (ON == m_FlagDectPeckOn) )
	{

	    if ( (tParam[PM_CH1].LB <= m_MaxPeckAdVal) && (tParam[PM_CH1].UB >= m_MaxPeckAdVal) ) { m_SelCurrPdAdcRng = PM0_5; tPmResult1[PM_CH1].nPdChAdc = nAdcVal;}
		if ( (tParam[PM_CH2].LB <= m_MaxPeckAdVal) && (tParam[PM_CH2].UB >= m_MaxPeckAdVal) ) { m_SelCurrPdAdcRng = PM1_0; tPmResult1[PM_CH2].nPdChAdc = nAdcVal;}
		if ( (tParam[PM_CH3].LB <= m_MaxPeckAdVal) && (tParam[PM_CH3].UB >= m_MaxPeckAdVal) ) { m_SelCurrPdAdcRng = PM2_5; tPmResult1[PM_CH3].nPdChAdc = nAdcVal;}
		if ( (tParam[PM_CH4].LB <= m_MaxPeckAdVal) && (tParam[PM_CH4].UB >= m_MaxPeckAdVal) ) { m_SelCurrPdAdcRng = PM4_0; tPmResult1[PM_CH4].nPdChAdc = nAdcVal;}
		if ( (tParam[PM_CH5].LB <= m_MaxPeckAdVal) && (tParam[PM_CH5].UB >= m_MaxPeckAdVal) ) { m_SelCurrPdAdcRng = PM_10; tPmResult1[PM_CH5].nPdChAdc = nAdcVal;}


		m_PeckPDAdVal = m_MaxPeckAdVal;

		//m_MaxPeckAdVal = tParam[PM_CH1].LB + 1;
		m_MaxPeckAdVal = tParam[PM_CH1].LB;
		m_FlagDectPeckOn = OFF;
		m_FlagAdcSendOn = ON;
		FlagGetOn = ON;

	}

	if( ON == FlagGetOn)
	{
	    if(PM0_5 == m_SelCurrPdAdcRng) tPmResult1[PM_CH1].nTmpNumConcet++;
	    if(PM1_0 == m_SelCurrPdAdcRng) tPmResult1[PM_CH2].nTmpNumConcet++;
		if(PM2_5 == m_SelCurrPdAdcRng) tPmResult1[PM_CH3].nTmpNumConcet++;
		if(PM4_0 == m_SelCurrPdAdcRng) tPmResult1[PM_CH4].nTmpNumConcet++;
		if(PM_10 == m_SelCurrPdAdcRng) tPmResult1[PM_CH5].nTmpNumConcet++;
	}

//	// 25.07.11 YJYOON 수정 - 차이없음
//    static uint16_t maxVal = 0;
//    static uint8_t detecting = 0;
//
//    // 피크 검출(상승): 기준값 이상이면 maxVal 갱신, detecting ON
//    if (nAdcVal > tParam[PM_CH1].LB) {
//        if (!detecting) {
//            maxVal = nAdcVal;
//            detecting = 1;
//        } else if (nAdcVal > maxVal) {
//            maxVal = nAdcVal;
//        }
//    }
//
//    // 피크 종료(하강): 기준값 이하로 떨어졌을 때만 카운트, detecting ON 상태에서만 동작
//    if (nAdcVal <= tParam[PM_CH1].LB && detecting) {
//        for (int ch = 0; ch < 5; ch++) {
//            if (tParam[ch].LB <= maxVal && maxVal < tParam[ch].UB) {
//                tPmResult1[ch].nTmpNumConcet++;
//                break;
//            }
//        }
//        // 피크 종료 후 초기화
//        maxVal = 0;
//        detecting = 0;
//    }
}

void RngOfSiPmAdcSamplingVal(uint16_t nAdcVal)
{
	//volatile uint16_t Tmp = nAdcVal;

	if( (tParam[PM_SiPM].LB < nAdcVal) && (tParam[PM_SiPM].UB > nAdcVal) )
	{
		if(m_MaxPeckAdVal2 < nAdcVal)
		{
			m_MaxPeckAdVal2 = nAdcVal;
			m_FlagDectPeckOn2 = ON;
		}
	}

	if( (tParam[PM_SiPM].LB > nAdcVal) && (ON == m_FlagDectPeckOn2) )
	{
		tPmResult1[PM_SiPM].nTmpNumConcet++;

		m_PeckSiPMAdVal = m_MaxPeckAdVal2;
		m_MaxPeckAdVal2 = tParam[PM_SiPM].LB + 1;
		m_FlagAdcSendOn = ON;
		m_FlagDectPeckOn2 = OFF;
	}
}

void Built_In_Test_Proc(void)
{
	switch(m_BITProcSeq)
	{
		// Configuration Read & Verification
		case BIT_SEQ_START :
		{
			FAN_Ctrl(LO, OFF);
			EXT_SiPM_Ctrl(OFF);

			EXT_PD_Ctrl(OFF);
			EXT_HV_Ctrl(OFF);
			EXT_LD_Ctrl(OFF);

			WaitTime_Init();
			m_BITProcSeq = BIT_SEQ_EEPROM_WRITE_READ;
		}break;

//////------------------------------------------------------------------------------------------- STEP 1 <EEPROM WRITE CHK>
		// Parameter Read & Verification
		case BIT_SEQ_EEPROM_WRITE_READ	:
		{
			tConfig.DeviceUseCount = GetCfg_DeviceUseCount();
			m_BITProcSeq = BIT_SEQ_EEPROM_WRITE_TIME_CHK;
		}break;

		case BIT_SEQ_EEPROM_WRITE_TIME_CHK	:
		{
			if ( TRUE == WaitHoldTime_Sec(ON, EEP_EEPROM_CHK_TIME) )
			{
				SetCfg_DeviceUseCount(tConfig.DeviceUseCount+1);
				m_BITProcSeq = BIT_SEQ_EEPROM_WRITE_VERIFY_CHK;
			}
		}break;

		case BIT_SEQ_EEPROM_WRITE_VERIFY_CHK :
		{
			if((tConfig.DeviceUseCount+1) == GetCfg_DeviceUseCount())
			{
				WaitHoldTime_Sec(OFF, CHK_TIME_OFF);
				tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_EEPROM_OK, OFF);
				m_BITProcSeq = BIT_SEQ_FAN_CON_LO_SPD_CTRL_ON;
			}
			else
			{
				tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_EEPROM_OK, ON);
				m_BITProcSeq = BIT_SEQ_ERROR;
			}
		}break;

//////------------------------------------------------------------------------------------------- STEP 2 <FAN _LO_ CONTROL, RPM CHK>
		case BIT_SEQ_FAN_CON_LO_SPD_CTRL_ON	:
		{
			FAN_Ctrl(LO, ON);
			WaitTime_Init();
			WaitHoldTime_Sec(OFF, CHK_TIME_OFF);
			m_BITProcSeq = BIT_SEQ_FAN_CON_LO_SPD_STAB_TIME;

		}break;

		case BIT_SEQ_FAN_CON_LO_SPD_STAB_TIME	:
		{
			if ( TRUE == WaitHoldTime_Sec(ON, FAN_CON_ON_TIME) )
			{
				tFan.RpmCurrCnt = 0;
				FAN_RpmMeasuerment(ON);
				WaitTime_Init();

				m_BITProcSeq = BIT_SEQ_FAN_CON_LO_SPD_CHK;
			}
		}break;

		case BIT_SEQ_FAN_CON_LO_SPD_CHK	:
		{
			if ( TRUE == WaitHoldTime_Sec(ON, FAN_MEASURE_TIME) )
			{
				if (ON == FAN_ChkCondition(DEFAULT_FAN_RPM_LOW_SPD_COUNT, tFan.RpmCurrCnt))
				{
					tFan.RpmCurrCnt = 0;
					FAN_RpmMeasuerment(OFF);
					FAN_Ctrl(HI, ON);

					tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_SPD_N, OFF);
					tFan.FlagErrOn = ON;
					m_BITProcSeq = BIT_SEQ_FAN_CON_HI_SPD_CTRL_ON;

					tSysState.ErrRetry = 0;
				}

				else
				{
					tFan.RpmCurrCnt = 0;
					if(3 >= tSysState.ErrRetry++) { m_BITProcSeq = BIT_SEQ_FAN_CON_LO_SPD_CTRL_ON; }
					else
					{
						tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_SPD_N, ON);
						tFan.FlagErrOn = ON;
						m_BITProcSeq = BIT_SEQ_ERROR;

						tSysState.ErrRetry = 0;
					}
				}

				WaitTime_Init();
			}
		}break;

//////------------------------------------------------------------------------------------------- STEP 3 <FAN _HI_ CONTROL, RPM CHK>
		case BIT_SEQ_FAN_CON_HI_SPD_CTRL_ON	:
		{
			FAN_Ctrl(HI, ON);
			WaitTime_Init();
			WaitHoldTime_Sec(OFF, CHK_TIME_OFF);
			m_BITProcSeq = BIT_SEQ_FAN_CON_HI_SPD_STAB_TIME;
		}break;

		case BIT_SEQ_FAN_CON_HI_SPD_STAB_TIME	:
		{
			if ( TRUE == WaitHoldTime_Sec(ON, FAN_CON_ON_TIME) )
			{
				tFan.RpmCurrCnt = 0;
				FAN_RpmMeasuerment(ON);
				WaitTime_Init();

				m_BITProcSeq = BIT_SEQ_FAN_CON_HI_SPD_CHK;
			}
		}break;

		case BIT_SEQ_FAN_CON_HI_SPD_CHK	:
		{
			if ( TRUE == WaitHoldTime_Sec(ON, FAN_MEASURE_TIME) )
			{
				if (ON == FAN_ChkCondition(DEFAULT_FAN_RPM_HIGH_SPD_COUNT, tFan.RpmCurrCnt))
				{
					FAN_RpmMeasuerment(OFF);
					FAN_Ctrl(LO, ON);

					tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_SPD_H, OFF);
					tFan.FlagErrOn = OFF;

					m_BITProcSeq = BIT_SEQ_LD_MON_ADC_CTRL_ON;

					tSysState.ErrRetry = 0;
				}

				else
				{
					tFan.RpmCurrCnt = 0;
					if(3 >= tSysState.ErrRetry++) { m_BITProcSeq = BIT_SEQ_FAN_CON_HI_SPD_CTRL_ON; }
					else
					{
						tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_SPD_H, ON);
						tFan.FlagErrOn = ON;

						tSysState.ErrRetry = 0;
						m_BITProcSeq = BIT_SEQ_ERROR;

					}
				}

				WaitTime_Init();
			}
		}break;

//////------------------------------------------------------------------------------------------- STEP 4 <LD _HI_ CONTROL, LD ADC CHK>
		case BIT_SEQ_LD_MON_ADC_CTRL_ON	:
		{
			EXT_LD_Ctrl(ON);
			ADC_LD_Measuerment(OFF); // Initial
			WaitTime_Init();
			m_BITProcSeq = BIT_SEQ_LD_MON_ADC_STAB_TIME;
			tSysState.FlagGetAdcOn = OFF;
		}break;

		case BIT_SEQ_LD_MON_ADC_STAB_TIME	:
		{
			tSysState.FlagGetAdcOn = ON;

			if ( TRUE == WaitHoldTime_Sec(ON, LD_CTRL_ON_TIME) )
			{
				WaitTime_Init();
				m_BITProcSeq = BIT_SEQ_LD_MON_ADC_CHK_ON;
			}
		}break;

		case BIT_SEQ_LD_MON_ADC_CHK_ON	:
		{
			if ( TRUE == WaitHoldTime_mSec(ON, LD_ADC_MEASURE_TIME) )
			{
				tSysState.FlagGetAdcOn = OFF;

				if(TRUE == ADC_ChkCondition(DEFAULT_ADC_LD_NORMAL_VAL, tLdCtrl.AvgAdcVal))
				{
					ADC_LD_Measuerment(OFF);

					tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_LD_OK, OFF);
					tLdCtrl.FlagErrOn = OFF;
					m_BITProcSeq = BIT_SEQ_HV_CHK_ADC_CTRL_ON;

					tSysState.ErrRetry = 0;
				}
				else
				{
					if(3 >= tSysState.ErrRetry++) { m_BITProcSeq = BIT_SEQ_LD_MON_ADC_CTRL_ON; }
					else
					{
						tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_LD_OK, ON);
						tLdCtrl.FlagErrOn = ON;
						m_BITProcSeq = BIT_SEQ_ERROR;

						tSysState.ErrRetry = 0;
					}

					tSysState.FlagGetAdcOn  = ON;
				}

				WaitTime_Init();
			}

			else
			{
				ADC_LD_Measuerment(ON);
			}
		}break;

//////------------------------------------------------------------------------------------------- STEP 5 <HV _HI_ CONTROL, HV ADC CHK>
		case BIT_SEQ_HV_CHK_ADC_CTRL_ON :
		{
			EXT_HV_Ctrl(ON);
			ADC_HV_Measuerment(OFF);
			WaitTime_Init();

			m_BITProcSeq = BIT_SEQ_HV_CHK_ADC_STAB_TIME;
		} break;

		case BIT_SEQ_HV_CHK_ADC_STAB_TIME	:
		{
			tSysState.FlagGetAdcOn = ON;

			if ( TRUE == WaitHoldTime_Sec(ON, HV_CTRL_ON_TIME) )
			{
				WaitTime_Init();
				m_BITProcSeq = BIT_SEQ_HV_CHK_ADC_CHK_ON;
			}
		}break;

		case BIT_SEQ_HV_CHK_ADC_CHK_ON	:
		{
			if ( true == WaitHoldTime_mSec(ON, HV_ADC_MEASURE_TIME) )
			{

				tSysState.FlagGetAdcOn = OFF;

				if(TRUE == ADC_ChkCondition(DEFAULT_ADC_HV_NORMAL_VAL, tHvCtrl.AvgAdcVal) )
				{
					ADC_HV_Measuerment(OFF);

					tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_HV_OK, OFF);
					tHvCtrl.FlagErrOn = OFF;
					m_BITProcSeq = BIT_SEQ_PD_SIG_ADC_CTRL_ON;

					tSysState.ErrRetry = 0;
				}
				else
				{
					if(3 >= tSysState.ErrRetry++) { m_BITProcSeq = BIT_SEQ_HV_CHK_ADC_CTRL_ON; }
					else
					{
						tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_HV_OK, ON);
						tHvCtrl.FlagErrOn = ON;
						m_BITProcSeq = BIT_SEQ_ERROR;

						tSysState.ErrRetry = 0;
					}

					tSysState.FlagGetAdcOn  = ON;
				}

				WaitTime_Init();
			}
			else
			{
				ADC_HV_Measuerment(ON);
			}
		}break;

//////------------------------------------------------------------------------------------------- STEP 6 <LD _HI_ CONTROL, PD ADC CHK>
		case BIT_SEQ_PD_SIG_ADC_CTRL_ON :
		{
			EXT_LD_Ctrl(ON);
			EXT_PD_Ctrl(ON);

			ADC_PD_Measuerment(OFF);
			WaitTime_Init();

			m_BITProcSeq = BIT_SEQ_PD_SIG_ADC_STAB_TIME;
		}break;

		case BIT_SEQ_PD_SIG_ADC_STAB_TIME	:
		{
			tSysState.FlagGetAdcOn = ON;

			if ( TRUE == WaitHoldTime_Sec(ON, PD_CTRL_ON_TIME) )
			{
				WaitTime_Init();
				m_BITProcSeq = BIT_SEQ_PD_SIG_ADC_CHK_ON;
			}
		}break;

		case BIT_SEQ_PD_SIG_ADC_CHK_ON	:
		{
			if ( true == WaitHoldTime_mSec(ON, PD_ADC_MEASURE_TIME) )
			{
				tSysState.FlagGetAdcOn = OFF;

				//if(TRUE ==  ADC_ChkCondition(DEFAULT_ADC_PD_NORMAL_VAL, tPdCtrl.AvgAdcVal)) <-----------------------------------------250619_CHECK! Error Logic Check
				if(DEFAULT_ADC_PD_ERROR_CNT > tPdCtrl.ErrorCount)
				{
					ADC_PD_Measuerment(OFF);

					tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_PD_OK, OFF);
					tHvCtrl.FlagErrOn = OFF;

					m_BITProcSeq = BIT_SEQ_SiPM_SIG_ADC_CTRL_ON;

					tSysState.ErrRetry = 0;
				}
				else
				{
					tSysState.ErrRetry++;

					if(3 >= tSysState.ErrRetry) { m_BITProcSeq = BIT_SEQ_PD_SIG_ADC_CTRL_ON; }
					else
					{
						tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_PD_OK, OFF);
						tPdCtrl.FlagErrOn = ON;
						m_BITProcSeq = BIT_SEQ_ERROR;

						tSysState.ErrRetry = 0;
					}

					tSysState.FlagGetAdcOn  = ON;
				}

				WaitTime_Init();
			}
			else
			{
				ADC_PD_Measuerment(ON);
			}
		}break;

//////------------------------------------------------------------------------------------------- STEP 7 <LD _HI_ CONTROL, SiPM ADC CHK>
		case BIT_SEQ_SiPM_SIG_ADC_CTRL_ON :
		{
			ADC_SiPM_Measuerment(OFF);
			EXT_SiPM_Ctrl(ON);
			WaitTime_Init();

			m_BITProcSeq = BIT_SEQ_SiPM_SIG_ADC_STAB_TIME;
		}break;

		case BIT_SEQ_SiPM_SIG_ADC_STAB_TIME	:
		{

			tSysState.FlagGetAdcOn = ON;

			if ( TRUE == WaitHoldTime_Sec(ON, SiPM_CTRL_ON_TIME) )
			{
				WaitTime_Init();
				m_BITProcSeq = BIT_SEQ_SiPM_SIG_ADC_CHK_ON;
			}
		}break;

		case BIT_SEQ_SiPM_SIG_ADC_CHK_ON	:
		{
			if ( true == WaitHoldTime_mSec(ON, PD_ADC_MEASURE_TIME) )
			{
				tSysState.FlagGetAdcOn = OFF;

				//if(TRUE == ADC_ChkCondition(DEFAULT_ADC_SiPM_NORMAL_VAL, tSiPmCtrl.AvgAdcVal) ) <-----------------------------------------250619_CHECK! Error Logic Check
				if(DEFAULT_ADC_SiPM_ERROR_CNT > tSiPmCtrl.ErrorCount)
				{
					ADC_SiPM_Measuerment(OFF);

					tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_SiPM_OK, OFF);
					tSiPmCtrl.FlagErrOn = OFF;

					m_BITProcSeq = BIT_SEQ_COMPLETE;
					tSysState.ErrRetry = 0;
				}
				else
				{
					tSysState.ErrRetry++;
					if(3 >= tSysState.ErrRetry) { m_BITProcSeq = BIT_SEQ_SiPM_SIG_ADC_CTRL_ON; }
					else
					{
						tSysState.DeviceState = DeviceSetState(DEVICE_BIT_ADDR_SiPM_OK, ON);
						tSiPmCtrl.FlagErrOn = ON;
						m_BITProcSeq = BIT_SEQ_ERROR;

						tSysState.ErrRetry = 0;
					}

					tSysState.FlagGetAdcOn  = ON;
				}

				WaitTime_Init();
			}
			else
			{
				ADC_SiPM_Measuerment(ON);
			}
		}break;

//////------------------------------------------------------------------------------------------- STEP 8
		case BIT_SEQ_COMPLETE:
		{
			WaitTime_Init();

			tSysState.BitChkOn = ON;
			tSysState.flagBitErrOn = OFF;

			RspAssamblyTxMsg(CMD_BIT_MANUAL_START);

			if(ON == tSysState.FlagBitChkOn)
			{

				tSysState.FlagBitChkOn = OFF;
			}

			m_BITProcSeq = BIT_SEQ_IDLE;
		}break;
		//Normal Seq------------------------------------------------------------------


//////------------------------------------------------------------------------------------------- STEP 9
		case BIT_SEQ_FAIL :
		{

		}break;

		case BIT_SEQ_IDLE :
		{

		}break;

		case BIT_SEQ_ERROR:
		{
			tSysState.BitChkOn = OFF;
			tSysState.flagBitErrOn = ON;
			tSysState.FlagBitChkOn = OFF;

			m_BITProcSeq = BIT_SEQ_FAIL;


			RspAssamblyTxMsg(CMD_DEVICE_STATE);
		}break;
	}
}
void SYSTEM_SEQ(void)
{
	switch(m_SysProcSeq)
	{
		case SYS_SEQ_IDLE :
		{
			m_SysProcSeq = SYS_SEQ_INIT;
		}break;


		case SYS_SEQ_READY :
		{

		}break;

		case SYS_SEQ_INIT :
		{
			FAN_Init();

			//m_SysProcSeq = SYS_SEQ_BIT_START;			//Debug  BIT Check
			m_SysProcSeq = SYS_SEQ_READY; 				//Debug_ None BIT Check
			m_MaxPeckAdVal =  tParam[PM_CH1].LB;
			m_MaxPeckAdVal2 = tParam[PM_SiPM].LB;

		}break;


		case SYS_SEQ_GET_CONFIG :
		{

		}break;

		case SYS_SEQ_GET_PARAM :
		{

		}break;

		case SYS_SEQ_BIT_START :
		{
			m_BITProcSeq = BIT_SEQ_START; //Debug
			m_SysProcSeq = SYS_SEQ_BIT_WAIT;
		}break;

		case SYS_SEQ_BIT_WAIT :
		{
			Built_In_Test_Proc();

			if(BIT_SEQ_IDLE == m_BITProcSeq )
			{
				if(ON == tSysState.flagBitErrOn ) 	{ m_SysProcSeq = SYS_SEQ_FAULT; }
				if(ON == tSysState.BitChkOn)
				{
					m_SysProcSeq = SYS_SEQ_STANDBY_MODE;
				}
			}
		}break;

	//=================================================================================================================
		case SYS_SEQ_STANDBY_MODE :
		{
			Measure_Stop();
			m_SysProcSeq = SYS_SEQ_READY;
		}break;

		case SYS_SEQ_OPRATION_MODE :
		{
			//tSysState.FlagOperModeOn = ON;
			//tSysState.FlagSendPmResultOn = ON;
			//tSysState.FlagGetAdcOn = ON;
			Operating_Process();
		}break;

		case SYS_SEQ_MANUAL_FAN_CLEAN_MODE :
		{
			if(TRUE == FanCleanCtrl()) {
				m_SysProcSeq = SYS_SEQ_READY;
			}
		}break;

		case SYS_SEQ_GET_ADC_TEST_MODE :
		{

			//tSysState.FlagOperModeOn = ON;
			//tSysState.FlagGetAdcOn = ON;
			//tSysState.SelGetAdcCh = ADC_GET_SiPM_CH;

			Manual_GetAdcMsg();

		}break;

		case SYS_SEQ_STOP :
		{

		}break;

		case SYS_SEQ_POWER_DOWN_MODE :
		{
			Measure_Stop();
		}break;

		case SYS_SEQ_RETRY :
		{

		}break;

		case SYS_SEQ_ERROR_REPORT :
		{

		}break;

		case SYS_SEQ_FAULT :
		{

		}break;

		case SYS_SEQ_RESET :
		{
			SCB->AIRCR = AIRCR_VECTKEY_MASK | 0x04;
		}break;
	}
}

void SYSTEM_Process(void)
{
	UART_MsgProc();
	PostMessage_Proc();

	SYSTEM_SEQ();
	SystemTimeCtrl();
}

void SystemTimeCtrl(void)
{
	if(ON == tSysTime.FlagSysTickOn)
	{
		tSysTime.Sys10mSecTick++;

		if(ON == tSysTime.FlagWaitmSecTimeOn) { tSysTime.WaitmSecTime++; }
		tSysTime.FlagSysTickOn = OFF;


	}

	if (TICK_100mSEC <= tSysTime.Sys10mSecTick)
	{
		tSysTime.Sys10mSecTick = 0;
		tSysTime.Sys100mSecTick++;
	}

	if (TICK_1SEC <= tSysTime.Sys100mSecTick)
	{
		tSysTime.Sys100mSecTick = 0;
		if(ON == tSysState.FlagOperModeOn)
		{
			if(ON == tFan.FlagCleanModeOn)
			{
				tFan.CleWorkTime++;

				if(tFan.CleInterval <= tFan.CleWorkTime)
				{
					tSysState.FlagStatClenaOn = ON;
				}
			}
		}
/*
		  uint8_t temp[32] = "Hello2 World!";
		  memcpy(tUartMsg.RingTxTemp, temp, 14);
		  tUartMsg.TxTempCnt = 13;
		  tMsgPkt.FlagTxPktOn = ON;
*/

		MCU_AlarmTempMonitor(tSysState.Temp);
		if(ON == tSysTime.FlagWaitSecTimeOn) { tSysTime.WaitSecTime++; }
	}
}
