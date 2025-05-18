#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_utils_struct.h"
#include "ad5940_electrochemical_utils_struct.h"

typedef enum {
    AD5940_ELECTROCHEMICAL_EIS_FREQ_FIXED,
    AD5940_ELECTROCHEMICAL_EIS_FREQ_LOG,
    AD5940_ELECTROCHEMICAL_EIS_FREQ_LINEAR,
    AD5940_ELECTROCHEMICAL_EIS_FREQ_CUSTOM,
} AD5940_ELECTROCHEMICAL_EIS_FREQ;

typedef struct {

    struct {
        float e_begin;
        float e_end;
        float e_step;
        float e_ac;         // AC amplitude (RMS)
        float t_interval;   // in seconds
        float t_run;        // in seconds
    } scan_params;

    AD5940_ELECTROCHEMICAL_EIS_FREQ freq_type;
    union {
        struct {
            float f;
        } fixed;
        struct {
            uint32_t num;
            float f_max;
            float f_min;
        } linear, log;
        struct {
            uint32_t num;
            float *f_list;
        } custom;
    } freq_params;

} AD5940_ELECTROCHEMICAL_EIS_PARAMETERS;


AD5940Err AD5940_ELECTROCHEMICAL_EIS_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_EIS_PARAMETERS *const parameters
);

/**
 * @brief Calculates the number of remaining FIFO data points required to complete the 
 *        Electrochemical impedance spectroscopy (EIS) operation.
 * 
 * @param parameters    DPV parameter settings.
 * @param FIFO_count    Pointer to a variable where the calculated remaining FIFO count 
 *                      will be stored.
 * 
 * @return AD5940Err                 Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_EIS_get_fifo_count(
    const AD5940_ELECTROCHEMICAL_EIS_PARAMETERS *parameters,
    uint16_t *const FIFO_count
);

#ifdef __cplusplus
}
#endif
