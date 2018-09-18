#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "tag.h"
#include "mm.h"
#include "libspic_intf_a.h"
#include "libspic_intf_b.h"



// whole system reference
Spi_Param *SpiParam;
Spi_Param SPIParaBase;
UINT8 *hw_sys_buf;

// SPI Flash reference
UINT8 *hwBlockIndexBufBase;
UINT8 *SPIParamTblBaseBuf;
UINT8 *TagIDBaseBuf;
UINT32 hwfreeBlockNum = 0;;



 
/************************************************************
 *		void hw_sys_buf_initial()
 ************************************************************/
void hw_sys_buf_initial()
{
	hw_sys_buf = (UINT8 *)fLib_NC_malloc(0x1000);	
}

/************************************************************
 *		void hw_sys_buf_release()
 ************************************************************/
void hw_sys_buf_release()
{
	fLib_NC_free(hw_sys_buf);	
}
 

/************************************************************
 *		BOOL hw_FreeBlockGet()
 * 
 ************************************************************/
BOOL hw_FreeBlockGet(UINT32 *BlockIndexBuf, UINT16 *StartFreeBlock, UINT16 BlockNum)
{
 

	UINT32 bitmap;
	UINT32 bitmapIndex = 0;
	UINT16 BlockIndex;
	UINT16 freeBlockCnt = 0;
			
	bitmap = *BlockIndexBuf;
		
	for(BlockIndex = 0; BlockIndex <hwTotalBlockNum; BlockIndex++){
		
		
		if( !( bitmap & ( 1<< bitmapIndex) )	){
			
			if(!freeBlockCnt)
				*StartFreeBlock = BlockIndex;			
			
			freeBlockCnt++;
			
			if(freeBlockCnt == BlockNum){
			    hwfreeBlockNum -= BlockNum;
				return true;
		}
				
		}
		else		
			freeBlockCnt = 0;
		
		bitmapIndex++;
				
		if(  !(bitmapIndex % 32) && (BlockIndex) ){
			
			bitmapIndex = 0;
			BlockIndexBuf++;
			bitmap = *BlockIndexBuf;
		}
		
	}
	
	return false;
}

/************************************************************
 *		BOOL hw_BlockIndexFill()
 * 
 ************************************************************/
BOOL hw_BlockIndexFill(UINT32 *BlockIndexBuf, UINT16 StartBlockIndex, UINT16 BlockNum, UINT8 USED)
{


	UINT16 IndexGrp = StartBlockIndex / 32;
	UINT16 IndexBit = StartBlockIndex % 32;
	UINT32 bitmap;
	
	if( (StartBlockIndex + BlockNum) > hwTotalBlockNum)
		return false;
	
	
	BlockIndexBuf += IndexGrp;
	bitmap = *BlockIndexBuf;
	
	while(BlockNum--){
		
		if(USED)
			bitmap |=   ( 1 <<IndexBit );	
		
		else
			bitmap &=  ~( 1 <<IndexBit );	
		
				
		IndexBit++;
		
		if(IndexBit == 32){
		
			IndexBit = 0;			
			*BlockIndexBuf = bitmap;			
			
			BlockIndexBuf++;		
			bitmap = *BlockIndexBuf;
		}
	}
	
	*BlockIndexBuf = bitmap;
	return true;
}

/************************************************************
 *		BOOL hw_BlockIndexMapping()
 * 
 ************************************************************/
BOOL hw_BlockIndexMapping()
{
	
	
	MemID_Tag *TagIDIndex;
	UINT8 TagCnt = TagNum;
		
	
	hwfreeBlockNum = hwTotalBlockNum;		
	
	TagIDIndex = (MemID_Tag *)(UINT32)(TagIDBaseBuf);

	while(TagCnt--){
	
	
		if(TagIDIndex->ID & 0x80000000){	//The MSB(bit 31)use to verify the Tag ID to use already?
		
			if(TagIDIndex->StartBlock){
								
				if( !hw_BlockIndexFill((UINT32 *)hwBlockIndexBufBase, TagIDIndex->StartBlock, TagIDIndex->BlockNum, 1)	)				
					return false;		
				
				hwfreeBlockNum -= TagIDIndex->BlockNum;
			}
		}
	
		
		TagIDIndex++;
	}
	
	if( !hw_BlockIndexFill((UINT32 *)hwBlockIndexBufBase, 0, 1, 1) )	// blocking the first block to use by SPI flash table and tags	
		return false;	
		
	hwfreeBlockNum -= 1;

	return true;
	
}



/************************************************************
 *		BOOL hw_sys_Tag_Initialize()
 *
 * This stage only fetch system parameter and tag info
 * In this case, SPI Flash interface have not startup....
 ************************************************************/
BOOL hw_sys_Tag_Initialize()
{

	MemID_Tag *RecoverTag; 	
	UINT8    	CmdBuf[4];
    Spi_Str  	SpiStr; 
	
		
	// allocate ststem demend buf for system table	
 	hw_sys_buf_initial();
 	
 	
 	if( !hw_sys_buf)
 		return FALSE;
 	
 	// the defaut system info store in the master SPI Flash(SPI interface A, SPIC_Intf_A)	
	SpiParam = (Spi_Param *)&SPIParaBase;
	
	SPIParamTblBaseBuf = hw_sys_buf;				
	
	/* 
	memcpy(SpiParam, (UINT8 *)SPI_DummyBase,  sizeof(Spi_Param) );
	
  	SpiWRCmd = (Spi_WRParam *)&SpiParam->rdf; 
   				
	
	// calculte how many block number in SPI FLASH of board level mount 
	hwBlockIndexBufBase = (UINT8 *)malloc(hwTotalBlockNum / 8);		
	printf("hwTotalBlockNum=%x\n",hwTotalBlockNum);
	memset(hwBlockIndexBufBase, 0, hwTotalBlockNum / 8);
	
	// mapping the Tag base address in system define
	SPIParamTblBaseBuf = hw_sys_buf;				
	TagIDBaseBuf = SPIParamTblBaseBuf + SpiParam->TagBaseAddr;
    */
 

	
	hw_SPIC_Intf_A_Initial();
	hw_SPIC_Intf_B_Initial();

    
 	// the first 0x1000 bytes is system infomation of SPI Flash interface A
    //CmdBuf[0] = SpiWRCmd->bCmd;     
    CmdBuf[0] = 0x3;    // spi general read command 
    CmdBuf[3] = 0;				
    CmdBuf[2] = 0;
    CmdBuf[1] = 0;
    
    // Setup structure for reading
    SpiStr.CmdPtr   = CmdBuf;
    SpiStr.CmdLen   = 4;
    SpiStr.rwPtr    = SPIParamTblBaseBuf;
    SpiStr.rwLen    = 0x1000;
    SpiStr.Read     = 1;
    SpiStr.CheckSum = 0;
        
   
  	SpiStr.nChannel    = 1;
   	SpiStr.mAddrToggle = 0;
   	SpiStr.ndummybit   = 0; 	
	
	hw_SPIC_Intf_A(&SpiStr);	

	//SPIC_UnProctect();
 	
    // mapping the SPIC parameter configuration 		
 	memcpy(SpiParam, (UINT8 *)SPIParamTblBaseBuf,  sizeof(Spi_Param) );
 	
	
	// calculte how many block number in SPI FLASH of board level mount 
	hwBlockIndexBufBase = (UINT8 *)malloc(hwTotalBlockNum / 8);		
	printf("hwTotalBlockNum=%x\n",hwTotalBlockNum);
	memset(hwBlockIndexBufBase, 0, hwTotalBlockNum / 8);
	
	// mapping the Tag base address in system define
	TagIDBaseBuf = SPIParamTblBaseBuf + SpiParam->TagBaseAddr;	
 		
 		
	// mapping the free and used block index
	if( !hw_BlockIndexMapping() ){
		
		printf("SPI Tag area initialize error, system halt\n");
		hw_sys_buf_release();
		return FALSE;
	}
	
	// reserver Tag8 to recover tag 
	RecoverTag= (MemID_Tag *)(TagIDBaseBuf + RecoverTagID * sizeof(MemID_Tag) );
	RecoverTag->ID = RecoverTagID | 0x80000000; 
	
	// register the SPIC interface A interrupt handle of data transfer finish
	hw_SPIC_event_funcb_register(0, hw_SPI_Intf_A_Flag_Set, hw_SPI_Intf_A_Wait_interrupt);
	hw_SPIC_Intf_A_Interrupt_initial(); 
 

	return TRUE;
	
}
