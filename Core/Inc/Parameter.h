
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <Const.h>

#include "stm32l4xx.h"


struct _tPARAM{
	float 	Psize_lo;
	float 	Psize_Hi;
	unsigned int	BasePD;
	unsigned int	LB;
	unsigned int	UB;
	float	Dp;
	float	Dpv;
	float 	den;
	float	Cal;
	float	CfuCal;

};
/*
struct _tDEVICE{
	uint8_t ProcType[8];
	uint8_t SerialNum[16];
	uint8_t Ver[4];
};
*/

extern uint16_t SetParam_AdcRefVolt_LoRngVal(unsigned char PmCh, uint16_t nVal);
extern uint16_t SetParam_AdcRefVolt_HiRngVal(unsigned char PmCh, uint16_t nVal);

extern void SetCfg_CurrentFanUseTime(uint32_t nVal);
extern uint32_t GetCfg_CurrentFanUseTime(void);

extern void SetCfg_CurrentFanInterValTime(uint32_t nVal);
extern uint32_t GetCfg_CurrentFanInterValTime(void);
extern void SetCfg_Device_ProcType(uint8_t* nVal);
extern void GetCfg_Device_ProcSerial(uint8_t* nVal);
extern void SetCfg_Device_ProcSerial(uint8_t* nVal);
extern void GetCfg_Device_ProcVer(uint8_t* nVal);
extern void SetCfg_Device_ProcVer(uint8_t* nVal);

extern void SetParam_BoundaryVolt(uint8_t nCh,  uint16_t nVal);
extern uint16_t GetParam_BoundaryVolt(uint8_t nCh);

extern void SetParam_AdjCal(uint8_t nSel, float fVal);
extern float GetParam_AdjCal(uint8_t nSel);

extern struct _tPARAM 		tParam[PM_CH_MAX];

extern uint32_t GetCfg_DeviceUseCount(void);
extern void SetCfg_DeviceUseCount(uint32_t nVal);

extern uint8_t GetCfg_Flash_Firmware_Write(void);
extern void SetCfg_Flash_Firmware_Write(void);

extern void PARAM_Init(I2C_HandleTypeDef* hI2C);
extern bool PARAM_Verify(unsigned char PmCh);
extern void PARAM_Process(void);


