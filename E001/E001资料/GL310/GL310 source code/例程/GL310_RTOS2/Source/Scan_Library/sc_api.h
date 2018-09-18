#ifndef _sc_api_h_
#define _sc_api_h_
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
//-----------------
#include "Ef_cmd_handle.h"

//-- JOB ---------------------
typedef struct SC_JOB_STRUCT {
	UINT32	code;
	UINT8	request;
	UINT8	reserved[2];
	UINT8	id;
} SC_JOB_T;

typedef struct SC_JOB_STA_STRUCT {
	UINT32	code;
	UINT8	ack;
	UINT8	reserved[2];
	union {UINT8  id; UINT8 err;};
} SC_JOB_STA_T;

//-- PAR ---------------------
typedef struct SC_PAR_STRUCT {
	UINT32	code;
	UINT16	length;
	UINT8  reserved;
	UINT8	id;
} SC_PAR_T;

typedef struct SC_PAR_DATA_STRUCT {
	//- ACQUIRE --
	UINT32 source;	// 'ADF'/'FLB'/'POS'/'NEG', 'FW'/'PAR'/'HW', 'HOST', 'WIFI', 'ETH'
	UINT32 acquire;
	UINT16 acq_opt;// 0
	UINT8	duplex;	// 1:'A', 2:'B', 3:'D'
	UINT8	page;	// 0 for infinity pages 
	//- IMAGE ----
	UINT32 format;	// 'RAW', 'JPG', 'TIF', 'BMP', 'PDF', 'PNG'
	UINT16	img_opt;// 0
	UINT8	bit;	// 1:BW, 8:Gray8, 16:Gray16, 24:Color24, 48:Color48
	UINT8	mono;	// 0:'MONO', 1:'R', 2:'G', 4:'B', 8:'IR', 7:'NTSC'
	struct{UINT16 x; UINT16 y;} dpi;
	struct{UINT32 x; UINT32 y;} org;
	struct{UINT32 w; UINT32 h;} dot;
	//- shading ---
	UINT16	AFE_OffsetCode[2][3];
	UINT16	AFE_GainCode[2][3];
	float AFE_OffsetValue[2][3];
	float AFE_GainValue[2][3];
	float shutter_time[2][3];
	UINT32 shading_size;
} SC_PAR_DATA_T;

#define ACQ_AUTO_SCAN		0x01
#define ACQ_SHADING			0x02
#define ACQ_GAMMA			0x04
#define ACQ_CALIBRATION		0x08
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

typedef struct SC_PAR_STA_STRUCT {
	UINT32	code;
	UINT8	ack;
	UINT8	reserved[2];
	union {UINT8  id; UINT8 err;};
} SC_PAR_STA_T;

//-- SCAN ---------------------
typedef struct SC_SCAN_STRUCT {
	UINT32	code;
	UINT8  reserved[3];
	UINT8	id;
} SC_SCAN_T;

typedef struct SC_SCAN_STA_STRUCT {
	UINT32	code;
	UINT8	ack;
	UINT8	reserved[2];
	union {UINT8  id; UINT8 err;};
} SC_SCAN_STA_T;

//-- INFO ---------------------
typedef struct SC_INFO_STRUCT {
	UINT32	code;
	UINT8  reserved[3];
	UINT8	id;
} SC_INFO_T;

typedef struct SC_INFODATA_STRUCT {
	UINT32	code;
	UINT16 ValidPage[2];
	UINT32 ValidPageSize[2];
	UINT16 ImageWidth[2];
	UINT16 ImageLength[2];
	UINT8	PageNo[2];
	UINT8	EndPage[2];
	UINT8	UltraSonic; UINT8 PaperJam; UINT8 CoverOpen; UINT8 Cancel; UINT8 key; 
	UINT8	EndDocument;
	UINT8	AdfIsReady;
	UINT8	reserved[13+16];
} SC_INFODATA_T;

//-- IMG ---------------------
typedef struct SC_IMG_STRUCT {
	UINT32	code;
	UINT32 length: 24;
	UINT32 side:	8;
	//UINT8	length[3];
	//UINT8	side;
} SC_IMG_T;

typedef struct SC_IMG_STA_STRUCT {
	UINT32	code;
	UINT32 ack: 8;
	UINT32 length: 24;
	//UINT8	ack;
	//UINT8	length[2];
	//UINT8	err;
} SC_IMG_STA_T;
#define LENGTH

//-- CNL ---------------------
typedef struct SC_CNL_STRUCT {
	UINT32	code;
	UINT8	reserved[3];
	UINT8  id;
} SC_CNL_T;

typedef struct SC_CNL_STA_STRUCT {
	UINT32	code;
	UINT8	ack;
	UINT8	reserved[2];
	union {UINT8  id; UINT8 err;};
} SC_CNL_STA_T;

//-- STOP ---------------------
typedef struct SC_STOP_STRUCT {
	UINT32 code;
	UINT8	reserved[3];
	UINT8	id;
}SC_STOP_T;

typedef struct SC_STOP_STA_STRUCT {
	UINT32 code;
	UINT8	ack;
	UINT8	reserved[2];
	union {UINT8  id; UINT8 err;};
}SC_STOP_STA_T;

//---------------------
int SC_JOB(cmd_buf_parameter *cmd);
int SC_PAR(cmd_buf_parameter *cmd);
int SC_STAR(cmd_buf_parameter *cmd);
int SC_INFO(cmd_buf_parameter *cmd);
int SC_IMG(cmd_buf_parameter *cmd);
int SC_ABRT(cmd_buf_parameter *cmd);
int SC_STOP(cmd_buf_parameter *cmd);
int SC_FIN(cmd_buf_parameter *cmd);
//---------------------

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//_sc_api_h_
