#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * Clears all configuration settings related to GPIO and interrupt (INT) flags.
 * 
 * @note
 * Refer to pages 112 and 122 of the datasheet for detailed information. 
 * Also, check the `ad5940.c` file for implementation details.
 */
void AD5940_clear_GPIO_and_INT_flag(void);

/**
 * Retrieves the interrupt (INT) enable status based on the specified AGPIO configuration.
 * 
 * @note
 * Refer to pages 112 and 123 of the datasheet for more details. 
 * This function maps the relationship between GPIO and interrupt controllers (INT).
 * 
 * @param agpio_cfg         Pointer to the AGPIO configuration structure (`AGPIOCfg_Type`) to query.
 * @param AFEINTC_0_enable  Pointer to a `BoolFlag` to store the enable status of `AFEINTC_0`.
 * @param AFEINTC_1_enable  Pointer to a `BoolFlag` to store the enable status of `AFEINTC_1`.
 */
void AD5940_get_AfeIntcSel_by_AGPIOCfg_Type(
    const AGPIOCfg_Type *const agpio_cfg,
    BoolFlag *const AFEINTC_0_enable,
    BoolFlag *const AFEINTC_1_enable
);

/**
 * Configures the interrupt controller (INTC) settings based on the specified AGPIO configuration.
 * 
 * @note
 * This function is implemented based on @ref AD5940_get_AfeIntcSel_by_AGPIOCfg_Type,
 * which is defined in this file.
 * 
 * @param agpio_cfg Pointer to the AGPIO configuration structure (`AGPIOCfg_Type`) to apply.
 * @param AFEIntSrc Interrupt source to configure. Refer to @ref AFEINTC_SRC_Const 
 *                  for available interrupt source flags.
 */
void AD5940_set_INTCCfg_by_AGPIOCfg_Type(
    const AGPIOCfg_Type *const agpio_cfg,
    const int32_t AFEIntSrc
);

#ifdef __cplusplus
}
#endif
