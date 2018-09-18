
#ifndef _gl310_ccd_h_
#define _gl310_ccd_h_

//CIS
//0x080/0x084/0x088 --------------------------
#define EXPR(B)		(0x001fffff & (B))
#define EXPG(B)		(0x001fffff & (B))
#define EXPB(B)		(0x001fffff & (B))
//0x090 --------------------------
#define ONDUR(B)	(0x0000ffff & (B))
#define OFFDUR(B)	(0xffff0000 & ((B) << 16))
//0x094 --------------------------
#define PULSELEN(B)	(0xff000000 & ((B) << 24))
#define MODPULSE(B)	(0x00ff0000 & ((B) << 16))
#define MODSTR(B)	(0x0000ffff & (B))

//CCD
//0x100 --------------------------------
#define CKSEL(B)	(0x03000000 & ((B) << 24))
#define RSNEG(B)	(0x00100000 & ((B) << 16 << 4))
#define CPNEG(B)	(0x00080000 & ((B) << 16 << 3))
#define CK4MTGL(B)	(0x00040000 & ((B) << 16 << 2))
#define CK3MTGL(B)	(0x00020000 & ((B) << 16 << 1))
#define CK1MTGL(B)	(0x00010000 & ((B) << 16))
#define CK4LOW(B)	(0x00004000 & ((B) << 8 << 6))
#define CK3LOW(B)	(0x00002000 & ((B) << 8 << 5))
#define CK1LOW(B)	(0x00001000 & ((B) << 8 << 4))
#define CK4INV(B)	(0x00000800 & ((B) << 8 << 3))
#define CK3INV(B)	(0x00000400 & ((B) << 8 << 2))
#define LINECLP(B)	(0x00000200 & ((B) << 8 << 1))
#define CNSET(B)	(0x00000100 & ((B) << 8))
#define CTRLHI(B)	(0x00000080 & ((B) << 7))
#define TOSHIBA(B)	(0x00000040 & ((B) << 6))
#define TGINV(B)	(0x00000020 & ((B) << 5))
#define CK2INV(B)	(0x00000010 & ((B) << 4))
#define CK1INV(B)	(0x00000008 & ((B) << 3))
#define CTRLINV(B)	(0x00000004 & ((B) << 2))
#define CKDIS(B)	(0x00000002 & ((B) << 1))
#define CTRLDIS(B)	(0x00000001 & (B))
//0x104 ---------------------------------
#define TGSHLD(B)	(0x0fff0000 & ((B) << 16))
#define TGW(B)		(0x00001fff & (B))
//0x108 ----------------------------------
#define EXPDMY(B)	(0xffff0000 & ((B) << 16))
#define SW_SH(B)	(0x00000fff & (B))
//0x10c ----------------------------------
#define CPH(B)		(0x3f000000 & ((B) << 24))
#define CPL(B)		(0x003f0000 & ((B) << 16))
#define RSH(B)		(0x00003f00 & ((B) << 8))
#define RSL(B)		(0x0000003f & (B))
//0x110/0x710 --------------------------------
#define CK4_NEG(B)	(0x00080000 & ((B) << 16 << 3))
#define CK4_XOR(B)	(0x00040000 & ((B) << 16 << 2))
#define CK4_AND(B)	(0x00020000 & ((B) << 16 << 1))
#define CK4_OR(B)	(0x00010000 & ((B) << 16))
#define CK3_NEG(B)	(0x00000800 & ((B) << 8 << 3))
#define CK3_XOR(B)	(0x00000400 & ((B) << 8 << 2))
#define CK3_AND(B)	(0x00000200 & ((B) << 8 << 1))
#define CK3_OR(B)	(0x00000100 & ((B) << 8))
#define CK1_NEG(B)	(0x00000008 & ((B) << 3))
#define CK1_XOR(B)	(0x00000004 & ((B) << 2))
#define CK1_AND(B)	(0x00000002 & ((B) << 1))
#define CK1_OR(B)	(0x00000001 & (B & 1))
//0x114/0x714 --------------------------------
#define CK43_XOR(B)	(0x00200000 & ((B) << 16 << 5))
#define CK43_AND(B)	(0x00100000 & ((B) << 16 << 4))
#define CK43_OR(B)	(0x00080000 & ((B) << 16 << 3))
#define CK41_XOR(B)	(0x00040000 & ((B) << 16 << 2))
#define CK41_AND(B)	(0x00020000 & ((B) << 16 << 1))
#define CK41_OR(B)	(0x00010000 & ((B) << 16))
#define CK34_XOR(B)	(0x00002000 & ((B) << 8 << 5))
#define CK34_AND(B)	(0x00001000 & ((B) << 8 << 4))
#define CK34_OR(B)	(0x00000800 & ((B) << 8 << 3))
#define CK31_XOR(B)	(0x00000400 & ((B) << 8 << 2))
#define CK31_AND(B)	(0x00000200 & ((B) << 8 << 1))
#define CK31_OR(B)	(0x00000100 & ((B) << 8))
#define CK14_XOR(B)	(0x00000020 & ((B) << 5))
#define CK14_AND(B)	(0x00000010 & ((B) << 4))
#define CK14_OR(B)	(0x00000008 & ((B) << 3))
#define CK13_XOR(B)	(0x00000004 & ((B) << 2))
#define CK13_AND(B)	(0x00000002 & ((B) << 1))
#define CK13_OR(B)	(0x00000001 & (B))
//0x118-0x144 --------------------------
#define CK1POSMAP(B)  (0xffffffff & (B))	//0x118
#define CK1POSMAP2(B) (0xffffffff & (B))	//0x11c
#define CK1NEGMAP(B)  (0xffffffff & (B))	//0x120
#define CK1NEGMAP2(B) (0xffffffff & (B))	//0x124
#define CK3POSMAP(B)  (0xffffffff & (B))	//0x128
#define CK3POSMAP2(B) (0xffffffff & (B))	//0x12c
#define CK3NEGMAP(B)  (0xffffffff & (B))	//0x130
#define CK3NEGMAP2(B) (0xffffffff & (B))	//0x134
#define CK4POSMAP(B)  (0xffffffff & (B))	//0x138
#define CK4POSMAP2(B) (0xffffffff & (B))	//0x13c
#define CK4NEGMAP(B)  (0xffffffff & (B))	//0x140
#define CK4NEGMAP2(B) (0xffffffff & (B))	//0x144

#endif //_gl310_ccd_h_
