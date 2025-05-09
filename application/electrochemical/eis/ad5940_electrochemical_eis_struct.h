#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_utils_struct.h"
#include "ad5940_electrochemical_utils_struct.h"

/*
 * ================================================================
 * @brief This line below is for the AD5940 Electrochemical EIS scan operation.
 * ================================================================
*/

typedef enum {
    SCAN_POTENTIAL,
    SCAN_TIME,
    SCAN_FIXED
} AD5940_ELECTROCHEMICAL_EIS_Scan_Type;

typedef struct {
    float E_begin;
    float E_end;
    float E_step;
    float E_ac;        // AC amplitude (RMS)
} AD5940_ELECTROCHEMICAL_EIS_Scan_PotentialScanParams;

typedef struct {
    float E_dc;        // DC potential
    float E_ac;        // AC amplitude (RMS)
    float t_run;        // in seconds
    float t_interval;   // in seconds
} AD5940_ELECTROCHEMICAL_EIS_Scan_TimeScanParams;

typedef struct {
    float E_dc;        // DC potential
    float E_ac;        // AC amplitude (RMS)
} AD5940_ELECTROCHEMICAL_EIS_Scan_FixedParams;

/*
 * ================================================================
 * @brief This line below is for the AD5940 Electrochemical EIS frequency operation.
 * ================================================================
*/

typedef enum {
    FREQ_FIXED,
    FREQ_SCAN
} AD5940_ELECTROCHEMICAL_EIS_Frequency_Type;

typedef struct {
    float frequency;
} AD5940_ELECTROCHEMICAL_EIS_Frequency_FixedParams;

typedef struct {
    uint8_t n_frequencies;
    float f_max;
    float f_min;
} AD5940_ELECTROCHEMICAL_EIS_Frequency_ScanParams;

/*
 * ================================================================
 * @brief This line below is for the AD5940 Electrochemical EIS parameters.
 * ================================================================
*/

typedef struct {
    AD5940_ELECTROCHEMICAL_EIS_Scan_Type scan_type;
    union {
        const AD5940_ELECTROCHEMICAL_EIS_Scan_PotentialScanParams *potential;
        const AD5940_ELECTROCHEMICAL_EIS_Scan_TimeScanParams *time;
        const AD5940_ELECTROCHEMICAL_EIS_Scan_FixedParams *fixed;
    } scan_params;

    AD5940_ELECTROCHEMICAL_EIS_Frequency_Type frequency_type;
    union {
        const AD5940_ELECTROCHEMICAL_EIS_Frequency_FixedParams *fixed;
        const AD5940_ELECTROCHEMICAL_EIS_Frequency_ScanParams *scan;
    } frequency_params;
    
} AD5940_ELECTROCHEMICAL_EIS_PARAMETERS;


AD5940Err AD5940_ELECTROCHEMICAL_EIS_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_EIS_PARAMETERS *const parameters
);

#ifdef __cplusplus
}
#endif
