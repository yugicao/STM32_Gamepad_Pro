#ifndef __BUTTON_H
#define __BUTTON_H

#include "main.h"

#define SHORT_PRESS_DURATION 		1000
#define TIMEOUT_PRESS_DURATION		3000
#define TIMEOUT_RELEASE_DURATION	3000

typedef struct{
	uint8_t current_sta_button;
	uint8_t pre_sta_button;
	uint8_t filter_sta_button;
	uint8_t is_deboucing;
	uint32_t preTick;
	uint32_t pressTick;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	uint8_t isTimeoutPressCb;
}Button_Typedef;
void button_handler(Button_Typedef *ButtonX);
void button_init(Button_Typedef *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
#endif
