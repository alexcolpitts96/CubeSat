################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/drivers/src/sdramc/fsl_sdramc_common.c \
../SDK/platform/drivers/src/sdramc/fsl_sdramc_driver.c 

OBJS += \
./SDK/platform/drivers/src/sdramc/fsl_sdramc_common.o \
./SDK/platform/drivers/src/sdramc/fsl_sdramc_driver.o 

C_DEPS += \
./SDK/platform/drivers/src/sdramc/fsl_sdramc_common.d \
./SDK/platform/drivers/src/sdramc/fsl_sdramc_driver.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/sdramc/%.o: ../SDK/platform/drivers/src/sdramc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


