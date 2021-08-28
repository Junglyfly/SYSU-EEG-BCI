/****************************************************************/
/** \file
 * clk.h / clk.c: Functions to program the CY22393 PPL on the
 * MMB0.
 * \note If the PLL just needs to be programmed and started, a
 * call to the \p clk_set_outfreq() is sufficient. Additionally,
 */
#ifndef GUARD_clk_h
#define GUARD_clk_h

/* defintions to be used for the PLLs to be selected            */
#define CLK_SRC_REF     (0)
#define CLK_SRC_PLL1	(1)
#define CLK_SRC_PLL2	(2)
#define CLK_SRC_PLL3	(3)

/* where to connect the PLLs to                                 */
#define CLK_PIN_A		(0)
#define CLK_PIN_B		(1)
#define CLK_PIN_C		(2)
#define CLK_PIN_D		(3)


float clk_calc_div(float in, float out, u16 *p, u16 *q, u8 *post);
//! Initializes the registers on the PLL to their default values*/
/**
 * - Is called by the \p mmb0_init() function.
 * - Sets registers of the PLL chip to their default values
 * - There is no need to call this function from the application
 *   level.
*/
void clk_init(void);

//! Enables one of the PLLs
/**
 * - Can be called from the application level, but is also called
 *   by the \p clk_set_outfreq() function.
 * - Enables the PLL of the CY22393 passed in as argument by
 *   setting the PLLx_En bit in the respective register.
 *
 * \param pll The number of the PLL to be enabled. Use one of the
 *   CLK_SRC_PLLx macros to ensure correct operation
*/
void clk_enable_pll(int pll);

//! Disables one of the PLLs
/**
 * - Can be called from the application level, but is also called
 *   by the \p clk_set_outfreq() function.
 * - Disables the PLL of the CY22393 passed in as argument by
 *   setting the PLLx_En bit in the respective register.
 *
 * \param pll The number of the PLL to be disabled. Use one of the
 *   CLK_SRC_PLLx macros to ensure correct operation
*/
void clk_disable_pll(int pll);


//! Creates the register settings for the PLL channel to be used
/**
 * - Is local to the file and should not be called from the
 *   application level.
 * - Is called by the \p clk_set_freq() function.
 * - Creates the register settings for the PLL and writes them
 *   to the hardware.
 *
 * \param pll The number of the PLL to be programmed. Use one of
 *   the CLK_SRC_PLLx macros to ensure correct operation
 * \param p The calculated p value for the PLL
 * \param q The calculated q value for the PLL note that q<2 turns
 *   off the respective pll.
*/
void clk_set_pll(int pll, u16 p, u8 q);

//! Set the PLL frequency
/**
 * - Is local to the file and should not be called from the
 *   application level.
 * - Is called by the \p clk_set_outfreq() function.
 * - Calculates the closest frequency the PLL can represent and
 *   returns this frequency to the calling routine
 * - Calls clk_set_pll() to program the PLL with the calculated
 *   values.
 *
 * \param pll The number of the PLL to be programmed. Use one of
 *   the CLK_SRC_PLLx macros to ensure correct operation
 * \param freq The desired frequency in Hz
 * \param post The selctor for the DivSel field in the register
 *   off the respective pll.
 *
 * \retval 0 Fail. The frequency could not be programmed
 * \retval out The frequency, which was actually programmed into
 *   the PLL
*/
float clk_set_freq(int *pll, float freq, u8 *post);


//! Connect outputs to the PPL
/**
 * - Can be called from the application level.
 * - Is called by the \p clk_set_outfreq() function.
 * - Connects CLKA, CLKB, CLKC or CLKD to one of the PLLS
 *
 * \param output The pin which should be connected. Use one of
 *   the CLK_PIN_CLKx macros to ensure correct operation
 * \param pllsrc The PLL the pin should be connected to. use one
 *   of the CLK_SRC_PLLx macros to ensure correct operation.
 * \param div If == 0, the output will be turned of.If 0 < div <=127
 *   it programs the CLKx_Div bitfield in the respective register
*/
void clk_set_out(int output, int pllsrc, u8 div);

//! Sets the PLL output frequency
/**
 * - This is the function to call from the application level to
 *   set or change the output frequency of the PLL.
 * - Disables the PLL and the output pin and then calculates the
 *   register values for the PLL and finally turns on the PLL and
 *   the clock pin.
 *
 * \param output The pin which should be connected. Use one of
 *   the CLK_PIN_CLKx macros to ensure correct operation.
 * \param pll The PLL the pin should be connected to. use one
 *   of the CLK_SRC_PLLx macros to ensure correct operation.
 * \param freq The desired frequency in Hz.
 *
 * \retval 0 Fail. The frequency could not be programmed
 * \retval out The frequency, which was actually programmed into
 *   the PLL
*/
float clk_set_outfreq(int output, int pll, float freq);

#endif
