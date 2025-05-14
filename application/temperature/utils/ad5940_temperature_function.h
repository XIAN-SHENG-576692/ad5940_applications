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

#ifdef __cplusplus
}
#endif
