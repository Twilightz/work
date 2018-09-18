/*
********************************************************************************
*  Copyright (C), 2009-2013, 合众思壮西安研发中心
*
*  项目名称：xxxx
*  
*  文件名称: xxxx.c
*
*  文件描述：xxxx
*             
*             
*  创 建 者: 皇海明
*
*  创建日期：2013-03-18 
*
*  版 本 号：V1.0
*
*  修改记录： 
*             
*      1. 日    期： 
*         修 改 人： 
*         所作修改： 
*      2. ...
********************************************************************************
*/
#include "Flash.h"

u8 MemWriteData(u32 addr,u16 *data,u16 num)
{
    FLASH_Status temp_stat;
    u32 temp_addr = addr;
    u16 i;
     
    FLASH_Unlock(); 
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR); 
    temp_stat=FLASH_ErasePage(temp_addr); 
    if(temp_stat != FLASH_COMPLETE)
     {
         FLASH_Lock();
         return 0;
     }
    for(i=0;i<num;i++)
    { 
        temp_stat = FLASH_ProgramHalfWord((temp_addr +i*2),data[i]);  //flash  为一个字节存储，16位数据必须地址加2 
        if(temp_stat != FLASH_COMPLETE)
         {
              FLASH_Lock();
              return 0;
         }
        
    } 
    FLASH_Lock(); 
    return 1;
}

u8 MemReadData(u32 addr,u16 *data,u16 num)                                
 {
      u32 *temp_addr = (u32 *)addr;
      u16 i;
         
      for(i=0;i<num;i++)
      {
         data[i] = *(temp_addr +i*2);
      }                                                                                                
      return 1;                                                                                                        
 }







