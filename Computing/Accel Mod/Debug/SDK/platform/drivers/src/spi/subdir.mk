################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/drivers/src/spi/fsl_spi_common.c \
../SDK/platform/drivers/src/spi/fsl_spi_dma_irq.c \
../SDK/platform/drivers/src/spi/fsl_spi_dma_master_driver.c \
../SDK/platform/drivers/src/spi/fsl_spi_dma_shared_function.c \
../SDK/platform/drivers/src/spi/fsl_spi_dma_slave_driver.c \
../SDK/platform/drivers/src/spi/fsl_spi_irq.c \
../SDK/platform/drivers/src/spi/fsl_spi_lpm_callback.c \
../SDK/platform/drivers/src/spi/fsl_spi_master_driver.c \
../SDK/platform/drivers/src/spi/fsl_spi_shared_function.c \
../SDK/platform/drivers/src/spi/fsl_spi_slave_driver.c 

OBJS += \
./SDK/platform/drivers/src/spi/fsl_spi_common.o \
./SDK/platform/drivers/src/spi/fsl_spi_dma_irq.o \
./SDK/platform/drivers/src/spi/fsl_spi_dma_master_driver.o \
./SDK/platform/drivers/src/spi/fsl_spi_dma_shared_function.o \
./SDK/platform/drivers/src/spi/fsl_spi_dma_slave_driver.o \
./SDK/platform/drivers/src/spi/fsl_spi_irq.o \
./SDK/platform/drivers/src/spi/fsl_spi_lpm_callback.o \
./SDK/platform/drivers/src/spi/fsl_spi_master_driver.o \
./SDK/platform/drivers/src/spi/fsl_spi_shared_function.o \
./SDK/platform/drivers/src/spi/fsl_spi_slave_driver.o 

C_DEPS += \
./SDK/platform/drivers/src/spi/fsl_spi_common.d \
./SDK/platform/drivers/src/spi/fsl_spi_dma_irq.d \
./SDK/platform/drivers/src/spi/fsl_spi_dma_master_driver.d \
./SDK/platform/drivers/src/spi/fsl_spi_dma_shared_function.d \
./SDK/platform/drivers/src/spi/fsl_spi_dma_slave_driver.d \
./SDK/platform/drivers/src/spi/fsl_spi_irq.d \
./SDK/platform/drivers/src/spi/fsl_spi_lpm_callback.d \
./SDK/platform/drivers/src/spi/fsl_spi_master_driver.d \
./SDK/platform/drivers/src/spi/fsl_spi_shared_function.d \
./SDK/platform/drivers/src/spi/fsl_spi_slave_driver.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/spi/%.o: ../SDK/platform/drivers/src/spi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


