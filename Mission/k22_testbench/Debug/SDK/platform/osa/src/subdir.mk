################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/osa/src/fsl_os_abstraction_bm.c \
../SDK/platform/osa/src/fsl_os_abstraction_free_rtos.c \
../SDK/platform/osa/src/fsl_os_abstraction_mqx.c \
../SDK/platform/osa/src/fsl_os_abstraction_ucosii.c \
../SDK/platform/osa/src/fsl_os_abstraction_ucosiii.c 

OBJS += \
./SDK/platform/osa/src/fsl_os_abstraction_bm.o \
./SDK/platform/osa/src/fsl_os_abstraction_free_rtos.o \
./SDK/platform/osa/src/fsl_os_abstraction_mqx.o \
./SDK/platform/osa/src/fsl_os_abstraction_ucosii.o \
./SDK/platform/osa/src/fsl_os_abstraction_ucosiii.o 

C_DEPS += \
./SDK/platform/osa/src/fsl_os_abstraction_bm.d \
./SDK/platform/osa/src/fsl_os_abstraction_free_rtos.d \
./SDK/platform/osa/src/fsl_os_abstraction_mqx.d \
./SDK/platform/osa/src/fsl_os_abstraction_ucosii.d \
./SDK/platform/osa/src/fsl_os_abstraction_ucosiii.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/osa/src/%.o: ../SDK/platform/osa/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


