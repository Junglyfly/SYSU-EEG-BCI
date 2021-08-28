################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)
CG_TOOL_ROOT := C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9

ORDERED_OBJS += \
"./mmb0files.obj" \
"./BlockQueue.obj" \
"./ADSProLibcfg_c.obj" \
"./ADSProLibcfg.obj" \
"./src/utilfiles.obj" \
"./src/usbstyx.obj" \
"./src/usbhw.obj" \
"./src/usbctl.obj" \
"./src/usbconfig.obj" \
"./src/usb.obj" \
"./src/tidc_api.obj" \
"./src/mmb0ui.obj" \
"./src/mmb0task.obj" \
"./src/mmb0spibb.obj" \
"./src/mmb0i2c.obj" \
"./src/mmb0flash.obj" \
"./src/mmb0eeprom.obj" \
"./src/mmb0clk.obj" \
"./src/mmb0bfifo.obj" \
"./src/mmb0.obj" \
"./src/estyx.obj" \
"./src/convS2M.obj" \
"./src/convM2S.obj" \
"./src/convM2D.obj" \
"./src/convD2M.obj" \
"./src/acquire.obj" \
"../myLink.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
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
all: ADCProLib.lib

# Tool invocations
ADCProLib.lib: $(OBJS) $(CMD_SRCS)
	@echo 'Building target: $@'
	@echo 'Invoking: Archiver'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/bin/ar55" r "ADCProLib.lib" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C5500_LIBRARY_OUTPUTS__QTD)$(GEN_CMDS__QTD)$(GEN_SRCS__QTD)$(GEN_MISC_FILES__QTD) "ADCProLib.lib"
	-$(RM) ".\src\acquire.obj" ".\src\convD2M.obj" ".\src\convM2D.obj" ".\src\convM2S.obj" ".\src\convS2M.obj" ".\src\estyx.obj" ".\src\mmb0.obj" ".\src\mmb0bfifo.obj" ".\src\mmb0clk.obj" ".\src\mmb0eeprom.obj" ".\src\mmb0flash.obj" ".\src\mmb0i2c.obj" ".\src\mmb0spibb.obj" ".\src\mmb0task.obj" ".\src\mmb0ui.obj" ".\src\tidc_api.obj" ".\src\usb.obj" ".\src\usbconfig.obj" ".\src\usbctl.obj" ".\src\usbhw.obj" ".\src\usbstyx.obj" ".\src\utilfiles.obj" ".\ADSProLibcfg.obj" ".\ADSProLibcfg_c.obj" ".\BlockQueue.obj" ".\mmb0files.obj" 
	-$(RM) ".\ADSProLibcfg.pp" 
	-$(RM) ".\src\acquire.pp" ".\BlockQueue.pp" 
	-$(RM) ".\src\convD2M.pp" ".\src\convM2D.pp" ".\src\convM2S.pp" ".\src\convS2M.pp" ".\src\estyx.pp" ".\src\mmb0.pp" ".\src\mmb0bfifo.pp" ".\src\mmb0clk.pp" ".\src\mmb0eeprom.pp" ".\src\mmb0flash.pp" ".\src\mmb0i2c.pp" ".\src\mmb0spibb.pp" ".\src\mmb0task.pp" ".\src\mmb0ui.pp" ".\src\tidc_api.pp" ".\src\usb.pp" ".\src\usbconfig.pp" ".\src\usbctl.pp" ".\src\usbhw.pp" ".\src\usbstyx.pp" ".\src\utilfiles.pp" ".\ADSProLibcfg_c.pp" ".\mmb0files.pp" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
