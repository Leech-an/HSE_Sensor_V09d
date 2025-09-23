#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <Const.h>

#include "stm32l4xx_hal.h"


#define EXT_LD_CTRL_ON		1
#define EXT_LD_CTRL_OFF		0
#define EXT_PD_CTRL_ON		1
#define EXT_PD_CTRL_OFF		0
#define EXT_HV_CTRL_ON		1
#define EXT_HV_CTRL_OFF		0
#define EXT_SiPM_CTRL_ON		1
#define EXT_SiPM_CTRL_OFF		0



#define EXT_LD_MEASUR_ON		1
#define EXT_LD_MEASUR_OFF		0
#define EXT_PD_MEASUR_ON		1
#define EXT_PD_MEASUR_OFF		0
#define EXT_HV_MEASUR_ON		1
#define EXT_HV_MEASUR_OFF		0
#define EXT_SiPM_MEASURE_ON		1
#define EXT_SiPM_MEASURE_OFF	1


struct _tPDCTRL {

	uint8_t 	FlagCtrlOn;
	uint8_t 	FlagCmdBusyOn;
	uint8_t 	FlagMeasureOn;
	uint8_t 	FlagErrOn;
	uint16_t	AvgAdcVal;

	uint16_t CurrAdcVal;

	uint32_t  ChkCount;
	uint32_t  ErrorCount;

};

struct _tLDCTRL {

	uint8_t FlagCtrlOn;
	uint8_t FlagCmdBusyOn;
	uint8_t FlagMeasureOn;
	uint8_t FlagErrOn;
	uint16_t	AvgAdcVal;

	uint16_t  CurrAdcVal;
	uint32_t  ChkCount;
	uint32_t  ErrorCount;
};


struct _tSiPMCTRL {

	uint8_t FlagCtrlOn;
	uint8_t FlagCmdBusyOn;
	uint8_t FlagMeasureOn;
	uint8_t FlagErrOn;
	uint16_t	AvgAdcVal;

	uint16_t  CurrAdcVal;
	uint32_t  ChkCount;
	uint32_t  ErrorCount;
};


struct _tHVCTRL {

	uint8_t FlagCtrlOn;
	uint8_t FlagCmdBusyOn;
	uint8_t FlagMeasureOn;
	uint8_t FlagErrOn;
	uint16_t	AvgAdcVal;

	uint16_t  CurrAdcVal;
	uint32_t  ChkCount;
	uint32_t  ErrorCount;
};


extern struct _tPDCTRL		tPdCtrl;
extern struct _tLDCTRL		tLdCtrl;
extern struct _tHVCTRL		tHvCtrl;
extern struct _tSiPMCTRL	tSiPmCtrl;

extern void EXT_Init(void);

extern void EXT_LD_Ctrl(uint8_t nCtrl);
extern void EXT_HV_Ctrl(uint8_t nCtrl);
extern void EXT_PD_Ctrl(uint8_t nCtrl);
extern void EXT_SiPM_Ctrl(uint8_t nCtrl);

extern void ADC_PD_Measuerment(uint8_t OnChk);
extern void ADC_LD_Measuerment(uint8_t OnChk);
extern void ADC_HV_Measuerment(uint8_t OnChk);
extern void ADC_SiPM_Measuerment(uint8_t OnChk);

extern uint8_t ADC_ChkCondition(uint16_t DefSpd, uint16_t nAdcVal);
