#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

struct _tSYS_STAT
{
	uint8_t 	LDAdcChkOn;			//1byte
	uint8_t 	PDAdcChkOn;			//1byte
	uint8_t 	HVAdcChkOn;			//1byte
	uint8_t 	SiPmAdcChkOn;		//1byte

	uint8_t 	BitChkOn;			//1byte
	uint8_t		flagBitErrOn;		//1byte

	uint8_t		SelGetAdcCh;		//1byte
	uint8_t		FlagGetAdcOn;		//1byte
	uint8_t		FlagLoopOn;		//1byte

	uint8_t 	FlagAtCleanOn;		//1byte
	uint8_t		FlagSendPmResultOn;		//1byte
	uint8_t		FlagOperModeOn;		//1byte
	uint8_t		FlagSleepModeOn;	//1byte

	uint8_t		FlagStatClenaOn;	//1byte

	uint8_t		ClientReqDevInfo;
	//uint8_t

	uint8_t		ClientReqSelCh;		//1byte
	uint8_t		ClientReqType;		//1byte
	uint8_t		ClinetReqMode;		//1byte

	uint8_t		FlagBitChkOn;		//1byte

	uint32_t	AtCleanSec;			//4byte
	uint16_t	Temp;				//4byte

	uint8_t 	SelCh;				//1byte
	uint8_t		BitState;			//1byte
	uint16_t 	DeviceState;		//1byte
	uint8_t		ErrRetry;			//1byte
};

struct _tSYS_TIME
{
	uint8_t	 FlagSysTickOn;
	uint8_t	 FlagSysChkOn;
	uint8_t	 Flag100mSecTickOn;
	uint8_t	 Flag1SecTickOn;

	uint32_t SysTickTime;		//10msec
	uint32_t CurrAtCleanSec;

	uint8_t  FlagWChkTimeOn;
	uint8_t  RpmChkTime;

	uint8_t		Sys10mSecTick;
	uint16_t	Sys100mSecTick;
	uint16_t	Sys1SecTick;

	// CHK TIME
	uint8_t  FlagWaitSecTimeOn;
	uint32_t  WaitSecTime;

	uint8_t  FlagWaitmSecTimeOn;
	uint32_t  WaitmSecTime;

	uint32_t WaitChkCnt;

};

struct _tPM_RESULT1{
	float		fMass_Concet;
	float		fTmpMassConcet;
	float		C;
	float		M;
	float		fNum_Concet;
	uint32_t	nTmpNumConcet;
	float		fTps;

	uint16_t	nPdChAdc;
};

struct _tPM_RESULT2{
	float		fCFU;
	float		fNumConcetBio;
	float		fResultTps;
	float		fReverved;
};


extern volatile uint16_t Adc_Temp[16];
extern struct _tPM_RESULT1 tPmResult1[PM_CH_MAX];
extern struct _tPM_RESULT2 tPmResult2;

extern struct _tSYS_STAT	tSysState;
extern struct _tSYS_TIME	tSysTime;
