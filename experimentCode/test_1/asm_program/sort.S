#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>

    FILE_BEGIN()
    EXPORT_LABEL(sort)                                                  

FUNC_DEF(sort)                                                          
 
    SUBS  R1, R1, #1                                                   
L0:
    MOV   R2, #0
    MOV   R3, R1                                           

L1:
    LDR   R6, [R0, R2]                                            
    ADD   R4, R2, #4
    LDR   R7, [R0, R4]                                                 
    CMP   R6, R7
    BLT   L2
    STR   R7, [R0, R2]                                                
    STR   R6, [R0, R4]

L2:
    ADD   R2, R2, #4
    SUBS  R3, R3, #1                                                   
    BNE   L1

    SUBS  R1, R1, #1                                               
    BNE   L0
EXIT:
    MOV   PC, LR                                                        

    FUNC_END()
    FILE_END()

