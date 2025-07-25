/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "tim.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_mb_app.h"
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
/* Definitions for Start_Task */
osThreadId_t Start_TaskHandle;
const osThreadAttr_t Start_Task_attributes = {
  .name = "Start_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/////* Definitions for Mb_Master */
////osThreadId_t Mb_MasterHandle;
////const osThreadAttr_t Mb_Master_attributes = {
////  .name = "Mb_Master",
////  .stack_size = 512 * 4,
////  .priority = (osPriority_t) osPriorityLow,
////};
/////* Definitions for Mb_Slaver */
////osThreadId_t Mb_SlaverHandle;
////const osThreadAttr_t Mb_Slaver_attributes = {
////  .name = "Mb_Slaver",
////  .stack_size = 512 * 4,
////  .priority = (osPriority_t) osPriorityLow,
////};
/////* Definitions for PE_Comm */
////osThreadId_t PE_CommHandle;
////const osThreadAttr_t PE_Comm_attributes = {
////  .name = "PE_Comm",
////  .stack_size = 512 * 4,
////  .priority = (osPriority_t) osPriorityLow,
////};
/////* Definitions for Ymbdem */
////osThreadId_t YmbdemHandle;
////const osThreadAttr_t Ymbdem_attributes = {
////  .name = "Ymbdem",
////  .stack_size = 2048 * 4,
////  .priority = (osPriority_t) osPriorityLow,
////};
/////* Definitions for Task_Mem */
////osThreadId_t Task_MemHandle;
////const osThreadAttr_t Task_Mem_attributes = {
////  .name = "Task_Mem",
////  .stack_size = 1024 * 4,
////  .priority = (osPriority_t) osPriorityLow,
////};
/////* Definitions for ShortQueue */
////osMessageQueueId_t ShortQueueHandle;
////const osMessageQueueAttr_t ShortQueue_attributes = {
////  .name = "ShortQueue"
////};
/////* Definitions for LongQueue */
////osMessageQueueId_t LongQueueHandle;
////const osMessageQueueAttr_t LongQueue_attributes = {
////  .name = "LongQueue"
////};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_Thread(void *argument);
void Mb_Master_Thread(void *argument);
void Mb_Slaver_Thread(void *argument);
void PE_Comm_Thread(void *argument);
void Ymbdem_Thread(void *argument);
void Task_Mem_Thread(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
////void MX_FREERTOS_Init1(void) {
////  /* USER CODE BEGIN Init */

////  /* USER CODE END Init */

////  /* USER CODE BEGIN RTOS_MUTEX */
////  /* add mutexes, ... */
////  /* USER CODE END RTOS_MUTEX */

////  /* USER CODE BEGIN RTOS_SEMAPHORES */
////  /* add semaphores, ... */
////  /* USER CODE END RTOS_SEMAPHORES */

////  /* USER CODE BEGIN RTOS_TIMERS */
////  /* start timers, add new ones, ... */
////  /* USER CODE END RTOS_TIMERS */

////  /* Create the queue(s) */
////  /* creation of ShortQueue */
////  ShortQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &ShortQueue_attributes);

////  /* creation of LongQueue */
////  LongQueueHandle = osMessageQueueNew (128, sizeof(uint16_t), &LongQueue_attributes);

////  /* USER CODE BEGIN RTOS_QUEUES */
////  /* add queues, ... */
////  /* USER CODE END RTOS_QUEUES */

////  /* Create the thread(s) */
////  /* creation of Start_Task */
////  Start_TaskHandle = osThreadNew(Start_Thread, NULL, &Start_Task_attributes);

////  /* creation of Mb_Master */
////  Mb_MasterHandle = osThreadNew(Mb_Master_Thread, NULL, &Mb_Master_attributes);

////  /* creation of Mb_Slaver */
////  Mb_SlaverHandle = osThreadNew(Mb_Slaver_Thread, NULL, &Mb_Slaver_attributes);

////  /* creation of PE_Comm */
////  PE_CommHandle = osThreadNew(PE_Comm_Thread, NULL, &PE_Comm_attributes);

////  /* creation of Ymbdem */
////  YmbdemHandle = osThreadNew(Ymbdem_Thread, NULL, &Ymbdem_attributes);

////  /* creation of Task_Mem */
////  Task_MemHandle = osThreadNew(Task_Mem_Thread, NULL, &Task_Mem_attributes);

////  /* USER CODE BEGIN RTOS_THREADS */
////  /* add threads, ... */
////  /* USER CODE END RTOS_THREADS */

////  /* USER CODE BEGIN RTOS_EVENTS */
////  /* add events, ... */
////  /* USER CODE END RTOS_EVENTS */

////}



/*主机任务*/
osThreadId_t MasterTaskHandle;
const osThreadAttr_t MasterTask_attributes = {
    .name = "MasterTask",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128 * 4};

/*从机任务*/
osThreadId_t SlaveTaskHandle;
const osThreadAttr_t SlaveTask_attributes = {
    .name = "SlaveTask",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128 * 4};

/*写任务*/
osThreadId_t WriteTaskHandle;
const osThreadAttr_t Write_TASK = {
    .name = "Write_Task",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128 * 8};

/*读任务*/
osThreadId_t ReadTaskHandle;
const osThreadAttr_t Read_Task = {
    .name = "Read_Task",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128 * 8};

void MasterTask(void *argument);
void SlaveTask(void *argument);
void StartWriteTask(void *argument);
void StartReadTask(void *argument);
		
void MX_FREERTOS_Init(void)
{
  HAL_TIM_Base_Start(&htim6); //开启帧率测试
  eMBMasterInit(MB_RTU, 2, 115200, MB_PAR_NONE); /*主机初始化*/
  eMBMasterEnable();	/*主机使能*/
//  vMBMasterRunResRelease();
  /* creation of Start_Task */
  Start_TaskHandle = osThreadNew(Start_Thread, NULL, &Start_Task_attributes);

//  eMBInit(MB_RTU, 0x01, 3, 38400, MB_PAR_NONE); /*从机初始化*/
//  eMBEnable();	/*从机使能*/
	/*创建写任务*/
  WriteTaskHandle = osThreadNew(StartWriteTask, NULL, &Write_TASK);
	/*创建主机任务*/
  MasterTaskHandle = osThreadNew(MasterTask, NULL, &MasterTask_attributes);
	/*创建读任务*/
	ReadTaskHandle = osThreadNew(StartReadTask, NULL, &Read_Task);
	/*创建从机任务*/
// SlaveTaskHandle = osThreadNew(SlaveTask, NULL, &SlaveTask_attributes);

}

void StartWriteTask(void *argument)
{
//	vTaskSuspend(ReadTaskHandle);
	eMBMasterReqErrCode ret;
//  uint16_t data[] = {0,1,1,1};
	uint16_t data[10] = {6,2,3,4,5,8,2,50,10,11};
	unsigned long H_value;
//	ret = eMBMasterReqWriteHoldingRegister(1, 1, data[0], 100);
//	vMBMasterRunResRelease();
  for (;;)
  {
		H_value = uxTaskGetStackHighWaterMark(WriteTaskHandle);
		//printf("H_value:%d\r\n", (int)H_value);
		/*写单个寄存器*/
//		ret = eMBMasterReqWriteHoldingRegister(1, 5400, 8, 100);
		
		/*写多个寄存器*/
		ret = eMBMasterReqWriteMultipleHoldingRegister(1, 5400, 10, data, 100);
		//printf("ret values :%d ", ret);
   		 for(uint8_t i=0;i<sizeof(data)/sizeof(uint16_t);i++)
		{
     		 data[i]++;
		}
		osDelay(1000);
//		vTaskResume(ReadTaskHandle);
//		vTaskSuspend(WriteTaskHandle);
//		vTaskDelete(NULL);
  }
}

void StartReadTask(void *argument)
{
//	vTaskSuspend(WriteTaskHandle);
	extern USHORT usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
	eMBMasterReqErrCode ret;
	uint16_t slaveid;
	uint16_t regaddr;
	uint16_t regnums;
	uint8_t i;

	for (;;)
 	{
		slaveid = 1;
		regaddr = 5400;
		regnums = 10;
		/*读多个寄存器*/
		ret = eMBMasterReqReadHoldingRegister(slaveid, regaddr, regnums, 1000);
		if(ret == MB_MRE_NO_ERR)
		{
			//printf("===eMBMasterReqReadHoldingRegister successful! values : ");
			for(i = 0; i < regnums; i++)
			{
				//printf("%d ", usMRegHoldBuf[slaveid-1][i]);
			}
			//printf("\r\n");
		}
		else
		{
			//printf("===eMBMasterReqReadHoldingRegister failed! ret = %d \r\n", ret);
			for(i = 0; i < regnums; i++)
			{
				//printf("%d ", usMRegHoldBuf[slaveid-1][i]);
			}	
		}
		osDelay(1000);
//		vTaskResume(WriteTaskHandle);
//		vTaskSuspend(ReadTaskHandle);
//		vTaskDelete(ReadTaskHandle);
	}
}

void MasterTask(void *argument)
{
  for (;;)
  {
    eMBMasterPoll();		/*主机轮询*/
	osDelay(50);
  }
}

void SlaveTask(void *argument)
{
  for (;;)
  {
//    eMBPoll();		/*从机轮询*/
    osDelay(500);
  }
}

/* USER CODE BEGIN Header_Start_Thread */
/**
  * @brief  Function implementing the Start_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_Thread */
void Start_Thread(void *argument)
{
  /* USER CODE BEGIN Start_Thread */
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		osDelay(1000);
  }
  /* USER CODE END Start_Thread */
}

/* USER CODE BEGIN Header_Mb_Master_Thread */
/**
* @brief Function implementing the Mb_Master thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Mb_Master_Thread */
void Mb_Master_Thread(void *argument)
{
  /* USER CODE BEGIN Mb_Master_Thread */
  /* Infinite loop */
  for(;;)
  {
		osDelay(500);
  }
  /* USER CODE END Mb_Master_Thread */
}

/* USER CODE BEGIN Header_Mb_Slaver_Thread */
/**
* @brief Function implementing the Mb_Slaver thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Mb_Slaver_Thread */
void Mb_Slaver_Thread(void *argument)
{
  /* USER CODE BEGIN Mb_Slaver_Thread */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END Mb_Slaver_Thread */
}

/* USER CODE BEGIN Header_PE_Comm_Thread */
/**
* @brief Function implementing the PE_Comm thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PE_Comm_Thread */
void PE_Comm_Thread(void *argument)
{
  /* USER CODE BEGIN PE_Comm_Thread */
  /* Infinite loop */
  for(;;)
  {

    osDelay(1);
  }
  /* USER CODE END PE_Comm_Thread */
}

/* USER CODE BEGIN Header_Ymbdem_Thread */
/**
* @brief Function implementing the Ymbdem thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Ymbdem_Thread */
void Ymbdem_Thread(void *argument)
{
  /* USER CODE BEGIN Ymbdem_Thread */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Ymbdem_Thread */
}

/* USER CODE BEGIN Header_Task_Mem_Thread */
/**
* @brief Function implementing the Task_Mem thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_Mem_Thread */
void Task_Mem_Thread(void *argument)
{
  /* USER CODE BEGIN Task_Mem_Thread */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_Mem_Thread */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

