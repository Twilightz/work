#include "def.h"
#include "sysConfig.h"
#include "usrConfig.h"
#include "spi_tag.h" 
#include "UDev_CstApi.h"   


/************************************************************************
 *		BOOL sysConfig_initialize(TX_BYTE_POOL *stack_pool)
 * 
 ************************************************************************/
BOOL sysConfig_initialize()
{
    
    BOOL bRet = TRUE;;

	InitUDev_Task();
 
 	#ifdef _USER_CONFIG
	// user define configure routine
 	bRet = usrConfig_initialize();
	#endif
    
   	// serial flash interface initialize
	SPI_Initialize();  
	
	
	return bRet;
}

