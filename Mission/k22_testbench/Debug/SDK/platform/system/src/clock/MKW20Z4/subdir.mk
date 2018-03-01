################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/system/src/clock/MKW20Z4/fsl_clock_MKW20Z4.c 

OBJS += \
./SDK/platform/system/src/clock/MKW20Z4/fsl_clock_MKW20Z4.o 

C_DEPS += \
./SDK/platform/system/src/clock/MKW20Z4/fsl_clock_MKW20Z4.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/system/src/clock/MKW20Z4/%.o: ../SDK/platform/system/src/clock/MKW20Z4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I../SDK/platform/system/src -I../SDK/platform/system/inc -I../SDK/platform/osa/inc -I../SDK/platform/osa/src -I../SDK/platform/hal/inc/ -I../SDK/platform/hal/src/i2c/ -I../SDK/platform/drivers/inc/ -I../SDK/platform/drivers/src/i2c -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


