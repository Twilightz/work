/***************************************************************************
* Copyright  Faraday Technology Corp 2002-2003.  All rights reserved.      *
*--------------------------------------------------------------------------*
* Name:DMA.c                                                               *
* Description: DMA C Library routine                                       *
* Author: Fred Chien                                                       *
****************************************************************************/
#include "dtype.h"
#include "MacroUtil.h"
#include "LibDMA.h"
#include "cpe.h"
#include "mm.h"

void hw_SetDMAChannelCfg(UINT8 Channel, Lib_DMA_CH_CSR_t Csr)
{

	UINT32 Chanenel_Base = CPE_DMA_BASE + DMA_CHANNEL0_BASE + Channel * DMA_CHANNEL_OFFSET;
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_CSR_OFFSET, *(UINT32 *)&Csr);	
}



void hw_DMA_CHIntMask(UINT8 Channel, Lib_DMA_CH_CFG_t Mask)
{
	
	UINT32 Chanenel_Base = CPE_DMA_BASE + DMA_CHANNEL0_BASE + Channel * DMA_CHANNEL_OFFSET;
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_CFG_OFFSET,  *(UINT32 *)&Mask);	

}


void hw_DMA_CHLinkList(UINT8 Channel, UINT32 LLP)
{

	UINT32 Chanenel_Base = CPE_DMA_BASE + DMA_CHANNEL0_BASE + Channel * DMA_CHANNEL_OFFSET;

		
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_LLP_OFFSET,  LLP);	
	
}





void hw_DMA_CHDataCtrl(UINT8 Channel, UINT32 SrcAddr, UINT32 DstAddr, UINT32 Size)
{
	
	UINT32 Chanenel_Base = CPE_DMA_BASE + DMA_CHANNEL0_BASE + Channel * DMA_CHANNEL_OFFSET;
	
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_SRCADDR_OFFSET, SrcAddr);
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_DSTADDR_OFFSET, DstAddr);
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_SIZE_OFFSET, Size);
	
}



void hw_InitDMA(UINT32 M0_BigEndian, UINT32 M1_BigEndian, UINT32 Sync)
{		
	
	UINT32 Endian = 0;
	

	Endian = (M0_BigEndian ? DMA_CSR_M0ENDIAN : 0) | 
	(M1_BigEndian ? DMA_CSR_M1ENDIAN : 0) | DMA_CSR_DMACEN;
	
	WriteUINT32(CPE_DMA_BASE + DMA_CSR, Endian);
	WriteUINT32(CPE_DMA_BASE + DMA_SYNC, Sync);
	
}



void hw_EnableDMAChannel(UINT8 Channel)
{
	
	UINT32 Chanenel_Base = (CPE_DMA_BASE+DMA_CHANNEL0_BASE+ Channel*DMA_CHANNEL_OFFSET);
	
	UINT32 reg;
	
	
	reg = ReadUINT32(Chanenel_Base + DMA_CHANNEL_CSR_OFFSET);
	reg |= DMA_CSR_CH_ENABLE;
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_CSR_OFFSET, reg);		
	
}



void hw_DisableDMAChannel(UINT8 Channel)
{
	
	UINT32 Chanenel_Base = (CPE_DMA_BASE+DMA_CHANNEL0_BASE+Channel*DMA_CHANNEL_OFFSET);
	UINT32 reg;
	
	
	reg = ReadUINT32(Chanenel_Base + DMA_CHANNEL_CSR_OFFSET);
	reg &= ~DMA_CSR_CH_ENABLE;
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_CSR_OFFSET, reg);
	

}

void hw_DMA_ResetChannel(UINT8 Channel)
{

	UINT32 Chanenel_Base = (CPE_DMA_BASE+DMA_CHANNEL0_BASE+Channel*DMA_CHANNEL_OFFSET);
	
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_CSR_OFFSET, 0);
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_CFG_OFFSET, 7);
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_SRCADDR_OFFSET, 0);
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_DSTADDR_OFFSET, 0);
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_LLP_OFFSET, 0);
	
	WriteUINT32(Chanenel_Base + DMA_CHANNEL_SIZE_OFFSET, 0);
	
}


void hw_ClearDMAChannelIntStatus(UINT8 Channel)
{
	
	
	
	WriteUINT32(CPE_DMA_BASE + DMA_INT_TC_CLR, (1 << Channel)); 
	//WriteUINT32(CPE_DMA_BASE + DMA_INT_ERRABT, (1 << Channel)); 
	WriteUINT32(CPE_DMA_BASE + DMA_INT_ERRABT_CLR, (1 << Channel)); 
	
	
}


void hw_DMA_ClearAllInterrupt(void)
{
	
	
	
	WriteUINT32((CPE_DMA_BASE + DMA_INT_TC_CLR), 0xFF);
	
	WriteUINT32((CPE_DMA_BASE + DMA_INT_ERRABT_CLR), 0xFF00FF);	
}


 

void hw_DMA_WaitIntStatus(UINT8 Channel)
{
	UINT32 choffset;
	
	choffset = 1 << Channel;
	
 
	
	
  while( (ReadUINT32(CPE_DMA_BASE + DMA_TC) & choffset) == 0); //polling mode
	
	
	while(!fDMA_IntFnsh);                                        // Interrupt mode
	 
	fDMA_IntFnsh = 0;
	
	    	
	hw_DisableDMAChannel(Channel);
}








void Lib_DMASet(	

UINT32 LinkAddr,
UINT32 Channel,   // use which channel for AHB DMA, 0..7
UINT32 SrcAddr,   // source begin address
UINT32 DstAddr,   // dest begin address
UINT32 Size,      // total bytes
UINT32 SrcWidth,  // source width 8/16/32 bits -> 0/1/2
UINT32 DstWidth,  // dest width 8/16/32 bits -> 0/1/2
UINT32 SrcSize,   // source burst size, How many "SrcWidth" will be transmmited at one times ?
UINT32 SrcCtrl,   // source address change : Inc/dec/fixed --> 0/1/2
UINT32 DstCtrl,   // dest address change : Inc/dec/fixed --> 0/1/2
UINT32 Priority,  // priority for this chaanel 0(low)/1/2/3(high)
UINT32 Mode       // Normal/Hardwire,   0/1
) 
{

	UINT32 LLPSize = 0x3ff * (1 << SrcWidth);
	
	UINT32 LLPCount = 0;  // total link-list node
	UINT32 Count = 0;
	UINT32 RmainderSize;
	//UINT32 *LinkAddr;  // Link-List address
	
	UINT32 LLPSrcAddr = 0;
	UINT32 LLPDstAddr = 0;

	Lib_DMA_CH_t DMAChannel;
	LLP_Desc *LLP;
	
	UINT32 i;	

	
	
	
		
	*(UINT32 *)&DMAChannel.csr = 0;	
	*(UINT32 *)&DMAChannel.llp = 0;	
	*(UINT32 *)&DMAChannel.cfg = 0;	
	
	
 	LLP = (LLP_Desc *)LinkAddr;
 
	
	LLPCount = (Size + LLPSize-1 ) / LLPSize - 1;;
	RmainderSize = Size % LLPSize;	
		
	
			
 

	
	/*
	printf(" LinkAddr = %x  LLPCount=%x\n",LLP, LLPCount);
	

	printf("Ch%d, Src=%08X, Dst=%08X, Size=%08X SrcWidth=%db, DstWidth=%db, SrcSize=%dB\n"
           "SrcCtrl=%s, DstCtrl=%s, Priority=%d, Mode=%s, LLPCnt = %d\n",
	       Channel, SrcAddr, DstAddr, Size, 1 << (SrcWidth + 3), 1 << (DstWidth + 3), 
	       ((SrcSize == 0) ? 1 : 1 << (SrcSize+1)), 
	       ((SrcCtrl == 0) ? "Inc" : ((SrcCtrl == 1) ? "Dec" : "Fix")),
	       ((DstCtrl == 0) ? "Inc" : ((DstCtrl == 1) ? "Dec" : "Fix")),
	       Priority, ((Mode == 0) ? "Normal" : "HW"), LLPCount);
	 */      
	       
	       
	
	

	       
	if (LLPCount) {
	
		
		LLPSrcAddr = SrcAddr + LLPSize;
		LLPDstAddr = DstAddr + LLPSize;
	
	
		Count = LLPCount;
		
		if(RmainderSize)
			Count--;
	
		
		
		
	 
		
		i=0;
		
		while(Count--){
			
			
			LLP[i].src_addr = LLPSrcAddr;
		  	LLP[i].dst_addr = LLPDstAddr;
		  	
		 
	 		if(SrcCtrl == 0)  // increase
				 LLPSrcAddr += LLPSize;
		  
		  	else if(SrcCtrl == 1) // decrease
		  
				 LLPSrcAddr -= LLPSize;
				 
			if(DstCtrl == 0)  // increase
				 LLPDstAddr += LLPSize;
		  
		  	else if(DstCtrl == 1) // decrease
		  
				 LLPDstAddr -= LLPSize;	 
		  
		  
		  
		  	*((UINT32 *)&(LLP[i].llp)) = 0;
		  	
		  	if(i)
		  		*((UINT32 *)&(LLP[i-1].llp)) = (UINT32 )&LLP[i];
		  		
		  	
		
		  	*((UINT32 *)&(LLP[i].llp_ctrl)) = 0;
		  	LLP[i].llp_ctrl.tc_msk = 1;
		  	LLP[i].llp_ctrl.src_width = SrcWidth; 		/* source transfer size */
		  	LLP[i].llp_ctrl.dst_width = DstWidth; 		/* destination transfer size */
		  	LLP[i].llp_ctrl.src_ctrl = SrcCtrl; 		/* source increment, decrement or fix */
		  	LLP[i].llp_ctrl.dst_ctrl = DstCtrl; 		/* destination increment, decrement or fix */
		  	LLP[i].llp_ctrl.src_sel = 0; 				/* source AHB master id */
		  	LLP[i].llp_ctrl.dst_sel = 0; 				/* destination AHB master id */
		
		  	LLP[i].llp_ctrl.size = LLPSize / (1 << SrcWidth);
			
			i++;
			
		}
		
		if(RmainderSize){
		
			LLP[i].src_addr = LLPSrcAddr;
		  	LLP[i].dst_addr = LLPDstAddr;
		
			*((UINT32 *)&(LLP[i].llp)) = 0;
			
			
			if(i)
		
		  		*((UINT32 *)&(LLP[i-1].llp)) = (UINT32 )&LLP[i];
			
			*((UINT32 *)&(LLP[i].llp_ctrl)) = 0;
		  	LLP[i].llp_ctrl.tc_msk = 0;
		  	LLP[i].llp_ctrl.src_width = SrcWidth; 		/* source transfer size */
		  	LLP[i].llp_ctrl.dst_width = DstWidth; 		/* destination transfer size */
		  	LLP[i].llp_ctrl.src_ctrl = SrcCtrl; 		/* source increment, decrement or fix */
		  	LLP[i].llp_ctrl.dst_ctrl = DstCtrl; 		/* destination increment, decrement or fix */
		  	LLP[i].llp_ctrl.src_sel = 0; 				/* source AHB master id */
		  	LLP[i].llp_ctrl.dst_sel = 0; 				/* destination AHB master id */
		
		  	LLP[i].llp_ctrl.size = RmainderSize / (1 << SrcWidth);
			i++;
		}		
	
	
		LLP[i-1].llp_ctrl.tc_msk = 0;
	
		/*
	
		for(i = 0; i < LLPCount;i++){
		
			printf("src=%x, dst=%x, link=%0.8X, ctrl=%.8X\n", LLP[i].src_addr, LLP[i].dst_addr,
			*(UINT32 *)&(LLP[i].llp), *(UINT32 *)&(LLP[i].llp_ctrl));

		}		
		*/
		
		
	
	 }
	 
	 
	 hw_ClearDMAChannelIntStatus(Channel);
	 
	 if (LLPCount) {
	       
		/* program channel CSR */
	   DMAChannel.csr.tc_msk = 1; 					/* enable terminal count */
	   DMAChannel.csr.reserved0 = 0;
	   DMAChannel.csr.priority = Priority; 			/* priority */
	   DMAChannel.csr.prot = 0; 					/* PROT 1-3 bits */
	   DMAChannel.csr.src_size = SrcSize; 			/* source burst size */
	   DMAChannel.csr.abt = 0; 						/* NOT transaction abort */
	   DMAChannel.csr.reserved1 = 0;
	   DMAChannel.csr.src_width = SrcWidth; 		/* source transfer size */
	   DMAChannel.csr.dst_width = DstWidth; 		/* destination transfer size */
	   DMAChannel.csr.mode = Mode; 					/* Normal mode or Hardware handshake mode */
	   DMAChannel.csr.src_ctrl = SrcCtrl;			/* source increment, decrement or fix */
	   DMAChannel.csr.dst_ctrl = DstCtrl; 			/* destination increment, decrement or fix */
	   DMAChannel.csr.src_sel = 0;		 			/* source AHB master id */
	   DMAChannel.csr.dst_sel = 0; 					/* destination AHB master id */

	   hw_SetDMAChannelCfg(Channel, DMAChannel.csr);
	
		/* program channel CFG */
	   												//john DMAChannel.cfg.int_tc_msk = 1;	// Disable tc status
	   DMAChannel.cfg.int_tc_msk = 0;				// Enable tc status
	   DMAChannel.cfg.int_err_msk = 0;
	   DMAChannel.cfg.int_abt_msk = 0;
	
	   hw_DMA_CHIntMask(Channel, DMAChannel.cfg);
	
	   /* program channel llp */
	 	   
	   *((UINT32 *)&(DMAChannel.llp)) = ((UINT32 )(&LLP[0] ));
	   
	   hw_DMA_CHLinkList(Channel, DMAChannel.llp);
	
	   /* porgram address and size */
	   hw_DMA_CHDataCtrl(Channel, SrcAddr, DstAddr, LLPSize / (1 << SrcWidth));
	 }	
	else{
	
	   /* program channel CSR */
	   DMAChannel.csr.tc_msk = 0; 					/* no LLP */
	   DMAChannel.csr.reserved0 = 0;
	   DMAChannel.csr.priority = Priority; 			/* priority */
	   DMAChannel.csr.prot = 0; 					/* PROT 1-3 bits */
	   DMAChannel.csr.src_size = SrcSize; 			/* source burst size */
	   DMAChannel.csr.abt = 0; 						/* NOT transaction abort */
	   DMAChannel.csr.reserved1 = 0;
	   DMAChannel.csr.src_width = SrcWidth; 		/* source transfer size */
	   DMAChannel.csr.dst_width = DstWidth; 		/* destination transfer size */
	   DMAChannel.csr.mode = Mode; 					/* Normal mode or Hardware handshake mode */
	   DMAChannel.csr.src_ctrl = SrcCtrl; 			/* source increment, decrement or fix */
	   DMAChannel.csr.dst_ctrl = DstCtrl; 			/* destination increment, decrement or fix */
	   DMAChannel.csr.src_sel = 0; 					/* source AHB master id */
	   DMAChannel.csr.dst_sel = 0; 					/* destination AHB master id */

	   hw_SetDMAChannelCfg(Channel, DMAChannel.csr);
	
	   /* program channel CFG */
	   DMAChannel.cfg.int_tc_msk = 1;				// 	1 Disable tc interrupt
	   DMAChannel.cfg.int_err_msk = 0;
	   DMAChannel.cfg.int_abt_msk = 0;
	
	   hw_DMA_CHIntMask(Channel, DMAChannel.cfg);
	
	   /* program channel llp */
	   //*((UINT32 *)&(DMAChannel.llp)) = 0;
	   hw_DMA_CHLinkList(Channel, DMAChannel.llp);
	
	   /* porgram address and size */
	   hw_DMA_CHDataCtrl(Channel, SrcAddr, DstAddr, Size / (1 << SrcWidth));
	}

	

}




void SetDMA_Run(
	
UINT32 Channel,   // use which channel for AHB DMA, 0..7
UINT32 SrcAddr,   // source begin address
UINT32 DstAddr,   // dest begin address
UINT32 Size,      // total bytes
UINT32 SrcWidth,  // source width 8/16/32 bits -> 0/1/2
UINT32 DstWidth,  // dest width 8/16/32 bits -> 0/1/2
UINT32 SrcSize,   // source burst size, How many "SrcWidth" will be transmmited at one times ?
UINT32 SrcCtrl,   // source address change : Inc/dec/fixed --> 0/1/2
UINT32 DstCtrl,   // dest address change : Inc/dec/fixed --> 0/1/2
UINT32 Priority,  // priority for this chaanel 0(low)/1/2/3(high)
UINT32 Mode       // Normal/Hardwi
)
{
	
	
	
	UINT32 MaxLLPSize = (0x3ff * (1 << SrcWidth) ) ;
	
	UINT32 MaxLLPNode = 0; 
	UINT32 *LinkAddr = 0;
	UINT32 DMASize = 0;	
	
	
	hw_DMA_ResetChannel(AHBDMA_CHANNEL);
	hw_InitDMA(FALSE, FALSE, 0x0);


	
	
	MaxLLPNode = (DMAPageSize + MaxLLPSize -1 ) / MaxLLPSize - 1;;
	LinkAddr = (UINT32 *)fLib_NC_malloc(sizeof(LLP_Desc ) * MaxLLPNode);
		
	
	while(Size){
	
		if(Size >= DMAPageSize) 
			DMASize = DMAPageSize;
			
		else
			DMASize = Size;
			
		
	
	
		Lib_DMASet((UINT32)LinkAddr, Channel, SrcAddr, DstAddr, DMASize, 
				  SrcWidth, DstWidth,SrcSize, SrcCtrl, DstCtrl, Priority, Mode );
	
		hw_EnableDMAChannel(Channel);	
		hw_DMA_WaitIntStatus(Channel);
		
	
		if(SrcCtrl == 0)  		// increase
			 SrcAddr += DMASize;
		  
	  	else if(SrcCtrl == 1) 	// decrease
		  
			 SrcAddr -= DMASize;
				 
		if(DstCtrl == 0)  		// increase
			 DstAddr += DMASize;
		  
	  	else if(DstCtrl == 1) 	// decrease
		  
			 DstAddr -= DMASize;	 
	
		Size -= DMASize;
		
	}
	
	fLib_NC_free(LinkAddr);
	hw_DMA_ResetChannel(AHBDMA_CHANNEL);
	hw_DMA_ClearAllInterrupt();

   
}








BOOL Set_DMA_Byte(UINT32 SrcAddr, UINT32 DstAddr, UINT32 Size, UINT32 SrcCtrl, UINT32 DstCtrl)
{

	// source & Destnation Control :
	// Inc/dec/fixed --> 0/1/2
	
	// DMA Transaction width 8/16/32 ---> 0/1/2
	UINT32 SrcWidth = 0;
	UINT32 DstWidth = 0;


	
	SetDMA_Run(AHBDMA_CHANNEL, SrcAddr, DstAddr, Size, 
               SrcWidth, DstWidth, DMASrcSize, SrcCtrl, DstCtrl, DMAPriority, DMAMode );


	return TRUE;
	
}


BOOL Set_DMA_Word(UINT32 SrcAddr, UINT32 DstAddr, UINT32 Size, UINT32 SrcCtrl, UINT32 DstCtrl)
{

	// source & Destnation Control :
	// Inc/dec/fixed --> 0/1/2
	
	// DMA Transaction width 8/16/32 ---> 0/1/2
	UINT32 SrcWidth = 1;
	UINT32 DstWidth = 1;


	
	SetDMA_Run(AHBDMA_CHANNEL, SrcAddr, DstAddr, Size, 
               SrcWidth, DstWidth, DMASrcSize, SrcCtrl, DstCtrl, DMAPriority, DMAMode );


	return TRUE;
	
	
}

BOOL Set_DMA_DWord(UINT32 SrcAddr, UINT32 DstAddr, UINT32 Size, UINT32 SrcCtrl, UINT32 DstCtrl)
{

	// source & Destnation Control :
	// Inc/dec/fixed --> 0/1/2
	
	// DMA Transaction width 8/16/32 ---> 0/1/2
	UINT32 SrcWidth = 2;
	UINT32 DstWidth = 2;

	
	SetDMA_Run(AHBDMA_CHANNEL, SrcAddr, DstAddr, Size, 
               SrcWidth, DstWidth, DMASrcSize, SrcCtrl, DstCtrl, DMAPriority, DMAMode );


	return TRUE;
	
	
}









