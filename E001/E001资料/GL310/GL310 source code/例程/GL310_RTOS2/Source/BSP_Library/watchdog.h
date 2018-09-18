/***************************************************************************
* Name:WATCHDOG.H				                                           *
* Description: WATCHDOG													   *
*																		   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2007/03/02	                                                       *
****************************************************************************/
#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "def.h"


#define	Wd_Base			0x98500000
#define	MagicRestart	0x5AB9

#define xrWdCounter		(*(volatile UINT32 *) (Wd_Base + 0x00)) 	
#define xrWdLoad		(*(volatile UINT32 *) (Wd_Base + 0x04)) 	
#define xrWdRestart		(*(volatile UINT32 *) (Wd_Base + 0x08)) 	
#define xrWdCR			(*(volatile UINT32 *) (Wd_Base + 0x0c)) 	
#define xrWdStatus		(*(volatile UINT32 *) (Wd_Base + 0x10)) 	
#define xrWdClear		(*(volatile UINT32 *) (Wd_Base + 0x14)) 	
#define xrWdInterCter	(*(volatile UINT32 *) (Wd_Base + 0x18)) 	


#define	WdEnable		0x01
#define	WdRst			0x02
#define	WdIntr			0x04
#define	WdExt			0x08



#define Watchdog_VectorNum	16


void WDT_INIT(void);
void WDT_Reload(void);



#endif 

