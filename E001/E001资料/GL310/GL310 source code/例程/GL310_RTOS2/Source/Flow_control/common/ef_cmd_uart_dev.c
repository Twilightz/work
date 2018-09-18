#include <stdio.h>
#include "dtype.h"
#include "tx_api.h"
#include "os_user_api.h"
#include "libintrc.h"

#include "scansys.h"
#include "ef_cmd_uart.h"
#include "ef_cmd_uart_dev.h"



#define UART_BUF_LEN      256

void _DisableIRQ(void);
void _EnableIRQ(void);

UINT8 cmduart_datain_buffer[UART_BUF_LEN];


UINT32        BufRdIdx = 0,
              BufWtIdx = 0,
              BufIdx   = 0,
              dwBufCnt = 0;





void cmduart_serial_ISR() 
{
   volatile UINT32 dwLSR;
   volatile UINT32 uRegIIR;  
   //UINT32 uRegDataOut;
   UINT32 uRegDataIn;
   
    uRegIIR = ReadPort32(UART_IIR);
   
    if(uRegIIR & UART_IIR_DR){   
   
        //tx_semaphore_put(&CMD_UART_IN_SEMP);	   
        cmduart_set_msg(ID_UART_CMD, CUART_MSG_READ, NULL, NULL);
   
       
        dwLSR =   ReadPort32(UART_LSR);
        uRegDataIn = ReadPort32(UART_THR);
        if(dwBufCnt<UART_BUF_LEN){
            cmduart_datain_buffer[BufWtIdx++]= (unsigned char)(uRegDataIn &0x000000FF);
            dwBufCnt ++;
        }
        if(BufWtIdx >= 256)
            BufWtIdx  = 0;        
        
   }
   else if((uRegIIR & UART_IIR_TE) == UART_IIR_TE){  
   
        tx_semaphore_put(&CMD_Uart_Out_Semp);	   
        
        //uRegDataOut = *((volatile UINT8 *)acq_addr);
        //WritePort32(UART_THR, uRegDataOut);        
   }
   
   hw_ClearIntStatus(IRQ_UART4);
}



void cmduart_THR_empty_int_enable(UINT8 enabled)
{
    UINT32 uRegIER;

    if(enabled){
        uRegIER  = ReadPort32(UART_IER);
        uRegIER |= UART_IER_TE;   
        WritePort32(UART_IER, uRegIER);
    }
    else{
        uRegIER = ReadPort32(UART_IER);
        uRegIER &= ~UART_IER_TE;
        WritePort32(UART_IER, uRegIER);
    }    
}


void cmduart_init_serialPort() 
{
   UINT32 dwReg;

   dwReg = ReadPort32(UART_IER);      
   dwReg &= ~UART_IER_TE;   
   dwReg |= UART_IER_DR;
   WritePort32(UART_IER, dwReg);
   
   RegisterISR(IRQ_UART4, cmduart_serial_ISR);   
   hw_SetIntMask(IRQ_UART4);   
   //_EnableIRQ();
}

void cmduart_close_serialPort() 
{
   hw_ClearIntMask(IRQ_UART4);    
}






UINT32 cmduart_read_serialBufLen()
{
   return (UINT32) dwBufCnt;
}

BOOL cmduart_read_serialData(UINT8 *bData) 
{    
    
   _DisableIRQ();
   if(dwBufCnt==0){
      return false;
   }else{
      *bData = cmduart_datain_buffer[BufRdIdx];
      dwBufCnt--;
      BufRdIdx++;
      if (BufRdIdx >= 256){
         BufRdIdx = 0;
      }
   }
   _EnableIRQ();
   return  true;
}

BOOL cmduart_write_serialData(UINT8 *buf, UINT32 cnt) 
{
    UINT32 i;
    UINT32  TX_STATUS;     
    UINT32 uRegLSR;

    if(cnt){    
        
        cmduart_THR_empty_int_enable(1);                
        
        uRegLSR = ReadPort32(UART_LSR);
        
        if( !(uRegLSR & UART_LSR_THRE)){
        
            TX_STATUS = tx_semaphore_get(&CMD_Uart_Out_Semp, tick_waitmS(500));
            if(TX_STATUS != TX_SUCCESS){
                printf("error for wait\n");
                return false;
            }            
        }
        
    
        for(i=0; i<cnt; i++){
    
            WritePort32(UART_THR, buf[i]);                 
        
            TX_STATUS = tx_semaphore_get(&CMD_Uart_Out_Semp, tick_waitmS(500));
            if(TX_STATUS != TX_SUCCESS){
                printf("error for uart transfer\n");
                return false;
            }
        }

        cmduart_THR_empty_int_enable(0);                
    }

   return true;
   
}


