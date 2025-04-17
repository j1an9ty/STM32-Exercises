#ifndef LOG_H
#define LOG_H

#include "main.h"
#include "stm32f1xx_hal.h"

#define MAX_MSG_LENGTH 64
#define LogMode_232 0
#define LogMode_485 1

void Log_Init(UART_HandleTypeDef *huart,uint8_t mode,GPIO_TypeDef *RS485_DE_RE_GPIOx,uint16_t RS485_DE_RE_Pinx);
void Log_Print(const char *message);
void SendBytes(uint8_t *bytes,uint8_t len);

#endif
