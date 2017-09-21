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
# NOTICE: libsylixos, BSP and other kernel modules projects CAN NOT use vfp!
#*********************************************************************************************************
FPUFLAGS = 
CPUFLAGS = -mcpu=cortex-a8 $(FPUFLAGS)

