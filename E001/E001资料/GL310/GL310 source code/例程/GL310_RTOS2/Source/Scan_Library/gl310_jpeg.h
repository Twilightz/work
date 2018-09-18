
#ifndef _gl310_jpeg_h_
#define _gl310_jpeg_h_

//0x3d0 --------------------------------
#define Valid_Page(B)	(0x000001ff & (B))
#define	Jpeg_Valid_Page(B) (0x01ff0000 & ((B) << 16))
//0x3d4 ---------------------------------
#define Jpeg_Valid_size(B) (0xffffffc0 & (B))
#define page_flg(B)		(0x80000000 & ((B) << 24 << 7))
//0x3dc ---------------------------------
#define Clear_Jpeg_size(B) (0x7fffffc0 & (B))

#endif //_gl310_jpeg_h_
