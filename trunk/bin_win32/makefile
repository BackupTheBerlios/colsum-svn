################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := del

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: colsum.exe

dependents:
	-cd C:/Projekte/eclipse/c_workspace/cflib/libmgw && $(MAKE) clean all

# Tool invocations
colsum.exe: $(OBJS) $(USER_OBJS) C:/Projekte/eclipse/c_workspace/cflib/libmgw/libcf.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -static -L"C:\Projekte\eclipse\c_workspace\cflib\libmgw" -o "colsum.exe" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) colsum.exe
	-@echo ' '

post-build:
	-@echo 'Copying executable to bin release directory'
	-$(STRIP) colsum.exe && $(CP) colsum.exe "../bin/win32/" && ./colsum.exe -h 2> ../doc/include/examples/usage_output.txt
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
C:/Projekte/eclipse/c_workspace/cflib/libmgw/libcf.a:

-include ../makefile.targets
