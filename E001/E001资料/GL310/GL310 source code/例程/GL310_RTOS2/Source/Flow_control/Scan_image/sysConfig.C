#include "def.h"
#include "sysConfig.h"
#include "usrConfig.h"
#include "spi_tag.h" 
#include "UDev_CstApi.h"   


/************************************************************************
 *		BOOL sysConfig_initialize(TX_BYTE_POOL *stack_pool)
 * 
 ************************************************************************/
 
//HL modify
void DS_IO_01(void)
{
	unsigned int i,iRegNum, pri_reg[] = {
		0xb0000050, 0x0071271f, 
		0xb0000054, 0x000000a8, 
		0xb0000058, 0x20cfff00, 
		0xb000005c, 0x0016b7f8, 
		0xb0000060, 0x06921580, 
		0xb0000064, 0x00004000, 
		0xb0000068, 0x04050000, 

		//0xa0000058, 0x20cfff00, 
		//0xa000005c, 0x0016b7f8, 
		//0xa0000068, 0x04050000, 
	};

	iRegNum=sizeof(pri_reg)/sizeof(unsigned int);
	for (i=0; i<iRegNum; i=i+2) {
		*((volatile UINT32 *)pri_reg[i]) = pri_reg[i+1];
	}
}


void DS40_IO(void)
{
	UINT32 nGpio = 0;
	 // set GPIO88 to low
	nGpio = *((volatile UINT32 *)0xA0000068);
	nGpio &= 0xfffff3ff;
	*((volatile UINT32 *)0xA0000068) = nGpio;	 
	 
	nGpio = *((volatile UINT32 *)0xA000005C);
	nGpio |= 0x00020000;
	*((volatile UINT32 *)0xA000005C) = nGpio;
		 
	nGpio = *((volatile UINT32 *)0xA0000058);
	nGpio &= 0xFFFDFFFF;
	*((volatile UINT32 *)0xA0000058) = nGpio;    
    /*
	//GPIO 17 output hi
	nGpio = *((volatile UINT32 *)0xB0000060);
	nGpio &= 0xfffeffff;
	*((volatile UINT32 *)0xB0000060) = nGpio;
	
	nGpio = *((volatile UINT32 *)0xB0000058);
	nGpio |= 0x00010000;
	*((volatile UINT32 *)0xB0000058) = nGpio;	

	nGpio = *((volatile UINT32 *)0xB0000050);
	nGpio |= 0x00010000;
	*((volatile UINT32 *)0xB0000050) = nGpio;  	
	*/
}

 
 
BOOL sysConfig_initialize()
{
    
    BOOL bRet = TRUE;;
    

//HL modify
    
    DS_IO_01();
    
    DS40_IO();    
    
    

	InitUDev_Task();
 
 	#ifdef _USER_CONFIG
	// user define configure routine
 	bRet = usrConfig_initialize();
	#endif
    
   	// serial flash interface initialize
	SPI_Initialize();  
	
	
	return bRet;
}



