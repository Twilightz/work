#if !defined(_CLIBUTIL_H_)
#define _CLIBUTIL_H_

#include "dtype.h"
UINT32 Byte2Word(UINT8 *pPBS);
UINT16 Byte2HalfWord(UINT8 *pPBS);

INT16 strnicmp(INT8 *string1, INT8 *string2, UINT16 Len);
int   stricmp(INT8 *Str1, INT8 *Str2);

int MySprintf(char *out, const char *format, ...);
int MyMemCpy(void* pTarget, void* pSrc, int nSize);
int MyMemSet(void* pTarget, char val, int nSize);

#define sprintf       MySprintf
#define memcpy        MyMemCpy
#define memset        MyMemSet

#endif

