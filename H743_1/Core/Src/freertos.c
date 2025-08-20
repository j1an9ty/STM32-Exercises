/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void relay1_open(){
		HAL_GPIO_WritePin (CON_RELAY1_GPIO_Port,CON_RELAY1_Pin,GPIO_PIN_SET );
		HAL_GPIO_TogglePin (COM_LED_GPIO_Port,COM_LED_Pin );
}

void relay1_close(){
		HAL_GPIO_WritePin (CON_RELAY1_GPIO_Port,CON_RELAY1_Pin,GPIO_PIN_RESET );
}

void UART_SendChar(uint8_t ch)
{
    // 发送一个字节数据
    HAL_UART_Transmit(&huart3, &ch, 1, 0xFFFF);
}

/**
 * @brief  发送字符串到串口
 * @param  str: 要发送的字符串指针
 * @retval None
 */
void UART_SendString(const char *str)
{
    // 检查字符串是否为空
    if(str == NULL)
        return;
    
    // 循环发送字符串中的每个字符，直到遇到结束符'\0'
    while(*str != '\0')
    {
        UART_SendChar((uint8_t)*str);
        str++;
    }
}

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	uint8_t In1;
	uint8_t flag_coil_relaxes=1;

  /* Infinite loop */
  for(;;)
  {	
		HAL_GPIO_WritePin (CON3_485_GPIO_Port,CON3_485_Pin,GPIO_PIN_SET );
		
		if (flag_coil_relaxes){
			relay1_open();
			UART_SendString("吸合");
			//HAL_UART_Transmit_IT (&huart3,message_pull_in,sizeof (message_pull_in) );
			flag_coil_relaxes=0;
		}
		
		HAL_Delay (2000);
		In1 = !HAL_GPIO_ReadPin(IO_IN1_GPIO_Port, IO_IN1_Pin);
		if (In1){
		  relay1_close();
			UART_SendString("松开");
			flag_coil_relaxes=1;
		}
		else {
			UART_SendString("没有反馈");
			flag_coil_relaxes=0;
		}
		HAL_GPIO_TogglePin (RUN_LED_GPIO_Port,RUN_LED_Pin );

		HAL_Delay (5000);
		osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

