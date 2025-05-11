#include "ad5940_utils_gpio.h"

void AD5940_clear_GPIO_and_INT_flag(void)
{
    AD5940_INTCCfg(AFEINTC_1, AFEINTSRC_ALLINT, bFALSE);
    AD5940_INTCCfg(AFEINTC_0, AFEINTSRC_ALLINT, bFALSE);
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

    AGPIOCfg_Type gpio_cfg = {};
    AD5940_AGPIOCfg(&gpio_cfg);

    return;
}

void AD5940_get_AfeIntcSel_by_AGPIOCfg_Type(
    const AGPIOCfg_Type *const agpio_cfg,
    BoolFlag *const AFEINTC_0_enable,
    BoolFlag *const AFEINTC_1_enable
)
{
    if(
        ((agpio_cfg->FuncSet & BITM_AGPIO_GP0CON_PIN0CFG) == GP0_INT)
        || ((agpio_cfg->FuncSet & BITM_AGPIO_GP0CON_PIN3CFG) == GP3_INT0)
        || ((agpio_cfg->FuncSet & BITM_AGPIO_GP0CON_PIN6CFG) == GP6_INT0)
    ) *AFEINTC_0_enable = bTRUE;
    else *AFEINTC_0_enable = bFALSE;
    if(
        ((agpio_cfg->FuncSet & BITM_AGPIO_GP0CON_PIN4CFG) == GP4_INT1)
        || ((agpio_cfg->FuncSet & BITM_AGPIO_GP0CON_PIN7CFG) == GP7_INT)
    ) *AFEINTC_1_enable = bTRUE;
    else *AFEINTC_1_enable = bFALSE;
    return;
}

void AD5940_set_INTCCfg_by_AGPIOCfg_Type(
    const AGPIOCfg_Type *const agpio_cfg,
    const int32_t AFEIntSrc
)
{
    BoolFlag AFEINTC_0_enable;
    BoolFlag AFEINTC_1_enable;
    AD5940_get_AfeIntcSel_by_AGPIOCfg_Type(agpio_cfg, &AFEINTC_0_enable, &AFEINTC_1_enable);
    if(AFEINTC_0_enable) AD5940_INTCCfg(AFEINTC_0, AFEIntSrc, bTRUE);
    if(AFEINTC_1_enable) AD5940_INTCCfg(AFEINTC_1, AFEIntSrc, bTRUE);
    return;
}
