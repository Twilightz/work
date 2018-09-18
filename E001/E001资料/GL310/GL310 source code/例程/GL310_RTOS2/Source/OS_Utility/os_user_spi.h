#if !defined(_OS_USER_SPI_H_)
#define _OS_USER_SPI_H_

#include "OS_API.h"
#include "LibSPIC.h"


#define SPIH_Done 0x1

BOOL os_user_spi_transfer(Spi_Str *spistr);
void os_user_spi_transfer_initial(void);


#endif

