#if !defined(_LIB_INTRC_H_)
#define _LIB_INTRC_H_

#include "dtype.h"

#define   MAX_INTERRUPT_REQUEST       32

#define IRQ_CFC_CD                     0
#define IRQ_CFC_DMA                    1
#define IRQ_SSP1                       2
#define IRQ_I2C                        3
#define IRQ_KBD                        4
#define IRQ_SDC                        5
#define IRQ_I2S                        6
#define IRQ_UART4                      7
#define IRQ_PMU                        8
#define IRQ_MOUSE                      9
#define IRQ_UART1                     10
#define IRQ_UART2                     11
#define IRQ_SPI_HOST                  12
#define IRQ_GPIO                      13
#define IRQ_TIMER2                    14
#define IRQ_TIMER3                    15
#define IRQ_WATCHDOG                  16
#define IRQ_TIMER4                    17
#define IRQ_TIMER5                    18
#define IRQ_TIMER1                    19
#define IRQ_LCD                       20 
#define IRQ_DMA                       21
#define IRQ_USB_DEVICE                22
#define IRQ_USB_HOST_1                23
#define IRQ_USB_HOST_2                24
#define IRQ_MAC                       25
#define IRQ_USB_DEVICE_FA             26
#define IRQ_USB_HOST_FA               27
#define IRQ_TOUCH_PANEL               28
#define IRQ_UNUSED29                  29
#define IRQ_EXT_INT                   30
#define IRQ_UNUSED31                  31

#define F_BIT                         0x40
#define I_BIT                         0x80

#define IRQ_TRIG_LEVEL		           0
#define IRQ_TRIG_EDGE       			  1
#define H_ACTIVE                      0
#define L_ACTIVE                      1


typedef void (*IRQ_Handler)(void);

void        hw_SetIntMask(int Intr);
void        hw_ClearIntMask(int Intr);
void        hw_ClearIntStatus(int Intr);
void        hw_SetIntTriggerMode(UINT32);
void        hw_SetIntTriggerLevel(UINT32);
UINT32      hw_ReadIntSource(void);
UINT32      hw_ReadIntStatus(void);
UINT32      hw_ReadIntMask(void);
void        Init_LibIntrc(void);
int         RegisterISR(int IntNum, IRQ_Handler ISRPrc);
void        RemoveISR(int IrqNum);
IRQ_Handler GetCurrentISR(int IrqNum);
void        IntrDispatcher(void);
int         GetDispatchIRQ(void);
void        Enable_Interrupt(void);
void        Disable_Interrupt(void);
void        Enable_FastInterrupt(void);
void        Disable_FastInterrupt(void);
void        Hold_All_Interrupts(UINT32 *dwTmpCPSR);
void        Resume_All_Interrupts(UINT32 *dwTmpCPSR);
void        SetInterruptConfig(UINT32 dwIRQ, UINT32 TrigMode, UINT32 TrigLevel);
#endif
