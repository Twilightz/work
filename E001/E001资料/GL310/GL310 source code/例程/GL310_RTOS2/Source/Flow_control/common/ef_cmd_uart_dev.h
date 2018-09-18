/***************************************************************************
* Name: ef_cmd_uart.h		                                               *
* Description: event flow cmd uart device header       		         	   *
*											  							   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_CMD_UART_DEV_
#define _HEADER_EF_CMD_UART_DEV_

#include "dtype.h"
#include "libuart.h"








#define SERIAL_THR                     0x00	 		/*  Transmitter Holding Register(Write).*/
#define SERIAL_RBR                     0x00	 		/*  Receive Buffer register (Read).*/
#define SERIAL_IER                     0x04	 		/*  Interrupt Enable register.*/
#define SERIAL_IIR                     0x08	 		/*  Interrupt Identification register(Read).*/
#define SERIAL_FCR                     0x08	 		/*  FIFO control register(Write).*/
#define SERIAL_LCR                     0x0C	 		/*  Line Control register.*/
#define SERIAL_MCR                     0x10	 		/*  Modem Control Register.*/
#define SERIAL_LSR                     0x14	 		/*  Line status register(Read) .*/
#define SERIAL_MSR                     0x18	 		/*  Modem Status register (Read).*/
#define SERIAL_SPR                     0x1C     	/*  Scratch pad register */
#define SERIAL_DLL                     0x0      	/*  Divisor Register LSB */
#define SERIAL_DLM                     0x4      	/*  Divisor Register MSB */
#define SERIAL_PSR                     0x8     		/* Prescale Divison Factor */

/*
#define UART1_BASE            0x98200000
#define UART2_BASE            0x98300000
#define UART3_BASE            0x98900000
*/

#define UART_BASE         0x98900000
#define UART_THR          (UART_BASE + SERIAL_THR)
#define UART_IER          (UART_BASE + SERIAL_IER)
#define UART_IIR          (UART_BASE + SERIAL_IIR) 
#define UART_LSR          (UART_BASE + SERIAL_LSR) 

void cmduart_init_serialPort(void); 
UINT32 cmduart_read_serialBufLen(void);
BOOL cmduart_read_serialData(UINT8 *bData); 
BOOL cmduart_write_serialData(UINT8 *buf, UINT32 cnt); 




#endif
