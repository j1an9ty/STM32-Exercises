/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;          													 // T35超时计时器的计数值（单位：50微秒）
static TimerHandle_t timer = NULL;        												 // FreeRTOS软件定时器句柄
static void prvvTIMERExpiredISR(void);    												 // 定时器硬件中断服务函数（ISR）
static void timer_timeout_ind(TimerHandle_t xTimer); 							 // 定时器超时回调函数（任务上下文）
static BaseType_t  pxHigherPriorityTaskWoken;  										 // 用于ISR中标记是否需要任务切换的标志

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);														 //定时器到期硬件中断服务函数

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    /* backup T35 ticks */
    usT35TimeOut50us = usTimeOut50us;

	timer = xTimerCreate("Master timer",			//创建Modbus主站定时器																							 
											 (50*usT35TimeOut50us) / (1000 * 1000 / configTICK_RATE_HZ) + 1, 	// 计算超时总微秒数，转换为系统节拍数
																																												/* 50*usTimeOut50us：将50微秒单位转换为总微秒数
																																												 * 1000*1000/configTICK_RATE_HZ：每个系统节拍的微秒数 */
											  pdFALSE,						//单次触发模式（非周期）																								
											 (void *)1,						//定时器ID（可在回调中识别）
											 timer_timeout_ind);  // 超时回调函数
	if (timer != NULL) 												//若创建定时器非空（检查定时器创建是否成功 ）
	{
		//printf("Create Master Timer Success!\r\n");
		return TRUE;
	} 
	else 
	{
		//printf("Create Master Timer Faild!\r\n");
		return FALSE;
	}
}


#if 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;
static struct rt_timer timer;
static void prvvTIMERExpiredISR(void);
static void timer_timeout_ind(void* parameter);

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    /* backup T35 ticks */
    usT35TimeOut50us = usTimeOut50us;

    rt_timer_init(&timer, "master timer",
                   timer_timeout_ind, /* bind timeout callback function */
                   RT_NULL,
                   (50 * usT35TimeOut50us) / (1000 * 1000 / RT_TICK_PER_SECOND) + 1,
                   RT_TIMER_FLAG_ONE_SHOT); /* one shot */

    return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
    rt_tick_t timer_tick = (50 * usT35TimeOut50us)
            / (1000 * 1000 / RT_TICK_PER_SECOND);

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);

    rt_timer_control(&timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);

    rt_timer_start(&timer);
}

/*! \brief If master send a broadcast frame,the master will wait time of convert to delay,
 * then master can send other frame */
void vMBMasterPortTimersConvertDelayEnable()
{
    rt_tick_t timer_tick = MB_MASTER_DELAY_MS_CONVERT * RT_TICK_PER_SECOND / 1000; //每毫秒tick数=RT_TICK_PER_SECOND / 1000;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);

    rt_timer_control(&timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);

    rt_timer_start(&timer);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
    rt_tick_t timer_tick = MB_MASTER_TIMEOUT_MS_RESPOND * RT_TICK_PER_SECOND / 1000;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);

    rt_timer_control(&timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);

    rt_timer_start(&timer);
}

void vMBMasterPortTimersDisable()
{
    rt_timer_stop(&timer);
}

void prvvTIMERExpiredISR(void)
{
    (void) pxMBMasterPortCBTimerExpired();
}

static void timer_timeout_ind(void* parameter)
{
    prvvTIMERExpiredISR();
}

#endif
#endif
