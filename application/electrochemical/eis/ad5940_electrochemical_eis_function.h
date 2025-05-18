#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_electrochemical_eis_struct.h"

#include "ad5940_electrochemical_utils_loop.h"
#include "ad5940_electrochemical_utils_run.h"

/**
 * @brief Configuration structure for Electrochemical impedance spectroscopy (EIS).
 * 
 * This structure allows the user to configure the parameters for performing 
 * Electrochemical impedance spectroscopy (EIS) measurements.
 * 
 * The selected path determines the loop used to perform the electrochemical operation.
 */
typedef struct
{
    const AD5940_ELECTROCHEMICAL_EIS_PARAMETERS *parameters;                        /**< CA parameter settings */
    const AD5940_ELECTROCHEMICAL_RUN_CONFIG *run;                                   /**< Execution and timing configuration */
    const AD5940_ELECTROCHEMICAL_HSDAC_TO_HSTIA_CONFIG *hsdac_to_hstia;     /**< Configuration for HSDAC via MMR to HSTIA path */
}
AD5940_ELECTROCHEMICAL_EIS_CONFIG;

/**
 * @brief Starts the Electrochemical impedance spectroscopy (EIS) operation.
 * 
 * @param config Pointer to the EIS configuration structure.
 * 
 * @return AD5940Err Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_EIS_start(
    const AD5940_ELECTROCHEMICAL_EIS_CONFIG *const config
);

#ifdef __cplusplus
}
#endif
