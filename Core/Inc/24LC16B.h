
#include <stm32l4xx.h>
#include <stdint.h>
#include <stdbool.h>

#define I2C_TIMEOUT_MAX 100000

//extern I2C_HandleTypeDef hi2c1;
//#define EEPROM_I2C &hi2c1

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0



// Define the Page Size and number of pages
#define PAGE_SIZE 64     // in Bytes
#define PAGE_NUM  512    // number of pages

extern bool I2C_EEP_Init(I2C_HandleTypeDef *HI2c, uint8_t I2CAddress);



extern uint8_t I2C_EEP_READ_U08 (uint32_t Idx);
extern uint16_t I2C_EEP_READ_U16(uint32_t Idx);
extern uint32_t I2C_EEP_READ_U32(uint32_t Idx);
extern float I2C_EEP_READ_FLOAT (uint32_t Idx);

extern uint8_t I2C_EEP_WRITE_U08 (uint32_t Idx, uint8_t nData);
extern uint8_t I2C_EEP_WRITE_U16(uint32_t Idx, uint16_t nData);
extern uint8_t I2C_EEP_WRITE_U32(uint32_t Idx, uint32_t nData);
extern uint8_t I2C_EEP_WRITE_FLOAT(uint32_t Idx, float fData);

