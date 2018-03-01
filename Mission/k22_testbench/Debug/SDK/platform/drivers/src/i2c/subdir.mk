################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/drivers/src/i2c/fsl_i2c_common.c \
../SDK/platform/drivers/src/i2c/fsl_i2c_irq.c \
../SDK/platform/drivers/src/i2c/fsl_i2c_lpm_callback.c \
../SDK/platform/drivers/src/i2c/fsl_i2c_master_driver.c \
../SDK/platform/drivers/src/i2c/fsl_i2c_shared_function.c \
../SDK/platform/drivers/src/i2c/fsl_i2c_slave_driver.c 

OBJS += \
./SDK/platform/drivers/src/i2c/fsl_i2c_common.o \
./SDK/platform/drivers/src/i2c/fsl_i2c_irq.o \
./SDK/platform/drivers/src/i2c/fsl_i2c_lpm_callback.o \
./SDK/platform/drivers/src/i2c/fsl_i2c_master_driver.o \
./SDK/platform/drivers/src/i2c/fsl_i2c_shared_function.o \
./SDK/platform/drivers/src/i2c/fsl_i2c_slave_driver.o 

C_DEPS += \
./SDK/platform/drivers/src/i2c/fsl_i2c_common.d \
./SDK/platform/drivers/src/i2c/fsl_i2c_irq.d \
./SDK/platform/drivers/src/i2c/fsl_i2c_lpm_callback.d \
./SDK/platform/drivers/src/i2c/fsl_i2c_master_driver.d \
./SDK/platform/drivers/src/i2c/fsl_i2c_shared_function.d \
./SDK/platform/drivers/src/i2c/fsl_i2c_slave_driver.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/i2c/%.o: ../SDK/platform/drivers/src/i2c/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


