#include "ClibUtil.h"
/******************************************************************************
UINT32   Byte2Word(UINT8 *pPBS)
*Data type transfer
******************************************************************************/
UINT32 Byte2Word(UINT8 *pPBS)
{
   
   UINT32 Data = 0;
   
   Data = Data | (( UINT32) *pPBS);
   Data = Data | (((UINT32) *(pPBS+1))<<8);
   Data = Data | (((UINT32) *(pPBS+2))<<16);
   Data = Data | (((UINT32) *(pPBS+3))<<24);
   
   return Data;
   
      
}

/******************************************************************************
UINT16   Byte2HalfWord(UINT8 *pPBS)
*Data type transfer
******************************************************************************/
UINT16 Byte2HalfWord(UINT8 *pPBS)
{
   
   UINT16 Data = 0;
   
   Data = Data | (( UINT16) *pPBS);
   Data = Data | (((UINT16) *(pPBS+1))<<8);

   return (UINT16)Data;
   
      
}

/*********************************************************************************
 *    INT16 strnicmp(INT8 *string1,INT8 *string2, UINT16 Len)
 *********************************************************************************/
INT16 strnicmp(INT8 *string1,INT8 *string2, UINT16 Len)
{
 
   
   while(Len >0){
   
      if((*string2 == '\0')&&(*string1 == '\0'))
         break;
      
   
      
      
      if( *string1 > *string2){
         
         if(*string1 >='a'){
            
            if(*string1 - ('a' - 'A')  != *string2)
               return 1;
            
         }
         else
            return 1;

      }
      
      if( *string2 > *string1){
         
         if(*string2 >='a'){
            
            if(*string2 - ('a' - 'A')  != *string1)
               return -1;
            
         }
         else
            return -1;

      }
      
      
      
      string1++;
      string2++;
      Len--;
   }

   return 0;   
}

/*********************************************************************************
 *    int stricmp(INT8 *Str1, INT8 *Str2)
 *********************************************************************************/
int stricmp(INT8 *Str1, INT8 *Str2)
{
   while(*Str1 != '\0')
   {
      if(*Str2 == '\0')
         return 1;      
      if((*Str1 > *Str2) && ((*Str1 - ('a' - 'A')) != *Str2))
         return 1;
      if((*Str1 < *Str2) && ((*Str2 - ('a' - 'A')) != *Str1))
         return -1;
      Str1++;
      Str2++;
   }
   if(*Str2 != '\0')
      return -1;
      
   return 0;   
}

