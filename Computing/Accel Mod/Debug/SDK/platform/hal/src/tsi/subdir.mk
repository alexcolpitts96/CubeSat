################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/hal/src/tsi/fsl_tsi_hal.c \
../SDK/platform/hal/src/tsi/fsl_tsi_v2_hal_specific.c \
../SDK/platform/hal/src/tsi/fsl_tsi_v4_hal_specific.c 

OBJS += \
./SDK/platform/hal/src/tsi/fsl_tsi_hal.o \
./SDK/platform/hal/src/tsi/fsl_tsi_v2_hal_specific.o \
./SDK/platform/hal/src/tsi/fsl_tsi_v4_hal_specific.o 

C_DEPS += \
./SDK/platform/hal/src/tsi/fsl_tsi_hal.d \
./SDK/platform/hal/src/tsi/fsl_tsi_v2_hal_specific.d \
./SDK/platform/hal/src/tsi/fsl_tsi_v4_hal_specific.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/hal/src/tsi/%.o: ../SDK/platform/hal/src/tsi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


