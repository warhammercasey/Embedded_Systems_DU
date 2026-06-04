/*
 * motor.c
 *
 *  Created on: May 26, 2026
 *      Author: mmurr
 */

#include "motor.h"

#define ABS(x) ((x > 0) ? (x) : (-x))

void motor_init(motor_t* inst, GPIO_TypeDef* port_a, uint16_t pin_a, GPIO_TypeDef* port_b, uint16_t pin_b, TIM_HandleTypeDef* htim, uint16_t channel){
	inst->dir_pins[0].port = port_a;
	inst->dir_pins[0].pin = pin_a;

	inst->dir_pins[1].port = port_b;
	inst->dir_pins[1].pin = pin_b;

	inst->htim = htim;
	inst->channel = channel;

	inst->speed = 0;

	uint32_t channels[] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};
	HAL_TIM_PWM_Start(htim, channels[channel]);
	set_speed(inst, 0);
}

void set_speed(motor_t* inst, int32_t speed){
	inst->speed = speed;
	if(speed == 0){
		HAL_GPIO_WritePin(inst->dir_pins[0].port, inst->dir_pins[0].pin, 1);
		HAL_GPIO_WritePin(inst->dir_pins[1].port, inst->dir_pins[1].pin, 1);
		(&(inst->htim->Instance->CCR1))[inst->channel] = 0;

	} else{
		uint8_t dir_pin = speed < 0;
		HAL_GPIO_WritePin(inst->dir_pins[dir_pin].port, inst->dir_pins[dir_pin].pin, 1);
		HAL_GPIO_WritePin(inst->dir_pins[!dir_pin].port, inst->dir_pins[!dir_pin].pin, 0);

		(&(inst->htim->Instance->CCR1))[inst->channel] = ((uint32_t)ABS(speed));

	}
}
