;******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
;* File Name          : startup_stm32f10x_cl.s
;* Author             : MCD Application Team
;* Version            : V3.5.0
;* Date               : 11-March-2011
;* Description        : STM32F10x Connectivity line devices vector table for MDK-ARM 
;*                      toolchain. 
;*                      This module performs:
;*                      - Set the initial SP 设置初始SP
;*                      - Set the initial PC == Reset_Handler 设置初始PC==Reset_Handler
;*                        设置异常ISR地址的向量表条目
;*                      - Set the vector table entries with the exceptions ISR address
;*                        配置时钟系统
;*                      - Configure the clock system
;*                        在C库中分之到__main（最终调用main())
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                        复位后，CortexM3处理器处于线程模式
;*                      After Reset the CortexM3 processor is in Thread mode,
;*                         优先级为特权，堆栈设置为主
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack  为堆栈分配的内存量（以字节为单位）
; Tailor this value to your application needs      将此值定制到您的应用程序需求
; <h> Stack Configuration                          堆栈配置
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>   堆栈大小（以字节未单位）<0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400
;伪指令AREA，表示开辟一段大小为Stack_Size的内存空间作为栈，段名是STACK，可读可写
;NOINIT：指定此数据段仅仅保留了内存单元，而没有将各初始值写入内存单元，或者将各个内存
;单元值初始化为0
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
;分配0x400个连续字节，并初始化为0
Stack_Mem       SPACE   Stack_Size
;标号__initial_sp，表示栈空间顶地址
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000800   ;0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

;中断向量表定义
;实际上是在CODE区（假设STM32从FLASH启动，则此中断向量表起始地址即为0x8000000)
; Vector Table Mapped to Address 0 at Reset  在复位时向量表映射到地址0
;
                AREA    RESET, DATA, READONLY  
;在程序中声明一个全局的标号__Vectors，该标号可在其他的文件中引用。 
                EXPORT  __Vectors              
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
;__initial_sp所表示的地址值，大小为一个字(32bit)
__Vectors       DCD     __initial_sp               ;  0, Top of Stack
;复位中断服务入口地址
                DCD     Reset_Handler              ;  1, Reset Handler
;不可屏蔽中断
                DCD     NMI_Handler                ;  2, NMI Handler
;硬件错误处理
                DCD     HardFault_Handler          ;  3, Hard Fault Handler
;存储器错误处理
                DCD     MemManage_Handler          ;  4, MPU Fault Handler
;总线错误处理
                DCD     BusFault_Handler           ;  5, Bus Fault Handler
;用法错误处理
                DCD     UsageFault_Handler         ;  6, Usage Fault Handler
                DCD     0                          ;  7, Reserved
                DCD     0                          ;  8, Reserved
                DCD     0                          ;  9, Reserved
                DCD     0                          ; 10, Reserved
;执行系统服务调用指令（SVC）
                DCD     SVC_Handler                ; 11, SVCall Handler
;调试监视器（断点，数据观察点，或者是外部调试请求）
                DCD     DebugMon_Handler           ; 12, Debug Monitor Handler
                DCD     0                          ; 13, Reserved
;为系统设备而设的“可悬挂请求”(pendable request)
                DCD     PendSV_Handler             ; 14, PendSV Handler
;系统滴答定时器
                DCD     SysTick_Handler            ; 15, SysTick Handler

                ; External Interrupts   外设中断
;窗口看门狗
                DCD     WWDG_IRQHandler            ; 16, Window Watchdog
;电源电压检测(PVD)中断
                DCD     PVD_IRQHandler             ; 17, PVD through EXTI Line detect
                DCD     TAMPER_IRQHandler          ; 18, Tamper
                DCD     RTC_IRQHandler             ; 19, RTC
                DCD     FLASH_IRQHandler           ; 20, Flash
                DCD     RCC_IRQHandler             ; 21, RCC
                DCD     EXTI0_IRQHandler           ; 22, EXTI Line 0
                DCD     EXTI1_IRQHandler           ; 23, EXTI Line 1
                DCD     EXTI2_IRQHandler           ; 24, EXTI Line 2
                DCD     EXTI3_IRQHandler           ; 25, EXTI Line 3
                DCD     EXTI4_IRQHandler           ; 26, EXTI Line 4
                DCD     DMA1_Channel1_IRQHandler   ; 27, DMA1 Channel 1
                DCD     DMA1_Channel2_IRQHandler   ; 28, DMA1 Channel 2
                DCD     DMA1_Channel3_IRQHandler   ; 29, DMA1 Channel 3
                DCD     DMA1_Channel4_IRQHandler   ; 30, DMA1 Channel 4
                DCD     DMA1_Channel5_IRQHandler   ; 31, DMA1 Channel 5
                DCD     DMA1_Channel6_IRQHandler   ; 32, DMA1 Channel 6
                DCD     DMA1_Channel7_IRQHandler   ; 33, DMA1 Channel 7
                DCD     ADC1_2_IRQHandler          ; 34, ADC1 and ADC2
                DCD     CAN1_TX_IRQHandler         ; 35, CAN1 TX
                DCD     CAN1_RX0_IRQHandler        ; 36, CAN1 RX0
                DCD     CAN1_RX1_IRQHandler        ; 37, CAN1 RX1
                DCD     CAN1_SCE_IRQHandler        ; 38, CAN1 SCE
                DCD     EXTI9_5_IRQHandler         ; 39, EXTI Line 9..5
                DCD     TIM1_BRK_IRQHandler        ; 40, TIM1 Break
                DCD     TIM1_UP_IRQHandler         ; 41, TIM1 Update
                DCD     TIM1_TRG_COM_IRQHandler    ; 42, TIM1 Trigger and Commutation
                DCD     TIM1_CC_IRQHandler         ; 43, TIM1 Capture Compare
                DCD     TIM2_IRQHandler            ; 44, TIM2
                DCD     TIM3_IRQHandler            ; 45, TIM3
                DCD     TIM4_IRQHandler            ; 46, TIM4
                DCD     I2C1_EV_IRQHandler         ; 47, I2C1 Event
                DCD     I2C1_ER_IRQHandler         ; 48, I2C1 Error
                DCD     I2C2_EV_IRQHandler         ; 49, I2C2 Event
                DCD     I2C2_ER_IRQHandler         ; 50, I2C1 Error
                DCD     SPI1_IRQHandler            ; 51, SPI1
                DCD     SPI2_IRQHandler            ; 52, SPI2
                DCD     USART1_IRQHandler          ; 53, USART1
                DCD     USART2_IRQHandler          ; 54, USART2
                DCD     USART3_IRQHandler          ; 55, USART3
                DCD     EXTI15_10_IRQHandler       ; 56, EXTI Line 15..10
                DCD     RTCAlarm_IRQHandler        ; 57, RTC alarm through EXTI line
                DCD     OTG_FS_WKUP_IRQHandler     ; 58, USB OTG FS Wakeup through EXTI line
                DCD     0                          ; 59, Reserved
                DCD     0                          ; 60, Reserved
                DCD     0                          ; 61, Reserved
                DCD     0                          ; 62, Reserved
                DCD     0                          ; 63, Reserved
                DCD     0                          ; 64, Reserved
                DCD     0                          ; 65, Reserved
                DCD     TIM5_IRQHandler            ; 66, TIM5
                DCD     SPI3_IRQHandler            ; 67, SPI3
                DCD     UART4_IRQHandler           ; 68, UART4
                DCD     UART5_IRQHandler           ; 69, UART5
                DCD     TIM6_IRQHandler            ; 70, TIM6
                DCD     TIM7_IRQHandler            ; 71, TIM7
                DCD     DMA2_Channel1_IRQHandler   ; 72, DMA2 Channel1
                DCD     DMA2_Channel2_IRQHandler   ; 73, DMA2 Channel2
                DCD     DMA2_Channel3_IRQHandler   ; 74, DMA2 Channel3
                DCD     DMA2_Channel4_IRQHandler   ; 75, DMA2 Channel4
                DCD     DMA2_Channel5_IRQHandler   ; 76, DMA2 Channel5
                DCD     ETH_IRQHandler             ; 77, Ethernet
                DCD     ETH_WKUP_IRQHandler        ; 78, Ethernet Wakeup through EXTI line
                DCD     CAN2_TX_IRQHandler         ; 79, CAN2 TX
                DCD     CAN2_RX0_IRQHandler        ; 80, CAN2 RX0
                DCD     CAN2_RX1_IRQHandler        ; 81, CAN2 RX1
                DCD     CAN2_SCE_IRQHandler        ; 82, CAN2 SCE
                DCD     OTG_FS_IRQHandler          ; 83, USB OTG FS
                DCD     0                          ; 84, CRC Length               
                DCD     0                          ; 85, CRC Value.               
                DCD     0                          ; 86, Reserved.               
                DCD     0x86abcdef                 ; 87, Reserved.               
                DCD     0x77616C74                 ; 88, for App, 0x77616C74,'walt'  

                ;; WORK: please check soft version before release, by walnutcy 2013.01.07
                DCD     0x00000102            	   ; 89, 版本号，16进制，如版本是v1.0.2.66,设为0x01000266
                DCD     0x0E620200                 ; 90, 合法固件标示	
                DCD     620200                     ; 91, 设备型号		
                DCD     21                         ; 92, 年份XX(年 + 8，如：13年+8=21)，
                DCD     12                         ; 93, 批号XX,如：12 
                DCD     99999                      ; 94, 生产流水号XXXXX,如：99999
                DCD     0x00000000                 ; 95, Reserved
__Vectors_End
;得到向量表的大小
__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main
                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler            [WEAK]
                EXPORT  PVD_IRQHandler             [WEAK]
                EXPORT  TAMPER_IRQHandler          [WEAK]
                EXPORT  RTC_IRQHandler             [WEAK]
                EXPORT  FLASH_IRQHandler           [WEAK]
                EXPORT  RCC_IRQHandler             [WEAK]
                EXPORT  EXTI0_IRQHandler           [WEAK]
                EXPORT  EXTI1_IRQHandler           [WEAK]
                EXPORT  EXTI2_IRQHandler           [WEAK]
                EXPORT  EXTI3_IRQHandler           [WEAK]
                EXPORT  EXTI4_IRQHandler           [WEAK]
                EXPORT  DMA1_Channel1_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel2_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel3_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel4_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel5_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel6_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel7_IRQHandler   [WEAK]
                EXPORT  ADC1_2_IRQHandler          [WEAK]
                EXPORT  CAN1_TX_IRQHandler         [WEAK]
                EXPORT  CAN1_RX0_IRQHandler        [WEAK]
                EXPORT  CAN1_RX1_IRQHandler        [WEAK]
                EXPORT  CAN1_SCE_IRQHandler        [WEAK]
                EXPORT  EXTI9_5_IRQHandler         [WEAK]
                EXPORT  TIM1_BRK_IRQHandler        [WEAK]
                EXPORT  TIM1_UP_IRQHandler         [WEAK]
                EXPORT  TIM1_TRG_COM_IRQHandler    [WEAK]
                EXPORT  TIM1_CC_IRQHandler         [WEAK]
                EXPORT  TIM2_IRQHandler            [WEAK]
                EXPORT  TIM3_IRQHandler            [WEAK]
                EXPORT  TIM4_IRQHandler            [WEAK]
                EXPORT  I2C1_EV_IRQHandler         [WEAK]
                EXPORT  I2C1_ER_IRQHandler         [WEAK]
                EXPORT  I2C2_EV_IRQHandler         [WEAK]
                EXPORT  I2C2_ER_IRQHandler         [WEAK]
                EXPORT  SPI1_IRQHandler            [WEAK]
                EXPORT  SPI2_IRQHandler            [WEAK]
                EXPORT  USART1_IRQHandler          [WEAK]
                EXPORT  USART2_IRQHandler          [WEAK]
                EXPORT  USART3_IRQHandler          [WEAK]
                EXPORT  EXTI15_10_IRQHandler       [WEAK]
                EXPORT  RTCAlarm_IRQHandler        [WEAK]
                EXPORT  OTG_FS_WKUP_IRQHandler     [WEAK]
                EXPORT  TIM5_IRQHandler            [WEAK]
                EXPORT  SPI3_IRQHandler            [WEAK]
                EXPORT  UART4_IRQHandler           [WEAK]
                EXPORT  UART5_IRQHandler           [WEAK]
                EXPORT  TIM6_IRQHandler            [WEAK]
                EXPORT  TIM7_IRQHandler            [WEAK]
                EXPORT  DMA2_Channel1_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel2_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel3_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel4_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel5_IRQHandler   [WEAK]
                EXPORT  ETH_IRQHandler             [WEAK]
                EXPORT  ETH_WKUP_IRQHandler        [WEAK]
                EXPORT  CAN2_TX_IRQHandler         [WEAK]
                EXPORT  CAN2_RX0_IRQHandler        [WEAK]
                EXPORT  CAN2_RX1_IRQHandler        [WEAK]
                EXPORT  CAN2_SCE_IRQHandler        [WEAK]
                EXPORT  OTG_FS_IRQHandler          [WEAK]

WWDG_IRQHandler
PVD_IRQHandler
TAMPER_IRQHandler
RTC_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_IRQHandler
DMA1_Channel3_IRQHandler
DMA1_Channel4_IRQHandler
DMA1_Channel5_IRQHandler
DMA1_Channel6_IRQHandler
DMA1_Channel7_IRQHandler
ADC1_2_IRQHandler
CAN1_TX_IRQHandler
CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_IRQHandler
TIM1_UP_IRQHandler
TIM1_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTCAlarm_IRQHandler
OTG_FS_WKUP_IRQHandler
TIM5_IRQHandler
SPI3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIM6_IRQHandler
TIM7_IRQHandler
DMA2_Channel1_IRQHandler
DMA2_Channel2_IRQHandler
DMA2_Channel3_IRQHandler
DMA2_Channel4_IRQHandler
DMA2_Channel5_IRQHandler
ETH_IRQHandler
ETH_WKUP_IRQHandler
CAN2_TX_IRQHandler
CAN2_RX0_IRQHandler
CAN2_RX1_IRQHandler
CAN2_SCE_IRQHandler
OTG_FS_IRQHandler

                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE*****
