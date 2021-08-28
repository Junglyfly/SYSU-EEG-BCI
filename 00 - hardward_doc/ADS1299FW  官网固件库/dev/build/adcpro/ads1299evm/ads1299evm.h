#ifndef GUARD_ads1299evm_h
#define GUARD_ads1299evm_h

#include "estyx.h"

/* added for fix of EVM  GH 08JUL10 */
#define    AdsCSPin(VAL)                GPIO_pinWrite(GPIO_PIN6, VAL) //CSz pin

/****************************************************************/
/**
 * The following are the get and set callback functions for the
 * register accesses.
 *
 * Notes:
 * - Writes to a register do not verify that the contents was
 *   written correctly. Verification has to be done by calling
 *   the respective "get" function from the application level.
 * - Writes to a register do not mask out or mask in reserved bits,
 *   which need to be written with a specific value. This is the
 *   task of the program writing to the file.
 * - Reads from a register do not mask out N/A or reserved bit
 *   positions. This has to be done on the application level
 * - For the read-only registers, not set function is provided.
*/
/****************************************************************/
u32 ads1299evm_devid_get(void);
u32 ads1299evm_config1_get(void);
void ads1299evm_config1_set(u32 i);
u32 ads1299evm_config2_get(void);
void ads1299evm_config2_set(u32 i);
u32 ads1299evm_config3_get(void);
void ads1299evm_config3_set(u32 i);
u32 ads1299evm_loff_get(void);
void ads1299evm_loff_set(u32 i);
u32 ads1299evm_ch1set_get(void);
void ads1299evm_ch1set_set(u32 i);
u32 ads1299evm_ch2set_get(void);
void ads1299evm_ch2set_set(u32 i);
u32 ads1299evm_ch3set_get(void);
void ads1299evm_ch3set_set(u32 i);
u32 ads1299evm_ch4set_get(void);
void ads1299evm_ch4set_set(u32 i);
u32 ads1299evm_ch5set_get(void);
void ads1299evm_ch5set_set(u32 i);
u32 ads1299evm_ch6set_get(void);
void ads1299evm_ch6set_set(u32 i);
u32 ads1299evm_ch7set_get(void);
void ads1299evm_ch7set_set(u32 i);
u32 ads1299evm_ch8set_get(void);
void ads1299evm_ch8set_set(u32 i);
u32 ads1299evm_biasensp_get(void);
void ads1299evm_biassensp_set(u32 i);
u32 ads1299evm_rldsensn_get(void);
void ads1299evm_rldsensn_set(u32 i);
u32 ads1299evm_loffsensp_get(void);
void ads1299evm_loffsensp_set(u32 i);
u32 ads1299evm_loffsensn_get(void);
void ads1299evm_loffsensn_set(u32 i);
u32 ads1299evm_loffflip_get(void);
void ads1299evm_loffflip_set(u32 i);
u32 ads1299evm_loffstatp_get(void);
u32 ads1299evm_loffstatn_get(void);
u32 ads1299evm_gpio_get(void);
void ads1299evm_gpio_set(u32 i);
u32 ads1299evm_misc1_get(void);
void ads1299evm_misc1_set(u32 i);
u32 ads1299evm_config4_get(void);
void ads1299evm_config4_set(u32 i);
u32 ads1299evm_misc2_get(void);
void ads1299evm_misc2_set(u32 i);


//! Sets or removes the sleep mode from the device
/**
 * - Called once the sleep file is written to.
 *
 * \param[in] i Value to be written to the file. Valid are:
 *     '0' to remove the sleep mode from the device
 *     '1' to place the device into sleep mode
 *
 * \retval None
*/
void ads1299evm_sleep_set(int i);

//! Get the sleep mode of the device
/**
 * - Called once the sleep file is read. Returns the value of the
 *   global variable \var sleep, which mirrors the status the
 *   device is in.
 *
 * \retval 0 if the device is working
 * \retval 1 if the device is in sleep mode
*/
int ads1299evm_sleep_get(void);

//! Get the reset status of the device
/**
 * - Called once the reset file is read
 *
 * \retval 0 if the device is active
 * \retval 1 if the device is in reset
*/
u32 ads1299evm_reset_get(void);

//! Resets the device
/**
 * - Called once the reset file is written to.
 *
 * \param[in] i Value to be written to the file. Valid are:
 *     '0' will have no influence
 *     '1' to reset the device. During the execution of the reset,
 *         the file reads 1. Once the reset is completed, the file
 *         reads zero again.
 *
 * \retval None
*/
void ads1299evm_reset_set(u32 i);



int ads1299evm_mkdir(estyx_server_t *srv, estyx_file_t *parent);

#endif
