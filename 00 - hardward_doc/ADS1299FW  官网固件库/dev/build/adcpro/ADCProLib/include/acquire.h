#ifndef GUARD_ads1298_acquire_h
#define GUARD_ads1298_acquire_h

#ifdef __cplusplus
extern "C" {
#endif

#include "estyx.h"


//! Data-DMA callback function
/**
 * - Called at the end of each data block acquisition from the
 *   DMA interrupt service routine of the block transfer. As the
 *   code in this function is executed from inside the ISR, make
 *   it as short as possible, otherwise it may block other
 *   interrupts (HWI and SWI) or task switches.
 * - finalizes the write to the data queue, making the
 *   data visible inside the "data" file. This is done by a
 *   call to \p adcq.finishWrite().
 *
 * \param[in] p Pointer to whatever is needed inside this
 * function. As this function will be called from an interrupt
 * service routine, it is of no real use.
 *
 * \retval None
*/
void acquire_adc_done(void* p);

//! Returns the size of current transfer block
/**
 * Is called once the \p blocksize_samples file is read and
 * returns the number of samples requested per transfer.
 *
 * \param: None
 *
 * \retval: None
*/
u32 acquire_blocksize_get(void);

//! Sets the number of samples per transfer
/**
 * Is called once the \p blocksize_samples file is written and
 * places the value written to the file in the file-scope variable
 * \p acquire_blocksize. This value will be used for the next
 * as blocksize the next time \p acquire_next_block() is called.
 *
 * \param[in] n Number of samples to transfer
 *
 * \retval None
 *
 * \note The default as set by the \ acquire_init() function is
 * 256 samples to match the transfer size of the USB port. If any
 * other length is desired, it must be set before the \p
 * acquire file is set to one.
*/
void adcquire_blocksize_set(u32 n);

//! Closes the data file
/**
 * Is called once the \p data file is closed.
 * \note Not sure if this function is ever used.
 *
 * \param f File ID of the \p data file
 *
 * \retval None
*/
void acquire_data_close(estyx_fid_t* f);

//! Returns the length of the data file in bytes
/**
 * Is called once the host asks for the size of the \p data file.
 * Reads the length of the data acquisition buffer, converts it
 * to bytes and returns it to the caller
 *
 * \param f File ID of the \p data file
 *
 * \retval adcq.len Size of the \p data file in bytes as unsigned
 * long
 *
*/
u32 acquire_data_len(estyx_file_t* f);

//! Read method for the data file
/**
 * Reads the requested number of bytes from the \p data file and
 * places them in the file handle. Is called each time the \p
 * data file is read.
 *
 * <b>Operation</b>
 * - Gets the address of the next valid sample in the buffer
 *   queue
 * - Returns the requested number of bytes inside the file handle.
 * - Removes the requested number of bytes from the buffer queue
 *   (i.e. decreases the size of the \p data file by the requested
 *   number of bytes and sets the pointer for the \p
 *   adcq.startRead() function to the next valid sample).
 *
 * \param f File ID of the \p data file
 * \param len Number of bytes to be read
 * \param pos Position inside the \p data file. Unused parameter
 *
 * \retval 0 Success
*/
int acquire_data_read(estyx_fid_t* f, u16 len, u32 pos);

//! Write method for the data file
/**
 * - Is called as the result of a write access to the \p data file.
 * - Clears the buffer queue by calling \p adcq.clear(0 and sets
 *   the length of the buffer queue to zero if no current data
 *   transfer from the data converter is ongoing.
 * - The value written to the file is of no importance. It is the
 *   write access that matters.
 *
 * \param f File ID of the \p data file
 * \param *len Pointer to the buffer queue length.
 * \param *data Pointer to the data object. Unused parameter!
 * \param pos Position to where to write the data. Unused
 * parameter.
 *
 * \retval 0 Success
 *
 * \note There are several unused parameters. These are in here
 * to ensure compatibility with an upcomind DAC tool.
*/
int acquire_data_write(estyx_fid_t* f, u16 *len, u8 *data, u32 pos);

//! Read the acquisition status
/**
 * - Is called once the \p acquire file is read.
 * - Returns the value of the file-scope variable \p acquiring,
 *   indicating whether the data transfers from the data converter
 *   have been stopped. Returning a zero does not necessarily mean
 *   that there is no data transfer ongoing (i.e. stopping the
 *   data transfers do not necesserily abort an ongoing transfer).
 *   It only means that there is no new block transfer submitted.
 *
 * \retval 0 Data transfers have been stopped.
 * \retval 1 Data transfers are ongoing
*/
int acquire_get(void);

//! Initialize the file system for the data acquisition
/**
 * - Is called by the \p 1298_evm_mkdir() function.
 * - Sets the file-scope variables to their default values
 * - Creates the file system needed for the data acquisition
 *   Three files are created under the ads1298evm
 *   directory:
 *       - \p data: The data file itself.
 *       - \p acquire: Boolean file for starting and stopping data
 *         acquisitions.
 *       - \p blocksize_samples: File to hold the number of samples
 *          per block transfer.
 * - Creates a software interrupt for the submission of a new
 *   block in the DMA callback function.
 *
 * \param srv The file server
 * \param parent The parent directory where the files shall be
 * placed. This will tbe the ads1298evm directory.
 *
 * \retval 0 Success
 * \retval -1 One or more files could not be initialized.
 *
 * \warning <b>These three files are essential and the LabView
 * plugin depends on them for its operation. Please do not alter
 * those files in any way!</b>
*/
int acquire_init(estyx_server_t* srv, estyx_file_t* parent);

//! Submit the next block for transfer
/**
 * - Is called by either the \p acquire_set() function or by the
 *   \p acquire_swi software interrupt service routine.
 * - Reserves \p blocksize_samples number of samples in the buffer
 *   queue.
 * - Submits a new buffer for transfer to the low level DMA
 *   driver.
 *
 * \note This is one of the functions most likely to be modified!
*/
void acquire_next_block(void);

//! Starting acquisitions
/**
 * - Is called once the \p acquire file is written to.
 * - No action if \p acquiring has the same value as the parameter.
 * - Sets the file-scope variable \p acquiring to either zero or
 *   one.
 * - If it set to 1, the buffer queue is cleared first and two
 *   block transfers are initiatedby calling \p acquire_next_block().
 *
 * \param i Either 0 or 1. Zero means stop transfers and one start
 * transfers.
*/
void acquire_set(int i);

#ifdef __cplusplus
}
#endif

#endif

