#include "ad5940.h"

/**
 * @brief Handles interrupts during measurement on the AD5940.
 *
 * This function processes interrupts by reading data from the AD5940 FIFO buffer, 
 * transferring it to the MCU buffer, and optionally updating the FIFO threshold. 
 * If the new FIFO threshold is set to 0, the AD5940 will shut down measurement.
 *
 * @param new_fifo_thresh       New FIFO threshold value to set.
 *                              - If set to 0, the AD5940 will halt the ongoing measurements.
 *                              - If set to a negative value, the threshold will remain unchanged.
 * @param buffer_max_length     Maximum allowable length of the MCU buffer.
 * @param buffer                Pointer to the MCU buffer to store FIFO data from the AD5940.
 * @param buffer_length         Pointer to a variable where the current FIFO count will be stored.
 *                              This count can be retrieved even if an error is raised.
 * 
 * @return AD5940Err Returns an error code of type `AD5940Err`. A value of 0 indicates success, 
 *                   while any other value represents an error encountered during interrupt handling.
 */
AD5940Err AD5940_irq_handler(
    const int32_t new_fifo_thresh,
    const uint16_t buffer_max_length,
    uint32_t* buffer, 
    uint16_t* buffer_length
);
