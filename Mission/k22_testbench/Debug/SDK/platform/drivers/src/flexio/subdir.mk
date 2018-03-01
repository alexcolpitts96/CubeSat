################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/drivers/src/flexio/fsl_flexio_camera_edma_driver.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_common.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_driver.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_i2c_master_driver.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_i2s_driver.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_irq.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_lpm_callback.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_spi_driver.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_uart_dma_driver.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_uart_driver.c \
../SDK/platform/drivers/src/flexio/fsl_flexio_uart_edma_driver.c 

OBJS += \
./SDK/platform/drivers/src/flexio/fsl_flexio_camera_edma_driver.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_common.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_driver.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_i2c_master_driver.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_i2s_driver.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_irq.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_lpm_callback.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_spi_driver.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_uart_dma_driver.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_uart_driver.o \
./SDK/platform/drivers/src/flexio/fsl_flexio_uart_edma_driver.o 

C_DEPS += \
./SDK/platform/drivers/src/flexio/fsl_flexio_camera_edma_driver.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_common.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_driver.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_i2c_master_driver.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_i2s_driver.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_irq.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_lpm_callback.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_spi_driver.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_uart_dma_driver.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_uart_driver.d \
./SDK/platform/drivers/src/flexio/fsl_flexio_uart_edma_driver.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/flexio/%.o: ../SDK/platform/drivers/src/flexio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


