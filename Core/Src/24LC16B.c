
#include <stdio.h>
#include <stdint.h>

#include <Const.h>
//#include "stm32l4xx_hal_i2c.h"
//#include "stm32l4xx_hal_rcc.h"
//#include "stm32l4xx_hal_gpio.h"

#include "24LC16B.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
//#include "stm32l4xx_hal_def.h"

//--------------------------------Device Define-------------------- S
#define DEVICE_ADDR 0xa0
#define RD_BIT	0x02
#define WR_BIT	0x00
//--------------------------------Device Define-------------------- E


#define EE24_PSIZE 32

void I2C_Delay(uint32_t Delay);
void I2C_Lock(void);
void I2C_UnLock(void);


static bool I2C_EEP_Write(uint32_t MemAddress, uint8_t Data);
static uint8_t I2C_EEP_Read(uint32_t Address);

typedef struct
{
  I2C_HandleTypeDef*     HI2c;
  uint8_t                Page;
  uint8_t                Address;
  uint8_t                Lock;

} EEP_HandleTypeDef;

EEP_HandleTypeDef I2C_HI2C;


//EEPROM Init
bool I2C_EEP_Init(I2C_HandleTypeDef *HI2c, uint8_t I2CAddress)
{
	bool answer = false;

	I2C_HI2C.HI2c = HI2c;
	I2C_HI2C.Address = I2CAddress;

	HAL_GPIO_WritePin(PROM_WP_GPIO_Port, PROM_WP_Pin, GPIO_PIN_SET);

	if (HAL_I2C_IsDeviceReady(I2C_HI2C.HI2c, I2C_HI2C.Address , 2, 100) == HAL_OK)
	{
		answer = true;
	}
	return answer;
}

uint8_t I2C_EEP_Read(uint32_t MemAddress)
{
	uint8_t result[2] = {0,};

	HAL_GPIO_WritePin(PROM_WP_GPIO_Port, PROM_WP_Pin, GPIO_PIN_SET);
	//HAL_Delay(10);
	while(HAL_I2C_IsDeviceReady(I2C_HI2C.HI2c, I2C_HI2C.Address, 1, HAL_MAX_DELAY) != HAL_OK);

	HAL_I2C_Mem_Read(I2C_HI2C.HI2c, I2C_HI2C.Address, MemAddress, I2C_MEMADD_SIZE_8BIT, result, 1, HAL_MAX_DELAY);
	//HAL_Delay(10);
	return result[0];
}

bool I2C_EEP_Write(uint32_t MemAddress, uint8_t Data)
{
	bool answer = false;
	uint8_t result[2] = {Data,};
	HAL_GPIO_WritePin(PROM_WP_GPIO_Port, PROM_WP_Pin, GPIO_PIN_RESET);
	//HAL_Delay(10);
	while(HAL_I2C_IsDeviceReady(I2C_HI2C.HI2c, I2C_HI2C.Address, 1, HAL_MAX_DELAY) != HAL_OK);

	HAL_I2C_Mem_Write(I2C_HI2C.HI2c, I2C_HI2C.Address, MemAddress, I2C_MEMADD_SIZE_8BIT, result, 1, HAL_MAX_DELAY);
	//HAL_Delay(10);
	HAL_GPIO_WritePin(PROM_WP_GPIO_Port, PROM_WP_Pin, GPIO_PIN_SET);
	//HAL_Delay(10);
	return answer;
}

//EEPROM Read
/*
uint8_t  I2C_EEP_Write(uint8_t Page, uint8_t Idx, uint8_t data)
{
	uint8_t* rbuf[2] = {0,data};
	HAL_GPIO_WritePin(PROM_WP_GPIO_Port, PROM_WP_Pin, GPIO_PIN_RESET);
	HAL_Delay(15);
	HAL_I2C_Mem_Write(&hI2C1, DEVICE_ADDR, Idx, I2C_MEMADD_SIZE_8BIT, &data, Page, HAL_MAX_DELAY);
	HAL_Delay(15);

	//while(HAL_I2C_IsDeviceReady(&hI2C1, DEVICE_ADDR, 1, HAL_MAX_DELAY) != HAL_OK);

	HAL_GPIO_WritePin(PROM_WP_GPIO_Port, PROM_WP_Pin, GPIO_PIN_SET);
}

//EEPROM Wirte

uint8_t  I2C_EEP_Read(uint8_t Page, uint8_t Idx)
{
	uint8_t rbuf = 0;
	HAL_I2C_Mem_Read(&hI2C1, DEVICE_ADDR, Idx, I2C_MEMADD_SIZE_8BIT, &rbuf, Page, HAL_MAX_DELAY);//(hI2C1, DEVICE_ADDR, Idx, I2C_MEMADD_SIZE_16BIT, rbuf, Page, 1000);
	return rbuf;
}
*/
uint8_t I2C_EEP_READ_U08 (uint32_t Idx)
{
	uint8_t result = 0;
	result = I2C_EEP_Read(Idx);
	return result;
}

uint16_t I2C_EEP_READ_U16(uint32_t Idx)
{
	uint16_t result = 0;

	result =  (uint16_t)(I2C_EEP_Read(Idx) << 8) ;
	result |= (uint16_t)(I2C_EEP_Read(Idx+1));

	return result;
}

uint32_t I2C_EEP_READ_U32(uint32_t Idx)
{
	uint32_t result = 0;

	result  = (uint32_t)(I2C_EEP_Read(Idx)   << 24);
	result |= (uint32_t)(I2C_EEP_Read(Idx+1) << 16);
	result |= (uint32_t)(I2C_EEP_Read(Idx+2) << 8);
	result |= (uint32_t)(I2C_EEP_Read(Idx+3));

	return result;
}

float I2C_EEP_READ_FLOAT (uint32_t Idx)
{
	uint32_t result  = 0;

	result  = (uint32_t)(I2C_EEP_Read(Idx)   << 24);
	result |= (uint32_t)(I2C_EEP_Read(Idx+1) << 16);
	result |= (uint32_t)(I2C_EEP_Read(Idx+2) << 8);
	result |= (uint32_t)(I2C_EEP_Read(Idx+3));


	return *((float*)&result);
}

uint8_t I2C_EEP_WRITE_U08 (uint32_t Idx, uint8_t nData)
{

	uint8_t ret1 		= 0;
	uint8_t ret2 		= 0;

	uint8_t nVerifyOk 	= FAULT;

	ret1 = I2C_EEP_Write(Idx, nData);
	ret2 = I2C_EEP_Read (Idx);

	if(ret1 == ret2) nVerifyOk = OK;

	return nVerifyOk;

}

uint8_t I2C_EEP_WRITE_U16(uint32_t Idx, uint16_t nData)
{
	uint16_t ret1 		= 0;
	uint8_t nVerifyOk 	= FAULT;


	I2C_EEP_Write(Idx  , ((nData >>  8)  & 0xFF)	);
	I2C_EEP_Write(Idx+1, ((nData      )  & 0xFF) 	);

	ret1  = (uint16_t) (I2C_EEP_Read(Idx) << 8);
	ret1 |= (uint16_t) (I2C_EEP_Read(Idx+1) 	 );


	if(ret1 == nData) nVerifyOk = OK;

	return nVerifyOk;
}

uint8_t I2C_EEP_WRITE_U32(uint32_t Idx, uint32_t nData)
{
	uint32_t ret1 		= 0;
	uint8_t nVerifyOk 	= FAULT;

	I2C_EEP_Write(Idx,   ((nData >> 24)  & 0xFF) 	);
	I2C_EEP_Write(Idx+1, ((nData >> 16)  & 0xFF) 	);
	I2C_EEP_Write(Idx+2, ((nData >>  8)  & 0xFF)	);
	I2C_EEP_Write(Idx+3, ((nData      )  & 0xFF) 	);

	ret1  = (uint32_t) (I2C_EEP_Read (Idx  ) << 24);
	ret1 |= (uint32_t) (I2C_EEP_Read (Idx+1) << 16);
	ret1 |= (uint32_t) (I2C_EEP_Read (Idx+2) <<  8);
	ret1 |= (uint32_t) (I2C_EEP_Read (Idx+3)      );

	if(ret1 == nData) nVerifyOk = OK;

	return nVerifyOk;
}

uint8_t I2C_EEP_WRITE_FLOAT (uint32_t Idx, float fData)
{
	uint32_t nRelt = *((uint32_t*)&fData);
	uint32_t ret1 		= 0;

	uint8_t nVerifyOk 	= FAULT;

	I2C_EEP_Write(Idx,   ((nRelt >> 24)  & 0xFF) 	);
	I2C_EEP_Write(Idx+1, ((nRelt >> 16)  & 0xFF) 	);
	I2C_EEP_Write(Idx+2, ((nRelt >>  8)  & 0xFF)	);
	I2C_EEP_Write(Idx+3, ((nRelt      )  & 0xFF) 	);

	ret1  = (uint32_t) (I2C_EEP_Read (Idx  ) << 24);
	ret1 |= (uint32_t) (I2C_EEP_Read (Idx+1) << 16);
	ret1 |= (uint32_t) (I2C_EEP_Read (Idx+2) <<  8);
	ret1 |= (uint32_t) (I2C_EEP_Read (Idx+3)      );

	if(ret1 == nRelt) nVerifyOk = OK;

	return nVerifyOk;
}

