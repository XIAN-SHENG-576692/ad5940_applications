#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_temperature_struct.h"

/**
 * @brief Starts the temperature measurement process on the AD5940.
 *
 * This function initializes the AD5940 for temperature measurement by configuring GPIOs, 
 * ADC settings, FIFO thresholds, and interrupts. It uses the specified configuration 
 * structure to define measurement parameters such as sampling frequency and FIFO settings.
 *
 * @param config A pointer to an `AD5940_TEMPERATURE_START_CONFIG` structure that 
 *               contains all the required configuration parameters.
 * 
 * @return AD5940Err Returns an error code of type `AD5940Err`. A value of 0 indicates 
 *                   success, while any other value represents an error during initialization.
 */
AD5940Err AD5940_TEMPERATURE_start(
    const AD5940_TEMPERATURE_START_CONFIG *const config
);

/**
 * @brief Stops the temperature measurement process and retrieves any remaining FIFO data.
 *
 * This function halts the AD5940's temperature measurement operation, retrieves the 
 * remaining FIFO data, and transfers it to the MCU buffer.
 *
 * @param MCU_FIFO_buffer            Pointer to the MCU buffer to store the remaining AD5940 FIFO data.
 * @param MCU_FIFO_buffer_max_length Maximum allowable length of the MCU buffer.
 * @param AD5940_FIFO_count          Pointer to a variable where the remaining FIFO count will be stored.
 * 
 * @return AD5940Err Returns an error code of type `AD5940Err`. A value of 0 indicates success, 
 *                   while any other value represents an error during the stop operation.
 */
AD5940Err AD5940_TEMPERATURE_stop(
    uint32_t* MCU_FIFO_buffer, 
    const uint16_t MCU_FIFO_buffer_max_length,
    uint16_t *const AD5940_FIFO_count
);

/**
 * @brief Handles FIFO interrupts during temperature measurement on the AD5940.
 *
 * This function processes FIFO interrupts by reading data from the AD5940 FIFO buffer, 
 * transferring it to the MCU buffer, and optionally updating the FIFO threshold. 
 * If the new FIFO threshold is set to 0, the AD5940 will shut down temperature measurements.
 *
 * @param MCU_FIFO_buffer            Pointer to the MCU buffer to store FIFO data from the AD5940.
 * @param MCU_FIFO_buffer_max_length Maximum allowable length of the MCU buffer.
 * @param AD5940_FIFO_new_thresh     New FIFO threshold to set. If 0, the AD5940 will stop measurements.
 * @param AD5940_FIFO_count          Pointer to a variable where the current FIFO count will be stored.
 *                                   This count can be retrieved even if an error is raised.
 * 
 * @return AD5940Err Returns an error code of type `AD5940Err`. A value of 0 indicates success, 
 *                   while any other value represents an error encountered during interrupt handling.
 */
AD5940Err AD5940_TEMPERATURE_interrupt(
    uint32_t* MCU_FIFO_buffer, 
    const uint16_t MCU_FIFO_buffer_max_length,
    const uint16_t AD5940_FIFO_new_thresh,
    uint16_t* AD5940_FIFO_count
);

#ifdef __cplusplus
}
#endif
