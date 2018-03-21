################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/system/src/hwtimer/fsl_hwtimer.c \
../SDK/platform/system/src/hwtimer/fsl_hwtimer_pit.c \
../SDK/platform/system/src/hwtimer/fsl_hwtimer_pit_irq.c \
../SDK/platform/system/src/hwtimer/fsl_hwtimer_systick.c \
../SDK/platform/system/src/hwtimer/fsl_hwtimer_systick_irq.c 

OBJS += \
./SDK/platform/system/src/hwtimer/fsl_hwtimer.o \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_pit.o \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_pit_irq.o \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_systick.o \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_systick_irq.o 

C_DEPS += \
./SDK/platform/system/src/hwtimer/fsl_hwtimer.d \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_pit.d \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_pit_irq.d \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_systick.d \
./SDK/platform/system/src/hwtimer/fsl_hwtimer_systick_irq.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/system/src/hwtimer/%.o: ../SDK/platform/system/src/hwtimer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


