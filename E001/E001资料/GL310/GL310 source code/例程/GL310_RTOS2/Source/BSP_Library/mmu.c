#include "dtype.h"

#define D_CACHE_BIT                 2
#define I_CACHE_BIT                 12
#define MMU_BIT                     0
#define MPU_BIT                     0
#define NF_BIT                      30
#define ROUND_ROBIN_REPLACE         14

#define CP15_CFG_MMU_EN_BIT         0
#define CP15_CFG_ALIGN_EN_BIT       1
#define CP15_CFG_DCACHE_EN_BIT      2
#define CP15_CFG_WRITEBUF_EN_BIT    3
#define CP15_CFG_BIGENDIAN_EN_BIT   7
#define CP15_CFG_BTB_EN_BIT         11
#define CP15_CFG_ICACHE_EN_BIT      12
#define CP15_CFG_VECT_BASE_BIT      13

#define CP15_EXT_ECE_EN_MASK        (1<<CP15_EXT_ECE_EN_BIT)
#define CP15_EXT_PDMS_EN_MASK       (1<<CP15_EXT_PDMS_EN_BIT)

#define CP15_CFG_MMU_EN_MASK        (1<<CP15_CFG_MMU_EN_BIT)
#define CP15_CFG_ALIGN_EN_MASK      (1<<CP15_CFG_ALIGN_EN_BIT)
#define CP15_CFG_DCACHE_EN_MASK     (1<<CP15_CFG_DCACHE_EN_BIT)
#define CP15_CFG_WRITEBUF_EN_MASK   (1<<CP15_CFG_WRITEBUF_EN_BIT)
#define CP15_CFG_BIGENDIAN_EN_MASK	(1<<CP15_CFG_BIGENDIAN_EN_BIT)
#define CP15_CFG_BTB_EN_MASK        (1<<CP15_CFG_BTB_EN_BIT)
#define CP15_CFG_ICACHE_EN_MASK     (1<<CP15_CFG_ICACHE_EN_BIT) 
#define CP15_CFG_VECT_BASE_MASK     (1<<CP15_CFG_VECT_BASE_BIT)

#define MEM_CFG_6M  // 121004 G-Kerwing add for DDR 58+6MB

#define SYS_MMU_TBL_SIZE   0x10000
// 121004 G-Kerwing add for DDR 58+6MB >>
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

typedef struct {
   unsigned int tlb_type : 2;
   unsigned int buffered : 1;
   unsigned int cached   : 1;
   unsigned int reserved1: 1;     
   unsigned int domain   : 4;
   unsigned int reserved2: 1;
   unsigned int ap       : 2;          
   unsigned int reserved3: 8;
   unsigned int sec_base : 12;
}sec_tlb;

typedef struct {
   unsigned int StartAddr;
   unsigned int Length;
   int          CacheEn;
   int          BufferEn;
} MM_Ctrl_Entry;

MM_Ctrl_Entry mmu_cfg[] = {
      {0x00000000,   0x01C00000,    0x01,    0x01}, // first 28M memory is cached & buffered
      {0x01C00000,   0x00400000,    0x00,    0x00}, // 29M to 32M is non-cached, non-buffered
      {0x02000000,   0x7E000000,    0x00,    0x00}, // dummy memory space
      {0x80000000,   0x80000000,    0x00,    0x00}  // IO Space is non-cached, non-buffered
//    StartAddr,     Length,      Buffered, Cached,       
};   

// 121004 G-Kerwing add for DDR 58+6MB >>
MM_Ctrl_Entry mmu_8m_cfg[] = {
      {0x00000000,   0x00600000,    0x01,    0x01}, // first 6M memory is cached & buffered
      {0x00600000,   0x00200000,    0x00,    0x00}, // 7M to 8M is non-cached, non-buffered
      {0x00800000,   0x7F800000,    0x00,    0x00}, // dummy memory space
      {0x80000000,   0x80000000,    0x00,    0x00}  // IO Space is non-cached, non-buffered
//    StartAddr,     Length,      Buffered, Cached,       
};   

MM_Ctrl_Entry mmu_6m_cfg[] = {
      {0x00000000,   0x00400000,    0x01,    0x01}, // first 4M memory is cached & buffered
      {0x00400000,   0x00200000,    0x00,    0x00}, // 5M to 6M is non-cached, non-buffered
      {0x00600000,   0x7FA00000,    0x00,    0x00}, // dummy memory space
      {0x80000000,   0x80000000,    0x00,    0x00}  // IO Space is non-cached, non-buffered
//    StartAddr,     Length,      Buffered, Cached,       
};   
// 121004 G-Kerwing add for DDR 58+6MB <<
const sec_tlb   default_tbl_cfg = {2, 0, 0, 1, 0, 0, 3, 0, 0};

void Create_MMU_Table(unsigned int TableAddr, MM_Ctrl_Entry *cfg_tbl, int CfgNum)
{
   MM_Ctrl_Entry  *cfg_ptr  = cfg_tbl;
   sec_tlb        *tlb_ptr = (sec_tlb*)TableAddr,
                  tlb_entry = default_tbl_cfg;
   int            iCfgCnt, 
                  iEntryCnt,
                  iEntryNum;
   int            iDbgCnt;
   
   for(iCfgCnt = 0; iCfgCnt< CfgNum; iCfgCnt++){   
      tlb_entry.buffered   = (cfg_ptr->BufferEn == 0)?0:1;
      tlb_entry.cached     = (cfg_ptr->CacheEn == 0)?0:1;
      tlb_entry.sec_base   = (cfg_ptr->StartAddr)>>20;
      iEntryNum            = (cfg_ptr->Length)>>20;
      iDbgCnt              = 0;
      tlb_ptr              = (sec_tlb*)(TableAddr) + tlb_entry.sec_base;
      
      for (iEntryCnt=0; iEntryCnt < iEntryNum; iEntryCnt++) {
         *tlb_ptr++= tlb_entry;
         tlb_entry.sec_base ++;
         iDbgCnt++;         
      }
      
      cfg_ptr++;
   }
}

//unsigned int dbg_tbl[4096];

void InitMMU(void)
{
   unsigned   mm_tbl_addr = SYS_MMU_TBL_ADDR;
   unsigned   dwReg;
   
   Create_MMU_Table((unsigned int)mm_tbl_addr, mmu_6m_cfg/*mmu_cfg*/, sizeof(mmu_cfg)/sizeof(MM_Ctrl_Entry)); // 121004 G-Kerwing add for DDR 58+6MB
   
   // set TTB, locate at 16K boundary
   // enable data/instruction cache 
   __asm{ MCR p15, 0, mm_tbl_addr, c2, c0, 0;}
   __asm{ nop;nop;nop}
   
   dwReg = 0x01;
   __asm{ MCR p15, 0, dwReg, c3, c0, 0;}    
   __asm{ nop;nop;nop}    
   
   // enable cache
   __asm{ MRC p15, 0, dwReg, c1, c0, 0;}
   
   dwReg = dwReg | (1 << I_CACHE_BIT) | (1 << D_CACHE_BIT )|(1 << MMU_BIT)|(1 <<ROUND_ROBIN_REPLACE);//|(1<< NF_BIT);
   
   __asm{ MCR p15, 0, dwReg, c1, c0, 0;}
   __asm{ nop;nop;nop}
   
   // invalidate TLB
   __asm{ MCR p15, 0, dwReg, c8, c7, 0;} 

   // invalidate ICache DCache
   dwReg = 0;
   __asm{ MCR p15, 0, dwReg, c7, c7, 0;}   
  
}

void FA526_CPUSetDomainAccessCtrl(UINT32 domain, UINT32 ctrl)
{
   UINT32 reg, temp;
	
	__asm {
      mov      temp, #0x3
 	   mov      temp, temp, lsl domain
 	   mvn      temp, temp
   	MRC      p15,0,reg,c3,c0,0
   	and      reg, reg,temp
   	mov      ctrl, ctrl, lsl domain
   	orr      reg, reg, ctrl
   	MCR      p15,0,reg,c3,c0,0
  	}
}

void FA526_CPUSetICacheEnable(UINT32 enable)
{
   UINT32 tmp; 
 
 	__asm {
      MRC      p15,0,tmp,c1,c0,0
      CMP      enable,#0   
      ORRNE    tmp,tmp,#CP15_CFG_ICACHE_EN_MASK
      BICEQ    tmp,tmp,#CP15_CFG_ICACHE_EN_MASK
      MCR      p15,0,tmp,c1,c0,0
  	}
}

void FA526_CPUSetDCacheEnable(UINT32 enable)
{
   UINT32 tmp; 

 	__asm {
      MRC      p15,0,tmp,c1,c0,0
      CMP      enable,#0   
      ORRNE    tmp,tmp,#CP15_CFG_DCACHE_EN_MASK
      BICEQ    tmp,tmp,#CP15_CFG_DCACHE_EN_MASK  	
      MCR      p15,0,tmp,c7,c10, 0		   	     //clean cache 
   	MCR      p15,0,tmp,c1,c0, 0
  	}
}

void FA526_CPUSetMMUEnable(UINT32 enable)
{
   UINT32 tmp;
  
 	__asm {
      MRC      p15,0,tmp,c1,c0,0
      CMP      enable,#0   
      ORRNE    tmp,tmp,#CP15_CFG_MMU_EN_MASK
      BICEQ    tmp,tmp,#CP15_CFG_MMU_EN_MASK
      MCR      p15,0,tmp,c1,c0,0
  	}	
}


void Disable_MMU(void)
{

    UINT32 tmp;
    
    tmp = 0;

	__asm
	{
	
	    NOP
		NOP
		NOP
		NOP
 	
 		
		MCR		p15, 0, tmp, c7, c14, 0   // Invalid ICache and DCache all   
    	MCR		p15, 0, tmp, c7, c5,  0  
    	MCR		p15, 0, tmp, c7, c10, 4   // Sync - drain write buffer
		MCR		p15, 0, tmp, c7, c5,  5   // Invalidate IScratchpad
		MCR		p15, 0, tmp, c7, c6,  5   // Invalidate BTB
	
	
		// c1 is Icache & Dcache En/Diaanle
		MRC		p15, 0, tmp, c1, c0, 0; 	 // Read c1
	


		BIC     tmp,tmp,#0x1000			 //temp &= ~(1<<I_CACHE_BIT);    BIT 12		
		BIC     tmp,tmp,#0x0002			 //temp &= ~(1<<D_CACHE_BIT);    BIT 2
		BIC		tmp,tmp,#0x1			 //temp &= ~(1<<MMU_BIT);	     BIT 0	 	
		BIC		tmp,tmp,#0x4000			 //temp &= ~(1<<Round rounbin);	 BIT 14	 	
		
		MCR 	p15, 0, tmp, c1, c0, 0 
	
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		

	}
}


void Enable_MMU(void)
{
   unsigned   mm_tbl_addr = SYS_MMU_TBL_ADDR;
   unsigned   dwReg;
   
   __asm{ MCR p15, 0, mm_tbl_addr, c2, c0, 0;}
   __asm{ nop;nop;nop}
   
   dwReg = 0x01;
   __asm{ MCR p15, 0, dwReg, c3, c0, 0;}    
   __asm{ nop;nop;nop}    
   
   // enable cache
   __asm{ MRC p15, 0, dwReg, c1, c0, 0;}
   
   dwReg = dwReg | (1 << I_CACHE_BIT) | (1 << D_CACHE_BIT )|(1 << MMU_BIT)|(1 <<ROUND_ROBIN_REPLACE);//|(1<< NF_BIT);
   
   __asm{ MCR p15, 0, dwReg, c1, c0, 0;}
   __asm{ nop;nop;nop}
   
   // invalidate TLB
   __asm{ MCR p15, 0, dwReg, c8, c7, 0;} 

   // invalidate ICache DCache
   dwReg = 0;
   __asm{ MCR p15, 0, dwReg, c7, c7, 0;}   
  
  
}
	


