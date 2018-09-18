/***************************************************************************
 * Name:WATCHDOG.C 	                                                       *
 * Description: WATCHDOG				                    		       *
 *																		   *	
 * Author: JohnKe                                                 		   *
 * Date:   2009/12/03                                                      *
 * Version:0.0.1                                                           *
 *																		   *	
 *																		   *
 * Maintainer : JOHN KE 		                                           *
 ***************************************************************************/
#include "watchdog.h"

 


/*********************************************************************************
 *
*		void WDT_Reload()
 *
 *********************************************************************************/ 
void WDT_Reload()
{

	xrWdCR &= ~WdEnable;
		
	xrWdRestart = MagicRestart;
	
	xrWdCR |= WdEnable;
	
}


/*********************************************************************************
 *
*		void WDT_INIT()
 *
 *********************************************************************************/ 
void WDT_INIT()
{
	xrWdCR &= ~WdEnable;
	
	xrWdCR |= WdRst;
	xrWdLoad = 0x100;
	xrWdRestart = MagicRestart;	
	
	xrWdCR |= WdEnable;
}

