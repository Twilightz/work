#ifndef _sc_calibration_h_
#define _sc_calibration_h_
//-----------------
#include "cmd.h"

typedef struct CALIBRATION_STRUCT {
	U16 GainCode[2][3];
	U16 OffsetCode[2][3];
	U16 Exposure_Pix[2][3];
} CALIBRATION_T;

//-----------------
#endif//_sc_calibration_h_