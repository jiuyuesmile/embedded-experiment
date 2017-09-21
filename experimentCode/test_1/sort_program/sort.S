;/*选择法排序 By Q.*/
#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
	FILE_BEGIN()
	EXPORT_LABEL(sort)
	FUNC_DEF(sort)

    MOV R10, R1
	MOV R1, #1             ;/*第一层循环数R1*/
	MOV R2, #-4

 L0:
    ADD R2, R2, #4
	MOV R3, R1              ;/*第二层循环数R3*/
	MOV R4, R2

 L1:
    ADD R4, R4, #4
    LDR R6, [R0, R2]        ;/*第一个操作数*/

    LDR R7, [R0, R4]        ;/*第二个操作数*/

    CMP R6, R7              ;/*比较两个操作数大小*/
    BLT L2

    STR R7, [R0, R2]        ;/*交换两个操作数*/
    STR R6, [R0, R4]

 L2:
    ADD R3, R3, #1
    CMP R10, R3

    BNE L1                  ;/*二层循环控制*/

    ADD R1, R1, #1
    CMP R10, R1
    BNE L0                  ;/*一层循环控制*/

 EXIT:
    MOV PC, LR

	FUNC_END()
	FILE_END()
