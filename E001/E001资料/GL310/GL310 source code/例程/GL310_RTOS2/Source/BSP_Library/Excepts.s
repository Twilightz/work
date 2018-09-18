    INCLUDE         CPSR_DEF.INC
    
    EXPORT          Interrupt_Handler
    EXPORT			  Enable_Interrupt
    EXPORT			  Disable_Interrupt
    EXPORT			  Enable_FastInterrupt
    EXPORT			  Disable_FastInterrupt
    IMPORT          IntrDispatcher
    
    PRESERVE8
    AREA Except_Handler, CODE, READONLY
    
Interrupt_Handler
    SUB             LR,       LR, #4
    STMFD           sp!,      {LR}        ; save r14_irq/lr
    MRS             r14,      spsr
    STMFD           sp!,      {r14}       ; save spsr
    MRS             r14,      cpsr
    STMFD           sp!,      {r14}       ; save cpsr
    BIC             r14,      #0x01F
    ORR             r14,      #Sys32Mode  
    MSR             cpsr_c,   r14         ; switch to system mode
    STMFD           sp!,      {r0-r12}
    BL              IntrDispatcher 
    LDMFD           sp!,      {r0-r12}
    MRS             r14,      cpsr
    BIC             r14,      #0x01F
    ORR             r14,      #IRQ32Mode
    MSR             cpsr_c,   r14 
    LDMFD           sp!,      {r14}
    MSR             cpsr_c,   r14
    LDMFD           sp!,      {r14}
    MSR             spsr_c,   r14
    LDMFD           SP!,      {PC}^
    
Enable_Interrupt
    MRS             r0, CPSR
    BIC             r0, r0, #I_BIT
    MSR             CPSR_c, r0
    BX              LR
    
Disable_Interrupt
    MRS             r0, CPSR
    ORR             r0, r0, #I_BIT
    MSR             CPSR_c, r0
    BX              LR    
    
Enable_FastInterrupt
    MRS             r0, CPSR
    BIC             r0, r0, #F_BIT
    MSR             CPSR_c, r0
    BX              LR
    
Disable_FastInterrupt
    MRS             r0, CPSR
    ORR             r0, r0, #F_BIT
    MSR             CPSR_c, r0
    BX              LR    
    

    END