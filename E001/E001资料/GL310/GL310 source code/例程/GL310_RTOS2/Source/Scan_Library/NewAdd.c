
typedef struct CALIBRATION_STRUCT {
	U16	AFE_OffsetCode[2][3];
	U16	AFE_GainCode[2][3];
	float AFE_OffsetValue[2][3];
	float AFE_GainValue[2][3];
	float shutter_time[2][3];	// ms
} CALIBRATION_T;

typedef struct SHADING_STRUCT {
	U32 Shad_sensor;	// 'CCD', 'CIS', 'CIS3'
	U8 Shad_gain;		// 4 or 8
	U8 Shad_type;		// 0: raw, 16: 16+16 mode,  12: 12+4 mode, 10: 10+6 mode, 8: 8+8mode
	U8 Shad_bit;		// 8, 16, 24, 48
	U8 Shad_Mono;		// 
	U32 Shad_size;		
} SHADING_T;

typedef struct MOTOR_STRUCT {
	U32 Motor_source;	// 'FLB', 'TPA', 'ADF', 'PICK', 'LOAD'
	U16 Motor_GearChain;	// 150, 200, 300..600..1200..
	U8 Motor_current;	// 0, 1, 2, 3..8
	U8 Motor_StepType;	// 1, 2, 4, 8, 16
	U32 table_size;
} MOTOR_T;

typedef struct GAMMA_STRUCT {
	U8	Gamma_Contrast;
	U8	Gamma_Brightness;
	U16 Gamma_size;
} GAMMA_T;

typedef struct COLOR_MATRIX_STRUCT {
	U16 R[3]; U16 G[3]; U16 B[3];
} COLOR_MATRIX_T;

typedef struct CONVOLUTION_STRUCT {
	U16 R[5]; U16 G[5]; U16 B[5];
} CONVOLUTION_T;

typedef struct POWER_MANAGEMENT_STRUCT {
	U32 sleep_time;
	U32 shutdown_time;
} POWER_MANAGEMENT_T;

