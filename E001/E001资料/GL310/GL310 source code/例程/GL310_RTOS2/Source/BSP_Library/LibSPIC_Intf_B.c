#include "dtype.h"
#include "libspic_intf_b.h"
#include "tag.h"
#include "LibIntrc.h"


UINT8 spic_intf_B_config = 0;
void hw_SPIC_Intf_B_config()
{

	xrSIO_cr1 = 0x1D;
								// Bit7 =0 			SPI Controller Disable
								// Bit[6:2]=00111   8 bit transfer	
								// Bit1 =x 		    Don't care
								// Bit0 =1 		    CS is High
					
	xrSIO_cr2 = 0x02;							
								// Bit7 =X 			Don't care 
								// Bit6 =x 			Don't care
								// Bit5 =0			MSB Transfer serial
								// Bit4 =0			Serial data no delay								
								// Bit3 =0			SCK remains low when SPI bus idle
								// Bit2 =0			SPI interface select
								// Bit[1:0] =00		Serial clock = system clock/2
								// Bit[1:0] =01		Serial clock = system clock/4
								// Bit[1:0] =10		Serial clock = system clock/6	
								
	spic_intf_B_config = 1;							
}


void hw_SPIC_Intf_B_Set(UINT8 Enable)
{
	
	if(Enable){
		
		xrIntF_Sel &= 0xffffffe7;	// SPI0 interface de-select
		xrIntF_Sel |= 0x00000010;	// SPI1 interface select	
	
		xrSIO_cr1 |= 0x80;			// Bit7 =1 			SPI Controller enable	
	
		*((volatile UINT32 *)0xB000005C) |= 0x20;	
		
	}
	else{
	
		xrSIO_cr1 &= ~0x80;			// Bit7 =0 			SPI Controller disable
	
		xrIntF_Sel &= 0xffffffe7;	// SPI1 interface de-select
		xrIntF_Sel |= 0x00000008;	// SPI0 interface select	
	
		*((volatile UINT32 *)0xB000005c) &= ~0x20;
		
	}
	
}


void hw_SPIC_Intf_B_Sub_Ch_Set(UINT8 SubCh)
{

	
	if(SubCh == 0)
		*((volatile UINT32 *)0xB0000054) &= ~0x20;	
	else
		*((volatile UINT32 *)0xB0000054) |= 0x20;	
		
}


void hw_SPIC_Intf_B_cs_low()
{
	xrSIO_cr1 &= 0xfffffffe;	// Bit 0 =0          cs goes low
	
}

void hw_SPIC_Intf_B_cs_high()
{
	xrSIO_cr1 |= 0x1;			// Bit 0 =1          cs goes high
}


void hw_SPIC_Intf_B_Set_Cmd_transfer(Spi_Str *spistr)
{

	UINT8 i;
	UINT8 cmd_len = spistr->CmdLen;
	

	if(cmd_len >4 )
		return;
	
	
	for(i=0; i<cmd_len; i++){
		xrSIO_wr = spistr->CmdPtr[i];	
	}
	
}
/*
void hw_SPIC_Intf_B_Set_OPCode(Spi_Str *spistr)
{
	xrSIO_wr = spistr->CmdPtr[0];
}

void hw_SPIC_Intf_B_Set_Address(Spi_Str *spistr)
{
	xrSIO_wr = spistr->CmdPtr[1];
	xrSIO_wr = spistr->CmdPtr[2];
	xrSIO_wr = spistr->CmdPtr[3];
	
}*/

void hw_SPIC_Intf_B_Set_Data_transfer(Spi_Str *spistr)
{
	UINT8 *targetbuf = (UINT8 *)spistr->rwPtr;
	UINT32 transfer_len = spistr->rwLen;
	
		
	//set 32 bit mode
	
	while(transfer_len--){
		
		if(spistr->Read)		
			*targetbuf = (UINT8)xrSIO_rd;	
		
		else
			xrSIO_wr = *targetbuf;
				
		targetbuf++;
	}
	// restore 8 bit mode
	
}



void hw_SPIC_Intf_B(Spi_Str *spistr)
{
	
	if( !spic_intf_B_config)
		hw_SPIC_Intf_B_config();
	
	//Ch = 0, IO_EXPLANDER, otherwise spi flash 2
	hw_SPIC_Intf_B_Sub_Ch_Set(spistr->Intf_Ch);

	// Enable, means spi interface 1 select
	// Enable, means spi interface 0 select
	hw_SPIC_Intf_B_Set(TRUE);
	
	
	hw_SPIC_Intf_B_cs_low();

	/*	
	hw_SPIC_Intf_B_Set_OPCode(spistr);
	
	if(spistr->CmdLen > 1)	
		hw_SPIC_Intf_B_Set_Address(spistr);
	*/	
	
	if(spistr->CmdLen){
		
		hw_SPIC_Intf_B_Set_Cmd_transfer(spistr);
	
		if(spistr->rwLen)	
			hw_SPIC_Intf_B_Set_Data_transfer(spistr);
	
		//while transfer done
		
	}
		
	
	
	
	
	
	
	hw_SPIC_Intf_B_cs_high();
	
	hw_SPIC_Intf_B_Set(FALSE);
}


void hw_SPIC_Intf_B_Initial()
{

	hw_SPIC_Intf_B_config();
	

	/*
	spi_transfer *spi_t = &spi_intf_tbl[0];
	
	spi_t->SPI_Fnsh_SET  = hw_SPI_Intf_A_Flag_Set;
	spi_t->SPI_Fnsh_WAIT = hw_SPI_Intf_A_Wait_loop;
 
 	*/
}

