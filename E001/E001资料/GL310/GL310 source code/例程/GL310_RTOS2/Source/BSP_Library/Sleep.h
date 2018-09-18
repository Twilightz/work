/***************************************************************************
* Name:SLEEP.H			     	                                           *
* Description: Power down sleep function header							   *
*																		   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2010/04/01	                                                       *
****************************************************************************/
#ifndef _SLEEP_H_
#define _SLEEP_H_

#include "dtype.h"

#define KEY_WAKEUP		0x1
#define RESUME_WAKEUP	0x2

#define	Sleep_Vector	0x800000B0

void hw_sleep(void);
void hw_sleep_config(UINT8 Type, UINT8 Keyselect);
#endif
