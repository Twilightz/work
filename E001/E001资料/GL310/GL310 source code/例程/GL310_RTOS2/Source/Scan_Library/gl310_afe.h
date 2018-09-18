
#ifndef _gl310_afe_h_
#define _gl310_afe_h_

//0x0a0 --------------------------
#define LEDCNT(B)	(0x0f000000 & ((B) << 24))
#define RLCSEL(B)	(0x00400000 & ((B) << 16 << 6))
#define CDSREF(B)	(0x00300000 & ((B) << 16 << 4))
#define RLC(B)		(0x000f0000 & ((B) << 16))
#define mclk_dly(B)	(0x0000f000 & ((B) << 8 << 4))
#define ADC_NEG(B)	(0x00000800 & ((B) << 8 << 3))
#define ADC_XOR(B)	(0x00000400 & ((B) << 8 << 2))
#define ADC_AND(B)	(0x00000200 & ((B) << 8 << 1))
#define ADC_OR(B)	(0x00000100 & ((B) << 8))
#define BSMPNEG(A)	(0x00000040 & ((A) << 6))
#define VSMPNEG(A)	(0x00000020 & ((A) << 5))
#define VSMPSEL(B)	(0x00000010 & ((B) << 4))
#define ACYCNRLC(B)	(0x00000008 & ((B) << 3))
#define ENOFFSET(B)	(0x00000004 & ((B) << 2))
#define ADCLKINV(B)	(0x00000002 & ((B) << 1))
#define AUTOCONF(B)	(0x00000001 & (B))
//0x0a4 --------------------------
#define FEWRA(B)	(0x00ff0000 & ((B) << 16))
#define FEWDATA(B)	(0x0000ffff & (B))
//0x0ac --------------------------
#define GLOW(B)		(0x3f000000 & ((B) << 24))
#define GHI(B)		(0x003f0000 & ((B) << 16))
#define RLOW(B)		(0x00003f00 & ((B) << 8))
#define RHI(B)		(0x0000003f & (B))
//0x0b0 --------------------------
#define P7L(B)		(0x3f000000 & ((B) << 24))
#define P6L(B)		(0x003f0000 & ((B) << 16))
#define BLOW(B)		(0x00003f00 & ((B) << 8))
#define BHI(B)		(0x0000003f & (B))
//0x0b4 --------------------------
#define P11L(B)		(0x3f000000 & ((B) << 24))
#define P10L(B)		(0x003f0000 & ((B) << 16))
#define P9L(B)		(0x00003f00 & ((B) << 8))
#define P8L(B)		(0x0000003f & (B))
//0x0b8 --------------------------------------------
#define VSMPL(B)	(0x00003f00 & ((B) << 8))
#define VSMPH(B)	(0x0000003f & (B))
//0x0c4-0x0d0 --------------------------
#define ADCPOSMAP(B)  (0xffffffff & (B))	//0x0c4
#define ADCPOSMAP2(B) (0xffffffff & (B))	//0x0c8
#define ADCNEGMAP(B)  (0xffffffff & (B))	//0x0cc
#define ADCNEGMAP2(B) (0xffffffff & (B))	//0x0d0

#endif //_gl310_afe_h_
