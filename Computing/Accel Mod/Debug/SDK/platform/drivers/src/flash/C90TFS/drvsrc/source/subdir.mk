################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/CopyToRam.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DEFlashPartition.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashGetProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashSetProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EEEWrite.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMGetProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMSetProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCheckSum.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCommandSequence.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseAllBlock.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseAllBlockUnsecure.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseBlock.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseResume.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSector.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSuspend.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashGetSecurityState.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashInit.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgram.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramCheck.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramOnce.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramSection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadOnce.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadResource.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashSecurityBypass.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyAllBlock.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyBlock.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifySection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashFacGetSAProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashFacGetXAProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashGetProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSetProtection.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwap.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwapCtl.c \
../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/SetEEEEnable.c 

OBJS += \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/CopyToRam.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DEFlashPartition.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashGetProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashSetProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EEEWrite.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMGetProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMSetProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCheckSum.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCommandSequence.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseAllBlock.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseAllBlockUnsecure.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseBlock.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseResume.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSector.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSuspend.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashGetSecurityState.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashInit.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgram.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramCheck.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramOnce.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramSection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadOnce.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadResource.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashSecurityBypass.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyAllBlock.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyBlock.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifySection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashFacGetSAProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashFacGetXAProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashGetProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSetProtection.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwap.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwapCtl.o \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/SetEEEEnable.o 

C_DEPS += \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/CopyToRam.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DEFlashPartition.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashGetProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashSetProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EEEWrite.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMGetProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMSetProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCheckSum.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCommandSequence.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseAllBlock.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseAllBlockUnsecure.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseBlock.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseResume.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSector.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSuspend.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashGetSecurityState.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashInit.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgram.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramCheck.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramOnce.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramSection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadOnce.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadResource.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashSecurityBypass.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyAllBlock.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyBlock.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifySection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashFacGetSAProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashFacGetXAProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashGetProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSetProtection.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwap.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwapCtl.d \
./SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/SetEEEEnable.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/%.o: ../SDK/platform/drivers/src/flash/C90TFS/drvsrc/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK22FN512VDC12" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK22F51212/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


