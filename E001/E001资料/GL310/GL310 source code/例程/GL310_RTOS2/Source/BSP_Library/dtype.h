#if !defined(_D_TYPE_H_)     
#define _D_TYPE_H_

#define  FALSE             (0)
#define  TRUE              (!FALSE)

#define false		FALSE
#define true		TRUE

#define OFF         false
#define ON          true

#if !defined(NULL)
#define  NULL              ((void*)(0))
#endif


typedef int                BOOL;
typedef unsigned long long UINT64;
typedef unsigned int       UINT32;
typedef unsigned short     UINT16;
typedef unsigned char      UINT8;

typedef long long          INT64;
typedef int                INT32;
typedef short              INT16;
typedef char               INT8;

typedef unsigned int       DWORD;
typedef unsigned short     WORD;
typedef unsigned char      BYTE;


#endif



