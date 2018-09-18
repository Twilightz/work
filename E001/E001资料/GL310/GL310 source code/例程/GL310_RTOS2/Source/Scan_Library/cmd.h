
#ifndef _cmd_h_
#define _cmd_h_

#include "uDebug.h"
#include "uType.h"
#include "gl310.h"

typedef struct REG_STRUCT {
	U32 addr; U32 data;
} REG_T;

typedef struct REG_BIT_STRUCT {
	U32 addr; U32 clear; U32 set;
} REG_BIT_T;

typedef struct AFE_REG_STRUCT {
	U16 addr; U16 data;
} AFE_REG_T;

//--<cmd.h> --------------------------------------------------------------------------------
__inline U32 GET_REG(U32 addr) {return R32(addr);}
__inline void SET_REG(U32 addr,U32 data) {R32(addr)=data;}
__inline void SET_REG2(U32 addr,U32 data)	 {SET_REG(addr,data); SET_REG(addr+BENG,data);}
__inline void SET_REGBIT(U32 addr,U32 clear,U32 set) {R32(addr)=(R32(addr)&(~clear))|set;}
__inline void SET_REGBIT2(U32 addr,U32 clear,U32 set) {SET_REGBIT(addr,clear,set); SET_REGBIT(addr+BENG,clear,set);}
__inline U32 GET_AFEREG(U32 addr) {SET_REG(SCR(0x0a8),addr<<16); return R32(SCR(0x0a8));}
__inline U32 GET_b_AFEREG(U32 addr) {SET_REG(SCR(0x6a8),addr<<16); return R32(SCR(0x6a8));}
#ifdef EMBEDDED_ARM
__inline void WAIT_AFEREG() {int status; do{status=GET_REG(SCR(0x400));}while(status & FEBUSY(1));}
__inline void WAIT_b_AFEREG() {int status; do{status=GET_REG(SCR(0xa00));}while(status & b_FEBUSY(1));}
__inline void SET_AFEREG(U32 addr, U32 data) {data|=(addr << 16); SET_REG(SCR(0x0a4),data); WAIT_AFEREG();}
__inline void SET_b_AFEREG(U32 addr, U32 data) {data|=(addr << 16); SET_REG(SCR(0x6a4),data); WAIT_b_AFEREG();}
#else
__inline void SET_AFEREG(U32 addr, U32 data) {data|=(addr << 16); SET_REG(SCR(0x0a4),data);}
__inline void SET_b_AFEREG(U32 addr, U32 data) {data|=(addr << 16); SET_REG(SCR(0x6a4),data);}
#endif
//--<cmd.c> --------------------------------------------------------------------------------
int set_group_reg(REG_T *reg, int size);
int set_group_regbit(REG_BIT_T *reg, int size);
int set_group_afereg(AFE_REG_T *reg, int size);

//--<cmd_io.c> --------------------------------------------------------------------------------
int set_gpio40(int on);
int set_gpio71_82(U32 io, int on);
int set_gpio83(int on);
#define set_gpio78(on)	set_gpio71_82(GPIO78(1), on)
#define set_gpio79(on)	set_gpio71_82(GPIO79(1), on)
#define set_gpio80(on)	set_gpio71_82(GPIO80(1), on)
#define set_gpio81(on)	set_gpio71_82(GPIO81(1), on)
#define set_gpio82(on)	set_gpio71_82(GPIO82(1), on)

//common
//0x000 --------------------------
#define SET_BITSET(B)	SET_REGBIT2(SCR(0x000), BITSET(clr), BITSET(B))
#define SET_LAMPPWR(B)	SET_REGBIT2(SCR(0x000), LAMPPWR(clr), LAMPPWR(B))
#define SET_DPIHW(B)	SET_REGBIT2(SCR(0x000), DPIHW(clr), DPIHW(B))
#define SET_ACDCDIS(A)	SET_REGBIT(SCR(0x000), ACDCDIS(clr), ACDCDIS(A))
#define SET_AGOHOME(A)	SET_REGBIT(SCR(0x000), AGOHOME(clr), AGOHOME(A))
#define SET_MTRPWR(A)	SET_REGBIT(SCR(0x000), MTRPWR(clr), MTRPWR(A))
#define SET_FASTFED(A) SET_REGBIT(SCR(0x000), FASTFED(clr), FASTFED(A))
#define SET_MTRREV(A)	SET_REGBIT(SCR(0x000), MTRREV(clr), MTRREV(A))
#define SET_CISSET(B)	SET_REGBIT2(SCR(0x000), CISSET(clr), CISSET(B))
#define SET_CIS_LINE(B)	SET_REGBIT2(SCR(0x000), CIS_LINE(clr), CIS_LINE(B))
#define SET_CISPIXEL(B)	SET_REGBIT2(SCR(0x000), CISPIXEL(clr), CISPIXEL(B))
#define SET_LINEART(B)	SET_REGBIT2(SCR(0x000), LINEART(clr), LINEART(B))
#define SET_SEG_EN(B)	SET_REGBIT2(SCR(0x000), SEG_EN(clr), SEG_EN(B))
#define SET_SHDAREA(B) SET_REGBIT2(SCR(0x000), SHDAREA(clr), SHDAREA(B))
#define SET_shad_bk_idx(B)	SET_REGBIT2(SCR(0x000), shad_bk_idx(clr), shad_bk_idx(B))
#define SET_shad_one_ch(B)	SET_REGBIT2(SCR(0x000), shad_one_ch(clr), shad_one_ch(B))
//0x004 --------------------------------
#define SET_DVDSET(B) SET_REGBIT2(SCR(0x004), DVDSET(clr), DVDSET(B))
#define SET_GAIN4(B) SET_REGBIT2(SCR(0x004), GAIN4(clr), GAIN4(B))
#define SET_DVDTYPE(B) SET_REGBIT2(SCR(0x004), DVDTYPE(clr), DVDTYPE(B))
#define SET_CCDLMT(B)	SET_REGBIT2(SCR(0x004),CCDLMT(clr),CCDLMT(B))
#define SET_PIXEL_CLOCK(A)	SET_REGBIT(SCR(0x004), SCANMOD(clr), SCANMOD(A))
#define SET_DARKSHIFT(B) SET_REGBIT2(SCR(0x004), DARKSHIFT(clr), DARKSHIFT(B))
#define SET_FILTER(B) SET_REGBIT2(SCR(0x004), FILTER(clr), FILTER(B))
#define	SET_CLKSET(A)	SET_REGBIT(SCR(0x004), CLKSET(clr), CLKSET(A))
//0x008 --------------------------------
#define SET_PSCALE_X(B) SET_REGBIT2(SCR(0x008), PSCALE_X(clr), PSCALE_X(B))
#define SET_LINEOUT(B)	SET_REGBIT2(SCR(0x008), LINEOUT(clr), LINEOUT(B))
#define SET_PINORDER(B)	SET_REGBIT2(SCR(0x008), PINORDER(clr), PINORDER(B))
#define SET_CIS_2CH(B)	SET_REGBIT2(SCR(0x008), CIS_2CH(clr), CIS_2CH(B))
#define SET_CIS_3CH(B)	SET_REGBIT2(SCR(0x008), CIS_3CH(clr), CIS_3CH(B))
#define SET_gmm_para(B)	SET_REGBIT2(SCR(0x008), gmm_para(clr), gmm_para(B))
#define SET_FULLPAGE(A)	SET_REGBIT(SCR(0x008), FULLPAGE(clr), FULLPAGE(A))
#define SET_AUTO_ADF(A)	SET_REGBIT(SCR(0x008), AUTO_ADF(clr), AUTO_ADF(A))
#define SET_GMMENB(B)	SET_REGBIT2(SCR(0x008), GMMENB(clr), GMMENB(B))
#define SET_SCNUMENB(B)	SET_REGBIT2(SCR(0x008), SCNUMENB(clr), SCNUMENB(B))
#define SET_MPENB(A)	SET_REGBIT(SCR(0x008), MPENB(clr), MPENB(A))
//0x00c -------------------------------
#define SET_CLRDOCJM(A)	SET_REGBIT(SCR(0x00c), CLRDOCJM(clr), CLRDOCJM(A))
//0x010 -------------------------------
#define SET_TBTIME(A) SET_REGBIT(SCR(0x010), TBTIME(clr), TBTIME(A))

//status
//0x400 --------------------------------
#define GET_STATUS(A)	(GET_REG(SCR(0x400)) & (A))
#define GET_b_STATUS(A)	(GET_REG(b_SCR(0x400)) & (A))
//0x408 --------------------------------
#define GET_SW_VALID_LINE(D) GET_REG(SCR(0x408)+D*BENG)
#define SET_SW_VALID_LINE(A) SET_REG(SCR(0x408), (A))
#define SET_b_SW_VALID_LINE(B) SET_REG(b_SCR(0x408), (B))

//GPIO
//0x050 --------------------------------
#define SET_GPIO9(A) SET_REGBIT(SCR(0x050), GPIO9(clr), GPIO9(A))
//0x058 --------------------------------
#define SET_GPOE9(A) SET_REGBIT(SCR(0x058), GPOE9(clr), GPOE9(A))
//0x060 --------------------------------
#define SET_gpo9_cmode(A)	SET_REGBIT(SCR(0x60), gpo9_cmode(clr), gpo9_cmode(A))
#define SET_gpo2_gpobt(A)	SET_REGBIT(SCR(0x60), gpo2_gpobt(clr), gpo2_gpobt(A))
//0x068 --------------------------------
#define SET_SSLEDEN(A) SET_REGBIT(SCR(0x068), SSLEDEN(clr), SSLEDEN(A))
#define SET_CIS_LED(A) SET_REGBIT(SCR(0x068), CIS_LED(clr), CIS_LED(A))

//CIS
//0x080/0x084/0x088 --------------------------
#define SET_EXPR(B,D) SET_REG(SCR(0x080)+D*BENG, B)
#define SET_EXPG(B,D) SET_REG(SCR(0x084)+D*BENG, B)
#define SET_EXPB(B,D) SET_REG(SCR(0x088)+D*BENG, B)

//motor
//0x03c --------------------------------
#define SET_FSTPSEL(A) SET_REGBIT(SCR(0x03c), FSTPSEL(clr), FSTPSEL(A))
#define SET_STEPSEL(A) SET_REGBIT(SCR(0x03c), STEPSEL(clr), STEPSEL(A))
//0x150 --------------------------------
#define SET_MOTOR_TABLE_ADD(A)	SET_REG(SCR(0x150), (A))
//0x154 --------------------------------
#define SET_STEPNO(A)	SET_REGBIT(SCR(0x154), STEPNO(clr), STEPNO(A))
//0x15c --------------------------------
#define SET_FSHDEC(A)	SET_REGBIT(SCR(0x15c), FSHDEC(clr), FSHDEC(A))
#define SET_FASTNO(A)	SET_REGBIT(SCR(0x15c), FASTNO(clr), FASTNO(A))
//0x160 --------------------------------
#define SET_FMOVDEC(A)	SET_REGBIT(SCR(0x160), FMOVDEC(clr), FMOVDEC(A))
#define SET_FMOVNO(A)	SET_REGBIT(SCR(0x160), FMOVNO(clr), FMOVNO(A))
//0x170 --------------------------------
#define SET_FEEDL(A)	SET_REG(SCR(0x170), (A))
//0x174 --------------------------------
#define SET_PREFED(A)	SET_REG(SCR(0x174), (A))
//0x178 --------------------------------
#define SET_POSTFED(A)	SET_REGBIT(SCR(0x178), POSTFED(clr), POSTFED(A))
//0x17c --------------------------------
#define SET_SCANLEN(A)	SET_REG(SCR(0x17c), SCANLEN(clr), SCANLEN(A))

//0x1000 -------------------------------
#define GAMMA_ADDR_A(A)	(SCR(0x1000) + (A)*0x400)
#define GAMMA_ADDR_B(B)	(SCR(0x5000) + (B)*0x400)
//0x4000 --------------------------------
#define GET_SW_PAGE_LINE(D)	GET_REG(SCR(0x4000)+D*0x2000)
#define GET_SW_PAGE_SIZE(D)	GET_REG(SCR(0x4800)+D*0x2000)

//0x200 --------------------------------
#define SET_LPERIOD(B)	SET_REG2(SCR(0x200), (B))
#define GET_LPERIOD(D)	GET_REG(SCR(0x200)+D*BENG)
//0x204 --------------------------------
#define SET_LINCNT(B)	SET_REG2(SCR(0x204), (B))
//0x208 --------------------------------
#define SET_DPISET(B)	SET_REG2(SCR(0x208), (B))
//0x20c ---------------------------------
#define SET_LNOFSET(B) SET_REGBIT2(SCR(0x20c), LNOFSET(clr), LNOFSET(B))
//0x210 --------------------------------
#define SET_LINESEL(A)	SET_REGBIT(SCR(0x210), LINESEL(clr), LINESEL(A))
#define GET_DUMMY(D)	(GET_REG(SCR(0x210)+D*BENG) & DUMMY(clr))
//0x214/0x218 --------------------------------
#define SET_STRPIXEL(B) SET_REG2(SCR(0x214), (B))
#define SET_ENDPIXEL(B) SET_REG2(SCR(0x218), (B))
//0x220 --------------------------------
#define SET_VALIDIMG(B) SET_REG2(SCR(0x220), (B))
#define SET_SEGCNT(B) SET_REG2(SCR(0x224), (B))
//0x228 ---------------------------------------
#define SET_SEGNO(B) SET_REGBIT2(SCR(0x228), SEGNO(clr), SEGNO(B))
#define SET_TG0CNT(B) SET_REGBIT2(SCR(0x228), TG0CNT(clr), TG0CNT(B))

//0x2a0 ---------------------------------------
#define SET_MINSEL(B)	SET_REGBIT2(SCR(0x2a0), MINSEL(clr), MINSEL(B))
#define SET_GMM_F(B)	SET_REGBIT2(SCR(0x2a0), 0xff000000, B*0x70000000)
#define SET_GMM_N(B)	SET_REGBIT2(SCR(0x2a0), 0xff000000, B*0x07000000)
//0x2a4 --------------------------------
#define SET_MAXLINE(B) SET_REG2(SCR(0x2a4), (B))
#define SET_MINLINE(B) SET_REG2(SCR(0x2a8), (B))
//0x2ac-2c8 --------------------------------
#define REG_SH0DWN(D)	SCR(0x2ac+D*BENG)
//0x2cc-0x2f8 -------------------------------
#define SET_R1DWN(I,B) SET_REG(SCR(0x2cc)+I, B)
#define SET_b_R1DWN(I,B) SET_REG(SCR_B(0x2cc)+I, B)
//0x2fc/0x300/0x304 --------------------------------
#define SET_CIS1_SEG(B) SET_REG2(SCR(0x2fc), (B))
#define SET_CIS2_SEG(B) SET_REG2(SCR(0x300), (B))
#define SET_CIS3_SEG(B) SET_REG2(SCR(0x304), (B))

//0x3d0 --------------------------------
#define GET_Valid_Page(D)	(GET_REG(SCR(0x3d0)+D*BENG) & Valid_Page(clr))
#define GET_Jpeg_Valid_Page(D) (GET_REG(SCR(0x3d0)+D*BENG) >> 16)
//0x3d4 ---------------------------------
#define GET_Jpeg_Valid_size(D)	GET_REG(SCR(0x3d4)+D*BENG)
//0x3dc ---------------------------------
#define SET_Clear_Jpeg_size(A)	SET_REG(SCR(0x3dc), (A))
#define SET_b_Clear_Jpeg_size(B) SET_REG(b_SCR(0x3dc), (B))

#endif //_cmd_h_
