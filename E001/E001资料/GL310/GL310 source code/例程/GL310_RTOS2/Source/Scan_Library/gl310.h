
#ifndef _GL310_h_
#define _GL310_h_

#include "gl310_top.h"
#include "gl310_common.h"
#include "gl310_gpio.h"
#include "gl310_ccd.h"
#include "gl310_afe.h"
#include "gl310_motor.h"
#include "gl310_dcmotor.h"
#include "gl310_image.h"
#include "gl310_jpeg.h"

#define clr			0xffffffff
#define TCR(A)		((A)+0xa0000000)	// Top Control Register (base address)
#define SCR(A)		((A)+0xb0000000)	// Scanner Control Register (base address)
#define b_SCR(B)	((B)+0xb0000600)	// bEngine Scanner Control Register (base address)
#define BENG		0x600				// bEngine (offset address)

#endif //_GL310_h_
