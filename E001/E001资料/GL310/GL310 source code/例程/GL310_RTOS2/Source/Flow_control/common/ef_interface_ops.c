/***************************************************************************
* Name: ef_interface_ops.c                                                 *
* Description: event flow interface operation source           	           *
*																		   *
*																		   *
* Author: John KE      				                                       *
* Date: 2013/10/25	                                                       *
****************************************************************************/
//#include "tx_api.h"
#include "os_user_api.h"
#include "ef_interface_ops.h" 
 

UINT32 iop_operations_tbl[]=
{
    NULL,
    NULL,
    NULL,    
    NULL,
    NULL,
    NULL,    
    NULL,
    NULL,
    NULL    
};





BOOL iop_operations_register(UINT8 iops_id, iop_operations *iops)
{
    iop_operations_tbl[iops_id] = (UINT32)iops;
    
    if(iops->init) 
        return iops->init();            
    
    return true;
}
