/*
 * WS2812.h
 *
 *  Created on: May 26, 2026
 *      Author: mmurr
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

#include "main.h"

typedef struct ws2812_t {
	TIM_HandleTypeDef* htim;
	uint16_t channel;
	uint16_t rgb_data[24];
	uint32_t idx;
} ws2812_t;

void ws2812_init(ws2812_t* inst, TIM_HandleTypeDef* htim, uint16_t channel);

void ws2812_set_color(ws2812_t* inst, uint8_t red, uint8_t green, uint8_t blue);

void ws2812_callback(ws2812_t* inst);


#endif /* INC_WS2812_H_ */
