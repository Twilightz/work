#include "def.h"
#include "LibTimer.H"
#include "LibIntrc.h"
#include "LibGPIO.h"
#include "LibUart.h"
#include "tag.h"
#include "ClibUtil.h"
 
void __hardware_init(void);

extern int $Super$$main(int, void**);
extern void InitMMU(void); 
extern _clock_init(void);
void LED(UINT8 value);

	
// 121004 G-Kerwing add for DDR 58+6MB >>
#define	MEM_CFG_6M
#define SYS_MMU_TBL_SIZE   0x10000
#if defined(MEM_CFG_8M)
#define SYS_MMU_TBL_ADDR   (0x00800000 - SYS_MMU_TBL_SIZE)
#define SYS_NC_HEAP_BASE   0x00600000
#define SYS_NC_HEAP_SIZE   0x00200000
#elif defined(MEM_CFG_6M)
#define SYS_MMU_TBL_ADDR   (0x00600000 - SYS_MMU_TBL_SIZE)
#define SYS_NC_HEAP_BASE   0x00400000
#define SYS_NC_HEAP_SIZE   0x00200000
#else
#define SYS_MMU_TBL_ADDR   (0x02000000 - SYS_MMU_TBL_SIZE)
#define SYS_NC_HEAP_BASE   0x01C00000
#define SYS_NC_HEAP_SIZE   0x00400000
#endif
// 121004 G-Kerwing add for DDR 58+6MB <<


   
int $Sub$$main(int argc, void **argv)
{
	UINT8 VID; 
	UINT8 PID; 

   InitMMU();
  

// fLib_NC_HeapInit((void *)0x1c00000, 0x3ffffc);
   fLib_NC_HeapInit((void *)SYS_NC_HEAP_BASE, (SYS_NC_HEAP_SIZE-SYS_MMU_TBL_SIZE)); // 121004 G-Kerwing add for DDR 58+6MB // 121015 G-Kerwin correct issue (SYS_NC_HEAP_SIZE-SYS_NC_HEAP_SIZE)=0 --> wrong
  

   hw_SetIntTriggerMode(0xFFFFFFFF);
   //DisableTimer(1);	// enable timer tick
   DisableTimer(2);
   DisableTimer(3);   
   DisableTimer(4);
   DisableTimer(5);
   InitiateUART(CONSOLE_PORT,57600,8,'N',1);
   
      
   // system initialize
   if( !hw_sys_Tag_Initialize() ){
		printf("system infomation error!\n");
		while(1); // or do somethings
   }

   SPIC_buf_Initialize();      
   
   // sync the spif parameter with MainBoard Flash vender type
   tag_SPI_RDID(&VID, &PID);   
   
   if(VID == 0xEF)
      SpiParam->IDType = 1;     // WINBOND
   
   else if(VID == 0xC2)
      SpiParam->IDType = 0;     // MXIC
   
   else{
        printf("SPI flash vender not support\n");
        while(1);
   }
   
   return $Super$$main(argc, argv);
}


//HL modify ??
void __hardware_init()
{
	UINT32 wValue;
    //================================

    // set GPIO85 enable    
    wValue = ReadUINT32(0xA0000068);    
    wValue &= ~(0x8);                    // bit 3                    
    WriteUINT32(0xA0000068, wValue);
    
    // set GPIO85 OE = output
    wValue = ReadUINT32(0xA000005C);    
    wValue |= 0x4000;                    // bit 14 = 1
    WriteUINT32(0xA000005C, wValue);
 	
    wValue = ReadUINT32(0xA0000058);    
    wValue &= ~0x4000;                    // bit 14 = 0
    WriteUINT32(0xA0000058, wValue);

}
