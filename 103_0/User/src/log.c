// uart_log.c
#include "log.h"
#include <string.h>

static UART_HandleTypeDef *log_uart;
static volatile uint8_t tx_busy = 0; // 发送状态标志
static char pending_msg[MAX_MSG_LENGTH];
static uint8_t Log_enabled=0;
static uint8_t log_485_enabled=0;

static GPIO_TypeDef *RS485_DE_RE_GPIO;
static uint16_t RS485_DE_RE_Pin;



void Log_Init(UART_HandleTypeDef *huart,uint8_t mode,GPIO_TypeDef *RS485_DE_RE_GPIOx,uint16_t RS485_DE_RE_Pinx)
{
    if(Log_enabled==0){
		Log_enabled=1;
		log_uart = huart;
		}
		
		if(mode==LogMode_485){
			log_485_enabled=0;
		}
		
		if(mode==LogMode_485){
			log_uart = huart;
			log_485_enabled=1;
			RS485_DE_RE_GPIO=RS485_DE_RE_GPIOx;
			RS485_DE_RE_Pin=RS485_DE_RE_Pinx;
		}

	
		
}


// UART发送完成中断回调
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == log_uart) {
        tx_busy = 0; // 标记发送完成
    }
		
		if(log_485_enabled == 1){
			HAL_GPIO_WritePin (RS485_DE_RE_GPIO,RS485_DE_RE_Pin,GPIO_PIN_RESET );		
		}
			
}

void Log_Print(const char *message)
{
    if(log_uart == NULL || tx_busy) return;
    
    uint16_t len = strlen(message);
		strncpy(pending_msg, message, MAX_MSG_LENGTH);
    tx_busy = 1; // 标记开始发送
	
	if(log_485_enabled == 1){
		HAL_GPIO_WritePin (RS485_DE_RE_GPIO,RS485_DE_RE_Pin,GPIO_PIN_SET );		
	}
	
    else{
		HAL_UART_Transmit_IT(log_uart, (uint8_t*)message, len);
	}
}

void SendBytes(uint8_t *bytes,uint8_t len)
{
    if(log_uart == NULL || tx_busy) return;

    tx_busy = 1; // 标记开始发送
	
	if(log_485_enabled == 1){
		HAL_GPIO_WritePin (RS485_DE_RE_GPIO,RS485_DE_RE_Pin,GPIO_PIN_SET );		
	}
	
    else{
		HAL_UART_Transmit_IT(log_uart,bytes, len);
	}
}
