################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/colmod.c \
../src/colsum.c 

OBJS += \
./src/colmod.o \
./src/colsum.o 

C_DEPS += \
./src/colmod.d \
./src/colsum.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"C:\Projekte\eclipse\c_workspace\colsum\include" -I"C:\Projekte\eclipse\c_workspace\cflib\include" -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


