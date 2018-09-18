/*
**********************************************************************************************************
*  Copyright (C), 2010-2013, 
*
*  ��Ŀ����: XXXX
*  
*  �ļ�����: bsp.c
*
*  �ļ�����: 
*             
*             
*  �� �� ��: �ʺ���
*
*  ��������: 2013-06-24 
*
*  �� �� ��: V1.0
*
*  �޸ļ�¼:
*             
*      1. ��    ��:
*         �� �� ��:
*         �޸�����:
*      2. ...
**********************************************************************************************************
*/


#define   BSP_GLOBALS
#include  <bsp.h>
#include "flash_if.h"
#include "Flash.h"
#include "Shell.h"

/*
********************************************************************************
*  ��������: SysSoftRst
*
*  ��������: ϵͳ�����λ
*
*  �������: ��
*
*  �������: ��
*
*  �� �� ֵ: ��
*
********************************************************************************
*/
__asm void SysSoftRst(void)  
{   
	MOV R0, #1           //;    
	MSR FAULTMASK, R0    //;  �����ж�
	LDR R0, =0xE000ED0C  //;   
	LDR R1, =0x05FA0004  //;   
	STR R1, [R0]         //;  ����ж�    
deadloop      
	B deadloop           //;   ��ѭ�� 
}

/*
*********************************************************************************************************
*  �������ƣ� BSP_CPU_IntDis
*
*  ���������� ��ֹCPU���ж�
*
*  ��������� ��
*
*  ��������� ��
*
*  ���ز���:  ��
*
*********************************************************************************************************
*/
__asm void BSP_CPU_IntDis(void)
{
	CPSID   I
	BX      LR
}

/*
*********************************************************************************************************
*  �������ƣ� BSP_CPU_IntEn
*
*  ���������� ʹ��CPU���ж�
*
*  ��������� ��
*
*  ��������� ��
*
*  ���ز���:  ��
*
*********************************************************************************************************
*/
__asm void BSP_CPU_IntEn(void)
{
	CPSIE   I
	BX      LR
}

/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/
void  BSP_Init (void)
{
    // Configures the priority grouping: pre-emption priority and subpriority.
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // Sets the vector table location and Offset.
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, VECTTAB_FLASH_OFFSET);

}
/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*  
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

uint32_t  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);
    return ((uint32_t)rcc_clocks.HCLK_Frequency);
}


/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Initialize all the peripherals that required OS Tick services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
// void BSP_Tick_Init (void)
// {
//     uint32_t  cpu_clk_freq;
//     uint32_t  cnts;
//     
//     cpu_clk_freq = BSP_CPU_ClkFreq();                       /* Determine SysTick reference freq.                    */
//     
// #if (OS_VERSION >= 30000u)
//     cnts  = cpu_clk_freq / (INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
// #else
//     cnts  = cpu_clk_freq / (uint32_t)OS_TICKS_PER_SEC;      /* Determine nbr SysTick increments.                    */
// #endif
//     
//     OS_CPU_SysTickInit(cnts);                               /* Init uC/OS periodic time src (SysTick).              */
// }


