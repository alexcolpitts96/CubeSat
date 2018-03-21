################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/hal/src/flexio/fsl_flexio_camera_hal.c \
../SDK/platform/hal/src/flexio/fsl_flexio_hal.c \
../SDK/platform/hal/src/flexio/fsl_flexio_i2c_hal.c \
../SDK/platform/hal/src/flexio/fsl_flexio_i2s_hal.c \
../SDK/platform/hal/src/flexio/fsl_flexio_spi_hal.c \
../SDK/platform/hal/src/flexio/fsl_flexio_uart_hal.c 

OBJS += \
./SDK/platform/hal/src/flexio/fsl_flexio_camera_hal.o \
./SDK/platform/hal/src/flexio/fsl_flexio_hal.o \
./SDK/platform/hal/src/flexio/fsl_flexio_i2c_hal.o \
./SDK/platform/hal/src/flexio/fsl_flexio_i2s_hal.o \
./SDK/platform/hal/src/flexio/fsl_flexio_spi_hal.o \
./SDK/platform/hal/src/flexio/fsl_flexio_uart_hal.o 

C_DEPS += \
./SDK/platform/hal/src/flexio/fsl_flexio_camera_hal.d \
./SDK/platform/hal/src/flexio/fsl_flexio_hal.d \
./SDK/platform/hal/src/flexio/fsl_flexio_i2c_hal.d \
./SDK/platform/hal/src/flexio/fsl_flexio_i2s_hal.d \
./SDK/platform/hal/src/flexio/fsl_flexio_spi_hal.d \
./SDK/platform/hal/src/flexio/fsl_flexio_uart_hal.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/hal/src/flexio/%.o: ../SDK/platform/hal/src/flexio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


