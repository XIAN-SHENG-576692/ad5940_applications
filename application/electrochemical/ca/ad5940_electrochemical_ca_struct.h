#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_utils_struct.h"
#include "ad5940_electrochemical_utils_struct.h"

/**
 * @brief Parameters for the AD5940 Electrochemical Chronoamperometry (CA) operation.
 */
typedef struct 
{
    float e_dc;            /**< DC potential applied during the experiment in volts (V). */
    float t_interval;      /**< Time interval between measurements in seconds (s). */
} 
AD5940_ELECTROCHEMICAL_CA_PARAMETERS;

AD5940Err AD5940_ELECTROCHEMICAL_CA_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_CA_PARAMETERS *const parameters
);

#ifdef __cplusplus
}
#endif
