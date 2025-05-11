#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_utils_struct.h"
#include "ad5940_electrochemical_utils_struct.h"

/**
 * @brief Options for signal inversion in Differential Pulse Voltammetry (DPV) measurements.
 * 
 * This enumeration defines the possible inversion options that can be applied during DPV measurements.
 */
typedef enum {
    AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_NONE,      /**< No inversion is applied. */
    AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_BOTH,      /**< Invert both anodic and cathodic signals. */
    AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_CATHODIC,  /**< Invert only the cathodic signal. */
    AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_ANODIC,    /**< Invert only the anodic signal. */
} AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION;

/**
 * @brief Macro to validate if a given inversion option is valid.
 * 
 * This macro checks if the provided option is one of the defined inversion options.
 * 
 * @param option The inversion option to validate.
 * 
 * @return Non-zero if the option is valid, zero otherwise.
 */
#define AD5940_ELECTROCHEMICAL_DPV_is_INVERSION_OPTION(option) (\
    (option == AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_NONE) || \
    (option == AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_BOTH) || \
    (option == AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_CATHODIC) || \
    (option == AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION_INVERT_ANODIC) \
)

/**
 * @brief Parameters for the AD5940 Electrochemical Differential Pulse Voltammetry (DPV) operation.
 */
typedef struct 
{
    float e_begin;    /**< Starting potential of the scan, in millivolts (mV). */
    float e_end;      /**< Ending potential of the scan, in millivolts (mV). */
    float e_step;    /**< Step potential between measurements, in millivolts (mV). */
    float e_pulse;   /**< Pulse amplitude, in millivolts (mV). */
    float t_pulse;   /**< Pulse duration, in milliseconds (ms). */
    float scan_rate; /**< Rate of potential change during the scan, in millivolts per second (mV/s). */
    AD5940_ELECTROCHEMICAL_DPV_INVERSION_OPTION inversion_option; /**< Option to invert the signal during measurement. */
}
AD5940_ELECTROCHEMICAL_DPV_PARAMETERS;

AD5940Err AD5940_ELECTROCHEMICAL_DPV_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_DPV_PARAMETERS *const parameters
);

AD5940Err AD5940_ELECTROCHEMICAL_DPV_get_t_interval(
    const AD5940_ELECTROCHEMICAL_DPV_PARAMETERS *const parameters,
    float *const t_interval
);

/**
 * @brief Calculates the number of remaining FIFO data points required to complete the 
 *        Differential Pulse Voltammetry (DPV) operation.
 * 
 * @param parameters    DPV parameter settings.
 * @param FIFO_count    Pointer to a variable where the calculated remaining FIFO count 
 *                      will be stored.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_DPV_get_fifo_count(
    const AD5940_ELECTROCHEMICAL_DPV_PARAMETERS *parameters,
    uint16_t *const FIFO_count
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
