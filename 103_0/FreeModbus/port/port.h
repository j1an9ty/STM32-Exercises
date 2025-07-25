/*
 * FreeModbus Libary: BARE Port
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
 * File: $Id: port.h ,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

//#ifndef _PORT_H
#define _PORT_H

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "event_groups.h"
#include "main.h"
#include "usart.h"
#include "mbconfig.h"
#include "mbproto.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

#define INLINE
#define PR_BEGIN_EXTERN_C           extern "C" {
#define PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION()    EnterCriticalSection()
#define EXIT_CRITICAL_SECTION()    ExitCriticalSection()

  /* 使用了lwrb，可能不需要 _serial_fifo*/
//typedef struct _serial_fifo
//{
//  /* software fifo */
//  volatile uint8_t *buffer;     // 指向缓冲区的指针（存储实际数据）
//  volatile uint16_t put_index;  // 写指针：指向下一个写入位置
//  volatile uint16_t get_index;  // 读指针：指向下一个读取位置
//} Serial_fifo;
#define FIFO_SIZE_MAX 265

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

void EnterCriticalSection(void);
void ExitCriticalSection(void);
  
/* 使用了lwrb，可能不需要以下两条 */
//void Put_in_fifo(Serial_fifo *buff, uint8_t *putdata, int length);
//int Get_from_fifo(Serial_fifo *buff, uint8_t *getdata, int length);

extern __inline bool IS_IRQ(void); //bool报错

//#endif
