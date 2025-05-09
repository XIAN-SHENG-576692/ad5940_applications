#include "ad5940_utils_afe.h"

AD5940Err AD5940_get_recommended_afe_ref_cfg(
    const BoolFlag Hp1V8BuffEn, 
    const BoolFlag Lp1V1BuffEn, 
    const BoolFlag Hp1V8ThemBuff, 
    const BoolFlag LPDAC_enable,
    AFERefCfg_Type *const afe_ref_cfg
)
{
    // This option is for the DAC, but other functions are available to set it, so please ignore it.
    afe_ref_cfg->HpBandgapEn = bFALSE;

    // Set to recommanded options.
    afe_ref_cfg->Hp1V8ThemBuff = Hp1V8ThemBuff;
    afe_ref_cfg->Disc1V1Cap = bFALSE;
    afe_ref_cfg->Lp1V1BuffEn = Lp1V1BuffEn;
    afe_ref_cfg->Hp1V1BuffEn = bTRUE;
    afe_ref_cfg->Disc1V8Cap = bFALSE;
    afe_ref_cfg->Lp1V8BuffEn = bTRUE;
    afe_ref_cfg->Hp1V8Ilimit = bTRUE;
    afe_ref_cfg->Hp1V8BuffEn = Hp1V8BuffEn;
    
    // The option is useless
    afe_ref_cfg->LpRefBoostEn = bFALSE;

    /* LP reference control - turn off them to save power*/
    if(LPDAC_enable == bTRUE)    /* With bias voltage */
    {
        afe_ref_cfg->LpBandgapEn = bTRUE;
        afe_ref_cfg->LpRefBufEn = bTRUE;
    }
    else
    {
        afe_ref_cfg->LpBandgapEn = bFALSE;
        afe_ref_cfg->LpRefBufEn = bFALSE;
    }

    return AD5940ERR_OK;
}
