#ifndef __UART_HANDLER_H__
#define __UART_HANDLER_H__

#include "main.h"
#include "stdbool.h"

#define COMM_HUART huart1


#define UART_DATA_MAXLENGTH 64
#define CHECKSUM_LEN 1
#define SSID_DATA_FUNC    0x00
#define NEW_CONFIG_FUNC   0x01
typedef struct
{
	uint8_t length;
	uint8_t data[UART_DATA_MAXLENGTH];
	bool transfer_complete;
	uint8_t rxTemp;
	uint8_t rxSta;
}__attribute__((__packed__))Uart_Comm_Handle_t;

HAL_StatusTypeDef UH_GetCommData(uint8_t *func, uint8_t *data, uint8_t* data_length);
HAL_StatusTypeDef UH_SendACK(void);
HAL_StatusTypeDef UH_SendNACK(void);
void UH_Init(void);


#endif
