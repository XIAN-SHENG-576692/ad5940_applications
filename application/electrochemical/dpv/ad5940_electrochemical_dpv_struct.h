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

#ifdef __cplusplus
}
#endif
