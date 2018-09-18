#include <stdio.h>
#include "MacroUtil.h"
#include "dtype.h"
#include "libuart.h"
#include "libintrc.h"


#define GL310

#if defined(GL310)
#define UART_BASE         UART2_BASE  
#define IRQ_CONSOLE       IRQ_UART2
//#define UART_BASE       UART3_BASE
//#define IRQ_CONSOLE     IRQ_UART4
#else
#define UART_BASE         0xD0003000
#define IRQ_CONSOLE       HAL_IRQ_UART1
#endif


#define UART_THR          (UART_BASE + UART_REG_THR)
#define UART_IER          (UART_BASE + UART_REG_IER)
#define UART_IIR          (UART_BASE + UART_REG_IIR) 
#define UART_MSR          (UART_BASE + UART_REG_MSR) 
#define UART_LSR          (UART_BASE + UART_REG_LSR) 

#define UART_BUF_LEN      256
#define UART_TXD_LEN      512

extern void _DisableIRQ(void);
extern void _EnableIRQ(void);

unsigned char buffer_rx[UART_BUF_LEN];
unsigned char buffer_tx[UART_TXD_LEN];
UINT32        BufRdIdx = 0,
              BufWtIdx = 0,
              BufIdx   = 0,
              dwBufCnt = 0;
              
int           iTxDataCount = 0,
              iTxDataIndex = 0;



void SerialISR() 
{
   volatile UINT32 dwInput, dwLSR, dwVal;
   volatile UINT8  uIntr;
            int    iIntrClr = 1;
      
   uIntr = (UINT8) ReadPort32(UART_IIR);
   uIntr &= 0x0F;
   
   switch(uIntr){
      case 0x01:
         //There is no interrupt pending. 
         break;
      case 0x00: 
         // Modem Status change
         dwVal = ReadPort32(UART_MSR);
         break;
      case 0x02:
         //Transmitter Holding Register Empty 
         uIntr = ReadPort32(UART_IIR);
         hw_ClearIntStatus(IRQ_CONSOLE);
         iIntrClr = 0;
         if (iTxDataIndex < iTxDataCount){
            WritePort8(UART_THR, buffer_tx[iTxDataIndex++]);   
         }
         break;
      case 0x06:
         //Receiver Line Status
         dwVal  = ReadPort32(UART_LSR);
      case 0x04:
         //Received Data Ready
      case 0x0C:
         //Character Reception Timeout
         dwInput = ReadPort32(UART_THR);
         if (dwBufCnt<UART_BUF_LEN){
            buffer_rx[BufWtIdx++]= (unsigned char)(dwInput &0x000000FF);
            dwBufCnt ++;
         }
         if(BufWtIdx >= 256){
            BufWtIdx  = 0;
         }
         break;
   }
   if (iIntrClr)
      hw_ClearIntStatus(IRQ_CONSOLE);
}

void InitSerialPort(void) 
{
   UINT32 dwReg;
   int status = 0;
	
   InitiateUART(CONSOLE_PORT,57600,8,'N',1);
   RegisterISR(IRQ_CONSOLE, SerialISR);   // Regiseter UART ISR
   WritePort32(UART_IIR, 0x00); 
   dwReg  = ReadPort8(UART_IER);     
   dwReg |= UART_IER_DR|UART_IER_TE;
   WritePort32(UART_IER, dwReg);          // Clear Data Ready Interrupt
   ReadPort32(UART_IIR);	               // Clear THE interrupt 	
   hw_SetIntMask(IRQ_CONSOLE);   
   _EnableIRQ();
}

void CloseSerialPort() 
{
	int status = 0;
	hw_ClearIntMask(IRQ_CONSOLE);    
}

UINT32 ReadSerialBufLen()
{
   #if defined(_FIFO_)
   char rxff_cnt;
   rxff_cnt = ReadPort8(UART_BASE+0x5C);
   #endif
   return (UINT32) dwBufCnt;
}

int ReadSerialData() 
{
   int iRet;
   
   if(dwBufCnt==0){
      iRet = -1;
   }else{
      _DisableIRQ();
      iRet = (int)buffer_rx[BufRdIdx];
      dwBufCnt--;
      BufRdIdx++;
      if (BufRdIdx >= 256){
         BufRdIdx = 0;
      }
      _EnableIRQ();
   }
   return  iRet;
}

void WriteSerialData(UINT8 *Buf, int iLength) 
{  
   if (iLength <UART_TXD_LEN){
      iTxDataCount = iLength;
   }else {
      iTxDataCount = UART_TXD_LEN;
   }
   memcpy(buffer_tx, Buf, iTxDataCount);
   iTxDataIndex = 1;
   WritePort8(UART_THR, buffer_tx[0]);   
}

void WriteSerialChar(UINT8 uChar) 
{
   UINT32 dwLSR;
   do {
		dwLSR = ReadPort32((UART_LSR));
	}while(!((dwLSR & UART_LSR_THRE) == UART_LSR_THRE));
   WritePort8(UART_THR, uChar);
}

