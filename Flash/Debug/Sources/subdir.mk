################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/flash_swap.c \
../Sources/fsl_interrupt_manager.c \
../Sources/main.c 

OBJS += \
./Sources/flash_swap.o \
./Sources/fsl_interrupt_manager.o \
./Sources/main.o 

C_DEPS += \
./Sources/flash_swap.d \
./Sources/fsl_interrupt_manager.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -DCLOCK_INIT_CONFIG=CLOCK_RUN -DDEBUG -DFRDM_K22F -DCLOCK_SETUP=0 -DFREEDOM -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/osa" -I/home/alex/KSDK_1.3.0/platform/devices/MK22F51212/include -I/home/alex/KSDK_1.3.0/platform/devices/MK22F51212/startup -I"/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/source/" -I/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/include -I/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/source -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/CMSIS/Include" -I"/home/alex/KSDK_1.3.0/platform/drivers/inc" -I"/home/alex/KSDK_1.3.0/platform/osa/inc" -I"/home/alex/KSDK_1.3.0/platform/hal/inc" -I"/home/alex/KSDK_1.3.0/platform/system/inc" -I"/home/alex/GitHub/CubeSat/Flash/SDK/board" -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/devices" -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/utilities" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -I"/home/alex/KSDK_1.3.0/platform/utilities/inc" -I"/home/alex/KSDK_1.3.0/platform/utilities/src" -I"/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/source" -I"/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/include" -I"/home/alex/KSDK_1.3.0/platform/system/inc" -I"/home/alex/KSDK_1.3.0/platform/system/src/interrupt" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


