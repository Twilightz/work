#include "os_user_spi.h"



OS_Mutex mutex_SPI;
OS_Mutex mutex_SPI_transfer;
OS_EventGroup event_SPI_transfer;


BOOL os_user_spi_transfer(Spi_Str *spistr)
{
	OS_Status os_status;
	spic_fun_entry *spic_fun_ptr = &spic_fun_entry_tbl[spistr->Interface];

	if(spistr->Interface >= NumSPIC_IntfLimint)
		return FALSE;

	os_status = mutexGet(mutex_SPI_transfer, OS_WAIT_FOREVER);
	if(os_status != OS_SUCCESS)
		return FALSE;

	spic_fun_ptr->fun_entry(spistr);

	mutexPut(mutex_SPI_transfer);

	return TRUE;
}

BOOL os_user_SPI_Intf_A_Wait()
{
	OS_Status os_status;
	UINT32 actual_flag;

	while(1){
		os_status = evtgrpWaitBits(event_SPI_transfer, SPIH_Done, OS_EVT_OR_CLR, &actual_flag, OS_WAIT_FOREVER);
		if(os_status != OS_SUCCESS)
			return FALSE;

		if(actual_flag & SPIH_Done)
			break;
	}

	return TRUE;
}

BOOL os_user_SPI_Intf_A_Set()
{
	evtgrpSetBitsFromISR(event_SPI_transfer, SPIH_Done);
	return TRUE;
}



void os_user_spi_transfer_initial()
{
	mutexCreate(&mutex_SPI, OS_NO_INHERIT);
	mutexCreate(&mutex_SPI_transfer, OS_NO_INHERIT);
	evtgrpCreate(&event_SPI_transfer);

	hw_SPIC_event_funcb_register(spi_intf_A, os_user_SPI_Intf_A_Set, os_user_SPI_Intf_A_Wait);

}


