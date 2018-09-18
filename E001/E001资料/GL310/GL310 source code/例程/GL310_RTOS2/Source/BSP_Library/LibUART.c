#include "dtype.h"
#include "LibIntrc.h"
#include "LibUART.h"

UINT32 UartCtrlBase[4]= { 0x0, UART1_BASE, UART2_BASE, UART3_BASE};
UINT32 UartIntr[4]    = { 0x0, IRQ_UART1, IRQ_UART2, IRQ_UART4};

void hw_SetBaudRate(UINT32 Base, UINT32 BaudRate);
void hw_SetCharFormat(UINT32 Base, UINT32 CharLen, UINT32 Parity, UINT32 PLen);
void hw_SetUARTMode(UINT32 Base, UINT32 dwMode);
void hw_SetUARTFiFo(UINT32 Base, UINT32 level_tx, UINT32 level_rx, UINT32 reset_tx, UINT32 reset_rx);

int InitiateUART(UINT32 Port, UINT32 BaudRate, UINT32 CharLen, UINT32 Parity, UINT32 PLen)
{
   DWORD    dwUartAddr;
   int      iRet=0;
   
   if ((Port>0)&&(Port <4)){
      dwUartAddr = UartCtrlBase[Port];
      hw_SetUARTMode(dwUartAddr, UART_MDR_UART); 
      hw_SetUARTFiFo(dwUartAddr, 0, 0, 1, 1);
      hw_SetBaudRate(dwUartAddr, BaudRate);
      hw_SetCharFormat(dwUartAddr, CharLen, Parity, PLen);
   }else{								     
      iRet = -1;
   }
   return iRet;
}

void hw_SetBaudRate(UINT32 Base, UINT32 BaudRate)
{
   UINT32         dwClk, dwBRate;
   UINT32         dwLCR;
      
   dwClk      = 30000000;  
   dwLCR      = ReadPort32((Base+UART_REG_LCR)) & ~(UART_LCR_DLAB);   
   WritePort32((Base+UART_REG_LCR), (dwLCR|UART_LCR_DLAB));
   
   switch(BaudRate){
      case 115200:
         dwBRate = dwClk / 1843200;
         break;
      case 57600:
         dwBRate = dwClk / 921600;
         break;
      case 38400:
         dwBRate = dwClk / 614400;
         break;
      case 19200:
         dwBRate = dwClk / 307200;
         break;
      case 14400:
         dwBRate = dwClk / 230400;
         break;
      case 9600:
         dwBRate = dwClk / 153600;
         break;
      case 4800:
         dwBRate = dwClk / 76800;
         break;
      case 2400:
         dwBRate = dwClk / 38400;
         break;
      case 1200:
      default:
         dwBRate = dwClk / 19200;
   }
   WritePort32((Base+UART_REG_DLM), ((dwBRate &0x0FF00)>>8));
   WritePort32((Base+UART_REG_DLL), (dwBRate &0x00FF));
   WritePort32((Base+UART_REG_LCR), (dwLCR));
}

void hw_SetCharFormat(UINT32 Base, UINT32 CharLen, UINT32 Parity, UINT32 PLen)
{
   UINT32	dwLCR;
      
   dwLCR = ReadPort32((Base + UART_REG_LCR)) & (~UART_LCR_DLAB);
   WritePort32((Base+UART_REG_LCR),  (dwLCR|UART_LCR_DLAB));
   dwLCR &= 0xC0;
   
   if ((CharLen > 4) && (CharLen < 9)){
   	CharLen -= 5;
   }else{
   	CharLen = 0;
   }   
   dwLCR |= CharLen;
   switch(Parity){
   	case 'N':
   		dwLCR |= UART_PARITY_NONE;
   		break;
   	case 'O':
   		dwLCR |= UART_PARITY_ODD;
   		break;
   	case 'E':
   		dwLCR |= UART_PARITY_EVEN;
   		break;	
   	case 'M':
   		dwLCR |= UART_PARITY_MARK;
   		break;	
   	case 'S':
   		dwLCR |= UART_PARITY_SPACE;
   		break;	
   }
   if (PLen == UART_STOP_2BIT){
   	dwLCR |= UART_STOP_2BIT;
   }
   
   WritePort32((Base + UART_REG_LCR), (dwLCR|UART_LCR_DLAB));	
   WritePort32((Base + UART_REG_LCR), dwLCR);	
}

void hw_SetUARTMode(UINT32 Base, UINT32 dwMode)
{
	UINT32 dwMDR;
	
	dwMDR  = ReadPort32((Base + UART_REG_MDR));
	dwMDR &= (~UART_MDR_MODE_SEL);
	WritePort32((Base + UART_REG_MDR), (dwMDR | dwMode));
	WritePort32((Base + UART_REG_MCR), 0x0B);
}

void hw_SetUARTFiFo(UINT32 Base, UINT32 level_tx, UINT32 level_rx, UINT32 reset_tx, UINT32 reset_rx)
{
	UINT32	dwFCR;
	
	dwFCR = UART_FCR_FE;
	
	switch(level_rx) {
		case 0x04:
 			dwFCR |= 0x40;
 			break;
 		case 0x08:
 			dwFCR |= 0x80;
 			break;
 		case 0x0E:
 			dwFCR |= 0xc0;
 			break;
 		default:
 			break;
	}
	switch(level_tx){
		case 0x03:
 			dwFCR |= 0x01<<4;
 			break;
 		case 0x09:
 			dwFCR |= 0x02<<4;
 			break;
 		case 0x0D:
 			dwFCR |= 0x03<<4;
 			break;
 		default:
 			break;
		
	}
	if(reset_tx)
		dwFCR |= UART_FCR_TXFR;

	if(reset_rx)
		dwFCR |= UART_FCR_RXFR;
		
	WritePort32( (Base+UART_REG_FCR), (dwFCR & 0x00FE));
}

void UART_EnableLoopback(UINT32 Port, int OnOff)
{
	UINT32 dwMCR, dwUartBase;
	
	if ((Port>0)&(Port<4)){
		dwUartBase = UartCtrlBase[Port];
	   dwMCR = ReadPort32((dwUartBase+UART_REG_MCR));
	   if (OnOff != 0){
	   	dwMCR |= UART_MCR_LPBK;
	   }else{
	   	dwMCR &= (~UART_MCR_LPBK);
	   }
	   WritePort32((dwUartBase+UART_REG_MCR), dwMCR);
	}
}

void UART_WriteChar(UINT32 Port, char ch)
{
	UINT32 dwUartBase = UartCtrlBase[Port &0x03];
	UINT32 dwLSR;
	volatile UINT8  nLSR;
	
	do {
		dwLSR = ReadPort32((dwUartBase+UART_REG_LSR));
		nLSR  = ReadPort8((dwUartBase+UART_REG_LSR));
	}while(!((dwLSR & UART_LSR_THRE) == UART_LSR_THRE));
	WritePort32((dwUartBase+UART_REG_THR), (UINT32)ch);
}

char UART_ReadChar(UINT32 Port)
{
	UINT32 dwUartBase = UartCtrlBase[Port &0x03];
	UINT32 dwLSR, dwSR;
	
	
	do {
		dwLSR = ReadPort32((dwUartBase+UART_REG_LSR));
	}while (!((dwLSR & UART_LSR_DR)==UART_LSR_DR));
	dwSR = ReadPort32(dwUartBase + UART_REG_RBR);
	return (char)(dwSR & 0x00FF);
}



#define SERIAL_IER_DR                  0x1      	/* Data ready Enable */
#define SERIAL_IER_TE                  0x2      	/* THR Empty Enable */
#define SERIAL_IER_RLS                 0x4      	/* Receive Line Status Enable */
#define SERIAL_IER_MS                  0x8      	/* Modem Staus Enable */

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

#define SERIAL_LSR_THRE                0x20  



UINT32 GetUartStatus(UINT32 port)
{
    UINT32 status; 
    status=ReadPort32(port+SERIAL_LSR);
    return status; 
}

UINT32 IsThrEmpty(UINT32 status)
{
    if((status & SERIAL_LSR_THRE)==SERIAL_LSR_THRE)
        return TRUE;
    else
        return FALSE;  
}

UINT32 IsDataReady(UINT32 status)
{
    if((status & SERIAL_IER_DR)==SERIAL_IER_DR)
        return TRUE;
    else
        return FALSE;  

}

void CheckRxStatus(UINT32 port)
{
    UINT32 Status;
    do
    {
		Status = GetUartStatus(port);
	}
	while (!IsDataReady(Status));	// wait until Rx ready
}

void CheckTxStatus(UINT32 port)
{   
    UINT32 Status;
    do
    {
	    Status = GetUartStatus(port);	    
    }while (!IsThrEmpty(Status));	// wait until Tx ready	   
}

UINT32 fLib_kbhit(UINT32 port)
{
   	UINT32 Status;
	Status = GetUartStatus(port);
	if(IsDataReady(Status))
		return 1;
	else
		return 0;
}

char fLib_getch(UINT32 port)
{   
    char ch;    
	
	if(fLib_kbhit(port))
    	ch=ReadPort32(port+SERIAL_RBR);    
   	else
   		ch=0;
   		
    return ch;
}	

UINT32 DebugPort = 0x98900000;

char fLib_getchar()
{   
    char Ch;    
	
	CheckRxStatus(DebugPort);
    Ch = ReadPort32(DebugPort+SERIAL_RBR);    
    return (Ch);
}				

void fLib_putchar(char Ch)
{
    if(Ch!='\0')
    {        
        CheckTxStatus(DebugPort);
        WritePort32(DebugPort+SERIAL_THR,Ch);
    }
    
    if (Ch == '\n')
    {
	    CheckTxStatus(DebugPort);
        WritePort32( DebugPort+SERIAL_THR,'\r');
    }      
}


void fLib_putc(char Ch)
{
    CheckTxStatus(DebugPort);
    WritePort32(DebugPort+SERIAL_THR,Ch);
    
    if (Ch == '\n')
    {
	    CheckTxStatus(DebugPort);
        WritePort32(DebugPort+SERIAL_THR,'\r');
    }      
}

