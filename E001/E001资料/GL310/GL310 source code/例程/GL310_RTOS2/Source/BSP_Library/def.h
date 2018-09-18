/***************************************************************************
* Name:DEF.H				                                               *
* Description: SYSTEM Variable Type Declar DEFINED						   *
*																		   *		
*																		   *
* Author: John KE      				                                       *
* Date: 2007/03/02	                                                       *
****************************************************************************/
#ifndef _DEF_H_
#define _DEF_H_

#include <stdio.h>
#include <string.h>

 
#include "dtype.h"				//Variable type define
#include "mm.h"
 

typedef void *HANDLE;



#define EXIT_FAIL 	-1
      
#define H_U8(w)     ((UINT8) (w>>8))
#define LOBYTE(w)   ((UINT8) (w)) 
#define LO_U16(l)   ((UINT16) (l))
#define H_U16(l)    ((UINT16) (l>>16)) 

 



#endif 

