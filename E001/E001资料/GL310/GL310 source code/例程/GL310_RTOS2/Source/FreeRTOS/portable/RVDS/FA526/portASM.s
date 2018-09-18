;/*
;    FreeRTOS is free software; you can redistribute it and/or modify it under
;    the terms of the GNU General Public License (version 2) as published by the
;    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.
;
;    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
;    >>!   distribute a combined work that includes FreeRTOS without being   !<<
;    >>!   obliged to provide the source code for proprietary components     !<<
;    >>!   outside of the FreeRTOS kernel.                                   !<<
;
;    Full license text is available from the following
;    link: http://www.freertos.org/a00114.html
;
;    1 tab == 4 spaces!
;*/

    IMPORT  ulCriticalNesting
    IMPORT  pxCurrentTCB
    IMPORT  vTaskSwitchContext
    IMPORT  IntrDispatcher
    IMPORT  AddressOfIrqISR
    IMPORT  BaseOfFreeMemory

    EXPORT  vPortStartFirstTask
    EXPORT  vPortYield
    EXPORT  pvGetFirstFreeMemory


    ARM
    AREA    PORT_ASM, CODE, READONLY


pvGetFirstFreeMemory
    LDR     R0, =BaseOfFreeMemory
    LDR     R0, [R0]
    ADD     R0, R0, #4
    MOV     PC, LR


vPortStartFirstTask
    LDR     R1, =AddressOfIrqISR
    LDR     R0, =__irq_handler
    STR     R0, [R1]                ; Register IRQ ISR
    LDR     PC, =vRestoreContext    ; Restore context of the first task


vPortYield

    PRESERVE8

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Save the context of the current task
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    STMFD   SP!, {R0-R2, LR}        ; Push R0-R2 and the return address onto the task stack.

    MRS     R1, CPSR                ; Keep CPSR in R1
    ORR     R0, R1, #0x80           ; Enter the critical section
    MSR     CPSR_c, R0

    LDR     R2, =ulCriticalNesting  ; Keep ulCriticalNesting in R0
    LDR     R0, [R2]

    STMFD   SP, {R0-R1, R3-R14}     ; Push the critical nesting depth, the CPSR
    SUB     SP, SP, #56             ;   and R3-R14 of the task onto the task stack.

    LDR     R0, =pxCurrentTCB       ; Store the new top of stack for the task.
    LDR     R0, [R0]
    STR     SP, [R0]


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Perform the context switch.
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    LDR     R0, =vTaskSwitchContext ; Get the address of the context switch function
    LDR     LR, =vRestoreContext    ; Store the return address
    MOV     PC, R0                  ; Call the contedxt switch function


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Vectored IRQ handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

__irq_handler

    PRESERVE8

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Save the context of the current task
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    LDMFD   SP!, {R0-R1}            ; Restore R0-R1 of the task
    SUB     R3, LR, #4              ; Keep return address in R3

    MOV     LR, #0x93               ; Enter SVC mode
    MSR     CPSR_c, LR

    STMFD   SP!, {R0-R3}            ; Push R0-R2 and the return address onto the task stack.
                                    ;   (R2 can be pushed directly as well since it is unchanged)

    MOV     R0, SP                  ; Keep stack pointer of the task in R0
    MOV     R1, LR                  ; Keep LR of the task in R1

    MOV     R3, #0x92               ; Go back to IRQ mode
    MSR     CPSR_c, R3

    LDMFD   SP!, {R2-R3}            ; Restore R3 of the task

    MOV     LR, R1                  ; Keep LR of the task in LR of IRQ mode
    MRS     R2, SPSR                ; Keep SPSR in R2
    LDR     R1, =ulCriticalNesting  ; Keep ulCriticalNesting in R1
    LDR     R1, [R1]

    STMFD   R0!, {R1-R14}           ; Push the critical nesting depth, the SPSR
                                    ;   and R3-R14 of the task onto the task stack.

    ADD     R1, R0, #56             ; Restore the original SP of the task in R1
    STR     R1, [R0, #+48]          ; Correct R13 of the task in the task stack

    LDR     R1, =pxCurrentTCB       ; Store the new top of stack for the task.
    LDR     R1, [R1]
    STR     R0, [R1]


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Dispatch the interrupts to the corresponding ISRs
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    BL      IntrDispatcher


    MOV     R0, #0x93               ; Enter SVC mode
    MSR     CPSR_c, R0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Starting the first task is done by just restoring the context
; setup by pxPortInitialiseStack
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

vRestoreContext

    PRESERVE8

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Restore the context of the selected task
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    LDR     R0, =pxCurrentTCB       ; Set the SP to the task stack.  The location was...
    LDR     R0, [R0]                ; ... stored in pxCurrentTCB
    LDR     SP, [R0]

    LDMFD   SP, {R0-R1, R3-R14}     ; Restore the critical nesting depth, the CPSR
                                    ;   and R3-R14 of the task from the task stack.

    LDR     R2, =ulCriticalNesting
    STR     R0, [R2]
    MSR     SPSR_cxsf, R1

    LDMFD   SP!, {R0-R2, PC}^       ; Restore R0-R2 and PC for returning to the task.


    END
