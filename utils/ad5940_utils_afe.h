#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * @brief This configuration must be set when using the ADC
 * @param LPDAC_enable: The excitation signal is DC+AC.
 *                      This parameter decides the DC value in mV unit.
 *                      0.0mV means no DC bias. (Refer to page 30 and page 45)
 * @note
 * Refer to page 87 of the datasheet.
 * - These reference voltages are set to recommended options.
 * 
 * Refer to page 34 of the datasheet.
 * - Enable LP reference control only when using LPDAC.
 * - Disable LP reference control when using HSDAC to save power.
*/
AD5940Err AD5940_get_recommended_afe_ref_cfg(
    const BoolFlag Hp1V8BuffEn, 
    const BoolFlag Lp1V1BuffEn, 
    const BoolFlag Hp1V8ThemBuff, 
    const BoolFlag LPDAC_enable,
    AFERefCfg_Type *const afe_ref_cfg
);

#ifdef __cplusplus
}
#endif
