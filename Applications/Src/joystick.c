
#include "joystick.h"

//static uint32_t joystick_x, joystick_y, adc_value[2];

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
//	joystick_x = MAP(adc_value[0], 0, 4096, 0, 100); //2^12 bit
//	joystick_y = MAP(adc_value[1], 0, 4096, 0, 100); // = 4096
//}


__weak void joystick_right_push_cb(Joystick_HandleTypeDef* hjoy)
{
}
__weak void joystick_right_release_cb(Joystick_HandleTypeDef* hjoy)
{
}
__weak void joystick_left_push_cb(Joystick_HandleTypeDef* hjoy)
{
}
__weak void joystick_left_release_cb(Joystick_HandleTypeDef* hjoy)
{
}
__weak void joystick_up_push_cb(Joystick_HandleTypeDef* hjoy)
{
}
__weak void joystick_up_release_cb(Joystick_HandleTypeDef* hjoy)
{
}
__weak void joystick_down_push_cb(Joystick_HandleTypeDef* hjoy)
{
}
__weak void joystick_down_release_cb(Joystick_HandleTypeDef* hjoy)
{
}

static uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax);


void joystick_handler(Joystick_HandleTypeDef* hjoy)
{
	hjoy->joystick_x = MAP(hjoy->adc_value[0], 0, 4096, 0, 100); //2^12 bit
	hjoy->joystick_y = MAP(hjoy->adc_value[1], 0, 4096, 0, 100); // = 4096

	if(hjoy->joystick_position[0])
	{
		if(hjoy->joystick_x < JOYSTICK_X_MAXIMUN_IDLE_DIRECTION)
		{
			hjoy->joystick_position[0] = 0;
			joystick_right_release_cb(hjoy);
		}
	}
	else
	{
		if(hjoy->joystick_x > JOYSTICK_X_MAXIMUN_IDLE_DIRECTION)
		{
			hjoy->joystick_position[0] = 1;
			joystick_right_push_cb(hjoy);
		}
	}



	if(hjoy->joystick_position[1])
	{
		if(hjoy->joystick_x > JOYSTICK_X_MINIMUM_IDLE_DIRECTION)
		{
			hjoy->joystick_position[1] = 0;
			joystick_left_release_cb(hjoy);
		}
	}
	else
	{
		if(hjoy->joystick_x < JOYSTICK_X_MINIMUM_IDLE_DIRECTION)
		{
			hjoy->joystick_position[1] = 1;
			joystick_left_push_cb(hjoy);
		}
	}



	if(hjoy->joystick_position[2])
	{
		if(hjoy->joystick_y > JOYSTICK_Y_MINIMUM_IDLE_DIRECTION)
		{
			hjoy->joystick_position[2] = 0;
			joystick_up_release_cb(hjoy);
		}
	}
	else
	{
		if(hjoy->joystick_y < JOYSTICK_Y_MINIMUM_IDLE_DIRECTION)
		{
			hjoy->joystick_position[2] = 1;
			joystick_up_push_cb(hjoy);
		}
	}



	if(hjoy->joystick_position[3])
	{
		if(hjoy->joystick_y < JOYSTICK_Y_MAXIMUN_IDLE_DIRECTION)
		{
			hjoy->joystick_position[3] = 0;
			joystick_down_release_cb(hjoy);
		}
	}
	else
	{
		if(hjoy->joystick_y > JOYSTICK_Y_MAXIMUN_IDLE_DIRECTION)
		{
			hjoy->joystick_position[3] = 1;
			joystick_down_push_cb(hjoy);
		}
	}

}

void joystick_init(ADC_HandleTypeDef* hadc, Joystick_HandleTypeDef* hjoy)
{
	for(uint8_t i = 0; i < 4; ++i)
	{
		hjoy->joystick_position[i] = 0;
	}
	hjoy->joystick_x = 0;
	hjoy->joystick_y = 0;
	HAL_ADC_Start_DMA(hadc, (uint32_t *)hjoy->adc_value, 2);
}

static uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax)
{
	return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}

