#ifndef _LIB_SPIC_H_
#define _LIB_SPIC_H_



#define spi_intf_A		0
#define spi_intf_B		1
#define SubIntf_Ch0		0
#define SubIntf_Ch1		1

#define _FLH_WRITE      0
#define _FLH_READ       1
#define _NULL_PTR       0
#define _NEED_CHKSUM    1
#define _NO_CHKSUM      0

typedef struct _Spi_Str{
    
    
    UINT8  CmdLen;
    UINT8  Interface;
    UINT8  Intf_Ch;
    UINT8  res1;
    
    
    UINT8  Read;
    UINT8  nChannel;
    UINT8  mAddrToggle;
    UINT8  ndummybit;
    
    UINT32 rwLen;
	UINT32 CheckSum;
    
    UINT8 *rwPtr;
    UINT8 *CmdPtr;

     
} Spi_Str;

typedef struct spi_transfer{
	
	BOOL  (*SPI_Fnsh_SET)(void);
	BOOL  (*SPI_Fnsh_WAIT)(void);
	
	
}spi_transfer;

typedef struct _spicmap{
	
	UINT8 Interface;
	UINT8 Intf_Ch;
	
}spicmap;

typedef struct _spic_fun_entry{

	void (*fun_entry)(Spi_Str *);		
	
}spic_fun_entry;



extern spi_transfer spi_intf_tbl[];
extern spicmap spicmap_tbl[];
extern spic_fun_entry spic_fun_entry_tbl[];
extern UINT8 NumSPIC_Limint;
extern UINT8 NumSPIC_IntfLimint;
#include "libspic_intf_a.h"

BOOL hw_SPIC_event_funcb_register(UINT8 Intf, BOOL (*funSet)(void), BOOL (*funWait)(void));


#endif
