#if !defined(_CHIP_UTIL_H_)
#define _ASIC_UTIL_H_
#include "dtype.h"
#include "cpe.h"

#define USBDEV_CLKSEL1        0xA0000054
#define UART_BASE_CNT         614800

#define CPE_STRAP_REG	(*(volatile UINT32 *) 0xA0000100) 
#define Pad_Debug		0x10
#define Pad_Update		0x08
#define Pad_Coloum10	0x04
#define Pad_Bank8		0x02
#define Pad_DDRM		0x01


//INT64 GetSystemClock(void);
UINT32 GetSystemClock(void);

#endif


