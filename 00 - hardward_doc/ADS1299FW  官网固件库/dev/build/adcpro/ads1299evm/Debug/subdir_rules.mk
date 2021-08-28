################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ADSProLibcfg.cmd: C:/work/CCSv4/DXPNextGen/dev/build/adcpro/ADCProLib/ADSProLib.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf Script Compiler'
	"C:/Program Files/Texas Instruments/xdctools_3_20_08_88/tconf" -b -Dconfig.importPath="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ADSProLibcfg.s??: ADSProLibcfg.cmd
ADSProLibcfg_c.c: ADSProLibcfg.cmd
ADSProLibcfg.h: ADSProLibcfg.cmd
ADSProLibcfg.h??: ADSProLibcfg.cmd
ADSProLib.cdb: ADSProLibcfg.cmd

ADSProLibcfg.obj: .//ADSProLibcfg.s?? $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/bin/cl55" -v5509A -g --define=CCS_C55XX --define=C55XX --define=__ccs_c55xx --define=USBHW_DMALOG --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/include" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ads1299evm/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/Debug" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/include" --include_path="C:/work/ADS1299FW/dev/src/hw/C55xxCSLmodified/include" --include_path="C:/work/ADS1299FW/dev/src/hw/mmb0" --include_path="C:/work/ADS1299FW/dev/3rdparty/estyx" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/src" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x/usbconfig_pid_5718" --diag_warning=225 --large_memory_model --ptrdiff_size=32 --memory_model=large --use_long_branch --preproc_with_compile --preproc_dependency="ADSProLibcfg.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

ADSProLibcfg_c.obj: .//ADSProLibcfg_c.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/bin/cl55" -v5509A -g --define=CCS_C55XX --define=C55XX --define=__ccs_c55xx --define=USBHW_DMALOG --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/include" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ads1299evm/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/Debug" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/include" --include_path="C:/work/ADS1299FW/dev/src/hw/C55xxCSLmodified/include" --include_path="C:/work/ADS1299FW/dev/src/hw/mmb0" --include_path="C:/work/ADS1299FW/dev/3rdparty/estyx" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/src" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x/usbconfig_pid_5718" --diag_warning=225 --large_memory_model --ptrdiff_size=32 --memory_model=large --use_long_branch --preproc_with_compile --preproc_dependency="ADSProLibcfg_c.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

ads1299evm_files.obj: ../ads1299evm_files.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/bin/cl55" -v5509A -g --define=CCS_C55XX --define=C55XX --define=__ccs_c55xx --define=USBHW_DMALOG --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/include" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ads1299evm/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/Debug" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/include" --include_path="C:/work/ADS1299FW/dev/src/hw/C55xxCSLmodified/include" --include_path="C:/work/ADS1299FW/dev/src/hw/mmb0" --include_path="C:/work/ADS1299FW/dev/3rdparty/estyx" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/src" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x/usbconfig_pid_5718" --diag_warning=225 --large_memory_model --ptrdiff_size=32 --memory_model=large --use_long_branch --preproc_with_compile --preproc_dependency="ads1299evm_files.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/bin/cl55" -v5509A -g --define=CCS_C55XX --define=C55XX --define=__ccs_c55xx --define=USBHW_DMALOG --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/include" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ads1299evm/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/Debug" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/include" --include_path="C:/work/ADS1299FW/dev/src/hw/C55xxCSLmodified/include" --include_path="C:/work/ADS1299FW/dev/src/hw/mmb0" --include_path="C:/work/ADS1299FW/dev/3rdparty/estyx" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/src" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x/usbconfig_pid_5718" --diag_warning=225 --large_memory_model --ptrdiff_size=32 --memory_model=large --use_long_branch --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

t1299_ob.obj: ../t1299_ob.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/bin/cl55" -v5509A -g --define=CCS_C55XX --define=C55XX --define=__ccs_c55xx --define=USBHW_DMALOG --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/C5500 Code Generation Tools 4.3.9/include" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ads1299evm/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_41_10_36/packages/ti/rtdx/include/c5500" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/Debug" --include_path="C:/work/ADS1299FW/dev/build/adcpro/ADCProLib/include" --include_path="C:/work/ADS1299FW/dev/src/hw/C55xxCSLmodified/include" --include_path="C:/work/ADS1299FW/dev/src/hw/mmb0" --include_path="C:/work/ADS1299FW/dev/3rdparty/estyx" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/src" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x" --include_path="C:/work/ADS1299FW/dev/3rdparty/porus/port/c55x/usbconfig_pid_5718" --diag_warning=225 --large_memory_model --ptrdiff_size=32 --memory_model=large --use_long_branch --preproc_with_compile --preproc_dependency="t1299_ob.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


