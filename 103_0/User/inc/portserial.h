#ifndef PORT_H
#define PORT_H

#include "main.h"

#define TRUE 1
#define FALSE 0
#define uart_485 huart1

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

enum eMBParity{
	MB_PAR_NONE,
	MB_PAR_EVEN,
	MB_PAR_ODD
};

BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, enum eMBParity eParity);

#endif
