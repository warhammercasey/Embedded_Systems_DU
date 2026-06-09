/*
 * motor.h
 *
 *  Created on: May 26, 2026
 *      Author: mmurr
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

typedef struct pin_t {
	GPIO_TypeDef* port;
	uint16_t pin;
} pin_t;

typedef struct motor_t {
	pin_t dir_pins[2];
	TIM_HandleTypeDef* htim;
	uint16_t channel;
	int32_t speed;
	uint8_t inverted;
} motor_t;

void motor_init(motor_t* inst, GPIO_TypeDef* port_a, uint16_t pin_a, GPIO_TypeDef* port_b, uint16_t pin_b, TIM_HandleTypeDef* htim, uint16_t channel, uint8_t inverted);

void set_speed(motor_t* inst, int32_t speed);

#endif /* INC_MOTOR_H_ */
