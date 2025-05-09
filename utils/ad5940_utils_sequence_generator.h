#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * Configures the sequence generator buffer used by AD5940_SEQxxx functions.
 * 
 * @note
 * The `sequence_generator_buffer` and `sequence_generator_buffer_length` are
 * exclusively utilized by functions in the AD5940_SEQxxx series. For additional
 * details, refer to the `ad5940.c` file.
 * 
 * When `AD5940_SEQGenCtrl(bTRUE)` is called:
 * - All `AD5940_WriteReg()` operations store commands in `sequence_generator_buffer`
 *   instead of writing directly to the AD5940.
 * - All `AD5940_ReadReg()` operations first check if the requested register data
 *   is available in `sequence_generator_buffer`. If not, the data is read directly
 *   from the AD5940.
 * 
 * When `AD5940_SEQGenCtrl(bFALSE)` is called:
 * - Both `AD5940_WriteReg()` and `AD5940_ReadReg()` revert to their normal behavior,
 *   directly interacting with the AD5940.
 * 
 * The buffered commands in `sequence_generator_buffer` are transmitted to the AD5940
 * when `AD5940_SEQCmdWrite()` is executed. After this, `sequence_generator_buffer`
 * can be reused as a standard `uint32_t` array.
 * 
 * @param sequence_generator_buffer Pointer to the buffer used for command storage.
 * @param sequence_generator_buffer_length Length of the buffer in units of 32-bit words.
 * 
 * @return An `AD5940Err` code indicating success or the type of error.
 */
AD5940Err AD5940_set_sequence_generator_buffer(
    uint32_t *const sequence_generator_buffer, 
    const uint16_t sequence_generator_buffer_length
);

/**
 * Clears the sequence generator buffer's internal pointer.
 * 
 * @note
 * The `AD5940_SEQGenCtrl()` function in `ad5940.c` resets the sequence pointer
 * of the `sequence_generator_buffer` but does not reset its register pointer.
 * This function ensures both pointers are reset, preparing the buffer for reuse.
 * 
 * @return An `AD5940Err` code indicating success or the type of error.
 */
AD5940Err AD5940_clear_sequence_generator_buffer(void);

AD5940Err AD5940_get_change_sequence_info_command(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen,
    uint32_t *const sequence_command
);

AD5940Err AD5940_write_change_sequence_info_command(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen
);

#ifdef __cplusplus
}
#endif
