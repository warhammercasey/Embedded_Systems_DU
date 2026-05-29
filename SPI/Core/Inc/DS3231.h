/*
 * DS3231 RTC Module
 */

#ifndef DS3231_H
#define DS3231_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

// RTC Registers
#define DS3231_ADDR             0x68  ///< I2C address for DS3231
#define DS3231_ADDR_HAL         0xD1  // HAL expects left-shifted address
//#define DS3231_WRITE            0xD0

#define DS3231_SECONDS          0x00  
#define DS3231_MINUTES          0x01  
#define DS3231_HOURS            0x02  
#define DS3231_DAY              0x03  
#define DS3231_DATE             0x04  
#define DS3231_CEN_MONTH        0x05
#define DS3231_DEC_YEAR         0x06
#define DS3231_ALARM1_SECONDS   0x07
#define DS3231_ALARM1_MINUTES   0x08
#define DS3231_ALARM1_HOURS     0x09
#define DS3231_ALARM1_DAY_DATE  0x0A
#define DS3231_ALARM2_MINUTES   0x0B
#define DS3231_ALARM2_HOURS     0x0C
#define DS3231_ALARM2_DAY_DATE  0x0D
#define DS3231_CONTROL          0x0E
#define DS3231_CTL_STATUS       0x0F
#define DS3231_AGING_OFFSET     0x10
#define DS3231_TEMP_MSB         0x11
#define DS3231_TEMP_LSB         0x12

typedef struct{
	uint8_t seconds;  	// 0-59
	uint8_t minutes;  	// 0-59
	uint8_t hours;		// 0-23 (24h format)
	uint8_t date;		// 1-31 (day of the month)
	uint8_t month;		// 1-12
	uint8_t year;		// 0-99 (represents 2000-2099)
}DS3231_Time_t;

uint8_t readRegister(I2C_HandleTypeDef *hi2c, uint8_t reg);
uint8_t readBCDRegister(I2C_HandleTypeDef *hi2c, uint8_t reg);
void writeRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data);
void writeBCDRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data);
uint8_t _toBcd(uint8_t num);
uint8_t _fromBcd(uint8_t bcd);

#endif /* DS3231_H */
