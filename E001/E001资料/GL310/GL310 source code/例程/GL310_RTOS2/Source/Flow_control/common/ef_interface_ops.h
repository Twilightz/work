/***************************************************************************
* Name: ef_interface_ops.h                                                 *
* Description: event flow interface operation header          	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
#ifndef _HEADER_EF_INTF_OPS_
#define _HEADER_EF_INTF_OPS_

#include "dtype.h"
#include "stdio.h"
#include "mm.h"

typedef struct _iop_operations{

    BOOL  (*init)       (void);
    UINT8 (*BurstWrite) (UINT8 *buf, UINT32 Len, UINT32 *Actual_Len, UINT32 ms_timeout);
	UINT8 (*BurstRead)	(UINT8 *buf, UINT32 Len, UINT32 ms_timeout);
	UINT8 (*Write)	    (UINT8 *buf, UINT32 Len, UINT32 *Actual_Len, UINT32 ms_timeout);
	UINT8 (*Read)	    (UINT8 *buf, UINT32 Len, UINT32 ms_timeout);
	UINT8 (*STALL)      (UINT16 EndP_map);
	
}iop_operations;


extern UINT32 iop_operations_tbl[];
BOOL iop_operations_register(UINT8 iops_id, iop_operations *iops);



#endif
