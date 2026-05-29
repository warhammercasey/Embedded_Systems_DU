/*
 * WS2812.c
 *
 *  Created on: May 26, 2026
 *      Author: mmurr
 */

#include "WS2812.h"

void ws2812_init(ws2812_t* inst, TIM_HandleTypeDef* htim, uint16_t channel){
	inst->htim = htim;
	inst->channel = channel;
	inst->idx = 0;

	htim->Instance->CR1 = (1 << 3);
	htim->Instance->CR2 = 0;
	htim->Instance->SMCR = 0;
	htim->Instance->DIER = 0x1; // Enable UIE
	htim->Instance->EGR = 0;
	if(channel < 2)
		htim->Instance->CCMR1 = (0b111 << 4) << (channel*8); // PWM mode 2
	else
		htim->Instance->CCMR2 = (0b111 << 4) << ((channel - 2)*8);
	htim->Instance->CCER = (1 << 0); // Output enabled
}

void ws2812_set_color(ws2812_t* inst, uint8_t red, uint8_t green, uint8_t blue){
	for(int i = 0; i < 8; i++){
		inst->rgb_data[i +  0] = ((green >> (7 - i)) & 0x1) ? (WS2812_1) : (WS2812_0);
		inst->rgb_data[i +  8] = ((red   >> (7 - i)) & 0x1) ? (WS2812_1) : (WS2812_0);
		inst->rgb_data[i + 16] = ((blue  >> (7 - i)) & 0x1) ? (WS2812_1) : (WS2812_0);
	}

	// Set period
	(&(inst->htim->Instance->CCR1))[inst->channel] = inst->htim->Instance->ARR - inst->rgb_data[0];
	inst->idx = 1;

	inst->htim->Instance->CR1 |= 1; // Start timer
}

void ws2812_callback(ws2812_t* inst){
	if(inst->idx < 24){
		(&(inst->htim->Instance->CCR1))[inst->channel] = inst->htim->Instance->ARR - inst->rgb_data[(inst->idx)++];
		inst->htim->Instance->CR1 |= 1; // Start timer

	}else{
		inst->idx = 0;
	}
}
