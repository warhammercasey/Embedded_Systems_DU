/*
 * MAX7219 LED Matrix
 */

#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

// MAX7219 Registers
#define DECODE_MODE     0x09 
#define INTENSITY       0x0A
#define SCAN_LIMIT      0x0B 
#define SHUTDOWN        0x0C
#define DISPLAY_TEST    0x0F

extern const uint8_t NUMBERS[][10];
extern const int NUMBERS_LEN;

void spiWriteRegister(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t reg, uint8_t value);

#endif /* MAX7219_H */
