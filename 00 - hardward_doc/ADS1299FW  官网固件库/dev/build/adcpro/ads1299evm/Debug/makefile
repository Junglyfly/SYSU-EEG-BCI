################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)
CG_TOOL_ROOT := C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"./t1299_ob.obj" \
"./main.obj" \
"./ads1299evm_files.obj" \
"./ADSProLibcfg_c.obj" \
"./ADSProLibcfg.obj" \
-l"C:/work/ADS1299FW/dev\src\hw\C55xxCSLmodified\lib\csl5509axmod.lib" \
-l"C:/work/ADS1299FW/dev\build\adcpro\ADCProLib\Debug\ADCProLib.lib" \
"C:/work/CCSv4/DXPNextGen/dev/build/adcpro/ADCProLib/myLink.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: ads1299evm-mmb0.out


# Tool invocations
ads1299evm-mmb0.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/bin/cl55" -v5509A -g --define=CCS_C55XX --define=C55XX --define=__ccs_c55xx --define=USBHW_DMALOG --diag_warning=225 --large_memory_model --ptrdiff_size=32 --memory_model=large --use_long_branch -z -m"ads1299evm.map" --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/include" -i"C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/lib/c5500" -i"C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/bios/lib" --reread_libs --rom_model -o "ads1299evm-mmb0.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_CMDS__QTD)$(GEN_SRCS__QTD)$(GEN_MISC_FILES__QTD)$(C5500_EXECUTABLE_OUTPUTS__QTD) "ads1299evm-mmb0.out"
	-$(RM) ".\ADSProLibcfg.obj" ".\ADSProLibcfg_c.obj" ".\ads1299evm_files.obj" ".\main.obj" ".\t1299_ob.obj" 
	-$(RM) ".\ADSProLibcfg.pp" 
	-$(RM) ".\ADSProLibcfg_c.pp" ".\ads1299evm_files.pp" ".\main.pp" ".\t1299_ob.pp" 
	-@echo ' '

pre-build:
	-@echo 'Increment build number'
	-"C:/work/ADS1299FW/dev/build/adcpro/ads1299evm/increase-build.pl" "C:/work/ADS1299FW/dev/build/adcpro/ads1299evm"
	-@echo ' '

post-build:
	-@echo 'Generate bin file'
	-"C:/work/ADS1299FW/dev/build/adcpro/ads1299evm/makebin.bat" "C:/Program Files/Texas Instruments/ccsv4" "C:/work/ADS1299FW/dev/build/adcpro/ads1299evm" DEBUG
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
