/**
 * @file    ad5940_electrochemical_utility_tia_adc_loop.h
 * @brief   Configuration functions for TIA and ADC in electrochemical measurements.
 *
 *          This file contains functions for configuring the Low Power TIA (LPTIA) 
 *          and High Speed TIA (HSTIA) measurement loops in conjunction with the ADC.
 *          It provides safeguards and details necessary for proper configuration.
 * 
 * @note    Important: The high-speed DAC (HSDAC) signal chain must NOT be used with the 
 *          low-power TIA (LPTIA). Refer to page 45 of the datasheet for further details.
 *          ```
 *          Note that the high speed DAC signal chain must never be used
 *          in conjunction with the low power TIA. The high speed DAC can
 *          become unstable, leading to incorrect measurements.
 *          ```
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_electrochemical_utils_struct.h"

/**
 * @brief Configures the Low Power DAC (LPDAC) and Low Power TIA (LPTIA) measurement loop.
 * 
 * @param lpdac_cfg  Pointer to the utility-specific reference configuration type
 *                              (`AD5940_ELECTROCHEMICAL_LPDACfg_Type`).
 * @param lptia_cfg  Pointer to the utility-specific reference configuration type
 *                              (`AD5940_ELECTROCHEMICAL_LPTIACfg_Type`).
 * @param dsp_cfg   Pointer to the utility-specific reference configuration type
 *                              (`AD5940_ELECTROCHEMICAL_DSPCfg_Type`).
 * @return        Returns an `AD5940Err` error code indicating the success or failure of the configuration.
 */
AD5940Err AD5940_ELECTROCHEMICAL_config_lpdac_lptia_adc(
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *const lpdac_cfg,
    const AD5940_ELECTROCHEMICAL_LPTIACfg_Type *const lptia_cfg,
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *const dsp_cfg
);

/**
 * @brief Configures the Low Power DAC (LPDAC) and High Speed TIA (HSTIA) measurement loop.
 * 
 * @param lpdac_cfg  Pointer to the utility-specific reference configuration type
 *                              (`AD5940_ELECTROCHEMICAL_LPDACfg_Type`).
 * @param hstia_cfg Pointer to the utility-specific reference configuration type
 *                              (`AD5940_ELECTROCHEMICAL_HSTIACfg_Type`).
 * @param dsp_cfg   Pointer to the utility-specific reference configuration type
 *                              (`AD5940_ELECTROCHEMICAL_DSPCfg_Type`).
 * @return        Returns an `AD5940Err` error code indicating the success or failure of the configuration.
 */
AD5940Err AD5940_ELECTROCHEMICAL_config_lpdac_hstia_adc(
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *const lpdac_cfg,
    const AD5940_ELECTROCHEMICAL_HSTIACfg_Type *const hstia_cfg,
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *const dsp_cfg,
    const AD5940_ELECTROCHEMICAL_ELECTRODE_ROUTING *const electrode_routing
);

// /**
//  * @ref AD5940_ELECTROCHEMICAL_STRUCT_get_MMR_HSLoopCfg_Type
//  * @param V_out_peak_to_peak: TDAC output voltage in mV peak to peak. Maximum value is 800mVpp. Peak to peak voltage. (Refer to page 43 and page 103)
// */
// void AD5940_ELECTROCHEMICAL_STRUCT_get_SIN_HSLoopCfg_Type(
//     HSLoopCfg_Type* type, 
//     const uint32_t ExcitBufGain, 
//     const uint32_t HsDacGain, 
//     const uint32_t HsDacUpdateRate, 
//     const uint32_t HstiaRtiaSel,
//     const uint32_t Dswitch,
//     const uint32_t Pswitch,
//     const uint32_t Nswitch,
//     const uint32_t Tswitch,
//     const BoolFlag LPDAC_enable,
//     const float V_out_peak_to_peak,
//     SoftSweepCfg_Type* softSweepCfg_Type,
//     const float sin_frequency,
//     float* sweep_current_frequency,
//     float* sweep_next_frequency,
//     float* frequency_of_latest_data_sampled,
//     const float system_clock_frequency
// );

// type->WgCfg.WgType = WGTYPE_SIN;    // Select from WGTYPE_MMR, WGTYPE_SIN, WGTYPE_TRAPZ. HSDAC is always connected to WG. (Refer to page 99)
// type->WgCfg.GainCalEn = bTRUE;      // Refer to page 100
// type->WgCfg.OffsetCalEn = bTRUE;    // Refer to page 100

#ifdef __cplusplus
}
#endif
