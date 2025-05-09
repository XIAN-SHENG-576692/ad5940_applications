#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_electrochemical_dpv_struct.h"

#include "ad5940_electrochemical_utils_loop.h"
#include "ad5940_electrochemical_utils_run.h"

/**
 * @brief Configuration structure for Differential Pulse Voltammetry (DPV).
 * 
 * This structure allows the user to configure the parameters and signal paths 
 * required for performing electrochemical DPV measurements.
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
    const AD5940_ELECTROCHEMICAL_DPV_PARAMETERS *parameters;                        /**< DPV parameter settings */
    const AD5940_ELECTROCHEMICAL_RUN_CONFIG *run;                                   /**< Execution and timing configuration */
    uint8_t path_type;                                                              /**< Choose which type of path to use */
    union {
        const AD5940_ELECTROCHEMICAL_LPDAC_TO_LPTIA_CONFIG *lpdac_to_lptia;         /**< Configuration for LPDAC to LPTIA path */
        const AD5940_ELECTROCHEMICAL_LPDAC_TO_HSTIA_CONFIG *lpdac_to_hstia;         /**< Configuration for LPDAC to HSTIA path */
        const AD5940_ELECTROCHEMICAL_HSDAC_MMR_TO_HSTIA_CONFIG *hsdac_mmr_to_hstia; /**< Configuration for HSDAC via MMR to HSTIA path */
    } path;
} 
AD5940_ELECTROCHEMICAL_DPV_CONFIG;

/**
 * @brief Starts the Differential Pulse Voltammetry (DPV) operation.
 * 
 * @param config Pointer to the DPV configuration structure.
 * 
 * @return AD5940Err Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_DPV_start(
    const AD5940_ELECTROCHEMICAL_DPV_CONFIG *const config
);

/**
 * @brief Stops the Differential Pulse Voltammetry (DPV) operation and shuts down the AD5940.
 * 
 * @param MCU_FIFO_buffer_max_length Maximum length of the MCU FIFO buffer.
 * @param MCU_FIFO_buffer            Pointer to the buffer where remaining FIFO data will be stored.
 * @param MCU_FIFO_count             Pointer to a variable to retrieve the remaining FIFO count.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_DPV_stop(
    const uint16_t MCU_FIFO_buffer_max_length,
    uint32_t *const MCU_FIFO_buffer, 
    uint16_t *const MCU_FIFO_count
);

/**
 * @brief Handles FIFO interrupts during Differential Pulse Voltammetry (DPV) operation.
 * 
 * @param MCU_FIFO_buffer_max_length Maximum length of the MCU FIFO buffer.
 * @param MCU_FIFO_buffer            Pointer to the buffer to store FIFO data.
 * @param MCU_FIFO_count             Pointer to retrieve the current FIFO count.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_DPV_interrupt(
    const uint16_t MCU_FIFO_buffer_max_length,
    uint32_t *const MCU_FIFO_buffer, 
    uint16_t *const MCU_FIFO_count
);

/**
 * @brief Converts ADC data to current values for Differential Pulse Voltammetry (DPV).
 * 
 * This function processes ADC data retrieved from the AD5940 and converts it to current 
 * values based on the provided calibration and configuration parameters. It is used in 
 * Differential Pulse Voltammetry (DPV) experiments.
 * 
 * @param adc_data_step         The ADC data retrieved from the FIFO.
 * @param adc_data_pulse        The ADC data retrieved from the FIFO.
 * @param RtiaCalValue          Pointer to the RTIA calibration value. This parameter is obtained from RTIA calibration functions 
 *                              like @ref AD5940_HSRtiaCal or @ref AD5940_LPRtiaCal.
 * @param ADC_PGA_gain          ADC Programmable Gain Amplifier (PGA) gain value. Refer to @ref ADCPGA_Const.
 * @param ADC_reference_volt    Reference voltage used for the ADC (in volts). Refer to the AD5940 datasheet for details.
 * @param current               Pointer to an array where the calculated current values (in microamperes) will be stored.
 *
 * @return AD5940Err            Returns an error code of type `AD5940Err`. A value of 0 indicates success, while other values 
 *                              indicate specific errors encountered during the conversion process.
 */
AD5940Err AD5940_ELECTROCHEMICAL_DPV_convert_ADC_to_current(
    const uint32_t adc_data_step,
    const uint32_t adc_data_pulse,
    const fImpPol_Type *const RtiaCalValue,
    const uint32_t ADC_PGA_gain,
    const float ADC_reference_volt,
    int32_t *const current
);

#ifdef __cplusplus
}
#endif
