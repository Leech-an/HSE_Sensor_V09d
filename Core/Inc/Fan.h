#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <Const.h>

#include "stm32l4xx_hal.h"


#define FAN_CTRL_ON			1
#define FAN_CTRL_OFF		0

#define FAN_SPD_HI			1
#define FAN_SPD_LO			0

struct _tFAN {

	uint8_t 	FlagCtrlOn;
	uint8_t 	FlagCmdBusyOn;
	uint8_t 	FlagMeasureOn;
	uint8_t 	FlagMeasureComp;
	uint8_t 	FlagErrOn;

	uint8_t 	SelSpd;

	uint8_t		FlagCleanModeOn;
	uint32_t	CleWorkTime;
	uint32_t	CleInterval;



	uint32_t	RpmCurrCnt;
	uint16_t  	RpmSpd;
	uint8_t 	RpmFaultCnt;

};


extern struct _tFAN	tFan;

extern void FAN_Init(void);
extern void FAN_Ctrl(uint8_t SelSpd, uint8_t nCtrlOn);
extern uint8_t FAN_RpmMeasuerment(uint8_t MeasureOn);
extern uint8_t FAN_ChkCondition(uint16_t DefSpd,uint32_t nRpmCnt);
