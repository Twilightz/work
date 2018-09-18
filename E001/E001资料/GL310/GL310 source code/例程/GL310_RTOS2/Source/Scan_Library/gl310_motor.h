
#ifndef _gl310_motor_h_
#define _gl310_motor_h_

//0x030 --------------------------------
#define MtrPSave(A)	(0x01000000 & ((A) << 24))
#define VRHSPED(A)	(0x00f00000 & ((A) << 16 << 4))
#define VRSTOP(A)	(0x000f0000 & ((A) << 16))
#define VRHOME(A)	(0x0000f000 & ((A) << 8 << 4))
#define VRMOVE(A)	(0x00000f00 & ((A) << 8))
#define VRBACK(A)	(0x000000f0 & ((A) << 4))
#define VRSCAN(A)	(0x0000000f & (A))
//0x034 --------------------------------
#define MTRPLS(A)	(0x000000ff & (A))
//0x03c --------------------------------
#define STOPTIM(A)	(0x1f000000 & ((A) << 24))
#define MULSTOP(A)	(0x00700000 & ((A) << 16 << 4))
#define DECSEL(A)	(0x00070000 & ((A) << 16))
#define FSTPSEL(A)	(0x00000700 & ((A) << 8))
#define STEPSEL(A)	(0x00000007 & (A))
//0x048 --------------------------------
#define MTRSET(A)	(0x07000000 & ((A) << 24))
#define MT_OFF(A)	(0x00003fff & (A))

//0x150 --------------------------------
#define MOTOR_TABLE_ADD(A)	(0xfff00000 & (A))
//0x154 --------------------------------
#define STEPNO(A)	(0xffff0000 & ((A) << 16))
#define SCANFED(A)	(0x0000ffff & (A))
//0x158 --------------------------------
#define BWDSTEP(A)	(0xffff0000 & ((A) << 16))
#define FWDSTEP(A)	(0x0000ffff & (A))
//0x15c --------------------------------
#define FSHDEC(A)	(0xffff0000 & ((A) << 16))
#define FASTNO(A)	(0x0000ffff & (A))
//0x160 --------------------------------
#define FMOVDEC(A)	(0xffff0000 & ((A) << 16))
#define FMOVNO(A)	(0x0000ffff & (A))
//0x164 --------------------------------
#define Z1MOD(A)	(0x001fffff & (A))
//0x168 --------------------------------
#define Z2MOD(A)	(0x001fffff & (A))
//0x170 --------------------------------
#define FEEDL(A)	(0x00ffffff & (A))
//0x174 --------------------------------
#define PREFED(A)	(0x00ffffff & (A))
//0x178 --------------------------------
#define PROTLEN(A)	(0xffff0000 & ((A) << 16))
#define POSTFED(A)	(0x0000ffff & (A))
//0x17c --------------------------------
#define SCANLEN(A)	(0x000fffff & (A))
//0x180 --------------------------------
#define EXITFED(A)	(0x00ffffff & (A))

#endif //_gl310_motor_h_
