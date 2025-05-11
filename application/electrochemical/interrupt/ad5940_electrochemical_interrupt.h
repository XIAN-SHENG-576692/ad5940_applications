#include "ad5940.h"

/**
 * @brief Handles FIFO interrupts during electrochemical measurement on the AD5940.
 *
 * This function processes FIFO interrupts by reading data from the AD5940 FIFO buffer, 
 * transferring it to the MCU buffer, and optionally updating the FIFO threshold. 
 * If the new FIFO threshold is set to 0, the AD5940 will shut down electrochemical measurements.
 *
 * @param MCU_FIFO_buffer_max_length    Maximum allowable length of the MCU buffer.
 * @param AD5940_FIFO_new_thresh        New FIFO threshold value to set.
 *                                      - If set to 0, the AD5940 will halt the ongoing measurements.
 *                                      - If set to a negative value, the threshold will remain unchanged.
 * @param MCU_FIFO_buffer               Pointer to the MCU buffer to store FIFO data from the AD5940.
 * @param MCU_FIFO_count                Pointer to a variable where the current FIFO count will be stored.
 *                                      This count can be retrieved even if an error is raised.
 * 
 * @return AD5940Err Returns an error code of type `AD5940Err`. A value of 0 indicates success, 
 *                   while any other value represents an error encountered during interrupt handling.
 */
AD5940Err AD5940_ELECTROCHEMICAL_interrupt(
    const uint16_t MCU_FIFO_buffer_max_length,
    const int32_t AD5940_FIFO_new_thresh,
    uint32_t* MCU_FIFO_buffer, 
    uint16_t* MCU_FIFO_count
);
