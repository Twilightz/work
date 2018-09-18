#ifndef _LIB_SPIC_INTFB_H_
#define _LIB_SPIC_INTFB_H_

#include "libspic.h"


#define Top_Reg_Base	0xA0000000
#define	xrIntF_Sel		*((volatile UINT32 *)(Top_Reg_Base + 0x68))


#define SIO_Base		0xB0500000
#define	xrSIO_wr		*((volatile UINT32 *)(SIO_Base + 0x0))
#define	xrSIO_rd		*((volatile UINT32 *)(SIO_Base + 0x4))
#define	xrSIO_cr1		*((volatile UINT32 *)(SIO_Base + 0x8))
#define	xrSIO_cr2		*((volatile UINT32 *)(SIO_Base + 0xc))


void hw_SPIC_Intf_B(Spi_Str *spistr);
void hw_SPIC_Intf_B_Initial(void);
#endif
