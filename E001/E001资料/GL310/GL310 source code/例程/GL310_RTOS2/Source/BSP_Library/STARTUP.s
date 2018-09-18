     INCLUDE        CPSR_DEF.INC
     INCLUDE        CHIP_DEF.INC


Reset               EQU        0
STACK_SIZE_USER     EQU        0x01400
STACK_SIZE_SVC      EQU        0x00800
STACK_SIZE_IRQ      EQU        0x00400
STACK_SIZE_UNDEF    EQU        0x00100
STACK_SIZE_FIQ      EQU        0x00100
STACK_SIZE_ABORT    EQU        0x00100

SYS_STACK_SIZE      EQU        0x00800
IRQ_STACK_SIZE      EQU        1024

TOP_OF_MEMORY       EQU        0x01C00000

;// 121004 G-Kerwing add for DDR 58+6MB >>
HEAP_SIZE           EQU        0x00200000

SYS_MMU_TBL_SIZE    EQU        0x00010000
NC_HEAP_MEM_SIZE    EQU        0x00200000
TOP_OF_MEMORY_8M    EQU        0x00800000
TOP_OF_MEMORY_6M    EQU        0x00600000

TOP_APP_MEMORY      EQU        TOP_OF_MEMORY_6M - NC_HEAP_MEM_SIZE - SYS_MMU_TBL_SIZE
;// 121004 G-Kerwing add for DDR 58+6MB <<


    IMPORT          |Image$$RO$$Limit|         ; ROM data starts after ROM program
    IMPORT          |Image$$RO$$Base|          ; Start of ROM program
    IMPORT          |Image$$RW$$Base|          ; Pre-initialised variables
    IMPORT          |Image$$RW$$Limit|
    IMPORT          |Image$$ZI$$Base|          ; uninitialised variables
    IMPORT          |Image$$ZI$$Limit|         ; End of variable RAM space
    IMPORT          |StartupData$$Base|
    IMPORT          |StartupData$$Limit|

    IMPORT          IntrDispatcher

    IMPORT          __rt_entry
    IMPORT          __hardware_init

    EXPORT          __main
    EXPORT          __user_initial_stackheap
    EXPORT          __scatterload
    EXPORT          Disp_7seg
    EXPORT          Disp_7segC

    EXPORT          _SetICacheEnable
    EXPORT			_EnableFIQ
    EXPORT			_EnableIRQ
    EXPORT          _SetICacheDisable
    EXPORT			_DisableFIQ
    EXPORT			_DisableIRQ

    PRESERVE8
    ;Invalid call from '~PRES8' function to 'REQ8' function

    AREA            Startup, CODE, READONLY
    ENTRY
Except_Vector
    LDR             PC,        ResetVector
    LDR             PC,        UndefVector
    LDR             PC,        SWIVector
    LDR             PC,        InstAbortVector
    LDR             PC,        DataAbortVector
    LDR             PC,        ReservdVector
    LDR             PC,        IRQ_Vector
    LDR             PC,        FIQ_Vector

ResetVector         DCD        Reset_Handler
UndefVector         DCD        UndefHandler
SWIVector           DCD        SWI_Handler
InstAbortVector     DCD        InstAbt_Handler
DataAbortVector     DCD        DataAbt_Handler
ReservdVector       DCD        Reserve_Handler
IRQ_Vector          DCD        IRQ_Exp_Handler
FIQ_Vector          DCD        FIQ_Exp_Handler

RT_StartOfROM       DCD        |Image$$RO$$Base|
RT_TopOfROM         DCD        |Image$$RO$$Limit|
RT_StartOfBSS       DCD        |Image$$RW$$Base|
RT_ZeroBSS          DCD        |Image$$ZI$$Base|
RT_EndOfBSS         DCD        |Image$$ZI$$Limit|

_Dual_DDR			EQU			1


;===================================================
; GLib_Sleepentry routine start
;
;===================================================
GLib_Sleepentry

	;:  Power-down mode , wakeup remapping , need some NOP instruction timing , Don't remove'

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
	NOP
 	NOP

   	NOP
	NOP
	NOP
	NOP

    LDR         r1, =0x90b00010
    LDR         r0, [r1]
    AND         r0, r0, #0x80
    CMP         r0, #0x80
    BEQ         wakeup_restore



	LDR			r1, =0xA0000103
	LDRB		r0, [r1]
	AND			r0, r0, #0x80
	CMP			r0, #0x80
	BEQ			Reset_Handler


wakeup_restore

    NOP
	NOP
	NOP
	NOP

    NOP
	NOP
	NOP
	NOP

   ;; prepare restore - Stack ,Lr , pc

    LDR     r2, =R13_svc
    LDR     r13, [r2]

    LDR     r2, =R14_svc
    LDR     r14, [r2]


    LDR     r2, =pc_svc
    LDR     r8, [r2]

    MOV     pc, r8


	NOP
   	NOP

    B       .


;===================================================
; GLib_Sleepentry routine end;
;===================================================




UndefHandler
    B               UndefHandler

SWI_Handler
    STMDB           SP!, {R0-R12, LR, PC}
    MRS             R0, CPSR
    BIC             R0, R0, #(I_BIT | F_BIT)
    MSR             CPSR_c, R0
    MRS             R1, SPSR
    ANDS            R1, R1, #T_BIT
    SUBEQ           R0, LR, #0x04
    SUBNE           R0, LR, #0x02
    MOVNE           R1, #1
    MOV             R2, SP
    LDMIA           SP!, {R0-R12, LR}
    ADD             SP, SP, #4
    BX              LR


InstAbt_Handler
    B               InstAbt_Handler

DataAbt_Handler
    LDR             R0, =0xFFFF
    MOV             R1, #0x099
    STR             R1, [R0]
    LDR             R1, =DataAbt_Handler
    MOV             PC, R1

Reserve_Handler
    B               Reserve_Handler

IRQ_Exp_Handler
    STMDB           SP!, {R0-R3}                        ; Save some scratch registers
    LDR             R0, =AddressOfIrqISR                ; Check whether AddressOfIrqISR is 0 or not
    LDR             R0, [R0]
    CMP             R0, #0
    BXNE            R0
    STMDB           SP!, {R4,LR}                        ; Save return address
                                                        ;  (as well as R4 just for 8-byte alignment)
    BL              IntrDispatcher
    LDMIA           SP!, {R4,LR}
    LDMIA           SP!, {R0-R3}
    SUBS            PC, LR, #4

FIQ_Exp_Handler
    B               FIQ_Exp_Handler







Reset_Handler       FUNCTION


    BL              __hardware_init

    ; bootloader interrupt mask patch

    MOV             r0,  #0x00
    LDR             r1,  =0x98800004   ;Interrupt Mask register
    STR             r0,  [R1]


	LDR				r1, =0xA0000054
	LDR				r0, [r1]

	AND				r0,r0,#0xFFFFFF00
	ORR				r0,r0,#0x00000022           ; Set UARTA/UARTB clock freq = 30Mhz
	STR				r0,[r1]


    LDR	            r1,=0xA0000050				; Clock select
    LDR				r0,=0x7FFFFFFF
    STR				r0,[r1]


	LDR	            r1,=0xA0000068				; Interface select
	LDR				r0,[r1]

	ORR				r0,r0,#0x00000c00			; Set UARTA/UARTB select
 	STR     		r0,[r1]



    MOV             R0,  #0x00
    MOV             R1,  #0x00
    MOV             R2,  #0x00
    MOV             R3,  #0x00
    MOV             R4,  #0x00
    MOV             R5,  #0x00
    MOV             R6,  #0x00
    MOV             R7,  #0x00
    MOV             R8,  #0x00
    MOV             R9,  #0x00
    MOV             R10, #0x00
    MOV             R11, #0x00

    LDR             r1, =TopOfStack
;   LDR             r9, =TOP_OF_MEMORY ;// 121004 G-Kerwing add for DDR 58+6MB
    LDR             r9, =TOP_APP_MEMORY ;// 121004 G-Kerwing add for DDR 58+6MB
    STR             r9, [r1]

    LDR             R1, =TopOfStack
    LDR             R0, [R1]
    BL              stack_init


    MRS             r1, cpsr
    BIC             r1, #(I_BIT | F_BIT)
    MSR             cpsr_c, r1
    LDR             pc, =__main
                    ENDFUNC

__main              FUNCTION
    BL              __scatterload
    LDR             r1,=0xb0000050
    LDR             r0,=0x00
    STR             r0,[r1]

    B               __rt_entry
__sys_halt
    B               __sys_halt
                    ENDFUNC

__user_initial_stackheap  FUNCTION
    LDR             R0, =BaseOfStack
    LDR             R3, [R0]
    LDR             R0, =TopOfHeap
    LDR             R2, [R0]
    LDR             R0, =TopOfStack
    LDR             R1, [R0]
    LDR             R0, =BaseOfHeap
    LDR             R0,[R0]
    BX              LR
                    ENDFUNC

__scatterload       FUNCTION
    STMFD           sp!, {r14}
    STMFD           sp!, {r0, r1, r2, r3}
    LDR             R0, =BaseOfStack
    LDR             R3,[R0]
    LDR             R0, =TopOfHeap
    LDR             R2,[R0]
    LDR             R0, =TopOfStack
    LDR             R1,[R0]
    LDR             R0, =BaseOfHeap
    LDR             R0,[R0]
    STMFD           sp!, {r0, r1, r2, r3}
    LDR             r0, =BaseOfFreeMemory
    LDR             R3, [R0]
    LDR             r0, =TopOfFreeMemory
    LDR             R2, [R0]
    STMFD           sp!, {r2, r3}
    BL              _InitBSSMemory
    LDMFD           sp!, {r2, r3}
    LDR             R0, =TopOfFreeMemory
    STR             R2, [R0]
    LDR             R0, =BaseOfFreeMemory
    STR             R3, [r0]
    LDMFD           sp!,{r1, r2, r3}
    LDR             R0, =BaseOfHeap
    STR             r1,[r0]
    LDR             R0, =TopOfStack
    STR             r2,[r0]
    LDR             R0, =TopOfHeap
    STR             r3,[r0]
    LDMFD           sp!,{r1}
    LDR             R0, =BaseOfStack
    STR             r1,[r0]
    LDMFD           sp!, {r0, r1, r2, r3}
    LDMFD           sp!, {PC}
                    ENDFUNC

stack_init          FUNCTION
    MOV             r6, lr

    MOV             r1, #(Undef32Mode | I_BIT | F_BIT)
    MSR             cpsr_c, r1
    BIC             r2, r0, #3
    MOV             sp, r2

    SUB             r2, r2, #STACK_SIZE_UNDEF
    BIC             r2, r2, #3

    MOV             r1, #(Abort32Mode | I_BIT | F_BIT)
    MSR             cpsr_c, r1
    MOV             sp, r2

    SUB             r2, r2, #STACK_SIZE_ABORT
    BIC             r2, r2, #3

    MOV             r1, #(FIQ32Mode | I_BIT | F_BIT)
    MSR             cpsr_c, r1
    MOV             sp, r2

    SUB             r2, r2, #STACK_SIZE_FIQ
    BIC             r2, r2, #3

    MOV             r1, #(IRQ32Mode | I_BIT | F_BIT)
    MSR             cpsr_c, r1
    MOV             sp, r2

    SUB             r2, r2, #STACK_SIZE_IRQ
    BIC             r2, r2, #3

    MOV             r1, #(Sys32Mode | I_BIT | F_BIT)
    MSR             cpsr_c, r1
    MOV             sp, r2

    SUB             r2, r2, #STACK_SIZE_USER
    BIC             r2, r2, #3

    MOV             r1, #(SVC32Mode | I_BIT | F_BIT)
    MSR             cpsr_c, r1
    MOV             sp, r2

    LDR             r1, =TopOfStack
    STR             r2, [r1]

    SUB             r2, r2, #STACK_SIZE_SVC
    BIC             r2, r2, #3

    LDR             r1, =BaseOfStack
    STR             r2, [r1]

    SUB             r2, r2, #4                 ;; Add for Threadx
    LDR             r1, =TopOfFreeMemory
    STR             r2, [r1]

;   SUB             r2, r2, #4                 ;; Remove for Threadx
;   LDR             r1, =TopOfHeap
;   STR             r2, [r1]

;   SUB             r2, r2, #HEAP_SIZE         ;; Remove for ThreadX
;   BIC             r2, r2, #3
;   LDR             r1, =BaseOfHeap
;   STR             r2, [r1]

    LDR             r2, =|Image$$ZI$$Limit|    ;; Change for threadx
    LDR             r1, =BaseOfHeap
    ADD             r2, r2, #8
    BIC             r2, r2, #3
    STR             r2, [r1]

    LDR             r1, =TopOfHeap             ;; Change for threadx
    ADD             r2, r2, #HEAP_SIZE
    ADD             r2, r2, #8
    BIC             r2, r2, #3
    STR             r2, [r1]

    ADD             r2, r2, #4                 ;; Add for threadx
    LDR             r1, =BaseOfFreeMemory
    STR             r2, [r1]


    MOV             r11, #0
    MOV             pc, r6
                    ENDFUNC


_InitBSSMemory      FUNCTION
    STMFD           sp!, {r4-r8, r14}
    LDR             r4, RT_TopOfROM         ; Copy predefined variables from here..
    LDR             r5, RT_StartOfBSS       ; ..to here
    LDR             r6, RT_ZeroBSS          ; 'C' BSS starts here
    CMP             r4, r5                  ; Make sure the RO and RW is linear if not ...
    BEQ             %7
6
    CMP             r5, r6                  ; Check if done..
    LDRCC           r7, [r4], #4            ; if not, get word and store to RAM
    STRCC           r7, [r5], #4
    BCC             %6
7
    LDR             r4, RT_EndOfBSS         ; Top of zero init memory
    ADD             r4, r4, #4

    MOV             r5, #0
8
    CMP             r6, r4                  ; Check if done..
    STRCC           r5, [r6], #4            ; if not, clear another word
    BCC             %8

    LDR             r0,=BaseOfFreeMemory    ;save base of Free memory
    STR             r4 ,[r0]

    ; restore registers and return
    LDMFD           sp!, {r4-r8, pc}
                    ENDFUNC


;========================================================
; Load SPI Parm
;========================================================
Load_SPI_Parm


	LDR				r1, =CPE_SPIC_BASE + SPI_PARAM_REG		; set AHBA Start Addr Reg.
	LDR				r0, =0x0								; set SPI clock Freq Div 2
	STR				r0, [r1]



	LDR				r1, =CPE_SPIC_BASE + SPI_AMBA_ADDR_REG	; set AHBA Start Addr Reg.
  	STR				r2, [r1]

	LDR				r1, =CPE_SPIC_BASE + SPI_TRANS_Len_REG	; set CmdBitLen & DataByteLen Reg
  	STR				r3, [r1]

	LDR				r1, =CPE_SPIC_BASE + SPI_C1CmdH_REG		; set start SPI addr & read cmd(03H)
 	STR				r4, [r1]

	LDR				r1, =CPE_SPIC_BASE + SPI_CTRL_REG		; set SPI ctrl reg enable
	LDR				r0, =0x01
	STR				r0, [r1]

	LDR				r1, =CPE_SPIC_BASE + SPI_FLAG_REG		; poll SPI read proc. finish


01
	LDR				r0, [r1]
	TST				r0, #0x1								; read done, z flag = 0
	BEQ				%b01									; Z flag = 1


	STR				r0, [r1]								; clear SPI finish flag

	MOV				pc, lr


;==============================================================================
;								Disp_7seg()
;
;r0 ==> display data
;r1,r3,r4,r6
;==============================================================================
Disp_7seg


	LDR				r1, =CPE_STRAP_REG			; Load HW Strap Pin status
 	LDRB			r5, [r1]

	AND				r6, r5, #Pad_Debug			; r5.4 Debug strap pin(1->debug enable)

	TST             r6, #Pad_Debug
    BEQ             %F7

	LDR				r3, =SevenSeg_Data			; Dedicate where the 7Seg_LED data physical address
 	MOV				r1, pc
	AND				r1, r1, #0x80000000
	ADD				r3, r3, r1

	MOV				r0, r0, LSL #2				; fetch 7-Seg display data
	LDR				r4, [r3, r0]



	LDR				r3, =GPIO_Data				; Load GPIO pin status
	LDR				r0, [r3]


	LDR				r1, =0xfffff800
	AND				r0, r0, r1					; Clear GPIO1~GPIO11(w/o GPIO5)
	ORR				r4, r0, r4

	STR				r4, [r3]					; Ouput GPIO with New 7-Seg data

7

	MOV				pc, lr



;==============================================================================
;								Disp_7seg()
;
;r0 ==> display data
;r1,r3,r4,r6
;==============================================================================
Disp_7segC

	STMFD 		    r13!, {r1-r12, r14}		    ; backup current mode r1-r12,lr
	MRS			    r1, cpsr			    	; cpsr->r1
	MRS			    r2, spsr			    	; spsr->r2
	STMFD 		    r13!, {r1, r2}		    	; backup cpsr, spsr


	LDR				r1, =CPE_STRAP_REG			; Load HW Strap Pin status
 	LDRB			r5, [r1]


    TST             r5, #Pad_Debug
    BEQ             %F7


	LDR				r3, =SevenSeg_Data			; Dedicate where the 7Seg_LED data physical address
 	MOV				r1, pc
	AND				r1, r1, #0x80000000
	ADD				r3, r3, r1

	MOV				r0, r0, LSL #2				; fetch 7-Seg display data
	LDR				r4, [r3, r0]



	LDR				r3, =GPIO_Data				; Load GPIO pin status
	LDR				r0, [r3]


	LDR				r1, =0xfffff800
	AND				r0, r0, r1					; Clear GPIO1~GPIO11(w/o GPIO5)
	ORR				r4,r0,r4

	STR				r4,[r3]						; Ouput GPIO with New 7-Seg data

7


	LDMFD		    r13!, {r1, r2}	        	; restore r1,r2
	MSR		    	cpsr_c, r1		        	; cpsr<-r1
	MSR			    spsr_c, r2		        	; spsr<-r2
	LDMFD	    	r13!, {r1-r12, pc}	        ; restore current mode r1-r12, pc

;===========================================================================
;	BSP_CPUSetICacheEnable
;===========================================================================
_SetICacheEnable

   	MRC 			p15, 0, r0, c1, c0, 0
   	ORR  			r0, r0, #0x1000
   	MCR				p15, 0, r0, c1, c0, 0

  	MOV				pc,lr


;===========================================================================
;	BSP__CPUSetICacheEnable
;===========================================================================
_SetICacheDisable

   	MRC 			p15, 0, r0, c1, c0, 0
   	BIC   			r0, r0, #0x1000
   	MCR				p15, 0, r0, c1, c0, 0

  	MOV				pc,lr


;===========================================================================
;	BSP_EnableFIQ
;===========================================================================
_EnableFIQ
	MRS 	        r0, CPSR
    BIC         	r0, r0, #F_BIT
    MSR         	CPSR_c, r0
	MOV	        	pc, lr


;===========================================================================
;	BSP_DisableFIQ
;===========================================================================
_DisableFIQ
	MRS         	r0, CPSR
    ORR         	r0, r0, #F_BIT
    MSR         	CPSR_c, r0
	MOV	        	pc, lr

;===========================================================================
;	BSP_EnableIRQ
;===========================================================================
_EnableIRQ
	MRS         	r0, CPSR
    BIC         	r0, r0, #I_BIT
    MSR         	CPSR_c, r0
	MOV	              pc, lr


;===========================================================================
;	BSP_DisableIRQ
;===========================================================================
_DisableIRQ

	MRS     	r0, CPSR
	ORR     	r0, r0, #I_BIT
    MSR     	CPSR_c, r0
	MOV	    	pc, lr





    LTORG



SevenSeg_Data       DCD     _0,_1,_2,_3,_4,_5,_6,_7,_8,_9, \
 							_0Dot,_1Dot,_2Dot,_3Dot,_4Dot,_5Dot,_6Dot,_7Dot,_8Dot,_9Dot,\
 							_A,_B,_C,_D,_E,_F,_U,_ADot,_BDot,_CDot,_DDot,_EDot,_FDot,_UDot




    AREA  StartupData, DATA, READWRITE

    EXPORT          AddressOfIrqISR
    EXPORT          BaseOfFreeMemory
    EXPORT          TopOfFreeMemory

AddressOfIrqISR     DCD    0                ; Address of IRQ ISR

TopOfStack          DCD    0                ; Top of stack (at top of RAM)
BaseOfStack         DCD    0
TopOfHeap           DCD    0                ; Top of heap (below bottom of stacks)
BaseOfHeap          DCD    0
TopOfFreeMemory     DCD    0                ; Top of Free memory
BaseOfFreeMemory    DCD    0
TEST_DDR			DCD	   0

    END
