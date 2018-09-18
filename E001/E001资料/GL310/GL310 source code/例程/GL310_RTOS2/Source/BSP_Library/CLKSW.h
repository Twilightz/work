/***************************************************************************
* Name:CLKSW.H			     	                                           *
* Description: system wokring frequency switch function header			   *
*																		   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2010/04/01	                                                       *
****************************************************************************/
#ifndef _CLKSW_H_
#define _CLKSW_H_

#include "dtype.h"

#define TagID_Start		0x100
#define TagID_Len		0x20

#define	CPUCLKSW_Vector	0x800000C0 

BOOL CPUCLKProc(UINT8 CPUSPEED, UINT32 SRAMAddr, UINT32 TmpDDRAddr);



#endif
