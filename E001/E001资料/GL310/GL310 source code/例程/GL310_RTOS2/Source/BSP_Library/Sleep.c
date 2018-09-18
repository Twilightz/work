/***************************************************************************
 * Name:SLEEP.C 	                                                       *
 * Description: Power down sleep and wakeup function	      		       *
 *																		   *
 * Author: JohnKe                                                 		   *
 * Date:   2010/04/01                                                      *
 * Version:0.0.1                                                           *
 *																		   *
 *																		   *
 * Maintainer : JOHN KE 		                                           *
 ***************************************************************************/
#include "sleep.h"


void (*CPUSleep)(UINT8 NUM)=(void (*)(UINT8 NUM))Sleep_Vector;

void Disable_MMU(void);
void Enable_MMU(void);
void _DisableFIQ(void);
void _DisableIRQ(void);
void _EnableIRQ(void);
void _EnableFIQ(void);

 
void hw_sleep(void)
{

	 Disable_MMU();
	
	_DisableIRQ();
	_DisableFIQ();	

	
	CPUSleep(0);
	
  	__asm{ 
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;
		
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;	
		nop;nop;nop;nop;nop;nop;
		nop;nop;nop;nop;nop;nop;
  	} 	  		
	
 	
	_EnableIRQ();
	_EnableFIQ();
	
	Enable_MMU(); 
	
}


void hw_sleep_config(UINT8 Type, UINT8 Keyselect)
{

//	(*(volatile UINT8 *) 0x90d00045) = 0;

	if(Type & RESUME_WAKEUP){
		
		(*(volatile UINT8 *) 0x90d00045) |= 0x00001000;
	}
	
	if(Type & KEY_WAKEUP){
		
		(*(volatile UINT8 *) 0x90d00045) |= 0x00000001;	
		(*(volatile UINT8 *) 0xA0000008) |= 0x1;				// GPIO wakeup select!
		(*(volatile UINT8 *) 0x90d00008) = Keyselect;			// Key Button select!	
	}
		
	
}






