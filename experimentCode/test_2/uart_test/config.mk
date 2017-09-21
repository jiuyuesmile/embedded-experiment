#*********************************************************************************************************
#
#                                    �й�������Դ��֯
#
#                                   Ƕ��ʽʵʱ����ϵͳ
#
#                                SylixOS(TM)  LW : long wing
#
#                               Copyright All Rights Reserved
#
#--------------�ļ���Ϣ--------------------------------------------------------------------------------
#
# ��   ��   ��: config.mk
#
# ��   ��   ��: RealEvo-IDE
#
# �ļ���������: 2016 ��11 ��23 ��
#
# ��        ��: ���ļ���RealEvo-IDE���ɣ���������Makefile���ܣ������ֶ��޸�
#*********************************************************************************************************
#*********************************************************************************************************
# SylixOS Base Project path
#*********************************************************************************************************
SYLIXOS_BASE_PATH = E:/Practice/Arm/QLMX/SylixOS

#*********************************************************************************************************
# Toolchain prefix
#*********************************************************************************************************
TOOLCHAIN_PREFIX = arm-sylixos-eabi-

#*********************************************************************************************************
# Debug options (debug or release)
#*********************************************************************************************************
DEBUG_LEVEL = debug

#*********************************************************************************************************
# configure area you can set the following config to you own system
# FPUFLAGS (-mfloat-abi=softfp -mfpu=vfpv3 ...)
# CPUFLAGS (-mcpu=arm920t ...)
# NOTICE: libsylixos, BSP and other kernel modules projects CAN NOT use vfp!
#*********************************************************************************************************
FPUFLAGS = 
CPUFLAGS = -mcpu=cortex-a8 $(FPUFLAGS)
