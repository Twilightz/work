#include "def.h"
#include "iopower.h"

BOOL key_Press_G28()
{

	if( *((volatile UINT32 *)0xB0000050) & 0x08000000 )
		return true;
	else
		return false;	
	
}

void key_PwrOn_G86()
{
	printf("Hello! I got power!\n");
	*((volatile UINT32 *)0xA0000058) |= 0x00008000;	
}


void key_PwrOff_G86()
{
	printf("Good bye\n");
	*((volatile UINT32 *)0xA0000058) &= ~0x00008000;
}


void key_Pwr_IODirection_G286()
{

	*((volatile UINT32 *)0xB0000058) &= 0xF7FFFFFF;
   
   	*((volatile UINT32 *)0xA000005C) |= 0x00008000;
   	*((volatile UINT32 *)0xA0000058) &= 0xFFFF7FFF;	
}

