#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_utils_struct.h"
#include "ad5940_electrochemical_utils_struct.h"

/**
 * @brief Parameters for the AD5940 Electrochemical Cyclic Voltammetry (CV) operation.
 */
typedef struct 
{
    float e_begin;             /**< Starting potential of the scan, in volts (V). */
    float e_vertex1;           /**< First vertex potential of the scan, in volts (V). */
    float e_vertex2;           /**< Second vertex potential of the scan, in volts (V). */
    float e_step;              /**< Step potential between measurements, in volts (V). */
    float scan_rate;           /**< Rate of potential change during the scan, in volts per second (V/s). */
}
AD5940_ELECTROCHEMICAL_CV_PARAMETERS;

AD5940Err AD5940_ELECTROCHEMICAL_CV_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_CV_PARAMETERS *const parameters
);

AD5940Err AD5940_ELECTROCHEMICAL_CV_get_t_interval(
    const AD5940_ELECTROCHEMICAL_CV_PARAMETERS *const parameters,
    float *const t_interval
);

/**
 * @brief Calculates the number of remaining FIFO data points required to complete the 
 *        Cyclic Voltammetry (CV) operation.
 * 
 * @param parameters    DPV parameter settings.
 * @param FIFO_count    Pointer to a variable where the calculated remaining FIFO count 
 *                      will be stored.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_CV_get_fifo_count(
    const AD5940_ELECTROCHEMICAL_CV_PARAMETERS *parameters,
    uint16_t *const FIFO_count
);

#ifdef __cplusplus
}
#endif
