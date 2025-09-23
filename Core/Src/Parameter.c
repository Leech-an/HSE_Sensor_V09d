#include <24LC16B.h>
#include "stdint.h"
#include "string.h"
#include "Fan.h"

#include "Const.h"
#include "Config.h"
#include "Parameter.h"


struct _tPARAM 		tParam[PM_CH_MAX];


void Default_FactorySet(void)
{
//-------- Param -------------//
	uint8_t  i = 0;
	uint8_t strDevType[8] 		= {0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00};
	uint8_t strDevSerial[16] 	= {0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t strDevVer[5] 		= {0x01, 0x00, 0x00, 0x01, 0x00};
	//Set Boundary

	SetCfg_CurrentFanUseTime(0);
	SetCfg_CurrentFanInterValTime(604800); // 1 Weak Sec

	for (i = 0; i <= 7; i++)
	{
			 if (0x00 == i) SetParam_BoundaryVolt(i, 0x0010);
		else if (0x01 == i) SetParam_BoundaryVolt(i, 0x001C);
		else if (0x02 == i) SetParam_BoundaryVolt(i, 0x006F);
		else if (0x03 == i) SetParam_BoundaryVolt(i, 0x0108);
		else if (0x04 == i) SetParam_BoundaryVolt(i, 0x05FA);
		else if (0x05 == i) SetParam_BoundaryVolt(i, 0x0CE4);
		else if (0x06 == i) SetParam_BoundaryVolt(i, 0x0064);
		else if (0x07 == i) SetParam_BoundaryVolt(i, 0x012C);
	}

	for (i = 0; i < 8; i++)
	{
		SetCfg_Device_ProcType(strDevType);
	}

	for (i = 0; i < 16; i++)
	{
		SetCfg_Device_ProcSerial(strDevSerial);
	}

	for (i = 0; i < 5; i++)
	{
		SetCfg_Device_ProcVer(strDevVer);
	}

	SetParam_AdjCal(0x00, 1.0f);
	SetParam_AdjCal(0x01, 1.0f);
	SetParam_AdjCal(0x02, 1.0f);
	SetParam_AdjCal(0x03, 1.0f);
	SetParam_AdjCal(0x04, 1.0f);
	SetParam_AdjCal(0x05, 1.0f);
	SetParam_AdjCal(0x06, 1.0f);

	//Set Correction Value
	//-- PD Density
	//-- CH1 Calibration
	//-- CH2 Calibration
	//-- CH3 Calibration
	//-- CH4 Calibration
	//-- CH5 Calibration
	//-- CFU Calibration
	//-- Correction Value



//-------- Config-------------//
	//Auto Cleaning Interval

	//Device Info (Product Type)

	//Device Info (Serial Num)

	//Device Info
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// Particle Size Low range value
//////////////////////////////////////////////////////////////////////
float SetParam_PartcleSize_LoRngVal(unsigned char PmCh)
{
	float result = 0;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// Particle Size High range value
//////////////////////////////////////////////////////////////////////
float SetParam_PartcleSize_HiRngVal(unsigned char PmCh)
{
	float result = 0;

	return result;
}


//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const Dp value
//////////////////////////////////////////////////////////////////////
float SetParam_Const_DpValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const Dpv value
//////////////////////////////////////////////////////////////////////
float SetParam_Const_DpvValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const calibration value
//////////////////////////////////////////////////////////////////////
float SetParam_Const_CalValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const CFU calibration value
//////////////////////////////////////////////////////////////////////
float SetParam_Const_CFU_CalValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}


// CONFIG
//=============================================================================

void SetCfg_CurrentFanUseTime(uint32_t nVal)
{
	I2C_EEP_WRITE_U32(EEP_IDX_CFG_FAN_USE_TIME, nVal);
}

uint32_t GetCfg_CurrentFanUseTime(void)
{
	return I2C_EEP_READ_U32(EEP_IDX_CFG_FAN_USE_TIME);
}


void SetCfg_CurrentFanInterValTime(uint32_t nVal)
{
	I2C_EEP_WRITE_U32(EEP_IDX_CFG_FAN_CLEAN_SET_TIME, nVal);
}

uint32_t GetCfg_CurrentFanInterValTime(void)
{
	return I2C_EEP_READ_U32(EEP_IDX_CFG_FAN_CLEAN_SET_TIME);
}

void SetCfg_Device_ProcType(uint8_t* nVal)
{
	uint8_t i = 0;
	for (i = 0; i<8; i++ )
	{
		I2C_EEP_WRITE_U08(EEP_IDX_CFG_DEVICE_PROC_TYPE+i, nVal[i]);
	}
}

void GetCfg_Device_ProcType(uint8_t* nVal)
{
	uint8_t i = 0;
	for (i = 0; i<8; i++ )
	{
		 nVal[i] = I2C_EEP_READ_U08(EEP_IDX_CFG_DEVICE_PROC_TYPE+i);
	}
}

void SetCfg_Device_ProcSerial(uint8_t* nVal)
{
	uint8_t i = 0;
	for (i = 0; i<16; i++ )
	{
		I2C_EEP_WRITE_U08(EEP_IDX_CFG_DEVICE_PROC_SERIAL+i, nVal[i]);
	}
}

void GetCfg_Device_ProcSerial(uint8_t* nVal)
{
	uint8_t i = 0;
	for (i = 0; i<16; i++ )
	{
		 nVal[i] = I2C_EEP_READ_U08(EEP_IDX_CFG_DEVICE_PROC_SERIAL+i);
	}
}

void SetCfg_Device_ProcVer(uint8_t* nVal)
{
	uint8_t i = 0;
	for (i = 0; i<5; i++ )
	{
		I2C_EEP_WRITE_U08(EEP_IDX_CFG_FIRMWARE_VERSION+i, nVal[i]);
	}
}

void GetCfg_Device_ProcVer(uint8_t* nVal)
{
	uint8_t i = 0;
	for (i = 0; i<5; i++ )
	{
		 nVal[i] = I2C_EEP_READ_U08(EEP_IDX_CFG_FIRMWARE_VERSION+i);
	}
}


void SetCfg_DeviceUseCount(uint32_t nVal)
{
	I2C_EEP_WRITE_U08(EEP_IDX_CFG_SET_EEP_CHK, 	 (uint8_t)((nVal >> 24)) );
	I2C_EEP_WRITE_U08(EEP_IDX_CFG_SET_EEP_CHK+1, (uint8_t)((nVal >> 16)) );
	I2C_EEP_WRITE_U08(EEP_IDX_CFG_SET_EEP_CHK+2, (uint8_t)((nVal >>  8)) );
	I2C_EEP_WRITE_U08(EEP_IDX_CFG_SET_EEP_CHK+3, (uint8_t)((nVal      )) );
}

uint32_t GetCfg_DeviceUseCount(void)
{
	uint32_t Result = 0;

	Result   = (uint32_t)(I2C_EEP_READ_U08(EEP_IDX_CFG_SET_EEP_CHK  ) << 24);
	Result  |= (uint32_t)(I2C_EEP_READ_U08(EEP_IDX_CFG_SET_EEP_CHK+1) << 16);
	Result  |= (uint32_t)(I2C_EEP_READ_U08(EEP_IDX_CFG_SET_EEP_CHK+2) <<  8);
	Result  |= (uint32_t)(I2C_EEP_READ_U08(EEP_IDX_CFG_SET_EEP_CHK+3)      );

	return Result;
}

uint8_t GetCfg_Flash_Firmware_Write(void)
{
	return  I2C_EEP_READ_U08(EEP_IDX_CFG_FLAG_FLASH_WR);
}

void SetCfg_Flash_Firmware_Write(void)
{
	I2C_EEP_WRITE_U08(EEP_IDX_CFG_FLAG_FLASH_WR, OFF);
}
//=============================================================================

// PARAM
void SetParam_BoundaryVolt(uint8_t nCh,  uint16_t nVal)
{
		 if(0x00 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_PD_BASE_ADC, nVal);
	else if(0x01 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_PD_CH1_ADC_UB, nVal);
	else if(0x02 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_PD_CH2_ADC_UB, nVal);
	else if(0x03 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_PD_CH3_ADC_UB, nVal);
	else if(0x04 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_PD_CH4_ADC_UB, nVal);
	else if(0x05 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_PD_CH5_ADC_UB, nVal);
	else if(0x06 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_SiPM_ADC_LB, nVal);
	else if(0x07 == nCh) I2C_EEP_WRITE_U16(EEP_IDX_PARAM_SiPM_ADC_UB, nVal);
}

uint16_t GetParam_BoundaryVolt(uint8_t nCh)
{
	uint16_t Result = 0;

		 if(0x00 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_PD_BASE_ADC);
	else if(0x01 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_PD_CH1_ADC_UB);
	else if(0x02 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_PD_CH2_ADC_UB);
	else if(0x03 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_PD_CH3_ADC_UB);
	else if(0x04 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_PD_CH4_ADC_UB);
	else if(0x05 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_PD_CH5_ADC_UB);
	else if(0x06 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_SiPM_ADC_LB);
	else if(0x07 == nCh) Result = I2C_EEP_READ_U16(EEP_IDX_PARAM_SiPM_ADC_UB);

	return Result;
}


// PARAM
void SetParam_AdjCal(uint8_t nSel, float fVal)
{
		 if(0x00 == nSel) I2C_EEP_WRITE_FLOAT(EEP_IDX_PARAM_PD_DENSITY, fVal);
	else if(0x01 == nSel) I2C_EEP_WRITE_FLOAT(EEP_IDX_PARAM_PD_CH1_CAL, fVal);
	else if(0x02 == nSel) I2C_EEP_WRITE_FLOAT(EEP_IDX_PARAM_PD_CH2_CAL, fVal);
	else if(0x03 == nSel) I2C_EEP_WRITE_FLOAT(EEP_IDX_PARAM_PD_CH3_CAL, fVal);
	else if(0x04 == nSel) I2C_EEP_WRITE_FLOAT(EEP_IDX_PARAM_PD_CH4_CAL, fVal);
	else if(0x05 == nSel) I2C_EEP_WRITE_FLOAT(EEP_IDX_PARAM_PD_CH5_CAL, fVal);
	else if(0x06 == nSel) I2C_EEP_WRITE_FLOAT(EEP_IDX_PARAM_CFU_CAL, fVal);
}

float GetParam_AdjCal(uint8_t nSel)
{
	float Result = 0.0f;

		 if(0x00 == nSel) Result = I2C_EEP_READ_FLOAT(EEP_IDX_PARAM_PD_DENSITY);
	else if(0x01 == nSel) Result = I2C_EEP_READ_FLOAT(EEP_IDX_PARAM_PD_CH1_CAL);
	else if(0x02 == nSel) Result = I2C_EEP_READ_FLOAT(EEP_IDX_PARAM_PD_CH2_CAL);
	else if(0x03 == nSel) Result = I2C_EEP_READ_FLOAT(EEP_IDX_PARAM_PD_CH3_CAL);
	else if(0x04 == nSel) Result = I2C_EEP_READ_FLOAT(EEP_IDX_PARAM_PD_CH4_CAL);
	else if(0x05 == nSel) Result = I2C_EEP_READ_FLOAT(EEP_IDX_PARAM_PD_CH5_CAL);
	else if(0x06 == nSel) Result = I2C_EEP_READ_FLOAT(EEP_IDX_PARAM_CFU_CAL);

	return Result;
}


//////////////////////////////////////////////////////////////////////
// Get parameter
// Particle Size Low range value
//////////////////////////////////////////////////////////////////////
float GetParam_PartcleSize_LoRngVal(unsigned char PmCh)
{
	float result = 0;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// Particle Size High range value
//////////////////////////////////////////////////////////////////////
float GetParam_PartcleSize_HiRngVal(unsigned char PmCh)
{
	float result = 0;

	return result;
}


//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const Dp value
//////////////////////////////////////////////////////////////////////
float GetParam_Const_DpValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const Dpv value
//////////////////////////////////////////////////////////////////////
float GetParam_Const_DpvValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const calibration value
//////////////////////////////////////////////////////////////////////
float GetParam_Const_CalValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const CFU calibration value
//////////////////////////////////////////////////////////////////////
float GetParam_Const_CFU_CalValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}


//////////////////////////////////////////////////////////////////////
// Get parameter
// Particle Size Low range value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_PartcleSize_LoRngVal(unsigned char PmCh)
{
	float result = 0;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// Particle Size High range value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_PartcleSize_HiRngVal(unsigned char PmCh)
{
	float result = 0;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel ADC reference Low range value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_AdcRefVolt_LoRngVal(unsigned char PmCh)
{
	unsigned int result = 0;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel ADC reference High range value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_AdcRefVolt_HiRngVal(unsigned char PmCh)
{
	unsigned int result = 0;

	return result;
}


//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const Dp value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_Const_DpValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const Dpv value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_Const_DpvValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const calibration value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_Const_CalValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}

//////////////////////////////////////////////////////////////////////
// Get parameter
// PD Channel Const CFU calibration value
//////////////////////////////////////////////////////////////////////
bool Verify_Param_Const_CFU_CalValue(unsigned char PmCh)
{
	float result = 0.0f;

	return result;
}


void PARAM_Init(I2C_HandleTypeDef* hI2C)
{
	uint8_t i = 0;
	I2C_EEP_Init(hI2C, 0xA0);
	for(i = 0; i <PM_CH_MAX; i++)
	{
		memset(&tParam[i], 0, sizeof(struct _tPARAM));

	}
}


void PARAM_Process(void)
{
	uint8_t  i = 0;

	if((DEFAULT_FACTORY_SET_ON	!= I2C_EEP_READ_U08(EEP_IDX_CFG_DEFAULT_FACTORY)))
	{
		Default_FactorySet();
		I2C_EEP_WRITE_U08(EEP_IDX_CFG_DEFAULT_FACTORY, DEFAULT_FACTORY_SET_ON);
	}

	tFan.CleWorkTime = GetCfg_CurrentFanUseTime();
	tFan.CleInterval = GetCfg_CurrentFanInterValTime();

	if (0 == tFan.CleInterval) 	{ tFan.FlagCleanModeOn = OFF; 	}
	else 						{ tFan.FlagCleanModeOn = ON;	}

	for (i = 0; i <= 7; i++)
	{
			 if (0x00 == i) { tParam[PM_CH1].BasePD = GetParam_BoundaryVolt(i); }
		else if (0x01 == i) { tParam[PM_CH1].LB = tParam[PM_CH1].BasePD ; 		tParam[PM_CH1].UB = GetParam_BoundaryVolt(i); }
		else if (0x02 == i) { tParam[PM_CH2].LB = tParam[PM_CH1].UB + 1; 		tParam[PM_CH2].UB = GetParam_BoundaryVolt(i); }
		else if (0x03 == i) { tParam[PM_CH3].LB = tParam[PM_CH2].UB + 1;		tParam[PM_CH3].UB = GetParam_BoundaryVolt(i); }
		else if (0x04 == i) { tParam[PM_CH4].LB = tParam[PM_CH3].UB + 1;		tParam[PM_CH4].UB = GetParam_BoundaryVolt(i); }
		else if (0x05 == i) { tParam[PM_CH5].LB = tParam[PM_CH4].UB + 1; 		tParam[PM_CH5].UB = GetParam_BoundaryVolt(i); }
		else if (0x06 == i)   tParam[PM_SiPM].LB = GetParam_BoundaryVolt(i);
		else if (0x07 == i)   tParam[PM_SiPM].UB = GetParam_BoundaryVolt(i);
	}

	//Set Boundary

	GetCfg_Device_ProcType(tConfig.prodtype);
	GetCfg_Device_ProcSerial(tConfig.prodSerial);
	GetCfg_Device_ProcVer(tConfig.FirmwareVer);

	//SetCfg_DeviceUseCount(1);
	tConfig.DeviceUseCount = GetCfg_DeviceUseCount();

	tParam[PM_CH1].den = GetParam_AdjCal(0x00);

	tParam[PM_CH1].Cal = GetParam_AdjCal(0x01);
	tParam[PM_CH2].Cal = GetParam_AdjCal(0x02);
	tParam[PM_CH3].Cal = GetParam_AdjCal(0x03);
	tParam[PM_CH4].Cal = GetParam_AdjCal(0x04);
	tParam[PM_CH5].Cal = GetParam_AdjCal(0x05);

	tParam[PM_SiPM].CfuCal = GetParam_AdjCal(0x06);


}
