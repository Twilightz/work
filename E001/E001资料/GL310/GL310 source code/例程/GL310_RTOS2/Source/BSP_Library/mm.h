#if !defined(_MM_H_)
#define _MM_H_
#include "dtype.h"
 
void fLib_NC_HeapInit(void *NC_HeapBase, unsigned int NC_HeapSize);
void *fLib_NC_malloc (unsigned int size);
void fLib_NC_free (void *p);
BOOL fLib_NC_CheckHeap(void); 


#endif
