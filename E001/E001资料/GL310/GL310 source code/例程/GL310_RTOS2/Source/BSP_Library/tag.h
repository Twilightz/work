/***************************************************************************
* Name:TAG.H				                                               *
* Description: SYSTEM TAG DEFINE                						   *
*																		   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2007/03/02	                                                       *
****************************************************************************/
#ifndef _TAG_H_
#define _TAG_H_

#include <stdio.h>
#include <string.h>

 
#include "dtype.h"				//Variable type define
#include "spiboot.h"

#define SPI_DummyBase	0xA0000010




#define TagIDSize		32

#define TagBase 		SpiParam->TagBaseAddr  
#define TagNum 			SpiParam->TagBaseSize / TagIDSize 
#define hwTotalBlockNum	(SpiParam->flash_i.ChipSize * 0x100000) / (SpiParam->flash_i.BlkSize)



#define ReserverBlockNum  	1

#define BootTagID 		0
#define RecoverTagID  	8 
#define BootloaderTagID	8 


typedef struct _MemID_Tag{

	UINT32  ID;
	UINT32  StartOff;
	UINT32  Len;
	UINT32  Checksum;
	UINT16  StartBlock;
	UINT16  BlockNum;
	INT8    IDString[12];	

} MemID_Tag;



typedef struct _Spi_WRParam{


	UINT8  bCmd;
	UINT8  nDummybit;
	UINT8  ToogleBit;
	UINT8  nChannel;
		
}Spi_WRParam;


typedef struct _chip_info{

	UINT16	BlkSize;	
	UINT16	ChipSize;

}chip_info;



typedef struct _Spi_Param{

	UINT32  signature;
	UINT32  SystemState;

	UINT32  IDType;
	chip_info  flash_i;
		
	Spi_WRParam	wrf;
	Spi_WRParam	rdf;
	
	
	UINT32  TagBaseAddr;
	UINT32  TagBaseSize;
	
	UINT32  AppImgTagID;
	UINT32  DDRTagID;
	
	UINT32  SYSCLK;
	UINT32  MemDelay;

	UINT32  ChipChecksum;
	UINT32  ClkEnable;
	UINT32  IntSelect;
	UINT32  ImageRemainSize;
	
	
}Spi_Param;


extern UINT8 *gTmp;
extern UINT8 *xTmp;
extern UINT8 *gData;

extern Spi_Param SPIParaBase;
extern Spi_Param *SpiParam;

extern UINT8 *hw_sys_buf;
extern UINT8 *TagIDBaseBuf;
extern UINT8 *hwBlockIndexBufBase;
extern UINT8 *SPIParamTblBaseBuf;;
extern UINT32 hwfreeBlockNum;

void hw_sys_buf_initial(void);
void hw_sys_buf_release(void);

BOOL hw_BlockIndexMapping(void);
BOOL hw_BlockIndexFill(UINT32 *BlockIndexBuf, UINT16 StartBlockIndex, UINT16 BlockNum, UINT8 USED);
BOOL hw_FreeBlockGet(UINT32 *BlockIndexBuf, UINT16 *StartFreeBlock, UINT16 BlockNum);
BOOL hw_sys_Tag_Initialize(void);



#endif 

