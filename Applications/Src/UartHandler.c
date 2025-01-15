#include "UartHandler.h"
#include "string.h"

Uart_Comm_Handle_t UartComm;
const uint8_t ACK = 0x06, NACK = 0x15;

static uint8_t Checksum(uint8_t *data, uint8_t len);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(UartComm.transfer_complete == 0 && huart->Instance == COMM_HUART.Instance)
	{
		// Frame: 0x55 0xAA FUNC(1byte) LEN(1byte) DATA(Depend on LEN) CS(1byte)
		switch(UartComm.rxSta)
		{
				case 0:
					if(UartComm.rxTemp == 0x55) UartComm.rxSta = 1;
					break;
				case 1:
					UartComm.rxSta =(UartComm.rxTemp == 0xAA)?2:0;
					break;
				case 2:
					UartComm.data[0] = UartComm.rxTemp;
					UartComm.rxSta = 3;
//					UartComm.length++; // added
					break;
				case 3:
					UartComm.data[1] = UartComm.rxTemp;
					UartComm.rxSta = 4;
					UartComm.length++;
					break;
				case 4:
					UartComm.data[++UartComm.length] = UartComm.rxTemp;
					if (UartComm.length > UartComm.data[1])
					{
						UartComm.rxSta = 0;
						UartComm.length++;
						UartComm.transfer_complete = 1;
					}
					break;
		}
		HAL_UART_Receive_IT(&COMM_HUART, &UartComm.rxTemp, 1);
	}
}

//HAL_StatusTypeDef UH_GetCommData(uint8_t *data,uint8_t* length)
//{
//	HAL_StatusTypeDef ret = HAL_ERROR;
//	if(UartComm.transfer_complete)
//	{
//		ret = HAL_OK;
//		for(uint8_t i= 0 ;i < UartComm.length; ++i)
//		{
//			*(data + i) = UartComm.data[i];
//			UartComm.data[i] = 0;
//		}
//		*length=UartComm.length;
//		UartComm.length = 0;
//		UartComm.transfer_complete = 0;
//	}
//	return ret;
//}

// Checks and removes the checksum from the received data.
HAL_StatusTypeDef UH_GetCommData(uint8_t *func, uint8_t *data, uint8_t* data_length)
{
	if(UartComm.transfer_complete)
	{
		uint8_t CS = Checksum(UartComm.data, UartComm.length - CHECKSUM_LEN);
		if(CS != UartComm.data[UartComm.length - 1])
		{
			UartComm.transfer_complete = 0;
			return HAL_ERROR;
		}

		*func = UartComm.data[0];
		*data_length = UartComm.data[1] - CHECKSUM_LEN;

		for(uint8_t i= 0 ;i < *data_length; ++i)
		{
			*(data + i) = UartComm.data[i + 2];
		}

		memset(UartComm.data, 0, UartComm.length);
		UartComm.transfer_complete = 0;
		return HAL_OK;
	}
	return HAL_BUSY;
}

HAL_StatusTypeDef UH_SendACK(void)
{
	return HAL_UART_Transmit(&COMM_HUART, &ACK, 1, 50);
}

HAL_StatusTypeDef UH_SendNACK(void)
{
	return HAL_UART_Transmit(&COMM_HUART, &NACK, 1, 50);
}

void UH_Init(void)
{
	UartComm.rxTemp = 0x00;
	UartComm.rxSta = 0x00;
	HAL_UART_Receive_IT(&COMM_HUART,&UartComm.rxTemp,1);
}

static uint8_t Checksum(uint8_t *data, uint8_t len)
{
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < len; ++i) {
        checksum += data[i];
    }
    return checksum;
}



