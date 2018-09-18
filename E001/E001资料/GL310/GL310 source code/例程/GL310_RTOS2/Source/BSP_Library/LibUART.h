#if !defined(_LIBUART_H_)
#define _LIBUART_H_

#include "MacroUtil.h"

#define UART1_BASE            0x98200000
#define UART2_BASE            0x98300000
#define UART3_BASE            0x98900000
#define UART4_BASE            0x99600000
                              
#define UART_REG_RBR          0x00
#define UART_REG_THR          0x00
#define UART_REG_DLL          0x00   
#define UART_REG_IER          0x04
#define UART_REG_DLM          0x04
#define UART_REG_IIR          0x08
#define UART_REG_FCR          0x08
#define UART_REG_PSR          0x08
#define UART_REG_LCR          0x0C
#define UART_REG_MCR          0x10
#define UART_REG_LSR          0x14
#define UART_REG_MSR          0x18
#define UART_REG_SPR          0x1C

                                    
#define UART_REG_MDR				0x20  
#define UART_REG_ACR				0x24  
#define UART_REG_TXLENL			0x28  
#define UART_REG_TXLENH			0x2C  
#define UART_REG_MRXLENL		0x30  
#define UART_REG_MRXLENH		0x34  
#define UART_REG_PLR				0x38  
#define UART_REG_FMIIR_PIO		0x3C  

                              
/* IER Register */            
#define UART_IER_DR           0x01        /* Data ready Enable */
#define UART_IER_TE           0x02        /* THR Empty Enable */
#define UART_IER_RLS          0x04        /* Receive Line Status Enable */
#define UART_IER_MS           0x08        /* Modem Staus Enable */
                              
/* IIR Register */            
#define UART_IIR_NONE         0x01        /* No interrupt pending */
#define UART_IIR_RLS          0x06        /* Receive Line Status */
#define UART_IIR_DR           0x04        /* Receive Data Ready */
#define UART_IIR_TIMEOUT      0x0c        /* Receive Time Out */
#define UART_IIR_TE           0x02        /* THR Empty */
#define UART_IIR_MODEM        0x00        /* Modem Status */
                              
/* FCR Register */            
#define UART_FCR_FE           0x01        /* FIFO Enable */
#define UART_FCR_RXFR         0x02        /* Rx FIFO Reset */
#define UART_FCR_TXFR         0x04        /* Tx FIFO Reset */
                              
/* LCR Register */            
#define UART_LCR_LEN5         0x00
#define UART_LCR_LEN6         0x01
#define UART_LCR_LEN7         0x02
#define UART_LCR_LEN8         0x03
#define UART_LCR_STOP         0x04

#define UART_LCR_EVEN         0x18        /* Even Parity */
#define UART_LCR_ODD          0x08        /* Odd Parity */
#define UART_LCR_PE           0x08        /* Parity Enable */
#define UART_LCR_SETBREAK     0x40        /* Set Break condition */
#define UART_LCR_STICKPARITY  0x20        /* Stick Parity Enable */
#define UART_LCR_DLAB         0x80        /* Divisor Latch Access Bit */

#define UART_LSR_DR           0x01        /* Data Ready */
#define UART_LSR_OE           0x02        /* Overrun Error */
#define UART_LSR_PE           0x04        /* Parity Error */
#define UART_LSR_FE           0x08        /* Framing Error */
#define UART_LSR_BI           0x10        /* Break Interrupt */
#define UART_LSR_THRE         0x20        /* THR Empty */
#define UART_LSR_TE           0x40        /* Transmitte Empty */
#define UART_LSR_DE           0x80        /* FIFO Data Error */

/* MCR Register */
#define UART_MCR_DTR          0x01		   /* Data Terminal Ready */
#define UART_MCR_RTS          0x02		   /* Request to Send */
#define UART_MCR_OUT1         0x04		   /* output	1 */
#define UART_MCR_OUT2         0x08		   /* output2 or global interrupt enable */
#define UART_MCR_LPBK         0x10	 	   /* loopback mode */
#define UART_MCR_DMAMODE2		0x20		   /* DMA mode2 */
#define UART_MCR_OUT3			0x40		   /* output 3 */ 

/* MDR register */
#define UART_MDR_UART			0x00
#define UART_MDR_SIR				0x01
#define UART_MDR_FIR				0x02
#define UART_MDR_MODE_SEL		0x03


#define UART_PARITY_NONE      0x00
#define UART_PARITY_ODD       0x08
#define UART_PARITY_EVEN      0x18
#define UART_PARITY_MARK      0x28
#define UART_PARITY_SPACE     0x38

#define UART_STOP_1BIT        0x00
#define UART_STOP_2BIT        0x04 

#define CONSOLE_PORT          3

int  InitiateUART(UINT32 Port, UINT32 BaudRate, UINT32 CharLen, UINT32 Parity, UINT32 PLen);
void UART_EnableLoopback(UINT32 Port, int OnOff);
void UART_WriteChar(UINT32 Port, char ch);
char UART_ReadChar(UINT32 Port);


#endif

