;/*ѡ������ By Q.*/
#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
	FILE_BEGIN()
	EXPORT_LABEL(sort)
	FUNC_DEF(sort)

    MOV R10, R1
	MOV R1, #1             ;/*��һ��ѭ����R1*/
	MOV R2, #-4

 L0:
    ADD R2, R2, #4
	MOV R3, R1              ;/*�ڶ���ѭ����R3*/
	MOV R4, R2

 L1:
    ADD R4, R4, #4
    LDR R6, [R0, R2]        ;/*��һ��������*/

    LDR R7, [R0, R4]        ;/*�ڶ���������*/

    CMP R6, R7              ;/*�Ƚ�������������С*/
    BLT L2

    STR R7, [R0, R2]        ;/*��������������*/
    STR R6, [R0, R4]

 L2:
    ADD R3, R3, #1
    CMP R10, R3

    BNE L1                  ;/*����ѭ������*/

    ADD R1, R1, #1
    CMP R10, R1
    BNE L0                  ;/*һ��ѭ������*/

 EXIT:
    MOV PC, LR

	FUNC_END()
	FILE_END()