
#ifndef _gl310_common_h_
#define _gl310_common_h_

//0x000 --------------------------
#define BITSET(B)	(0x80000000 & ((B) << 24 << 7))
#define AVEENB(B)	(0x40000000 & ((B) << 24 << 6))
#define LAMPPWR(B)	(0x10000000 & ((B) << 24 << 4))
#define DPIHW(B)	(0x06000000 & ((B) << 24 << 1))
#define PWRBIT(A)	(0x01000000 & ((A) << 24))
#define ACDCDIS(A)	(0x00400000 & ((A) << 16 << 6))
#define AGOHOME(A)	(0x00200000 & ((A) << 16 << 5))
#define MTRPWR(A)	(0x00100000 & ((A) << 16 << 4))
#define FASTFED(A)	(0x00080000 & ((A) << 16 << 3))
#define MTRREV(A)	(0x00040000 & ((A) << 16 << 2))
#define CISSET(B)	(0x00008000 & ((B) << 8 << 7))
#define CIS_LINE(B)	(0x00004000 & ((B) << 8 << 6))
#define CISPIXEL(B)	(0x00002000 & ((B) << 8 << 5))
#define LINEART(B)	(0x00001000 & ((B) << 8 << 4))
#define SEG_EN(B)	(0x00000800 & ((B) << 8 << 3))
#define SHDAREA(B)	(0x00000200 & ((B) << 8 << 1))
#define SCAN(A)		(0x00000100 & ((A) << 8))
#define shad_bk_idx(B)	(0x00000004 & ((B) << 2))
#define shad_one_ch(B)	(0x00000002 & ((B) << 1))
//0x004 --------------------------------
#define DVDSET(B)	(0x80000000 & ((B) << 24 << 7))
#define GAIN4(B)	(0x40000000 & ((B) << 24 << 6))
#define DVDTYPE(B)	(0x30000000 & ((B) << 24 << 4))
#define CCDLMT(B)	(0x0f000000 & ((B) << 24))
#define SCANMOD(A)	(0x007f0000 & ((A) << 16))
#define DARKSHIFT(B) (0x0000f000 & ((B) << 8 << 4))
#define OPTEST(B)	(0x00000700 & ((B) << 8))
#define FILTER(B)	(0x00000030 & ((B) << 4))
#define CLKSET(A)	(0x0000000f & (A))
//0x008 --------------------------------
#define PSCALE_X(B)	(0x20000000 & ((B) << 24 << 5))
#define SCALE_X(B)	(0x10000000 & ((B) << 24 << 4))
#define SCALE_Y(B)	(0x08000000 & ((B) << 24 << 3))
#define LINEOUT(B)	(0x01000000 & ((B) << 24))
#define MOD_INV(B)	(0x00800000 & ((B) << 16 << 7))
#define PINORDER(B)	(0x00400000 & ((B) << 16 << 6))
#define CIS_2CH(B)	(0x00200000 & ((B) << 16 << 5))
#define CIS_3CH(B)	(0x00100000 & ((B) << 16 << 4))
#define gmm_para(B)	(0x00080000 & ((B) << 16 << 3))
#define FULLPAGE(A)	(0x00040000 & ((A) << 16 << 2))
#define AUTO_ADF(A)	(0x00020000 & ((A) << 16 << 1))
#define ADFSEL(A)	(0x00010000 & ((A) << 16))
#define GMMENB(B)	(0x00008000 & ((B) << 8 << 7))
#define CONV_ENB(B)	(0x00000800 & ((B) << 8 << 3))
#define SCNUMENB(B)	(0x00000010 & ((B) << 4))
#define MPENB(A)	(0x00000008 & ((A) << 3))
//0x00c -------------------------------
#define MOVE(A)		(0x01000000 & ((A) << 24))
#define SCANRESET(A) (0x00800000 & ((A) << 16 << 7))
#define CLRERR(A)	(0x00002000 & ((A) << 8 << 5))
#define FULLSTP(A)	(0x00001000 & ((A) << 8 << 4))
#define CLRMCNT(A)	(0x00000400 & ((A) << 8 << 2))
#define CLRDOCJM(A)	(0x00000200 & ((A) << 8 << 1))
#define CLRLNCNT(A)	(0x00000100 & ((A) << 8))
#define CLRMTR2ERR(A) (0x00000020 & ((A) << 5))
#define CLRMTR1ERR(A) (0x00000010 & ((A) << 4))
#define CLRMTR0ERR(A) (0x00000008 & ((A) << 3))
#define CLRMTR2JM(A) (0x00000004 & ((A) << 2))
#define CLRMTR1JM(A) (0x00000002 & ((A) << 1))
#define CLRMTR0JM(A) (0x00000001 & (A))
//0x010 -------------------------------
#define TBTIME(A)	(0x00000070 & ((A) << 4))
//0x018 -------------------------------
#define WDTIME(A)	(0x0f000000 & ((A) << 24))
//0x01c -------------------------------
#define FEBUSSEL(B)	(0x00000300 & ((B) << 8))
#define SIFSEL(B)	(0x00000070 & ((B) << 4))
#define AFEMOD(B)	(0x00000007 & (B))
//0x630 --------------------------------
#define b_afe_sel(A)	(0x00000001 & (A))
#define b_engine(A)		(0x00000008 & ((A) << 3))
#define b_img_only(A)	(0x00000010 & ((A) << 4))

//status
//0x400 --------------------------------
#define MOTMFLG(A)		(0x00000002 & ((A) << 1))
#define DOCJAM(A)		(0x00000008 & ((A) << 3))
#define ADFSNR(A)		(0x00000040 & ((A) << 6))
#define DOCSNR(A)		(0x00000080 & ((A) << 7))
#define FEBUSY(A)		(0x00000200 & ((A) << 8 << 1))
#define MOVEERR(A)		(0x00080000 & ((A) << 16 << 3))
#define MOVEJAM(A)		(0x00100000 & ((A) << 16 << 4))
//0xa00 --------------------------------
#define b_FEBUSY(B)		(0x00020000 & ((B) << 16 << 1))	// 0xa00
//0x408 --------------------------------
#define SW_VALID_LINE(B) (0x801fffff & (B))
#define raw_page_flg(B) (0x80000000 & ((B) << 24 << 7))

#endif //_gl310_common_h_
