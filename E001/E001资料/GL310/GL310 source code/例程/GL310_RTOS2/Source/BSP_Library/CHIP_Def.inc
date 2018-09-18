;-------------------------------------------------------------------------------
; Decoder definitions
;-------------------------------------------------------------------------------

CPE_DECODER_BASE        EQU     0x90100000
CPE_AHB_SLAVE0_REG      EQU     0x0
CPE_AHB_SLAVE1_REG      EQU     0x4
CPE_AHB_SLAVE2_REG      EQU     0x8
CPE_AHB_SLAVE3_REG      EQU     0xc
CPE_AHB_SLAVE4_REG      EQU     0x10
CPE_AHB_SLAVE5_REG      EQU     0x14
CPE_AHB_SLAVE6_REG      EQU     0x18
CPE_AHB_SLAVE7_REG      EQU     0x1c
CPE_AHB_SLAVE8_REG      EQU     0x20
CPE_AHB_SLAVE9_REG      EQU     0x24
CPE_AHB_SLAVE10_REG     EQU     0x28

CPE_PRIORITY_REG        EQU     0x80
CPE_DEFAULT_MASTER_REG  EQU     0x84
CPE_REMAP_REG           EQU     0x88 


CPE_SPIC_BASE			EQU		0x90C00000
SPI_PARAM_REG			EQU		0x00
SPI_C1CmdH_REG			EQU		0x08
SPI_AMBA_ADDR_REG		EQU		0x44				
SPI_TRANS_Len_REG		EQU		0x48				
SPI_CTRL_REG			EQU		0x4C				
SPI_FLAG_REG			EQU		0x50				
SPI_CHECKSUM_REG		EQU		0x54				


TagID_size				EQU		0x20

Pad_Debug				EQU		0x10
Pad_Update				EQU		0x08
Pad_Coloum9 			EQU		0x04
Pad_Bank8				EQU		0x02
Pad_DDRM				EQU		0x01



CLK30					EQU		0x00011100
CLK96					EQU		0x000AAA00
CLK120					EQU		0x000BBB00


DebugImageBase          EQU     0xB0005000
DDRTmpBase              EQU     0xB0005C00 
SPI_DummyBase			EQU	    0xA0000010

DDRM200T				EQU		6
DDRII200T				EQU		12

GPIO_En			    	EQU		0xB0000058
GPIO_Data				EQU		0xB0000050


CPE_STATUS_REG			EQU		0xA0000103	; M[A0000103].7 , Byte Access 
CPE_CLK_REG				EQU		0xA0000054

CPE_STRAP_REG			EQU		0xA0000100	; M[A0000100], Byte Access 
ScratchPad 				EQU	    0xA0000010	;		
;Temp_Param				EQU	    0xA0000010	;				
ScratchSize				EQU	    0x40		; Allocate size 144 bytes (90h)		



; USED on CPU sleep and wakeup procedure!

R13_svc					EQU		 ScratchPad + (0 << 2)
R14_svc					EQU		 ScratchPad + (1 << 2)
pc_svc					EQU		 ScratchPad + (2 << 2)
	
svc_offset 				EQU	     (3 << 2)

R13_sys					EQU		 ScratchPad + svc_offset + (0 << 2)
R14_sys					EQU		 ScratchPad + svc_offset + (1 << 2)
R13_abt					EQU		 ScratchPad + svc_offset + (2 << 2)
R14_abt					EQU		 ScratchPad + svc_offset + (3 << 2)
R13_und					EQU		 ScratchPad + svc_offset + (4 << 2)
R14_und					EQU		 ScratchPad + svc_offset + (5 << 2)
R13_irq					EQU		 ScratchPad + svc_offset + (6 << 2)
R14_irq					EQU		 ScratchPad + svc_offset + (7 << 2)
R13_fiq					EQU		 ScratchPad + svc_offset + (8 << 2)
R14_fiq					EQU		 ScratchPad + svc_offset + (9 << 2)



 	

 IF :DEF: _FPGA
; new			
_0						EQU		0x088
_1					 	EQU		0x0EE
_2					  	EQU		0x1A0 	
_3						EQU		0x0A2
_4				  		EQU		0x0C6	
_5				  		EQU		0x083
_6				  		EQU		0x081
_7				  		EQU		0x0AE
_8				  		EQU		0x080
_9				  		EQU		0x082

_0Dot			  		EQU		0x008
_1Dot			  		EQU		0x06E
_2Dot			  		EQU		0x120
_3Dot			  		EQU		0x022
_4Dot			  		EQU		0x046
_5Dot			  		EQU		0x003
_6Dot			  		EQU		0x001 
_7Dot			  		EQU		0x02E
_8Dot			  		EQU		0x000
_9Dot			  		EQU		0x002

_A						EQU		0x084
_B						EQU		0x0C1
_C						EQU		0x189
_D						EQU		0x0E0
_E						EQU		0x181
_F						EQU		0x185
_U						EQU		0x0C8

_ADot					EQU		0x004
_BDot					EQU		0x041
_CDot					EQU		0x109
_DDot					EQU		0x060
_EDot					EQU		0x101
_FDot					EQU		0x105
_UDot					EQU		0x048

 ENDIF
 
 
 
 
 ;;IF :DEF: 128P
 
;   -9-
;10|   |8 
;   _2_
;
;11|   |7
;   _6_    .1
;  

; IF :DEF: _TAPEOUT

_0						EQU		0x003
_1					 	EQU		0x733
_2					  	EQU		0x241 	
_3						EQU		0x601
_4				  		EQU		0x521
_5				  		EQU		0x481
_6				  		EQU		0x081
_7				  		EQU		0x423
_8				  		EQU		0x001
_9				  		EQU		0x421


_0Dot					EQU		0x002
_1Dot				 	EQU		0x732
_2Dot				  	EQU		0x240	
_3Dot					EQU		0x600
_4Dot			  		EQU		0x520
_5Dot			  		EQU		0x480
_6Dot			  		EQU		0x080
_7Dot			  		EQU		0x422
_8Dot			  		EQU		0x000
_9Dot			  		EQU		0x420
 
 
 
 
_A						EQU		0x021
_B						EQU		0x181
_C						EQU		0x0C3
_D						EQU		0x301
_E						EQU		0x0C1
_F						EQU		0x0E1
_U						EQU		0x103

_ADot					EQU		0x020
_BDot					EQU		0x180
_CDot					EQU		0x0C2
_DDot					EQU		0x300
_EDot					EQU		0x0C0
_FDot					EQU		0x0E0
_UDot					EQU		0x102
 
 
; ENDIF
 
 
 
 

_XROM			 		EQU		0x58524F4D		;XROM

                    	
                    	END
