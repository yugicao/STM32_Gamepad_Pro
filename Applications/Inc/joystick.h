#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "main.h"

#define JOYSTICK_X_MINIMUM_IDLE_DIRECTION 10
#define JOYSTICK_X_MAXIMUN_IDLE_DIRECTION 90
#define JOYSTICK_Y_MINIMUM_IDLE_DIRECTION 10
#define JOYSTICK_Y_MAXIMUN_IDLE_DIRECTION 90

typedef struct{
	uint32_t joystick_x;
	uint32_t joystick_y;
	uint32_t adc_value[2];
	uint8_t joystick_position[4]; /* index0: right, index1: left, index2: up; index3: down */
}Joystick_HandleTypeDef;

void joystick_handler(Joystick_HandleTypeDef* hjoy);

void joystick_init(ADC_HandleTypeDef* hadc, Joystick_HandleTypeDef* hjoy);

#endif

