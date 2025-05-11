#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_electrochemical_cv_struct.h"

#include "ad5940_electrochemical_utils_loop.h"
#include "ad5940_electrochemical_utils_run.h"

/**
 * @brief Configuration structure for Cyclic Voltammetry (CV).
 * 
 * This structure allows the user to configure the parameters and signal paths 
 * required for performing electrochemical CV measurements.
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
    const AD5940_ELECTROCHEMICAL_CV_PARAMETERS *parameters;                         /**< CV parameter settings */
    const AD5940_ELECTROCHEMICAL_RUN_CONFIG *run;                                   /**< Execution and timing configuration */
    uint8_t path_type;                                                              /**< Choose which type of path to use */
    union {
        const AD5940_ELECTROCHEMICAL_LPDAC_TO_LPTIA_CONFIG *lpdac_to_lptia;         /**< Configuration for LPDAC to LPTIA path */
        const AD5940_ELECTROCHEMICAL_LPDAC_TO_HSTIA_CONFIG *lpdac_to_hstia;         /**< Configuration for LPDAC to HSTIA path */
        const AD5940_ELECTROCHEMICAL_HSDAC_MMR_TO_HSTIA_CONFIG *hsdac_mmr_to_hstia; /**< Configuration for HSDAC via MMR to HSTIA path */
    } path;
} 
AD5940_ELECTROCHEMICAL_CV_CONFIG;

/**
 * @brief Starts the Cyclic Voltammetry (CV) operation.
 * 
 * @param config Pointer to the CV configuration structure.
 * 
 * @return AD5940Err Error code indicating success (0) or failure.
 */
AD5940Err AD5940_ELECTROCHEMICAL_CV_start(
    const AD5940_ELECTROCHEMICAL_CV_CONFIG *const config
);

#ifdef __cplusplus
}
#endif
