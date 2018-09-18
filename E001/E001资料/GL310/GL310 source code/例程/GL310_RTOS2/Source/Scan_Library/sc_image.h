#ifndef _sc_image_h_
#define _sc_image_h_
//-----------------

#include <stdio.h>
#include "cmd.h"

typedef struct IMAGE_STRUCT {
	U32 format;	// 'RAW', 'JPG', 'TIF', 'BMP', 'PDF', 'PNG'
	U16	option;	// 0
	U8	bit;	// 1:BW, 8:Gray8, 16:Gray16, 24:Color24, 48:Color48
	U8	mono;	// 0:'MONO', 1:'R', 2:'G', 4:'B', 8:'IR', 7:'NTSC'
	struct{U16 x; U16 y;} dpi;
	struct{U32 x; U32 y;} org;
	struct{U32 w; U32 h;} dot;
} IMAGE_T;

typedef struct IMAGE_FILE_STRUCT {
	IMAGE_T img;
	FILE *stream;
	U32 width;
	U32	line;
	U32 size;
} IMG_FILE_T;

typedef struct IMAGE_MEMORY_STRUCT {
	IMAGE_T img;
	U32 width;
	U32 line;
	U32 size;
} IMG_MEM_T;

int Bmp_OpenFile(IMG_FILE_T *file, char *FileName);
int Bmp_WriteFile(IMG_FILE_T *file, void *data, int size);
int Bmp_CloseFile(IMG_FILE_T *file);

int Tiff_OpenFile(IMG_FILE_T *file, char *FileName);
int Tiff_WriteFile(IMG_FILE_T *file, void *data, int size);
int Tiff_CloseFile(IMG_FILE_T *file);

int Jpeg_OpenFile(IMG_FILE_T *file, char *FileName);
int Jpeg_WriteFile(IMG_FILE_T *file, void *data, int size);
int Jpeg_CloseFile(IMG_FILE_T *file, int lines);

//-----------------
#endif // _sc_image_h_
