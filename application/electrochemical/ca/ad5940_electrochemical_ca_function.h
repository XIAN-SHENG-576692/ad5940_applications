#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_electrochemical_ca_struct.h"

#include "ad5940_electrochemical_utils_loop.h"
#include "ad5940_electrochemical_utils_run.h"

/**
 * @brief Configuration structure for Chronoamperometry (CA).
 * 
 * This structure allows the user to configure the parameters and signal paths 
 * required for performing electrochemical CA measurements.
 * 
 * Specifies the signal path based on the selected path_type:
 * - 0: lpdac_to_lptia
 * - 1: lpdac_to_hstia
 * - 2: hsdac_mmr_to_hstia
 * 
 * The selected path determines the loop used to perform the electrochemical operation.
 */
typedef struct 
{
    const AD5940_ELECTROCHEMICAL_CA_PARAMETERS *parameters;                         /**< CA parameter settings */
    uint16_t fifo_thresh; /**< Sets a new FIFO threshold. */
    const AD5940_ELECTROCHEMICAL_RUN_CONFIG *run;                                   /**< Execution and timing configuration */
    uint8_t path_type;                                                              /**< Choose which type of path to use */
    union {
        const AD5940_ELECTROCHEMICAL_LPDAC_TO_LPTIA_CONFIG *lpdac_to_lptia;         /**< Configuration for LPDAC to LPTIA path */
        const AD5940_ELECTROCHEMICAL_LPDAC_TO_HSTIA_CONFIG *lpdac_to_hstia;         /**< Configuration for LPDAC to HSTIA path */
        const AD5940_ELECTROCHEMICAL_HSDAC_MMR_TO_HSTIA_CONFIG *hsdac_mmr_to_hstia; /**< Configuration for HSDAC via MMR to HSTIA path */
    } path;
} 
AD5940_ELECTROCHEMICAL_CA_CONFIG;

/**
 * @brief Starts the Chronoamperometry (CA) operation.
 * 
 * @param config Pointer to the CA configuration structure.
 * 
 * @return AD5940Err Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_CA_start(
    const AD5940_ELECTROCHEMICAL_CA_CONFIG *const config
);

/**
 * @brief Stops the Chronoamperometry (CA) operation and shuts down the AD5940.
 * 
 * @param MCU_FIFO_buffer_max_length Maximum length of the MCU FIFO buffer.
 * @param MCU_FIFO_buffer            Pointer to the buffer where remaining FIFO data will be stored.
 * @param MCU_FIFO_count             Pointer to a variable to retrieve the remaining FIFO count.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_CA_stop(
    const uint16_t MCU_FIFO_buffer_max_length,
    uint32_t *const MCU_FIFO_buffer, 
    uint16_t *const MCU_FIFO_count
);

/**
 * @brief Handles FIFO interrupts during Chronoamperometry (CA) operation.
 * 
 * @param MCU_FIFO_buffer_max_length Maximum length of the MCU FIFO buffer.
 * @param AD5940_FIFO_new_thresh     Sets a new FIFO threshold. If set to 0, the AD5940 shuts down.
 * @param MCU_FIFO_buffer            Pointer to the buffer to store FIFO data.
 * @param MCU_FIFO_count             Pointer to retrieve the current FIFO count.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_CA_interrupt(
    const uint16_t MCU_FIFO_buffer_max_length,
    const uint16_t AD5940_FIFO_new_thresh,
    uint32_t *const MCU_FIFO_buffer, 
    uint16_t *const MCU_FIFO_count
);

#ifdef __cplusplus
}
#endif
