#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * @brief
 * Initializes the AD5940 device and sets up necessary configurations.
 * 
 * This function prepares the AD5940 device for operation by setting up the 
 * required sequence generator buffer and related configurations. Ensure that 
 * the provided buffer and its length are correctly specified before calling 
 * this function.
 * 
 * @note
 * The initialization steps are based on the official AD5940 example code. 
 * For more details on the implementation and rationale behind these steps, 
 * refer to the AD5940 official GitHub repository.
 * 
 * @param sequence_generator_buffer Pointer to the buffer used for sequence generation.
 * @param sequence_generator_buffer_length Length of the sequence generator buffer (in words).
 * @param reset_option Specifies how the AD5940 should be reset:
 *                     - 0: Do not reset
 *                     - 1: Perform a hardware reset
 *                     - 2: Perform a software reset
 * 
 * @attention
 * @ref AD5940_set_sequence_generator_buffer() in the 
 * `utility/ad5940_utility_sequence_generator.h` file for additional information 
 * about sequence buffer setup.
 * 
 * @return AD5940Err Returns an error code indicating the success or failure of 
 * the initialization process.
 */
AD5940Err AD5940_MAIN_init(
    uint32_t *const sequence_generator_buffer,
    const uint16_t sequence_generator_buffer_length,
    const uint8_t reset_option
);

/**
 * @brief
 * Resets the AD5940 device to its default state.
 * 
 * This function performs a full reset of the AD5940 device, ensuring that 
 * all settings are restored to their default states. Use this function to 
 * reinitialize the device if needed.
 * 
 * @return AD5940Err Returns an error code indicating the success or failure of 
 * the reset process.
 */
AD5940Err AD5940_MAIN_reset(void);

#ifdef __cplusplus
}
#endif
