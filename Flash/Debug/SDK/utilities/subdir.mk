################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/utilities/fsl_debug_console.c \
../SDK/utilities/print_scan.c 

OBJS += \
./SDK/utilities/fsl_debug_console.o \
./SDK/utilities/print_scan.o 

C_DEPS += \
./SDK/utilities/fsl_debug_console.d \
./SDK/utilities/print_scan.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/utilities/%.o: ../SDK/utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc ${cross_toolchain_flags} -D"CPU_MK22FN512VDC12" -I"${KSDK_PATH}/platform/drivers/inc"
"${KSDK_PATH}/platform/osa/inc"
"${KSDK_PATH}/platform/hal/inc"
"${KSDK_PATH}/platform/system/inc"
"${ProjDirPath}/Sources"
"${ProjDirPath}/SDK/board"
"${ProjDirPath}/SDK/composite/inc"
"${ProjDirPath}/SDK/platform/utilities"
"${ProjDirPath}/SDK/platform/devices"
"${ProjDirPath}/SDK/platform/osa"
"${ProjDirPath}/SDK/platform/CMSIS/Include"
"/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/source/" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -I"/home/alex/KSDK_1.3.0/platform/utilities/inc" -I"/home/alex/KSDK_1.3.0/platform/utilities/src" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


