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
static USHORT usT35TimeOut50us;          													 // T35超时计数，1个 usT35TimeOut50us = 50us
static TimerHandle_t MBMastertimer = NULL;        								 // FreeRTOS软件定时器句柄
static void prvvTIMERExpiredISR(void);    												 // 定时器硬件中断服务函数（ISR）
static void timer_timeout_ind(TimerHandle_t xTimer); 							 // 定时器超时回调函数（任务上下文）
static BaseType_t  pxHigherPriorityTaskWoken;  										 // 用于ISR中标记是否需要任务切换的标志

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);														 //定时器到期硬件中断服务函数

/* ----------------------- Start implementation -----------------------------*/

/**
 * @param usTimeOut50us 超时计数，1计数=50us
 */
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)								 //软定时器初始化								
{
    /* backup T35 ticks */
    usT35TimeOut50us = usTimeOut50us;

	MBMastertimer = xTimerCreate("Master timer",			//创建Modbus主站定时器																							 
											 (50*usT35TimeOut50us) / (1000 * 1000 / configTICK_RATE_HZ) + 1, 	//系统节拍数（向上取整），n=t/T (n:次数，t:总时间,T:周期)
											  pdFALSE,						//单次触发模式（非周期）																								
											 (void *)1,						//定时器ID（可在回调中识别）
											 timer_timeout_ind);  // 超时回调函数
	if (MBMastertimer != NULL) 												//若创建定时器非空（检查定时器创建是否成功 ）
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

/**
 * 启用Modbus主站T35超时定时器
 * 
 * @note T35是Modbus协议中规定的"字符间超时时间"，用于判断一帧数据是否结束
 *       此函数会根据usT35TimeOut50us的值重新设置定时器周期并启动
 */
void vMBMasterPortTimersT35Enable()
{
	uint32_t timer_tick = (50 * usT35TimeOut50us) / (1000 * 1000 / configTICK_RATE_HZ) + 1; // 超时对应的系统节拍数（向上取整），n=t/T (n:次数，t:总时间,T:周期)

  	/* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);            //设置当前定时模式

    /* 根据当前上下文环境选择适当的API */
    if (IS_IRQ())  																		 //若在中断上下文中
    {
        /* 直接修改定时器周期（从中断）
         * 无需先停止再启动，xTimerChangePeriodFromISR内部会处理状态转换
         * pxHigherPriorityTaskWoken用于标记是否需要任务切换
         */
//        xTimerStopFromISR(timer, 0);  // 无需调用，ChangePeriod会自动处理
        xTimerChangePeriodFromISR((TimerHandle_t)MBMastertimer, timer_tick, &pxHigherPriorityTaskWoken); //修改定时数（从中断）
//        xTimerStartFromISR(timer, 0);  // 无需调用，ChangePeriod会自动启动
    } 
    else  /* 在任务上下文中 */
    {
        /* 直接修改定时器周期（从任务）
         * 超时时间为0表示不等待，立即执行
         */
//        xTimerStop(timer, 0);  // 无需调用，ChangePeriod会自动处理
        xTimerChangePeriod((TimerHandle_t)MBMastertimer, timer_tick, 0); //修改定时数
//        xTimerStart(timer, 0);  // 无需调用，ChangePeriod会自动启动
    }
}

/**
 * 启用Modbus主站转换延迟定时器
 * 
 * @note 转换延迟(Convert Delay)是Modbus协议中主站在连续请求之间必须等待的最小时间
 *       通常用于避免主站发送请求过快，给从站足够的处理时间
 *       此函数设置的延迟时间由MB_MASTER_DELAY_MS_CONVERT常量定义(单位：毫秒)
 */
void vMBMasterPortTimersConvertDelayEnable()
{
	uint32_t timer_tick = MB_MASTER_DELAY_MS_CONVERT * configTICK_RATE_HZ / 1000; //毫秒转为系统节拍数, n=t*f（f:每毫秒节拍数)
	/* Set current timer mode, don't change it.*/
	vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);//设置当前定时模式
	//	xTimerStop(timer, 0);
	if (IS_IRQ()) 
	{
		xTimerChangePeriodFromISR((TimerHandle_t)MBMastertimer, timer_tick, &pxHigherPriorityTaskWoken); //修改定时数（从中断）	
	} 
	else 
	{
		xTimerChangePeriod((TimerHandle_t)MBMastertimer, timer_tick, 0);//修改定时数
	}
}

/**
 * 启用Modbus主站响应超时定时器
 * 
 * @note 响应超时(Respond Timeout) 超时，主站将认为通信失败
 */
void vMBMasterPortTimersRespondTimeoutEnable()
{
	uint32_t timer_tick = MB_MASTER_TIMEOUT_MS_RESPOND * configTICK_RATE_HZ / 1000;
	/* Set current timer mode, don't change it.*/
	vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);//设置当前定时模式
	if (IS_IRQ()) 
	{
		xTimerChangePeriodFromISR((TimerHandle_t)MBMastertimer, timer_tick, &pxHigherPriorityTaskWoken);//修改定时数（从中断）	
	} 
	else 
	{
		xTimerChangePeriod((TimerHandle_t)MBMastertimer, timer_tick, 0);//修改定时数
	}
}

void vMBMasterPortTimersDisable()
{
	if (IS_IRQ()) 
	{
		xTimerStopFromISR((TimerHandle_t)timer, 0);
	} 
	else 
	{
		xTimerStop((TimerHandle_t)timer, 0);
	}
}

void prvvTIMERExpiredISR(void)
{
    (void) pxMBMasterPortCBTimerExpired();
}

static void timer_timeout_ind(void* parameter)
{
    prvvTIMERExpiredISR();
}

/**
在这个 Modbus 主站定时器实现中，所有启用定时器的函数都使用同一个timer变量，主要基于以下设计考虑：
1. 单定时器复用设计
Modbus 协议在不同阶段需要多种定时器（如 T35 超时、转换延迟、响应超时），但这些定时器不会同时使用。因此：

节省资源：只创建一个定时器，通过动态修改其周期和模式来满足不同阶段的需求，避免创建多个定时器占用额外内存。
简化管理：只需维护一个定时器句柄，减少代码复杂度。

2. 分时复用机制
代码通过vMBMasterSetCurTimerMode()函数设置当前定时器模式（如MB_TMODE_T35、MB_TMODE_CONVERT_DELAY等），并在回调函数中根据模式执行不同操作。
这种设计允许同一个定时器在不同阶段扮演不同角色，通过模式切换实现功能复用。

3. FreeRTOS 定时器特性支持
FreeRTOS 的定时器 API（如xTimerChangePeriod()）允许动态修改定时器周期，无需销毁重建：

动态调整：通过xTimerChangePeriod()直接修改定时器的超时值，适应不同场景的时间需求。
状态自动管理：该 API 会自动处理定时器的启动 / 停止状态，无需手动干预（如注释掉的xTimerStop()和xTimerStart()）。

4. 为什么不使用多个定时器？
资源限制：嵌入式系统中，定时器资源可能有限，复用单个定时器可减少开销。
逻辑简化：多个定时器需要管理多个句柄，增加代码复杂度，且可能导致回调函数逻辑分散。
协议特性：Modbus 主站在同一时刻只需一个定时器生效（如发送请求后等待响应时，只需响应超时定时器），无需并行运行多个定时器。

5. 潜在风险与注意事项
虽然复用单定时器高效，但需确保：
1.模式切换原子性：vMBMasterSetCurTimerMode()应在修改定时器周期前调用，避免竞态条件。
2.回调函数逻辑完备：timer_timeout_ind()需正确处理所有可能的模式，避免逻辑遗漏。
3.中断安全：在中断上下文中修改定时器时，必须使用带FromISR后缀的 API（如xTimerChangePeriodFromISR()）。
 */

/**
 *rtthread部分
 */
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
