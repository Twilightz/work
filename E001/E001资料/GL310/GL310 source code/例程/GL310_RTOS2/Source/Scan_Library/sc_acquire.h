#ifndef _sc_acquire_h_
#define _sc_acquire_h_
//-----------------
#include "cmd.h"

typedef struct ACQUIRE_STRUCT {
	U32 source;		// 'ADF'/'FLB'/'POS'/'NEG', 'FW'/'PAR'/'HW', 'HOST', 'WIFI', 'ETH'
	U32 acquire;
	U16 option;		// 0
	U8	duplex;		// 1:'A', 2:'B', 3:'D'
	U8	page;		// 0 for infinity pages 
} ACQUIRE_T;

//-- acquire ---------------
#if 0	// define in sc_api.h
#define ACQ_AUTO_SCAN		0x01
#define ACQ_SHADING			0x02
#define ACQ_GAMMA			0x04
#define ACQ_MIRROR			0x80
#define ACQ_START_HOME		(0x01 << 8)
#define ACQ_BACK_TRACK		(0x02 << 8)
#define ACQ_AUTO_GO_HOME	(0x04 << 8)
#define ACQ_EJECT_PAPER		(0x10 << 8)
#define ACQ_PICKUP_HOME		(0x20 << 8)

#define ACQ_STILL_SCAN		(0x01 << 16)
#define ACQ_TEST_PATTERN	(0x02 << 16)
#define ACQ_RUNIN_IMAGE		(0x04 << 16)
#define ACQ_RUNIN			(0x08 << 16)
#endif
//-----------------
#endif//_sc_acquire_h_
