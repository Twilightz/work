#include "dtype.h"
#include "libspic_intf_a.h"
#include "tag.h"
#include "LibIntrc.h"

UINT8 hw_SPIC_Intf_A_flag;
void hw_SPIC_Intf_B_Set(UINT8 Enable);

void hw_setSPIClkDivder(UINT8 CLKDivder)
{
	xrSPIParm &= 0xfffffffc;
	xrSPIParm |= CLKDivder;
}


void hw_setSPICnChannel(UINT8 nChannel)
{
	xrSPIParm &= 0xffffffCF;
	xrSPIParm |= (nChannel >> 1) << 4;
}


void hw_setSPINTE(UINT8 IntE)
{

	if(IntE)
		xrSPIParm |= (UINT32)(0x1 << 31);
	else
		xrSPIParm &= ~((UINT32)(1 << 31));
	
	return;
}


void hw_InitSPIC(void)
{
	hw_setSPIClkDivder(0);
	hw_setSPICnChannel(1);
	
}


void hw_setSPICmdRegH(UINT8 iChannel, UINT32 TmpCmdBuf)
{
	
	UINT32 *nCmdRegHPtr;
	
	
	nCmdRegHPtr = (UINT32 *)(SPInCmdBase + iChannel * 16 + 4); 
	*nCmdRegHPtr |= TmpCmdBuf;
	
}

void hw_setSPICmdRegL(UINT8 iChannel, UINT32 TmpCmdBuf)
{
	
	UINT32 *nCmdRegHPtr;
		
	nCmdRegHPtr = (UINT32 *)(SPInCmdBase + iChannel * 16); 
	*nCmdRegHPtr |= TmpCmdBuf;
	
}


void hw_setSPICmdDirReg(UINT8 iChannel, UINT8 startpos, UINT8 endpos, UINT8 BitSet)
{	 	
 	 
 	UINT64 iChannelDirMap = 0; 
 	
	
	if( (startpos <= 63) && (endpos <= 63) ){
	
		iChannelDirMap  =  ( (UINT64) *((UINT32 *)( SPInDirHBase +iChannel * 16)) ) << 32; 
		iChannelDirMap |= *( (UINT32 *)( SPInDirLBase +iChannel * 16) ); 		
	
	
		while(startpos <= endpos){
		
			if(BitSet)
				iChannelDirMap |= (( (UINT64)0x1 ) <<  (63 -  startpos) );
			else
				iChannelDirMap &= ~(( (UINT64)0x1 ) << (63 -  startpos) );
			
			startpos++;
			
		}		
			
		*((UINT32 *)( SPInDirLBase +iChannel * 16) ) = iChannelDirMap;
		*((UINT32 *)( SPInDirHBase +iChannel * 16)) = iChannelDirMap >> 32;			
	} 
}



void hw_setSPIWrEnable(void)
{
	xrSPICtrl = 0x2;
}

void hw_setSPIRdEnable(void)
{
	xrSPICtrl = 0x1;
}



void hw_setSPICmdLen(UINT32 CmdBitLen)
{
	xrSPITransLen &= 0xffffff00;
	xrSPITransLen |= CmdBitLen;
}


void hw_setSPIDataLen(UINT32 DataByteLen)
{
	
	xrSPITransLen &= 0x000000ff;
	xrSPITransLen |= (DataByteLen << 8);
}

void hw_setSPIOPCode(UINT32 OPCode)
{
	
	xrSPInC1CmdRegH &= 0x00ffffff;
	xrSPInC1CmdRegH |= (OPCode << nMaxAddrBit);
}


void hw_setDataPoolAddr(UINT32 Addr)
{
	xrSPIAMBAAddr = Addr;
}


void hw_spiSetAddr(Spi_Str *SpiStrTmp)
{

	 
	UINT32 nChannel = 0;
	UINT32 iChannel = 0;
	UINT32 BitIndex	= 0;
	UINT32 TmpCmdBuf = 0;
	UINT32 SetAddr = 0;
	UINT32 BitMap = 0;
	UINT32 channelBitMap = 0;
	 
	
	
	if(SpiStrTmp->CmdLen <= 1){
	
		hw_setSPICmdLen(nMaxOPCmdBit + SpiStrTmp->ndummybit);
		hw_setSPICnChannel(1);
	}
	else{
	
		nChannel = SpiStrTmp->nChannel;
		
		SetAddr = 0;
		
		SetAddr |= ( (UINT32)SpiStrTmp->CmdPtr[1] ) << 16;
		SetAddr |= ( (UINT32)SpiStrTmp->CmdPtr[2] ) << 8;
		SetAddr |= ( (UINT32)SpiStrTmp->CmdPtr[3] );
		
	
		
		if(SpiStrTmp->mAddrToggle){
			
			iChannel =  nChannel;
	
			while(iChannel){
		
				TmpCmdBuf = 0;
				
				channelBitMap = 0x00800000;
		
				BitMap = 0x00800000 >> (nChannel - iChannel);		
		
				for(BitIndex=0; BitIndex < (nMaxAddrBit / nChannel); BitIndex++){
			
					if(SetAddr & BitMap){
					
						//TmpCmdBuf |= (SetAddr & BitMap);
						TmpCmdBuf |= channelBitMap;
						
					}
						
			
					BitMap = BitMap >> nChannel;
					channelBitMap = channelBitMap >> 1;
				}
		
				iChannel--;
			
				hw_setSPICmdRegH(iChannel, TmpCmdBuf);				// set xxAddr 
			
			}
		
			hw_setSPICmdLen(nMaxOPCmdBit + nMaxAddrBit / nChannel + SpiStrTmp->ndummybit);
		}
		else{
			
			TmpCmdBuf = 0;
			TmpCmdBuf |= SetAddr;
			
			hw_setSPICmdRegH(0, TmpCmdBuf);						// set xxAddr 
			hw_setSPICmdLen(nMaxOPCmdBit + nMaxAddrBit + SpiStrTmp->ndummybit);
		}
			
		
		// Judgement how the channel is! Channel by Data nChannel and Addr toggle bits
		if( (SpiStrTmp->rwLen == 0) && (!SpiStrTmp->mAddrToggle) )
			hw_setSPICnChannel(1);	
	
		else
			hw_setSPICnChannel(nChannel);	
		
	}
	  
}





void hw_spiSetDir(Spi_Str *SpiStrTmp)
{
	

    memset((UINT8 *)SPInCmdBase ,0x0, 4 * 16); 	
 	hw_setSPICmdDirReg(0, 0, 63, 1);		
 	
 
 	if(SpiStrTmp->nChannel != 4){
 	
 	
 		hw_setSPICmdDirReg(2, 0, 63, 1);
		hw_setSPICmdRegH(2, 0xffffffff);
		hw_setSPICmdRegL(2, 0xffffffff);
			
		hw_setSPICmdDirReg(3, 0, 63, 1);	
		hw_setSPICmdRegH(3, 0xffffffff);
		hw_setSPICmdRegL(3, 0xffffffff);
 	
 	
 			
		if( (SpiStrTmp->nChannel == 2) && (SpiStrTmp->mAddrToggle) )
			hw_setSPICmdDirReg(1, 8, 19, 1);		// 2 channel && toggle
	}	
 	else{
 		
 		if(SpiParam->IDType == 0x1){
 		 		
 			hw_setSPICmdDirReg(0, 32, 39, 0);		
 			
 		}
 			
 		if(SpiStrTmp->mAddrToggle){
		
			hw_setSPICmdDirReg(1, 8, 15, 1);		// 4 channel and Address toggle 
			hw_setSPICmdDirReg(2, 8, 15, 1);	
			hw_setSPICmdDirReg(3, 8, 15, 1);	
			
		}
	} 
}




BOOL hw_chkSPICtrlFinish()									// check SPI controler transfer finish status
{

	spi_transfer *spi_t = &spi_intf_tbl[0];

	while(1){
		
		if(spi_t->SPI_Fnsh_WAIT() )
			break;		
	}

	return TRUE;
} 



void hw_SPIC_Intf_A(Spi_Str *spistr)
{
	
		
    hw_spiSetDir(spistr);    
    
    hw_spiSetAddr(spistr);
    
    hw_setSPIOPCode(*(UINT8 *)spistr->CmdPtr);
   
 
   	 
    xrSPIChecksum = spistr->CheckSum;
    
    if(spistr->rwLen){
    
    	
    
    	hw_setSPIDataLen(spistr->rwLen);
    	
    	hw_setDataPoolAddr((UINT32)spistr->rwPtr);	
    
    	if(spistr->Read)
    		hw_setSPIRdEnable();
    	else
    		hw_setSPIWrEnable();
    }
    else{
    
    	hw_setSPIDataLen(0); 	
    	hw_setSPIWrEnable();    	
    }
    
    
    
    while(!hw_chkSPICtrlFinish() );
    
    spistr->CheckSum = xrSPIChecksum;

 
	// SPI - write checksum error patch     
    if( spistr->rwLen && (!spistr->Read) ){
    
    	spistr->CheckSum = xrSPIChecksum -  *(UINT8*)(spistr->rwPtr + 0x100);
    }
      
	
}


BOOL hw_SPI_Intf_A_Wait_interrupt()
{


	
	if( !hw_SPIC_Intf_A_flag)
		return FALSE;	
	 
	hw_SPIC_Intf_A_flag = 0;
	
	return TRUE;	
}

BOOL hw_SPI_Intf_A_Wait_loop()
{

	if( !xrSPIFlag){
		
		return FALSE;
	}

	xrSPIFlag = 0x1;
 
	
	return TRUE;	
}

BOOL hw_SPI_Intf_A_Flag_Set()
{
 
	hw_SPIC_Intf_A_flag = 1;
	return TRUE;
}


void hw_SPIC_ISR()
{
	spi_transfer *spi_t = &spi_intf_tbl[0];
	
	xrSPIFlag = 0x1;
	
	spi_t->SPI_Fnsh_SET();			
	
	hw_ClearIntStatus(IRQ_SPI_HOST);
}
 

 
void hw_SPIC_Intf_A_Interrupt_initial() 
{

	hw_SPIC_Intf_A_flag = 0;

	hw_ClearIntStatus(IRQ_SPI_HOST);
	
	RegisterISR(IRQ_SPI_HOST, hw_SPIC_ISR);	
	
	SetInterruptConfig(IRQ_SPI_HOST, 0, 1);
	
	hw_SetIntMask(IRQ_SPI_HOST);

	hw_setSPINTE(1);											


} 




void hw_SPIC_Intf_A_Initial()
{

	spi_transfer *spi_t = &spi_intf_tbl[0];
	
	spi_t->SPI_Fnsh_SET  = hw_SPI_Intf_A_Flag_Set;
	spi_t->SPI_Fnsh_WAIT = hw_SPI_Intf_A_Wait_loop;
	hw_SPIC_Intf_B_Set(false); // makesure the GPIO 38/39 select the SPIC_Inrf_A
 
}





















