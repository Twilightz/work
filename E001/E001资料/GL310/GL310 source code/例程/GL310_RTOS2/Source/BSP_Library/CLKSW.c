/***************************************************************************
 * Name:CLKSW.C 	                                                       *
 * Description: system working frequency switch function       		       *
 *																		   *
 * Author: JohnKe                                                 		   *
 * Date:   2010/04/01                                                      *
 * Version:0.0.1                                                           *
 *																		   *
 *																		   *
 * Maintainer : JOHN KE 		                                           *
 ***************************************************************************/
//#include "def.h"
#include "dtype.h"
#include "clksw.h"
#include "Asicutil.h"


void Disable_MMU(void);
void Enable_MMU(void);
void _DisableFIQ(void);
void _DisableIRQ(void);
void _EnableIRQ(void);
void _EnableFIQ(void);
void _SetICacheEnable(void);

void (*CPUCLKSW)(UINT32 SPEED, UINT32 SRAMAddr, UINT32 TmpDDRAddr)=(void (*)(UINT32 SystemClk, UINT32 SRAMAddr, UINT32 TmpDDRAddr))CPUCLKSW_Vector;




BOOL CPUCLKProc(UINT8 CPUSPEED, UINT32 SRAMAddr, UINT32 TmpDDRAddr)
{
    UINT32 SYSTag;
    UINT8 CPUCLK;
   
	
	Disable_MMU();
	_SetICacheEnable();
    

	switch (CPUSPEED) {
		case 30:
			SYSTag = 0x00011100;
			break;

		case 40:
			SYSTag = 0x00022200;
			break;

		case 48:
			SYSTag = 0x00033300;
			break;

		case 60:
			SYSTag = 0x00044400;
			break;

		case 80:
			SYSTag = 0x00099900;
			break;

		case 96:
			SYSTag = 0x000AAA00;
			break;

		case 120:
			SYSTag = 0x000BBB00;
			break;

	 	default:
			SYSTag = 0x00011100;
			break;
	};

	if(!(CPE_STRAP_REG & Pad_DDRM) ){
	    SYSTag |= 0x1;
	}



	_DisableFIQ();
    _DisableIRQ();	
 	
 	// SPI frequency must be equal or larger than CPU frequency.
	CPUCLK = ((UINT8)(SYSTag >> 8)) & 0x0F;
	
	(*(volatile UINT32 *) 0xA0000054) &= 0xF00FFFFF;
	(*(volatile UINT32 *) 0xA0000054) |= (((UINT32)CPUCLK) << 20);		// update SPI frequency.
	(*(volatile UINT32 *) 0xA0000054) |= (((UINT32)CPUCLK) << 24);		// update scanner frequency. 
  	
	
	CPUCLKSW(SYSTag, SRAMAddr, TmpDDRAddr);
 	

	_EnableIRQ();
	_EnableFIQ();
	
 	Enable_MMU();
	
	return TRUE;
}



