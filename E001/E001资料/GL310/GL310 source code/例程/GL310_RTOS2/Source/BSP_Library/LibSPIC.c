#include "dtype.h"
#include "libspic.h"

//define each spi flash physical board level placement
spicmap spicmap_tbl[]={ 
	{spi_intf_A, SubIntf_Ch0},				// SPI flash 0 place on spi interface A and ch 0
//	{spi_intf_B, SubIntf_Ch1},				// SPI flash 1 place on spi interface B and ch 1
};

//define SPI controller transfer start and finish call back function tabel
spi_transfer spi_intf_tbl[] = {

	{NULL, NULL}, 
	{NULL, NULL}
};



spic_fun_entry spic_fun_entry_tbl[]={

	hw_SPIC_Intf_A,
	hw_SPIC_Intf_B
};

UINT8 NumSPIC_Limint 	 = sizeof(spicmap_tbl) / sizeof(spicmap);
UINT8 NumSPIC_IntfLimint = sizeof(spic_fun_entry_tbl) / sizeof(spic_fun_entry);



BOOL hw_SPIC_event_funcb_register(UINT8 Intf, BOOL (*funSet)(void), BOOL (*funWait)(void))
{

	spi_transfer *spi_t = (spi_transfer *)&spi_intf_tbl[Intf];

	if(Intf >= NumSPIC_Limint)
		return FALSE;
	
	spi_t->SPI_Fnsh_SET  = funSet;
	spi_t->SPI_Fnsh_WAIT = funWait;
	

	return TRUE;	
}















 
