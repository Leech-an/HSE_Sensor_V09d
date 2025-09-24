/////////////////////////////////////////////////////////////////////////////////////////////
//
// Define Const
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define HI	1
#define LO	0

#define ON	1
#define OFF 0

#define FALSE 	0
#define TRUE	1

#define _SET 1
#define _GET 0

#define OK	1
#define FAULT 0


#define FAN_CON_Pin GPIO_PIN_14
#define FAN_CON_GPIO_Port GPIOC
#define FAN_SPEED_Pin GPIO_PIN_15
#define FAN_SPEED_GPIO_Port GPIOC
#define SiPM_Sig_Pin GPIO_PIN_1
#define SiPM_Sig_GPIO_Port GPIOA
#define PD_Sig_Pin GPIO_PIN_2
#define PD_Sig_GPIO_Port GPIOA
#define LD_Mon_Pin GPIO_PIN_3
#define LD_Mon_GPIO_Port GPIOA
#define HV_Chk_Pin GPIO_PIN_4
#define HV_Chk_GPIO_Port GPIOA
#define HV_CON_Pin GPIO_PIN_1
#define HV_CON_GPIO_Port GPIOB
#define FAN_RPM_Pin GPIO_PIN_8
#define FAN_RPM_GPIO_Port GPIOA
#define LD_CON_Pin GPIO_PIN_4
#define LD_CON_GPIO_Port GPIOB
#define PROM_WP_Pin GPIO_PIN_5
#define PROM_WP_GPIO_Port GPIOB

#define USART_TX_Pin GPIO_PIN_9
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_10
#define USART_RX_GPIO_Port GPIOA


/////////////////////////////////////////////////////////////////////////////////////////////
//
// Define ADC Channel
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define ADC_GET_PD_CH		0x08
#define ADC_GET_HV_CH		2
#define ADC_GET_LD_CH		3
#define ADC_GET_SiPM_CH		0x09


/////////////////////////////////////////////////////////////////////////////////////////////
//
// Define PD Channel
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define PM_CH_MAX		6

#define PM_CH1			0
#define PM_CH2			1
#define PM_CH3			2
#define PM_CH4			3
#define PM_CH5			4
#define PM_SiPM			5

#define PM_None				99
#define PM0_5				0
#define PM1_0				1
#define PM2_5				2
#define PM4_0				3
#define PM_10				4

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Default Parameter Value
//
/////////////////////////////////////////////////////////////////////////////////////////////

//Default PD Channel particle size (um)
#define DEFAULT_PARAM_PDCH1_PSIZE_LO			0.3f
#define DEFAULT_PARAM_PDCH1_PSIZE_HI			0.5f
#define DEFAULT_PARAM_PDCH2_PSIZE_LO			0.5f
#define DEFAULT_PARAM_PDCH2_PSIZE_HI			1.0f
#define DEFAULT_PARAM_PDCH3_PSIZE_LO			1.0f
#define DEFAULT_PARAM_PDCH3_PSIZE_HI			2.5f
#define DEFAULT_PARAM_PDCH4_PSIZE_LO			2.5f
#define DEFAULT_PARAM_PDCH4_PSIZE_HI			4.0f
#define DEFAULT_PARAM_PDCH5_PSIZE_LO			4.0f
#define DEFAULT_PARAM_PDCH5_PSIZE_HI			10.0f

//Default Channel reference voltage (mV)
#define DEFAULT_PARAM_PDCH1_REFVOLT_BASE		0x10	//16
#define DEFAULT_PARAM_PDCH1_REFVOLT_LB			10
#define DEFAULT_PARAM_PDCH1_REFVOLT_UB			0x1C	//28
#define DEFAULT_PARAM_PDCH2_REFVOLT_LB			28
#define DEFAULT_PARAM_PDCH2_REFVOLT_UB			0x6F	//111
#define DEFAULT_PARAM_PDCH3_REFVOLT_LB			111
#define DEFAULT_PARAM_PDCH3_REFVOLT_UB			0x0108	//264
#define DEFAULT_PARAM_PDCH4_REFVOLT_LB			694
#define DEFAULT_PARAM_PDCH4_REFVOLT_UB			0x05FA	//1530
#define DEFAULT_PARAM_PDCH5_REFVOLT_LB			1530
#define DEFAULT_PARAM_PDCH5_REFVOLT_UB			0x0CE4  //3300
#define DEFAULT_PARAM_SiPM_REFVOLT_LB			0x64	//100
#define DEFAULT_PARAM_SiPM_REFVOLT_UB			0x012C 	//300

//Default PD Channel Dp (um)
#define DEFAULT_PARAM_PDCH1_DP_VAL				0.4f
#define DEFAULT_PARAM_PDCH2_DP_VAL				0.75f
#define DEFAULT_PARAM_PDCH3_DP_VAL				1.75f
#define DEFAULT_PARAM_PDCH4_DP_VAL				3.25f
#define DEFAULT_PARAM_PDCH5_DP_VAL				7.00f

//Default PD Channel Dpv (um)
#define DEFAULT_PARAM_PDCH1_DPV_VAL				0.408f
#define DEFAULT_PARAM_PDCH2_DPV_VAL				0.777f
#define DEFAULT_PARAM_PDCH3_DPV_VAL				1.851f
#define DEFAULT_PARAM_PDCH4_DPV_VAL				3.307f
#define DEFAULT_PARAM_PDCH5_DPV_VAL				7.405f

//Define PD Channel Density
#define DEFAULT_PARAM_PDCH1_DENSITY				1.0f
#define DEFAULT_PARAM_PDCH2_DENSITY				1.0f
#define DEFAULT_PARAM_PDCH3_DENSITY				1.0f
#define DEFAULT_PARAM_PDCH4_DENSITY				1.0f
#define DEFAULT_PARAM_PDCH5_DENSITY				1.0f

//Define PD Channel Calibration
#define DEFAULT_PARAM_PDCH1_CAL					1.0f
#define DEFAULT_PARAM_PDCH2_CAL					1.0f
#define DEFAULT_PARAM_PDCH3_CAL					1.0f
#define DEFAULT_PARAM_PDCH4_CAL					1.0f
#define DEFAULT_PARAM_PDCH5_CAL					1.0f
#define DEFAULT_PARAM_SIPM_CAL					1.0f


//Define PD Channel CFU_Calibration
#define DEFAULT_PARAM_SIPM_CFU_CAL				1.0f
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Temp Control
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_TEMP_OVER_VAL					60

/////////////////////////////////////////////////////////////////////////////////////////////
//
// LD Control
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_ADC_LD_LB_VAL					300
#define DEFAULT_ADC_LD_UB_VAL					4096
#define DEFAULT_ADC_LD_NORMAL_VAL				1220

/////////////////////////////////////////////////////////////////////////////////////////////
//
// HV Control
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_ADC_HV_LB_VAL					300
#define DEFAULT_ADC_HV_UB_VAL					4096
#define DEFAULT_ADC_HV_NORMAL_VAL				950

/////////////////////////////////////////////////////////////////////////////////////////////
//
// PD Control
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_ADC_PD_LB_VAL					0
#define DEFAULT_ADC_PD_UB_VAL					30
#define DEFAULT_ADC_PD_NORMAL_VAL				10
#define DEFAULT_ADC_PD_ERROR_CNT				25000UL
/////////////////////////////////////////////////////////////////////////////////////////////
//
// SiPM Control
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_ADC_SiPM_LB_VAL					0
#define DEFAULT_ADC_SiPM_UB_VAL					30
#define DEFAULT_ADC_SiPM_NORMAL_VAL				10
#define DEFAULT_ADC_SiPM_ERROR_CNT				25000UL
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Default configuration Value
//
/////////////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_REF_BOUNDARY					10.0f			//Percent
#define CONST_REF_GUIDE_VAL_LB(x)				(float)( x * ( (100.0 - DEFAULT_REF_BOUNDARY)/100.0 ))	// NormalVal * ((100-10)/100)
#define CONST_REF_GUIDE_VAL_UB(x)				(float)( x * ( (100.0 + DEFAULT_REF_BOUNDARY)/100.0 ))	// NormalVal * ((100+10)/100)

#define DEFAULT_FACTORY_SET_ON					1
#define DEFAULT_DEVICE_PROC_TYPE				1 			// Sec
#define DEFAULT_DEVICE_PROC_SERIAL				100100 		// Sec

#define DEFAULT_FIRMWARE_MAJ_VERSION			0x00
#define DEFAULT_FIRMWARE_MIN_VERSION			0x09

#define DEFAULT_FAN_CLEAN_SET_TIME				604800 		// Sec

#define DEFAULT_FAN_RPM_LOW_SPD_COUNT			172
#define DEFAULT_FAN_RPM_HIGH_SPD_COUNT			410



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	EEPROM Index
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EEP_PAGE_SYSTEM							0




#define EEP_PAGE_CONFIG							1
#define EEP_IDX_CFG_DEFAULT_FACTORY				1	//1byte

#define EEP_IDX_CFG_SET_EEP_CHK					2	//4byte

#define EEP_IDX_CFG_DEVICE_PROC_TYPE			6	//8byte
#define EEP_IDX_CFG_DEVICE_PROC_SERIAL			14	//16byte

#define EEP_IDX_CFG_FIRMWARE_VERSION			30   //5byte

#define EEP_IDX_CFG_FAN_USE_TIME				35   //4byte
#define EEP_IDX_CFG_FAN_CLEAN_SET_TIME			39   //4byte

#define EEP_IDX_CFG_FLAG_FLASH_WR				50   //1byte



#define EEP_PAGE_PARAM1							64
#define EEP_IDX_PARAM_PD_BASE_ADC				(EEP_PAGE_PARAM1 + 4)	//2byte
#define EEP_IDX_PARAM_PD_CH1_ADC_UB				(EEP_PAGE_PARAM1 + 6)	//2byte
#define EEP_IDX_PARAM_PD_CH2_ADC_UB				(EEP_PAGE_PARAM1 + 8)	//2byte
#define EEP_IDX_PARAM_PD_CH3_ADC_UB				(EEP_PAGE_PARAM1 + 10)	//2byte
#define EEP_IDX_PARAM_PD_CH4_ADC_UB				(EEP_PAGE_PARAM1 + 12)	//2byte
#define EEP_IDX_PARAM_PD_CH5_ADC_UB				(EEP_PAGE_PARAM1 + 14)	//2byte
#define EEP_IDX_PARAM_SiPM_ADC_LB				(EEP_PAGE_PARAM1 + 16)	//2byte
#define EEP_IDX_PARAM_SiPM_ADC_UB				(EEP_PAGE_PARAM1 + 18)	//2byte

#define EEP_PAGE_PARAM2							128
#define EEP_IDX_PARAM_PD_DENSITY				(EEP_PAGE_PARAM2 + 4)	//2byte
#define EEP_IDX_PARAM_PD_CH1_CAL				(EEP_PAGE_PARAM2 + 8)	//2byte
#define EEP_IDX_PARAM_PD_CH2_CAL				(EEP_PAGE_PARAM2 + 12)	//2byte
#define EEP_IDX_PARAM_PD_CH3_CAL				(EEP_PAGE_PARAM2 + 16)	//2byte
#define EEP_IDX_PARAM_PD_CH4_CAL				(EEP_PAGE_PARAM2 + 20)	//2byte
#define EEP_IDX_PARAM_PD_CH5_CAL				(EEP_PAGE_PARAM2 + 24)	//2byte
#define EEP_IDX_PARAM_CFU_CAL					(EEP_PAGE_PARAM2 + 28)	//2byte


//#define EEP_IDX_CFG_FAN_RPM_NORMAL_SPD			13  //2byte
//#define EEP_IDX_CFG_LD_MON_NORMAL_ADC_VAL		15  //2byte
//#define EEP_IDX_CFG_HV_CHK_NORMAL_ADC_VAL		17  //2byte
//#define EEP_IDX_CFG_PD_SIG_NORMAL_ADC_VAL		19  //2byte
//#define EEP_IDX_CFG_SiPM_SIG_NORMAL_ADC_VAL		21  //2byte
//#define EEP_IDX_CFG_BIT_PROCESS_COUNT			23	//2byte

//#define EEP_PAGE_PARAM1							64
//#define EEP_IDX_PARAM_PDCH1_ADC_LB				(EEP_PAGE_PARAM1 + 4)	//2byte
//#define EEP_IDX_PARAM_PDCH1_ADC_UB				8	//2byte
//#define EEP_IDX_PARAM_PDCH2_ADC_LB				12	//2byte
//#define EEP_IDX_PARAM_PDCH2_ADC_UB				16	//2byte
//#define EEP_IDX_PARAM_PDCH3_ADC_LB				20	//2byte
//#define EEP_IDX_PARAM_PDCH3_ADC_UB				24	//2byte
//#define EEP_IDX_PARAM_PDCH4_ADC_LB				28	//2byte
//#define EEP_IDX_PARAM_PDCH4_ADC_UB				32	//2byte
//#define EEP_IDX_PARAM_PDCH5_ADC_LB				36	//2byte
//#define EEP_IDX_PARAM_PDCH5_ADC_UB				40	//2byte
//#define EEP_IDX_PARAM_SiPM_ADC_LB				36	//2byte
//#define EEP_IDX_PARAM_SiPM_ADC_UB				40	//2byte


//#define EEP_IDX_PARAM_PDCH_DP_VAL(CH)			((1*CH)+44)	//4byte

//#define EEP_PAGE_PARAM2							3
//#define EEP_IDX_PARAM_PDCH_DPV_VAL(CH)			((1*CH)+4)	//4byte
//#define EEP_IDX_PARAM_PDCH_DENSITY(CH)			((1*CH)+24)	//4byte

//#define EEP_PAGE_PARAM3							4
//#define EEP_IDX_PARAM_PDCH_CAL(CH)				((1*CH)+4)	//4byte
//#define EEP_IDX_PARAM_SIPM_CFU_CAL				((1*CH)+24)	//4byte


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Communication (UART)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define STX	0xC0
#define SPC	0xDB
#define ETX	0xC2

#define CLIENT_REQ_GET	1
#define CLIENT_REQ_SET	2

#define ADDR	0x00
