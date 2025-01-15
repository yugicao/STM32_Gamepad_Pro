#include "button.h"

__weak void button_time_out_release_handler_cb(Button_Typedef *ButtonX)
{
}
__weak void button_time_out_press_handler_cb(Button_Typedef *ButtonX)
{
}
__weak void button_short_press_handler_cb(Button_Typedef *ButtonX)
{
}
__weak void button_press_handler_cb(Button_Typedef *ButtonX)
{
}
__weak void button_release_handler_cb(Button_Typedef *ButtonX)
{
}
void button_handler(Button_Typedef *ButtonX)
{ 
	uint8_t sta_button = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);
	if( sta_button != ButtonX->filter_sta_button)
	{
		ButtonX->filter_sta_button = sta_button;
		ButtonX->is_deboucing = 1;
		ButtonX->preTick = HAL_GetTick();
	}
	if((HAL_GetTick() - ButtonX->preTick >= 15) && ButtonX->is_deboucing)
	{
		ButtonX->current_sta_button = ButtonX->filter_sta_button;
		ButtonX->is_deboucing = 0;
	}
	
	if(ButtonX->current_sta_button != ButtonX->pre_sta_button)
	{
		if(ButtonX->current_sta_button == 0)
		{
			ButtonX->pressTick = HAL_GetTick();
			ButtonX->isTimeoutPressCb = 1;
			button_press_handler_cb(ButtonX);
		}
		else
		{
			ButtonX->isTimeoutPressCb = 0;
			if(HAL_GetTick() - ButtonX->pressTick <= SHORT_PRESS_DURATION)
				button_short_press_handler_cb(ButtonX);
			
			button_release_handler_cb(ButtonX);
			if(HAL_GetTick() - ButtonX->pressTick >= TIMEOUT_RELEASE_DURATION)
				button_time_out_release_handler_cb(ButtonX);
		}
		ButtonX->pre_sta_button = ButtonX->current_sta_button;
	}

	if(HAL_GetTick()- ButtonX->pressTick >= TIMEOUT_PRESS_DURATION && ButtonX->isTimeoutPressCb)
	{
		ButtonX->isTimeoutPressCb = 0;
		button_time_out_press_handler_cb(ButtonX);
	}
}
void button_init(Button_Typedef *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	ButtonX->current_sta_button = 1;
	ButtonX->filter_sta_button = 1;
	ButtonX->pre_sta_button = 1;
	ButtonX->isTimeoutPressCb = 0;
	ButtonX->GPIOx = GPIOx;
	ButtonX->GPIO_Pin = GPIO_Pin;
}
