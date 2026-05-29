/*
 * DS3231 RTC Module
 */
#include "DS3231.h"

uint8_t readRegister(I2C_HandleTypeDef *hi2c, uint8_t reg){
	uint8_t pData = 0;

	// Send command
	HAL_I2C_Master_Transmit(hi2c, DS3231_ADDR_HAL, &reg, 1, 1000);
	// Read answer
	HAL_I2C_Master_Receive(hi2c, DS3231_ADDR_HAL, &pData, 1, 1000);
	return pData;

  /*
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // Send command
  Wire.endTransmission();  // Stop bit

  // Request 1 byte
  Wire.requestFrom(DS3231_ADDR, 1);
  return Wire.read();  // read and return 1 byte
  */
}

uint8_t readBCDRegister(I2C_HandleTypeDef *hi2c, uint8_t reg){
	uint8_t pData = 0;

	// Send command
	HAL_I2C_Master_Transmit(hi2c, DS3231_ADDR_HAL, &reg, 1, 1000);
	// Read answer
	HAL_I2C_Master_Receive(hi2c, DS3231_ADDR_HAL, &pData, 1, 1000);
	return _fromBcd(pData);

  /*
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // send command
  Wire.endTransmission();  // Stop bit

  // Request 1 byte
  Wire.requestFrom(DS3231_ADDR, 1);
  return _fromBcd(Wire.read());  // read and return 1 byte, convert BCD to int
  */
}

void writeRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data){
	uint8_t xData[2];

	xData[0] = reg;  // command
	xData[1] = _toBcd(data);  // data

	// Send command and data
	HAL_I2C_Master_Transmit(hi2c, DS3231_ADDR_HAL, xData, 2, 1000);

  /*
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // send command
  Wire.write(data);  // send data
  Wire.endTransmission();  // Stop bit
  */
}

void writeBCDRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data){
	uint8_t xData[2];

	xData[0] = reg;  // command
	xData[1] = _toBcd(data);  // data

	// Send command and data
	HAL_I2C_Master_Transmit(hi2c, DS3231_ADDR_HAL, xData, 2, 1000);

  /*
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // send command
  Wire.write(_toBcd(data));  // send data, convert int to BCD
  Wire.endTransmission();  // Stop bit
  */
}

uint8_t _toBcd(uint8_t num)
{
	uint8_t bcd = ((num / 10) << 4) + (num % 10);
	return bcd;
}

uint8_t _fromBcd(uint8_t bcd) {
	uint8_t num = (10*((bcd&0xf0) >>4)) + (bcd & 0x0f);
	return num;
}

