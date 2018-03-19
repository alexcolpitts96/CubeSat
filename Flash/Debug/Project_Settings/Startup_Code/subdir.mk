################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Project_Settings/Startup_Code/startup.c \
../Project_Settings/Startup_Code/system_MK22F51212.c 

S_UPPER_SRCS += \
../Project_Settings/Startup_Code/startup_MK22F51212.S 

OBJS += \
./Project_Settings/Startup_Code/startup.o \
./Project_Settings/Startup_Code/startup_MK22F51212.o \
./Project_Settings/Startup_Code/system_MK22F51212.o 

C_DEPS += \
./Project_Settings/Startup_Code/startup.d \
./Project_Settings/Startup_Code/system_MK22F51212.d 

S_UPPER_DEPS += \
./Project_Settings/Startup_Code/startup_MK22F51212.d 


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/%.o: ../Project_Settings/Startup_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -DCLOCK_INIT_CONFIG=CLOCK_RUN -DDEBUG -DFRDM_K22F -DCLOCK_SETUP=0 -DFREEDOM -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/osa" -I/home/alex/KSDK_1.3.0/platform/devices/MK22F51212/include -I/home/alex/KSDK_1.3.0/platform/devices/MK22F51212/startup -I"/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/source/" -I/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/include -I/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/source -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/CMSIS/Include" -I"/home/alex/KSDK_1.3.0/platform/drivers/inc" -I"/home/alex/KSDK_1.3.0/platform/osa/inc" -I"/home/alex/KSDK_1.3.0/platform/hal/inc" -I"/home/alex/KSDK_1.3.0/platform/system/inc" -I"/home/alex/GitHub/CubeSat/Flash/SDK/board" -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/devices" -I"/home/alex/GitHub/CubeSat/Flash/SDK/platform/utilities" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -I"/home/alex/KSDK_1.3.0/platform/utilities/inc" -I"/home/alex/KSDK_1.3.0/platform/utilities/src" -I"/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/source" -I"/home/alex/KSDK_1.3.0/platform/drivers/src/flash/C90TFS/drvsrc/include" -I"/home/alex/KSDK_1.3.0/platform/system/inc" -I"/home/alex/KSDK_1.3.0/platform/system/src/interrupt" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/%.o: ../Project_Settings/Startup_Code/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -x assembler-with-cpp -DFREEDOM -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


