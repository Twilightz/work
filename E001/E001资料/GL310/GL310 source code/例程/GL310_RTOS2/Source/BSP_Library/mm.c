/******************************************************************************
* Module name: mm.c
*
* Copyright 2004 Faraday. 
* All Rights Reserved.
*
* The information contained herein is confidential property of Company.
* The user, copying, transfer or disclosure of such information is 
* prohibited except by express written agreement with Company.
*
* First written on 2004/12 by Peter Liao.
*
* Module Description:
* Memory management API for non cache-able memory allocation routines.
*
 *****************************************************************************/
#include    <stdio.h>
#include    "dtype.h"


char DumpStr[512];
typedef __packed struct _MEMHEADER MEMHEADER;

typedef __packed struct _MEMHEADER
{
  MEMHEADER *  next;
  MEMHEADER *  prev;
  unsigned int       len;
  unsigned char      mem;
} MEMHEADER;

#define HEADER_SIZE (12)
#define ALIGN(x,a) (((x)+(a)-1)&~((a)-1)) 

void *_fLIb_NC_HeapStart;
UINT32 _fLIb_NC_HeapSize;

void fLib_NC_HeapInit(void *NC_HeapBase, unsigned int NC_HeapSize)
{
  MEMHEADER *pbase = (MEMHEADER *)NC_HeapBase;
  unsigned int size = NC_HeapSize - 1;
  _fLIb_NC_HeapStart = NC_HeapBase;
  _fLIb_NC_HeapSize = NC_HeapSize;
  pbase->next = (MEMHEADER *)((char *)pbase + size - HEADER_SIZE);
  pbase->next->next = NULL; //And mark it as last
  pbase->prev       = NULL; //and mark first as first
  pbase->len        = 0;    //Empty and ready.

}
void *fLib_NC_malloc (unsigned int size)
{

  MEMHEADER * current_header;
  MEMHEADER * new_header;
  
  if (size>((_fLIb_NC_HeapSize-1)-HEADER_SIZE))
  {
    printf("Heap Overflow\n");
    return NULL; //To prevent overflow in next line
  }


  size += HEADER_SIZE; //We need a memory for header too
  size = ALIGN(size,4);
  current_header = (MEMHEADER *)_fLIb_NC_HeapStart;

  while (1)
  {
    //    current
    //    |   len       next
    //    v   v         v
    //....*****.........******....
    //         ^^^^^^^^^
    //           spare
    if ((((unsigned int)current_header->next) -
         ((unsigned int)current_header) -
         current_header->len) >= size){
         
        break; //if spare is more than need
    }
   
    current_header = current_header->next;    //else try next            
	
    if (!current_header->next) 
        return NULL;  //if end_of_list reached   
     
  }

  if (!current_header->len)
  {
    //This code works only for first_header in the list and only
    current_header->len = size; //for first allocation
     
    return (void *)&current_header->mem;
  }
  else
  {
    //else create new header at the begin of spare
    new_header = (MEMHEADER * )((char *)current_header + current_header->len);
    new_header->next = current_header->next; //and plug it into the chain
    new_header->prev = current_header;
    current_header->next  = new_header;
    if (new_header->next)
    {
      new_header->next->prev = new_header;
    }
    new_header->len  = size; //mark as used
    
    return (void *)&new_header->mem;
  }
}

void fLib_NC_free (void *p)
{
  MEMHEADER *prev_header, *pthis;

  if ( p ) //For allocated pointers only!
  {
    pthis = (MEMHEADER * )((char *)  p - HEADER_SIZE); //to start of header
  
    if ( (int)pthis->len < 0)
      printf("NC mem free error\n");
    
    if ( pthis->prev ) // For the regular header
    {
      prev_header = pthis->prev;
      prev_header->next = pthis->next;
      if (pthis->next)
      {
        pthis->next->prev = prev_header;
      }
    }
    else
    {
      pthis->len = 0; //For the first header
    }
  }
}

BOOL fLib_NC_CheckHeap(void)
{
  void *Buf;
  if ( (Buf=fLib_NC_malloc(1)) == NULL )
  {
    printf("Heap is full\n");
    return FALSE;
  }
  else
  {
    printf("Non-cacheable Heap remains %d bytes\n",_fLIb_NC_HeapSize - ((UINT32)Buf - (UINT32)_fLIb_NC_HeapStart)- HEADER_SIZE);
   }
  
  fLib_NC_free(Buf);
  return TRUE;
}

