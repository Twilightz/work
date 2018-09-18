#ifndef _LIB_SPIC_INTFA_H_
#define _LIB_SPIC_INTFA_H_

#include "libspic.h"





#define nMaxAddrBit		24
#define nMaxOPCmdBit	8


#define SPIC_Base		0x90c00000
#define SPInCmdBase		SPIC_Base + 0x4
#define SPInDirLBase	SPIC_Base + 0xC
#define SPInDirHBase	SPIC_Base + 0x10

#define xrSPIParm		(*(volatile UINT32 *) (SPIC_Base + 0x00)) 

#define xrSPInC1CmdRegL	(*(volatile UINT32 *) (SPIC_Base + 0x04)) 
#define xrSPInC1CmdRegH	(*(volatile UINT32 *) (SPIC_Base + 0x08)) 
#define xrSPInC1CmdDirL	(*(volatile UINT32 *) (SPIC_Base + 0x0C)) 
#define xrSPInC1CmdDirH	(*(volatile UINT32 *) (SPIC_Base + 0x10)) 

#define xrSPInC2CmdRegL	(*(volatile UINT32 *) (SPIC_Base + 0x14)) 
#define xrSPInC2CmdRegH	(*(volatile UINT32 *) (SPIC_Base + 0x18)) 
#define xrSPInC2CmdDirL	(*(volatile UINT32 *) (SPIC_Base + 0x1C)) 
#define xrSPInC2CmdDirH	(*(volatile UINT32 *) (SPIC_Base + 0x20)) 

#define xrSPInC3CmdRegL	(*(volatile UINT32 *) (SPIC_Base + 0x24)) 
#define xrSPInC3CmdRegH	(*(volatile UINT32 *) (SPIC_Base + 0x28)) 
#define xrSPInC3CmdDirL	(*(volatile UINT32 *) (SPIC_Base + 0x2C)) 
#define xrSPInC3CmdDirH	(*(volatile UINT32 *) (SPIC_Base + 0x30)) 

#define xrSPInC4CmdRegL	(*(volatile UINT32 *) (SPIC_Base + 0x34)) 
#define xrSPInC4CmdRegH	(*(volatile UINT32 *) (SPIC_Base + 0x38)) 
#define xrSPInC4CmdDirL	(*(volatile UINT32 *) (SPIC_Base + 0x3C)) 
#define xrSPInC4CmdDirH	(*(volatile UINT32 *) (SPIC_Base + 0x40)) 


#define xrSPIAMBAAddr	(*(volatile UINT32 *) (SPIC_Base + 0x44)) 
#define xrSPITransLen	(*(volatile UINT32 *) (SPIC_Base + 0x48)) 
#define xrSPICtrl		(*(volatile UINT32 *) (SPIC_Base + 0x4C)) 
#define xrSPIFlag		(*(volatile UINT32 *) (SPIC_Base + 0x50)) 
#define xrSPIChecksum	(*(volatile UINT32 *) (SPIC_Base + 0x54)) 

BOOL hw_SPI_Intf_A_Wait_interrupt(void);
BOOL hw_SPI_Intf_A_Wait_loop(void);
BOOL hw_SPI_Intf_A_Flag_Set(void);


void hw_SPIC_Intf_A(Spi_Str *spistr);
void hw_SPIC_Intf_A_Initial(void);
void hw_SPIC_Intf_A_Interrupt_initial(void); 
void hw_SPIC_Intf_B(Spi_Str *spistr);

#endif
