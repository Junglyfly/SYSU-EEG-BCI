
SECTIONS 
{
    /* cinit section must be contiguous and thus cannot be placed internally */
	csltext     : {csl5509axmod.lib(.text)} align(32) > SARAM
	csldata     : {csl5509axmod.lib(.csldata)} align(32) > SARAM
	csldat      : {csl5509axmod.lib(.csldat)} align(32) > SARAM

    .isr  : > DARAM
    dmascratch  : > SARAM
}
