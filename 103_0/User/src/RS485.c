#include "RS485.h"

void RS485SendByte(const uint8_t *ch){
	HAL_GPIO_WritePin (RS485_RE_GPIO_Port,RS485_RE_Pin,GPIO_PIN_SET );
	HAL_UART_Transmit_IT (&huart2 ,ch,1);
}

//void RS485_SendMessage(uint8_t *senddate){
//	
//}
