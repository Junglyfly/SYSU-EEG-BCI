
SECTIONS 
{
    /* cinit section must be contiguous and thus cannot be placed internally */
	csltext     : {csl5509axmod.lib(.text)} align(32) > DARAM
	csldata     : {csl5509axmod.lib(.csldata)} align(32) > DARAM
	csldat      : {csl5509axmod.lib(.csldat)} align(32) > DARAM

	rts55xlibbss     : {rts55x.lib(.bss)} align(32) > DARAM
	rts55xlibtext    : {rts55x.lib(.text)} align(32) > DARAM
	rts55xlibconst   : {rts55x.lib(.const)} align(32) > DARAM

	signaltext       : {signal.obj(.text)} align(32)  > DARAM
	signalbss        : {signal.obj(.bss)} align(32)   > DARAM
	signalconst      : {signal.obj(.const)} align(32) > DARAM

	dxp_acquiretext  : {dxp_acquire.obj(.text)} align(32)  > DARAM
	dxp_acquirebss   : {dxp_acquire.obj(.bss)} align(32)   > DARAM

	dxptext          : {dxp.obj(.text)} align(32)  > DARAM
	dxpbss           : {dxp.obj(.bss)} align(32)   > DARAM
	dxpconst         : {dxp.obj(.const)} align(32) > DARAM

    dmascratch  : > SARAM

	.sine00 > SINE
	.sine01 > SINE
	.sine02 > SINE
	.sine03 > SINE
	.sine04 > SINE
	.sine05 > SINE
	.sine06 > SINE
	.sine07 > SINE
	.sine08 > SINE
	.sine09 > SINE
	.sine0A > SINE
	.sine0B > SINE
	.sine0C > SINE
	.sine0D > SINE
	.sine0E > SINE
	.sine0F > SINE
	.sine10 > SINE
    
	.sdram_upper > SDRAM_UPPER 
}
