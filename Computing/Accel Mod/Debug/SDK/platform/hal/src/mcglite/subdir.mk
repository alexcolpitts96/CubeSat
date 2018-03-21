################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/hal/src/mcglite/fsl_mcglite_hal.c \
../SDK/platform/hal/src/mcglite/fsl_mcglite_hal_modes.c 

OBJS += \
./SDK/platform/hal/src/mcglite/fsl_mcglite_hal.o \
./SDK/platform/hal/src/mcglite/fsl_mcglite_hal_modes.o 

C_DEPS += \
./SDK/platform/hal/src/mcglite/fsl_mcglite_hal.d \
./SDK/platform/hal/src/mcglite/fsl_mcglite_hal_modes.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/hal/src/mcglite/%.o: ../SDK/platform/hal/src/mcglite/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


