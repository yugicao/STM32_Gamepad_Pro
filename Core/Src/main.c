/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "ssd1306.h"
//#include "fonts.h"
#include "stdio.h"
//#include "pong.h"
#include "button.h"
#include "menu_handle.h"
#include "UartHandler.h"
#include "saved_configs.h"
#include "joystick.h"
#include "keyboard.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATA_LEN 32

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t pre_usb_state = USBD_STATE_DEFAULT;
Button_Typedef btn_up;
Button_Typedef btn_down;
Button_Typedef btn_right;
Button_Typedef btn_left;
Button_Typedef btn_joystick;
Joystick_HandleTypeDef hjoystick;
typedef enum{
	STATE_DISPLAY_MENU,
	STATE_CONNECT,
}State_t;

State_t state = STATE_DISPLAY_MENU;

//uint32_t joystick_x, joystick_y, adc_value[2];

uint8_t SSID[DATA_LEN] = "SEARCHING...";

uint8_t func, len, data[DATA_LEN];

uint32_t t_blink = 0;

GamepadConfig_t config = {
		.mode = 0x00,
		.joystickButtonID = 0x00,
		.button1ID = 0x0D,
		.button2ID = 0x0E,
		.button3ID = 0x0F,
		.button4ID = 0x18,
		.joystickUpID = W_USAGE_ID,
		.joystickDownID = S_USAGE_ID,
		.joystickRightID = D_USAGE_ID,
		.joystickLeftID = A_USAGE_ID,
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void state_handle(void);
void pong_game_run(void);
//uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Blink_LED(void)
{
	t_blink = HAL_GetTick();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
}

void LED_handle(void)
{
	if (HAL_GetTick() - t_blink > 100)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
	}
}

//void uart_data_handle(void)
//{
//	HAL_StatusTypeDef ret = UH_GetCommData(&func, data, &len);
//	if(ret == HAL_OK)
//	{
//		Blink_LED();
//		UH_SendACK();
//		switch(func)
//		{
//			case SSID_DATA_FUNC:
//				memset(SSID, 0, DATA_LEN);
//				memcpy(SSID, data, sizeof(data));
//				switch(state)
//				{
//					case STATE_DISPLAY_MENU:
//						display_updated_wifi();
//						break;
//					case STATE_CONNECT:
//					{
//						display_connect_state(pre_usb_state);
//						break;
//					}
//					default: break;
//				}
//				break;
//			case NEW_CONFIG_FUNC:
//				write_config((GamepadConfig_t*)data);
//				break;
//			default: break;
//		}
//	}
//	else if(ret == HAL_ERROR)
//	{
//		UH_SendNACK();
//	}
//}

void uart_data_handle(void)
{
	HAL_StatusTypeDef ret = UH_GetCommData(&func, data, &len);
	if (ret == HAL_OK)
	{
		Blink_LED();
		if (UH_SendACK() != HAL_OK) {
		}
		switch (func)
		{
			case SSID_DATA_FUNC:
				memset(SSID, 0, DATA_LEN);
				memcpy(SSID, data, len < DATA_LEN ? len : DATA_LEN);
				if (state == STATE_DISPLAY_MENU)
				{
					display_updated_wifi();
				}
				else if (state == STATE_CONNECT)
				{
					display_connect_state(pre_usb_state);
				}
				break;

			case NEW_CONFIG_FUNC:
				write_config((GamepadConfig_t*)data);
				break;
			default: break;
		}
	}
	else if (ret == HAL_ERROR)
	{
		UH_SendNACK();
	}
}

void button_time_out_press_handler_cb(Button_Typedef *ButtonX)
{
}
void button_short_press_handler_cb(Button_Typedef *ButtonX)
{
	if(ButtonX == &btn_up)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				menu_click_up();
				break;
			case STATE_CONNECT:
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_down)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				menu_click_down();
				break;
			case STATE_CONNECT:
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_joystick)
	{
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
	if(ButtonX == &btn_left)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				menu_click_back();
				break;
			case STATE_CONNECT:
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_right)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				menu_click_select();
				break;
			case STATE_CONNECT:
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_joystick)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				if(config.joystickButtonID == 0x00)
				{
					menu_reset();
					state = STATE_DISPLAY_MENU;
				}
				break;
			default: break;
		}
	}
}
void button_press_handler_cb(Button_Typedef *ButtonX)
{
	if(ButtonX == &btn_up)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				pressKey(config.button1ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_down)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				pressKey(config.button2ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_left)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				pressKey(config.button3ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_right)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				pressKey(config.button4ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_joystick)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				break;
			default: break;
		}
	}
}

void button_release_handler_cb(Button_Typedef *ButtonX)
{
	if(ButtonX == &btn_up)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				releaseKey(config.button1ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_down)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				releaseKey(config.button2ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_left)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				releaseKey(config.button3ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_right)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				releaseKey(config.button4ID);
				break;
			default: break;
		}
	}
	if(ButtonX == &btn_joystick)
	{
		switch(state)
		{
			case STATE_DISPLAY_MENU:
				break;
			case STATE_CONNECT:
				break;
			default: break;
		}
	}
}

void connect_mode_callback(void)
{
	display_connect_state(pre_usb_state);
	state = STATE_CONNECT;
}

void connect_interface_handle(void)
{
	if (hUsbDeviceFS.dev_state != pre_usb_state)
	{
		pre_usb_state = hUsbDeviceFS.dev_state;
		display_connect_state(pre_usb_state);
	}
}


void joystick_right_push_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			button_short_press_handler_cb(&btn_right);
			break;
		case STATE_CONNECT:
			pressKey(config.joystickRightID);
			break;
		default: break;
	}
}
void joystick_right_release_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			break;
		case STATE_CONNECT:
			releaseKey(config.joystickRightID);
			break;
		default: break;
	}
}
void joystick_left_push_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			button_short_press_handler_cb(&btn_left);
			break;
		case STATE_CONNECT:
			pressKey(config.joystickLeftID);
			break;
		default: break;
	}
}
void joystick_left_release_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			break;
		case STATE_CONNECT:
			releaseKey(config.joystickLeftID);
			break;
		default: break;
	}
}
void joystick_up_push_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			button_short_press_handler_cb(&btn_up);
			break;
		case STATE_CONNECT:
			pressKey(config.joystickUpID);
			break;
		default: break;
	}
}
void joystick_up_release_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			break;
		case STATE_CONNECT:
			releaseKey(config.joystickUpID);
			break;
		default: break;
	}
}
void joystick_down_push_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			button_short_press_handler_cb(&btn_down);
			break;
		case STATE_CONNECT:
			pressKey(config.joystickDownID);
			break;
		default: break;
	}
}
void joystick_down_release_cb(Joystick_HandleTypeDef* hjoy)
{
	switch(state)
	{
		case STATE_DISPLAY_MENU:
			break;
		case STATE_CONNECT:
			releaseKey(config.joystickDownID);
			break;
		default: break;
	}
}

void state_handle(void)
{
	button_handler(&btn_up);
	button_handler(&btn_down);
	button_handler(&btn_right);
	button_handler(&btn_left);
	button_handler(&btn_joystick);
	joystick_handler(&hjoystick);
	uart_data_handle();
	LED_handle();

	switch(state)
	{
		case STATE_DISPLAY_MENU:
			break;
		case STATE_CONNECT:
			connect_interface_handle();
			break;
		default: break;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

//  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value, 2);

  button_init(&btn_up, GPIOB, GPIO_PIN_14);
  button_init(&btn_down, GPIOB, GPIO_PIN_15);
  button_init(&btn_left, GPIOB, GPIO_PIN_13);
  button_init(&btn_right, GPIOB, GPIO_PIN_12);
  button_init(&btn_joystick, GPIOA, GPIO_PIN_3);
  joystick_init(&hadc1, &hjoystick);
//  SSD1306_Init (); // initialize the display
  UH_Init();
  menu_init();

//  SSD1306_GotoXY (0,0);
//  SSD1306_Puts ("SSID: ", &Font_7x10, 1);
//  SSD1306_GotoXY (0, 20);
//  SSD1306_Puts ("CONNECT", &Font_7x10, 1);
//  SSD1306_UpdateScreen();
//  HAL_Delay (1000);
//
//  SSD1306_ScrollRight(1, 3);
//  HAL_Delay(3000);

//  SSD1306_ScrollLeft(0,1);
//  HAL_Delay(3000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  state_handle();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
