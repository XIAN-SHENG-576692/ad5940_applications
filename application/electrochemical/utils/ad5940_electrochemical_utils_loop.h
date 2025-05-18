 #pragma once

 #ifdef __cplusplus
 extern "C"
 {
 #endif

 #include "ad5940_electrochemical_utils.h"

/**
 * @brief Configuration LPDAC to LPTIA.
 */
typedef struct 
{
    const AD5940_ELECTROCHEMICAL_AFERefCfg_Type *afe_ref_cfg;           /**< Pointer to AFE reference configuration. */
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *lpdac_cfg;               /**< Pointer to LPPA configuration. */
    const AD5940_ELECTROCHEMICAL_LPTIACfg_Type *lptia_cfg;              /**< Pointer to LPTIA configuration. */
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *dsp_cfg;                  /**< Pointer to DSP configuration. */
} 
AD5940_ELECTROCHEMICAL_LPDAC_TO_LPTIA_CONFIG;

/**
 * @brief Configuration LPDAC to HSTIA.
 */
typedef struct 
{
    const AD5940_ELECTROCHEMICAL_ELECTRODE_ROUTING *electrode_routing;
    const AD5940_ELECTROCHEMICAL_AFERefCfg_Type *afe_ref_cfg;           /**< Pointer to AFE reference configuration. */
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *lpdac_cfg;               /**< Pointer to LPPA configuration. */
    const AD5940_ELECTROCHEMICAL_HSTIACfg_Type *hstia_cfg;              /**< Pointer to HSTIA configuration. */
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *dsp_cfg;                  /**< Pointer to DSP configuration. */
} 
AD5940_ELECTROCHEMICAL_LPDAC_TO_HSTIA_CONFIG;

/**
 * @brief Configuration HSDAC_MMR to HSTIA.
 */
typedef struct 
{
    const AD5940_ELECTROCHEMICAL_ELECTRODE_ROUTING *electrode_routing;
    const AD5940_ELECTROCHEMICAL_AFERefCfg_Type *afe_ref_cfg;           /**< Pointer to AFE reference configuration. */
    const AD5940_ELECTROCHEMICAL_HSDACCfg_Type *hsdac_cfg;              /**< Pointer to LPPA configuration. */
    const AD5940_ELECTROCHEMICAL_HSTIACfg_Type *hstia_cfg;              /**< Pointer to HSTIA configuration. */
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *dsp_cfg;                  /**< Pointer to DSP configuration. */
}
AD5940_ELECTROCHEMICAL_HSDAC_TO_HSTIA_CONFIG;
 
 #ifdef __cplusplus
 }
 #endif
 