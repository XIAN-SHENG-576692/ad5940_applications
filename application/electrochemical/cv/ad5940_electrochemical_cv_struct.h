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

#ifdef __cplusplus
}
#endif
