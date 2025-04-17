#include "usart.h"
#include "portserial.h"

BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
        enum eMBParity eParity)
{
			HAL_UART_DeInit(&uart_485);
			if (ucDataBits==8){
				uart_485.Init.WordLength = UART_WORDLENGTH_8B;
			}else if(ucDataBits==9){
				uart_485.Init.WordLength = UART_WORDLENGTH_9B;
			}else{return FALSE;}
			
			if (eParity==MB_PAR_NONE){
				uart_485.Init.Parity = UART_PARITY_NONE;
			}else if(eParity==MB_PAR_EVEN){
				uart_485.Init.Parity = UART_PARITY_EVEN;
			}else if(eParity==MB_PAR_ODD){
				uart_485.Init.Parity = UART_PARITY_ODD;
			}else{return FALSE;}
		
			uart_485.Init.BaudRate = ulBaudRate;

			if (HAL_UART_Init(&uart_485) != HAL_OK)
			{
				Error_Handler();
			}

    return TRUE;
}
