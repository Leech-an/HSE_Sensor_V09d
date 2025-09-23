
#include <stdio.h>
#include <string.h>

#include "stm32l4xx.h"

#include <Const.h>
#include <G_Value.h>

#include <Parameter.h>
#include <Fan.h>

struct _tFAN	tFan;

static void SetFanSpdCtrl(uint8_t nSpd)
{
	if(FAN_SPD_HI == nSpd) 	{ HAL_GPIO_WritePin(FAN_SPEED_GPIO_Port, FAN_SPEED_Pin, GPIO_PIN_SET); 		tFan.SelSpd = FAN_SPD_HI; }
	else					{ HAL_GPIO_WritePin(FAN_SPEED_GPIO_Port, FAN_SPEED_Pin, GPIO_PIN_RESET); 	tFan.SelSpd = FAN_SPD_LO; }
}

static void SetFanCtrl(uint8_t nCtrl)
{
	if(FAN_CTRL_ON == nCtrl) 	{ HAL_GPIO_WritePin(FAN_CON_GPIO_Port, FAN_CON_Pin, GPIO_PIN_SET); 	  tFan.FlagCtrlOn = FAN_CTRL_ON;  }
	else						{ HAL_GPIO_WritePin(FAN_CON_GPIO_Port, FAN_CON_Pin, GPIO_PIN_RESET);  tFan.FlagCtrlOn = FAN_CTRL_OFF; }
}



void FAN_Init(void)
{
	memset(&tFan, 0, sizeof(struct _tFAN));
	SetFanCtrl(FAN_CTRL_OFF);
}


void FAN_Ctrl(unsigned char SelSpd, unsigned char nCtrlOn)
{
	SetFanSpdCtrl(SelSpd);
	SetFanCtrl(nCtrlOn);

	if(FAN_CTRL_OFF == nCtrlOn) { tFan.RpmCurrCnt = 0; SetFanSpdCtrl(FAN_SPD_LO); }
}

uint8_t FAN_RpmMeasuerment(uint8_t MeasureOn)
{
	uint8_t result = FALSE;

	if(OFF == MeasureOn) { HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);  tFan.RpmCurrCnt = 0;  return;}
	else
	{
		 HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

		 if(ON == tFan.FlagMeasureComp)
		 {
			 result = TRUE;
		 }
	}
	 return result;
}

uint8_t FAN_ChkCondition(uint16_t DefSpd,uint32_t nRpmCnt)
{
	uint8_t result = FALSE;

	if( (nRpmCnt >= CONST_REF_GUIDE_VAL_LB(DefSpd)) && (nRpmCnt <= CONST_REF_GUIDE_VAL_UB(DefSpd)) )
	{
		result  = TRUE;
	}

	return result;
}
