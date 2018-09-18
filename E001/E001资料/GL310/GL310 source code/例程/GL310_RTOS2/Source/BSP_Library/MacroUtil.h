#if !defined(_MacroUtil_h_)
#define _MacroUtil_h_

#define ReadByte(ADDR)				(*((volatile unsigned char *)(ADDR)))
#define ReadWord(ADDR)        	(*((volatile unsigned short int *)(ADDR)))
#define ReadDWord(ADDR)       	(*((volatile unsigned int *)(ADDR)))

#define ReadUINT8(ADDR)				(*((volatile unsigned char *)(ADDR)))
#define ReadUINT16(ADDR)        	(*((volatile unsigned short int *)(ADDR)))
#define ReadUINT32(ADDR)       	(*((volatile unsigned int *)(ADDR)))

#define ReadPort8(ADDR)				(*((volatile unsigned char *)(ADDR)))
#define ReadPort16(ADDR)        	(*((volatile unsigned short int *)(ADDR)))
#define ReadPort32(ADDR)       	(*((volatile unsigned int *)(ADDR)))

#define WriteByte(ADDR, VALUE)	((*((volatile unsigned char *)(ADDR)))=(VALUE))
#define WriteWord(ADDR, VALUE)   ((*((volatile unsigned short int *)(ADDR)))=(VALUE))
#define WriteDWord(ADDR, VALUE)  ((*((volatile unsigned int *)(ADDR)))=(VALUE))

#define WriteUINT8(ADDR, VALUE)	((*((volatile unsigned char *)(ADDR)))=(VALUE))
#define WriteUINT16(ADDR, VALUE) ((*((volatile unsigned short int *)(ADDR)))=(VALUE))
#define WriteUINT32(ADDR, VALUE) ((*((volatile unsigned int *)(ADDR)))=(VALUE))

#define WritePort8(ADDR, VALUE)	((*((volatile unsigned char *)(ADDR)))=(VALUE))
#define WritePort16(ADDR, VALUE) ((*((volatile unsigned short int *)(ADDR)))=(VALUE))
#define WritePort32(ADDR, VALUE) ((*((volatile unsigned int *)(ADDR)))=(VALUE))
#endif
