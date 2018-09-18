
#ifndef _gl310_image_h_
#define _gl310_image_h_

//0x4000 --------------------------------
#define SW_PAGE_LINE(B) (0x001fffff & (B))
#define SW_PAGE_SIZE(B)	(B)

//0x200 --------------------------------
#define LPERIOD(B)	(0x001fffff & (B))
//0x204 --------------------------------
#define LINCNT(B)	(0x00ffffff & (B))
//0x208 --------------------------------
#define DPISET(B)	(0x000fffff & (B))
//0x20c ---------------------------------
#define LNOFSET(B)	(0x00ff0000 & ((B) << 16))
#define STGSET(B)	(0x00007f00 & ((B) << 8))
#define BLINE1ST(B)	(0x00000001 & (B))
//0x210 --------------------------------
#define LINESEL(A)	(0x1f000000 & ((A) << 24))	// 0x210
#define DUMMY(B)	(0x00007fff & (B))			// 0x210
//0x214/0x218 --------------------------------
#define STRPIXEL(B)	(0x0001ffff & (B))	// 0x214
#define ENDPIXEL(B)	(0x0001ffff & (B))	// 0x218
//0x21c --------------------------------
#define IMGGAP(A)	(0x0000ffff & (A))	// 0x21c
//0x220 --------------------------------
#define VALIDIMG(B)	(0x0001ffff & (B))	// 0x220
#define SEGCNT(B)	(0x001fffff & (B))	// 0x224
//0x228 ---------------------------------------
#define SEGNO(B)	(0x001f0000 & ((B) << 16))	// 0x228
#define TG0CNT(B)	(0x0000ffff & (B))			// 0x228
//0x230 ---------------------------------------
#define MinPixel(B)	(0xffff0000 & ((B) << 16))
//0x234 ---------------------------------------
#define PphLnSet(A)	(0x0000ffff & (A))


//0x2a0 ---------------------------------------
#define IR1GMM_F(B)	(0x80000000 & ((B) << 24 << 7))
#define BGMM_F(B)	(0x40000000 & ((B) << 24 << 6))
#define GGMM_F(B)	(0x20000000 & ((B) << 24 << 5))
#define RGMM_F(B)	(0x10000000 & ((B) << 24 << 4))
#define IR1GMM_N(B)	(0x08000000 & ((B) << 24 << 3))
#define BGMM_N(B)	(0x04000000 & ((B) << 24 << 2))
#define GGMM_N(B)	(0x02000000 & ((B) << 24 << 1))
#define RGMM_N(B)	(0x01000000 & ((B) << 24))
#define EndPgSet(B)	(0x001f0000 & ((B) << 16))
#define MINSEL(B)	(0x00000700 & ((B) << 8))
#define IMGLMT(B)	(0x0000000f & (B))
//0x2a4 --------------------------------
#define MAXLINE(B)	(0x000fffff & (B))	// 0x2a4
#define MINLINE(B)	(0x000fffff & (B))	// 0x2a8
//0x2ac-2c8 --------------------------------
#define SH0DWN(B)	(0xfffffc00 & (B))	//0x2ac
#define SH1DWN(B)	(0xfffffc00 & (B))	//0x2b0
#define SH2DWN(B)	(0xfffffc00 & (B))	//0x2b4
#define SH3DWN(B)	(0xfffffc00 & (B))	//0x2b8
#define SH4DWN(B)	(0xfffffc00 & (B))	//0x2bc
#define SH5DWN(B)	(0xfffffc00 & (B))	//0x2c0
#define SH6DWN(B)	(0xfffffc00 & (B))	//0x2c4
#define SH7DWN(B)	(0xfffffc00 & (B))	//0x2c8
//0x2cc-0x2f8 -------------------------------
#define R1DWN(B)	(0xffffffff & (B))	//0x2cc
#define R1UP(B)		(0xffffffff & (B))	//0x2d0
#define R2DWN(B)	(0xffffffff & (B))	//0x2d4
#define R2UP(B)		(0xffffffff & (B))	//0x2d8
#define G1DWN(B)	(0xffffffff & (B))	//0x2dc
#define G1UP(B)		(0xffffffff & (B))	//0x2e0
#define G2DWN(B)	(0xffffffff & (B))	//0x2e4
#define G2UP(B)		(0xffffffff & (B))	//0x2e8
#define B1DWN(B)	(0xffffffff & (B))	//0x2ec
#define B1UP(B)		(0xffffffff & (B))	//0x2f0
#define B2DWN(B)	(0xffffffff & (B))	//0x2f4
#define B2UP(B)		(0xffffffff & (B))	//0x2f8
//0x2fc/0x300/0x304 --------------------------------
#define CIS1_SEG(B)	(0x0001ffff & (B))	// 0x2fc
#define CIS2_SEG(B)	(0x0001ffff & (B))	// 0x300
#define CIS3_SEG(B)	(0x0001ffff & (B))	// 0x304
//0x3a0 --------------------------------
#define ARRAY_ENB(B) (0x00000001 & (B))
//0x308 --------------------------------
#define AUTO_S(B)	(0x0000ff00 & ((B) << 8))
#define AUTO_O(B)	(0x0000000f & (B))
//0x30c --------------------------------
#define CONV_RC1(B)	(0x0000ffff & (B))			//0x30C
#define CONV_RC2(B)	(0xffff0000 & ((B) << 16))	//0x30C
#define CONV_RC3(B)	(0x0000ffff & (B))			//0x310
#define CONV_RC4(B)	(0xffff0000 & ((B) << 16))	//0x310
#define CONV_RC5(B)	(0x0000ffff & (B))			//0x314
#define CONV_GC1(B)	(0x0000ffff & (B))			//0x318
#define CONV_GC2(B)	(0xffff0000 & ((B) << 16))	//0x318
#define CONV_GC3(B)	(0x0000ffff & (B))			//0x31C
#define CONV_GC4(B)	(0xffff0000 & ((B) << 16))	//0x31C
#define CONV_GC5(B)	(0x0000ffff & (B))			//0x320
#define CONV_BC1(B)	(0x0000ffff & (B))			//0x324
#define CONV_BC2(B)	(0xffff0000 & ((B) << 16))	//0x324
#define CONV_BC3(B)	(0x0000ffff & (B))			//0x328
#define CONV_BC4(B)	(0xffff0000 & ((B) << 16))	//0x328
#define CONV_BC5(B)	(0x0000ffff & (B))			//0x32C
//0x330 --------------------------------
#define R_MATRIX_R(B)	(0x0000ffff & (B))			// 0x330
#define R_MATRIX_G(B)	(0xffff0000 & ((B) << 16))	// 0x330
#define R_MATRIX_B(B)	(0x0000ffff & (B))			// 0x334
#define G_MATRIX_R(B)	(0x0000ffff & (B))			// 0x338
#define G_MATRIX_G(B)	(0xffff0000 & ((B) << 16))	// 0x338
#define G_MATRIX_B(B)	(0x0000ffff & (B))			// 0x33c
#define B_MATRIX_R(B)	(0x0000ffff & (B))			// 0x340
#define B_MATRIX_G(B)	(0xffff0000 & ((B) << 16))	// 0x340
#define B_MATRIX_B(B)	(0x0000ffff & (B))			// 0x344
//0x348 --------------------------------
#define BWLOW(B)		(0x000000ff & (B))			// 0x348
#define BWHI(B)			(0x0000ff00 & ((B) << 8))	// 0x348

#endif //_gl310_image_h_
