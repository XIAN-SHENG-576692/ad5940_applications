#include "ad5940_electrochemical_utils_afe_dac.h"

#include "ad5940_electrochemical_utils.h"

static void _config(
    AFERefCfg_Type *const aferef_cfg,
    LPLoopCfg_Type *const lp_loop_cfg,
    HSLoopCfg_Type *const hs_loop_cfg,
    DSPCfg_Type *const dsp_cfg,
    const uint32_t AfeCtrlSet
)
{
    AD5940_AFECtrlS(AFECTRL_ALL, bFALSE);  /* Init all to disable state */

    AD5940_REFCfgS(aferef_cfg);
    AD5940_LPLoopCfgS(lp_loop_cfg);
    AD5940_HSLoopCfgS(hs_loop_cfg);
    AD5940_DSPCfgS(dsp_cfg);

    /* Enable all of them. They are automatically turned off during hibernate mode to save power */
    AD5940_AFECtrlS(AfeCtrlSet, bTRUE);

    return;
}

/**
 * @brief Configures the AFE reference settings based on utility type and LPDAC state.
 * 
 * This function initializes the `AFERefCfg_Type` structure with recommended settings
 * for precision voltage references, taking into account whether the LPDAC is enabled.
 * 
 * @param type           Pointer to the `AFERefCfg_Type` structure to be configured.
 * @param utility_type   Pointer to the utility-specific reference configuration type
 *                       (`AD5940_ELECTROCHEMICAL_AFERefCfg_Type`).
 * @param LPDAC_enable   Boolean flag indicating whether the LPDAC is enabled (bTRUE) or disabled (bFALSE).
 * 
 * @note
 * - Refer to page 25 and Figure 37 (page 87) of the datasheet for reference details.
 * - Refer to page 34 for bias voltage considerations when LPDAC is enabled.
 */
static void _get_AFERefCfg_Type(
    AFERefCfg_Type *const type,
    const AD5940_ELECTROCHEMICAL_AFERefCfg_Type *const utility_type,
    const BoolFlag LPDAC_enable
)
{
    /**
     * Enable the high-precision voltage references.
     * Refer to page 25 and Figure 37 (page 87) of the datasheet.
     */
    type->HpBandgapEn = bTRUE;

    /**
     * Recommended high-precision buffer settings.
     * Refer to page 87 of the datasheet for details.
     */
    type->Hp1V1BuffEn = bTRUE;
    type->Hp1V8BuffEn = bTRUE;
    type->Disc1V1Cap = bFALSE;
    type->Disc1V8Cap = bFALSE;
    type->Hp1V8ThemBuff = bFALSE;
    type->Hp1V8Ilimit = bFALSE;
    type->Lp1V1BuffEn = bFALSE;
    type->Lp1V8BuffEn = bFALSE;
    
    // LP reference control - turn off them to save power
    type->LpRefBoostEn = bFALSE;

    // @see page 34 of the datasheet, With bias voltage
    if(LPDAC_enable == bTRUE)
    {
        type->LpBandgapEn = bTRUE;
        type->LpRefBufEn = bTRUE;
    }
    else
    {
        type->LpBandgapEn = bFALSE;
        type->LpRefBufEn = bFALSE;
    }

    return;
}

typedef enum
{
    _TIA_SELECTION_NULL,
    _TIA_SELECTION_LPTIA,
    _TIA_SELECTION_HSTIA,
}
_TIA_SELECTION;

static AD5940Err _get_LPDACCfg_Type(
    LPDACCfg_Type *const lpdac_cfg, 
    const _TIA_SELECTION tia_selection,
    const float e_start
)
{
    AD5940Err error;

    lpdac_cfg->LpdacSel = LPDAC0;
    lpdac_cfg->LpDacSrc = LPDACSRC_MMR;              // Use MMR data, we use LPDAC to generate bias voltage for LPTIA - the V_zero
    lpdac_cfg->LpDacRef = LPDACREF_2P5;
    lpdac_cfg->DataRst = bFALSE;                     // Do not reset data register
    lpdac_cfg->PowerEn = bTRUE;                      // Power up

    // Refer to page 39 of the datasheet, LPPA receives feedback to support LPDAC in producing a precise output signal.
    switch (tia_selection)
    {
    case _TIA_SELECTION_LPTIA:
        lpdac_cfg->LpDacSW = 0 
            | LPDACSW_VBIAS2LPPA 
            | LPDACSW_VZERO2LPTIA
        ;
        break;
    case _TIA_SELECTION_HSTIA:
        lpdac_cfg->LpDacSW = 0
            | LPDACSW_VBIAS2LPPA 
            | LPDACSW_VZERO2HSTIA
        ;
        break;
    default:
        lpdac_cfg->LpDacSW = 0 
            | LPDACSW_VBIAS2LPPA
        ;
        break;
    }

    lpdac_cfg->LpDacVzeroMux = LPDACVZERO_6BIT;
    lpdac_cfg->LpDacVbiasMux = LPDACVBIAS_12BIT;

    error = AD5940_ELECTROCHEMICAL_calculate_lpdac_dat_6_12_bits_by_potential(
        e_start,
        &(lpdac_cfg->DacData6Bit),
        &(lpdac_cfg->DacData12Bit)
    );
    if(error) return error;

    return AD5940ERR_OK;
}

AD5940Err AD5940_ELECTROCHEMICAL_config_afe_lpdac_lptia(
    const AD5940_ELECTROCHEMICAL_AFERefCfg_Type *const afe_ref_cfg,
    const float e_start
)
{
    AD5940Err error;

    AFERefCfg_Type aferef_cfg = {};
    LPLoopCfg_Type lp_loop_cfg = {};
    HSLoopCfg_Type hs_loop_cfg = {};
    DSPCfg_Type dsp_cfg = {};

    _get_AFERefCfg_Type(
        &aferef_cfg,
        afe_ref_cfg,
        bTRUE
    );

    error = _get_LPDACCfg_Type(
        &(lp_loop_cfg.LpDacCfg),
        _TIA_SELECTION_LPTIA,
        e_start
    );
    if(error) return error;

    AD5940_StructInit(
        &(lp_loop_cfg.LpAmpCfg), 
        sizeof(lp_loop_cfg.LpAmpCfg)
    );

    AD5940_StructInit(
        &hs_loop_cfg, 
        sizeof(hs_loop_cfg)
    );

    AD5940_StructInit(
        &dsp_cfg, 
        sizeof(dsp_cfg)
    );

    _config(
        &aferef_cfg,
        &lp_loop_cfg,
        &hs_loop_cfg,
        &dsp_cfg,
        0
    );

    return AD5940ERR_OK;
}

AD5940Err AD5940_ELECTROCHEMICAL_config_afe_lpdac_hstia(
    const AD5940_ELECTROCHEMICAL_AFERefCfg_Type *const afe_ref_cfg,
    const float e_start
)
{
    AD5940Err error;

    AFERefCfg_Type aferef_cfg = {};
    LPLoopCfg_Type lp_loop_cfg = {};
    HSLoopCfg_Type hs_loop_cfg = {};
    DSPCfg_Type dsp_cfg = {};

    _get_AFERefCfg_Type(
        &aferef_cfg,
        afe_ref_cfg,
        bTRUE
    );

    error = _get_LPDACCfg_Type(
        &(lp_loop_cfg.LpDacCfg),
        _TIA_SELECTION_HSTIA,
        e_start
    );
    if(error) return error;
    
    AD5940_StructInit(
        &(lp_loop_cfg.LpAmpCfg), 
        sizeof(lp_loop_cfg.LpAmpCfg)
    );

    AD5940_StructInit(
        &hs_loop_cfg, 
        sizeof(hs_loop_cfg)
    );

    AD5940_StructInit(
        &dsp_cfg, 
        sizeof(dsp_cfg)
    );

    _config(
        &aferef_cfg,
        &lp_loop_cfg,
        &hs_loop_cfg,
        &dsp_cfg,
        0
        | AFECTRL_HSTIAPWR
    );

    return AD5940ERR_OK;
}
