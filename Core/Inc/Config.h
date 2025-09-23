
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct _tCONFIG{
	unsigned char 	prodtype[8];		//8byte
	unsigned char 	prodSerial[16];		//16byte
	unsigned char	FirmwareVer[5];		//4byte

	unsigned long	FanCleanSetTime;    //4byte
	unsigned long	DeviceUseCount;    //4byte
	unsigned int	FanRpmNormalSpd;	//2byte
	unsigned int	LdMonNormalAdc;		//2byte
	unsigned int	HvChkNormalAdc;		//2byte
	unsigned int	PdSigNormalAdc;		//2byte
	unsigned int	SiPmSigNormalAdc;	//2byte
	unsigned int	BITProcCount;		//2byte

	unsigned char	FlagFlashWROn;		//1byte
};

extern struct _tCONFIG		tConfig;

extern void CONFIG_Init(void);

