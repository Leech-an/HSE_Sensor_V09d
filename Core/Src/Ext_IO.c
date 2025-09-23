
#include <Ext_IO.h>

#include <Const.h>
#include <G_Value.h>

#include "Parameter.h"

#include <string.h>

#define LD_MAX_COUNT		0xFFFFFE
#define PD_MAX_COUNT		0xFFFFFE
#define HV_MAX_COUNT		0xFFFFFE
#define SiPM_MAX_COUNT		0xFFFFFE

#define LD_CHK_OK_COUNT		30000
#define HV_CHK_OK_COUNT		30000

struct _tPDCTRL tPdCtrl;
struct _tLDCTRL	tLdCtrl;
struct _tHVCTRL	tHvCtrl;
struct _tSiPMCTRL	tSiPmCtrl;

void EXT_Init(void)
{
	memset(&tLdCtrl, 0, sizeof(struct _tLDCTRL));
	memset(&tPdCtrl, 0, sizeof(struct _tPDCTRL));
	memset(&tHvCtrl, 0, sizeof(struct _tHVCTRL));
	memset(&tSiPmCtrl, 0, sizeof(struct _tSiPMCTRL));

	tPdCtrl.AvgAdcVal 		= DEFAULT_ADC_PD_NORMAL_VAL;
	tLdCtrl.AvgAdcVal 		= DEFAULT_ADC_LD_NORMAL_VAL;
	tHvCtrl.AvgAdcVal		= DEFAULT_ADC_HV_NORMAL_VAL;
	tSiPmCtrl.AvgAdcVal 	= DEFAULT_ADC_SiPM_NORMAL_VAL;

	EXT_LD_Ctrl(EXT_LD_CTRL_OFF);
	EXT_HV_Ctrl(EXT_HV_CTRL_OFF);
	EXT_PD_Ctrl(EXT_PD_CTRL_OFF);
	EXT_SiPM_Ctrl(EXT_SiPM_CTRL_OFF);
}

// GPIO Control-----------------------------------------------------------------------------------------------------------------------
void EXT_LD_Ctrl(uint8_t nCtrl)
{
	if(EXT_LD_CTRL_ON == nCtrl) 	{ HAL_GPIO_WritePin(LD_CON_GPIO_Port, LD_CON_Pin, GPIO_PIN_SET);    tLdCtrl.FlagCtrlOn = EXT_LD_CTRL_ON;  }
	else							{ HAL_GPIO_WritePin(LD_CON_GPIO_Port, LD_CON_Pin, GPIO_PIN_RESET);  tLdCtrl.FlagCtrlOn = EXT_LD_CTRL_OFF; tLdCtrl.ErrorCount = 0;}
}

void EXT_HV_Ctrl(uint8_t nCtrl)
{
	if(EXT_HV_CTRL_ON == nCtrl) 	{ HAL_GPIO_WritePin(HV_CON_GPIO_Port, HV_CON_Pin, GPIO_PIN_SET);    tHvCtrl.FlagCtrlOn = EXT_HV_CTRL_ON;  }
	else							{ HAL_GPIO_WritePin(HV_CON_GPIO_Port, HV_CON_Pin, GPIO_PIN_RESET);  tHvCtrl.FlagCtrlOn = EXT_HV_CTRL_OFF; tHvCtrl.ErrorCount = 0;}
}

void EXT_PD_Ctrl(uint8_t nCtrl)
{
	if(EXT_PD_CTRL_ON == nCtrl) 	{ HAL_GPIO_WritePin(LD_CON_GPIO_Port, LD_CON_Pin, GPIO_PIN_SET);    tPdCtrl.FlagCtrlOn = EXT_PD_CTRL_ON;  }
	else							{ HAL_GPIO_WritePin(LD_CON_GPIO_Port, LD_CON_Pin, GPIO_PIN_RESET);  tPdCtrl.FlagCtrlOn = EXT_PD_CTRL_OFF; tPdCtrl.ErrorCount = 0;}
}

void EXT_SiPM_Ctrl(uint8_t nCtrl)
{
	if(EXT_SiPM_CTRL_ON == nCtrl) 	{ HAL_GPIO_WritePin(LD_CON_GPIO_Port, LD_CON_Pin, GPIO_PIN_SET);    tSiPmCtrl.FlagCtrlOn = EXT_SiPM_CTRL_ON;  }
	else							{ HAL_GPIO_WritePin(LD_CON_GPIO_Port, LD_CON_Pin, GPIO_PIN_RESET);  tSiPmCtrl.FlagCtrlOn = EXT_SiPM_CTRL_OFF; tSiPmCtrl.ErrorCount = 0;}
}
//------------------------------------------------------------------------------------------------------------------------------------

/*
void ADC_PD_Measuerment(uint8_t MeasureOn)
{
	if (ON == MeasureOn)
	{
		tPdCtrl.FlagCmdBusyOn = TRUE;

		if(FALSE == tPdCtrl.FlagMeasureOn)
		{
				 if((tParam[PM_CH1].LB <  tPdCtrl.CurrAdcVal) && (tParam[PM_CH1].UB > tPdCtrl.CurrAdcVal)) { if (PD_MAX_COUNT > tPdCtrl.PdChCount[PM_CH1]) tPdCtrl.PdChCount[PM_CH1]++; }
			else if((tParam[PM_CH2].LB <= tPdCtrl.CurrAdcVal) && (tParam[PM_CH2].UB > tPdCtrl.CurrAdcVal)) { if (PD_MAX_COUNT > tPdCtrl.PdChCount[PM_CH2]) tPdCtrl.PdChCount[PM_CH2]++; }
			else if((tParam[PM_CH3].LB <= tPdCtrl.CurrAdcVal) && (tParam[PM_CH3].UB > tPdCtrl.CurrAdcVal)) { if (PD_MAX_COUNT > tPdCtrl.PdChCount[PM_CH3]) tPdCtrl.PdChCount[PM_CH3]++; }
			else if((tParam[PM_CH4].LB <= tPdCtrl.CurrAdcVal) && (tParam[PM_CH4].UB > tPdCtrl.CurrAdcVal)) { if (PD_MAX_COUNT > tPdCtrl.PdChCount[PM_CH4]) tPdCtrl.PdChCount[PM_CH4]++; }
			else if((tParam[PM_CH5].LB <= tPdCtrl.CurrAdcVal) && (tParam[PM_CH5].UB > tPdCtrl.CurrAdcVal)) { if (PD_MAX_COUNT > tPdCtrl.PdChCount[PM_CH5]) tPdCtrl.PdChCount[PM_CH5]++; }
		}

		else
		{
			tPmResult1[PM_CH1].nNum_Concet = tPdCtrl.PdChCount[PM_CH1];
			tPmResult1[PM_CH2].nNum_Concet = tPdCtrl.PdChCount[PM_CH2];
			tPmResult1[PM_CH3].nNum_Concet = tPdCtrl.PdChCount[PM_CH3];
			tPmResult1[PM_CH4].nNum_Concet = tPdCtrl.PdChCount[PM_CH4];
			tPmResult1[PM_CH5].nNum_Concet = tPdCtrl.PdChCount[PM_CH5];
		}
	}

	else
	{
		tPmResult1[PM_CH1].nNum_Concet = 0;
		tPmResult1[PM_CH2].nNum_Concet = 0;
		tPmResult1[PM_CH3].nNum_Concet = 0;
		tPmResult1[PM_CH4].nNum_Concet = 0;
		tPmResult1[PM_CH5].nNum_Concet = 0;
		tPdCtrl.FlagMeasureOn = FALSE;
		tPdCtrl.FlagCmdBusyOn = FALSE;
	}
}
*/

void ADC_PD_Measuerment(uint8_t OnChk)
{
	if (ON == OnChk)
	{
		tPdCtrl.FlagCmdBusyOn = TRUE;

		if((DEFAULT_ADC_PD_LB_VAL < tPdCtrl.CurrAdcVal) && (DEFAULT_ADC_PD_UB_VAL > tPdCtrl.CurrAdcVal))
		{
			if (PD_MAX_COUNT > tPdCtrl.ChkCount++) {
				tPdCtrl.AvgAdcVal  = (tPdCtrl.CurrAdcVal + tPdCtrl.AvgAdcVal) / 2;
			}
			else
			{
				tPdCtrl.ChkCount = PD_MAX_COUNT;
			}
		}

		else
		{
			if (PD_MAX_COUNT < tPdCtrl.ErrorCount++) { tPdCtrl.ErrorCount = PD_MAX_COUNT; }
		}
	}

	else // Initial
	{
		tPdCtrl.FlagErrOn = OFF;
		tPdCtrl.FlagCmdBusyOn = FALSE;
		tPdCtrl.CurrAdcVal = 0;
		tPdCtrl.ChkCount = 0;
		tPdCtrl.AvgAdcVal = DEFAULT_ADC_PD_NORMAL_VAL;
	}
}

void ADC_LD_Measuerment(uint8_t OnChk)
{

	if (ON == OnChk)
	{
		tLdCtrl.FlagCmdBusyOn = TRUE;

		if((DEFAULT_ADC_LD_LB_VAL < tLdCtrl.CurrAdcVal) && (DEFAULT_ADC_LD_UB_VAL > tLdCtrl.CurrAdcVal))
		{
			if (LD_MAX_COUNT > tLdCtrl.ChkCount++) {
				tLdCtrl.AvgAdcVal  = (tLdCtrl.CurrAdcVal + tLdCtrl.AvgAdcVal) / 2;
			}
			else
			{
				tLdCtrl.ChkCount = LD_MAX_COUNT;
			}
		}

		else
		{
			if (LD_MAX_COUNT < tLdCtrl.ErrorCount++) { tLdCtrl.ErrorCount = LD_MAX_COUNT; }
		}
	}

	else // Initial
	{
		tLdCtrl.FlagErrOn = OFF;
		tLdCtrl.FlagCmdBusyOn = FALSE;
		tLdCtrl.CurrAdcVal = 0;

		tLdCtrl.ChkCount = 0;
		tLdCtrl.ErrorCount = 0;

		tLdCtrl.AvgAdcVal = DEFAULT_ADC_LD_NORMAL_VAL;
	}
}

void ADC_HV_Measuerment(uint8_t OnChk)
{

	if (ON == OnChk)
	{
		tHvCtrl.FlagCmdBusyOn = TRUE;

		if((DEFAULT_ADC_HV_LB_VAL < tHvCtrl.CurrAdcVal) && (DEFAULT_ADC_HV_UB_VAL > tHvCtrl.CurrAdcVal))
		{
			if (HV_MAX_COUNT > tHvCtrl.ChkCount++) {
				tHvCtrl.AvgAdcVal = (tHvCtrl.CurrAdcVal + tHvCtrl.AvgAdcVal) / 2;
			}
			else
			{
				tHvCtrl.ChkCount = HV_MAX_COUNT;
			}
		}

		else
		{
			if (HV_MAX_COUNT < tHvCtrl.ErrorCount++) { tHvCtrl.ErrorCount = HV_MAX_COUNT; }
		}
	}

	else // Initial
	{
		tHvCtrl.FlagErrOn = OFF;
		tHvCtrl.FlagCmdBusyOn = FALSE;
		tHvCtrl.CurrAdcVal = 0;
		tHvCtrl.ChkCount = 0;
		tHvCtrl.AvgAdcVal = DEFAULT_ADC_HV_NORMAL_VAL;
	}
}

void ADC_SiPM_Measuerment(uint8_t OnChk)
{

	if (ON == OnChk)
	{
		tSiPmCtrl.FlagCmdBusyOn = TRUE;

		if((DEFAULT_ADC_SiPM_LB_VAL < tSiPmCtrl.CurrAdcVal) && (DEFAULT_ADC_SiPM_UB_VAL > tSiPmCtrl.CurrAdcVal))
		{
			if (SiPM_MAX_COUNT > tSiPmCtrl.ChkCount++) { tSiPmCtrl.AvgAdcVal = (tSiPmCtrl.CurrAdcVal + tSiPmCtrl.AvgAdcVal) / 2; }
			else
			{
				tSiPmCtrl.ChkCount = SiPM_MAX_COUNT;
			}
		}

		else
		{
			if (SiPM_MAX_COUNT < tSiPmCtrl.ErrorCount++) { tSiPmCtrl.ErrorCount = HV_MAX_COUNT; }
		}
	}

	else // Initial
	{
		tSiPmCtrl.FlagErrOn = OFF;
		tSiPmCtrl.FlagCmdBusyOn = FALSE;
		tSiPmCtrl.CurrAdcVal = 0;
		tSiPmCtrl.ChkCount = 0;
		tSiPmCtrl.AvgAdcVal = DEFAULT_ADC_SiPM_NORMAL_VAL;
	}
}


uint8_t ADC_ChkCondition(uint16_t DefSpd, uint16_t nAdcVal)
{
	uint8_t result = FALSE;

	if( (CONST_REF_GUIDE_VAL_LB(DefSpd) <= nAdcVal) && (CONST_REF_GUIDE_VAL_UB(DefSpd) >= nAdcVal) )
	{
		result  = TRUE;
	}

	return result;
}



