#include "ad5940_electrochemical_utils_dac_tia_adc.h"

#include "ad5940_utils.h"

static void _get_LPAmpCfg_Type_with_LPTIA(
    LPAmpCfg_Type *const type,
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *const lpdac_cfg,
    const AD5940_ELECTROCHEMICAL_LPTIACfg_Type *const lptia_cfg
)
{
    #if defined(CHIPSEL_M355)
    type->LpAmpSel = lpdac_cfg->LpAmpSel;
    #else
    type->LpAmpSel = LPAMP0;
    #endif
    type->LpAmpPwrMod = lpdac_cfg->LpAmpPwrMod;
    type->LpPaPwrEn = bTRUE;
    type->LpTiaPwrEn = bTRUE;
    type->LpTiaRf = lptia_cfg->LpTiaRf;
    type->LpTiaRload = lptia_cfg->LpTiaRload;
    type->LpTiaRtia = lptia_cfg->LpTiaRtia;
    if(type->LpTiaRtia == LPTIARTIA_OPEN)
    {
        type->LpTiaSW = 0
            | LPTIASW(2)
            | LPTIASW(4)
            | LPTIASW(5)
            | LPTIASW(9)
            // | LPTIASW(12)
            // | LPTIASW(13)
        ;
    }
    else
    {
        type->LpTiaSW = 0
            | LPTIASW(2)
            | LPTIASW(4)
            | LPTIASW(5)
            // | LPTIASW(12)
            // | LPTIASW(13)
        ;
    }
    return;
}

static void _get_LPAmpCfg_Type_with_HSTIA(
    LPAmpCfg_Type *const type,
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *const lpdac_cfg
)
{
    #if defined(CHIPSEL_M355)
    type->LpAmpSel = lpdac_cfg->LpAmpSel;
    #else
    type->LpAmpSel = LPAMP0;
    #endif
    type->LpAmpPwrMod = lpdac_cfg->LpAmpPwrMod;
    type->LpPaPwrEn = bTRUE;
    type->LpTiaPwrEn = bFALSE;
    type->LpTiaSW = 0
        | LPTIASW(2)
        | LPTIASW(4)

        // When using LPDAC with HATIA to start the electrochemical reaction, 
        // close LPTIASW(6) to prevent the signal from being generated on SE0.
        | LPTIASW(6)

        | LPTIASW(7)
    ;
    return;
}

/**
 * @param LPDAC_enable  @see pages 30 and 45 of the datasheet. 
 *                      The excitation signal is DC+AC. This parameter decides the DC value in mV unit. 0.0mV means no DC bias. (Refer to page 30 and page 45)
 */
static void _get_HSTIACfg_Type(
    HSTIACfg_Type *const type,
    const AD5940_ELECTROCHEMICAL_HSTIACfg_Type *const utility_type,
    const BoolFlag LPDAC_enable
)
{
    type->DiodeClose = utility_type->DiodeClose;
    type->ExtRtia = utility_type->ExtRtia;
    type->HstiaCtia = utility_type->HstiaCtia;
    type->HstiaDeRload = utility_type->HstiaDeRload;
    type->HstiaDeRtia = utility_type->HstiaDeRtia;
    type->HstiaRtiaSel = utility_type->HstiaRtiaSel;
    #if defined(CHIPSEL_M355)
    type->HstiaDe1Rtia = utility_type->HstiaDe1Rtia;
    type->HstiaDe1Rload = utility_type->HstiaDe1Rload;
    #endif

    // @see page 45 of the datasheet, With bias voltage */
    if(LPDAC_enable)
    {
        type->HstiaBias = HSTIABIAS_VZERO0;
    }
    else
    {
        type->HstiaBias = HSTIABIAS_1P1;
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

void _get_DSPCfg_Type(
    DSPCfg_Type *const type, 
    const _TIA_SELECTION tia_selection,
    const uint32_t ADCRate,
    const BoolFlag WGClkEnable,
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *const utility_type
)
{
    switch (tia_selection)
    {
    case _TIA_SELECTION_LPTIA:
        type->ADCBaseCfg.ADCMuxN = ADCMUXN_LPTIA0_N;
        type->ADCBaseCfg.ADCMuxP = ADCMUXP_LPTIA0_P;
        break;
    case _TIA_SELECTION_HSTIA:
        type->ADCBaseCfg.ADCMuxN = ADCMUXN_HSTIA_N;
        type->ADCBaseCfg.ADCMuxP = ADCMUXP_HSTIA_P;
        break;
    default:
        break;
    }
    type->ADCBaseCfg.ADCPga = utility_type->ADCPga;

    memcpy(&(type->ADCDigCompCfg), &(utility_type->ADCDigCompCfg), sizeof(ADCDigComp_Type));
    type->ADCFilterCfg = (ADCFilterCfg_Type) {
        .ADCAvgNum = utility_type->ADCFilterCfg.ADCAvgNum,
        .ADCRate = ADCRate,
        .ADCSinc2Osr = utility_type->ADCFilterCfg.ADCSinc2Osr,
        .ADCSinc3Osr = utility_type->ADCFilterCfg.ADCSinc3Osr,
        .BpNotch = utility_type->ADCFilterCfg.BpNotch,
        .BpSinc3 = utility_type->ADCFilterCfg.BpSinc3,
        .Sinc2NotchClkEnable = utility_type->ADCFilterCfg.Sinc2NotchClkEnable,
        .Sinc2NotchEnable = utility_type->ADCFilterCfg.Sinc2NotchEnable,
        .Sinc3ClkEnable = utility_type->ADCFilterCfg.Sinc3ClkEnable,
        .DFTClkEnable = utility_type->ADCFilterCfg.DFTClkEnable,
        .WGClkEnable = WGClkEnable,
    };
    memcpy(&(type->DftCfg), &(utility_type->DftCfg), sizeof(DFTCfg_Type));
    memcpy(&(type->StatCfg), &(utility_type->StatCfg), sizeof(StatCfg_Type));

    return;
}

AD5940Err AD5940_ELECTROCHEMICAL_config_lpdac_lptia_adc(
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *const lpdac_cfg,
    const AD5940_ELECTROCHEMICAL_LPTIACfg_Type *const lptia_cfg,
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *const dsp_cfg,
    const uint32_t ADCRate,
    const BoolFlag WGClkEnable
)
{
    LPAmpCfg_Type lp_amp_cfg_type;
    DSPCfg_Type dsp_cfg_type;

    _get_LPAmpCfg_Type_with_LPTIA(
        &lp_amp_cfg_type,
        lpdac_cfg,
        lptia_cfg
    );
    _get_DSPCfg_Type(
        &dsp_cfg_type, 
        _TIA_SELECTION_LPTIA,
        ADCRate,
        WGClkEnable,
        dsp_cfg
    );

    AD5940_LPAMPCfgS(&lp_amp_cfg_type);
    AD5940_DSPCfgS(&dsp_cfg_type);

    return AD5940ERR_OK;
}

AD5940Err AD5940_ELECTROCHEMICAL_config_lpdac_hstia_adc(
    const AD5940_ELECTROCHEMICAL_LPDACfg_Type *const lpdac_cfg,
    const AD5940_ELECTROCHEMICAL_HSTIACfg_Type *const hstia_cfg,
    const AD5940_ELECTROCHEMICAL_DSPCfg_Type *const dsp_cfg,
    const AD5940_ELECTROCHEMICAL_ELECTRODE_ROUTING *const electrode_routing,
    const uint32_t ADCRate,
    const BoolFlag WGClkEnable
)
{
    LPAmpCfg_Type lp_amp_cfg_type;
    HSTIACfg_Type hstia_cfg_type;
    SWMatrixCfg_Type sw_matrix_cfg;
    DSPCfg_Type dsp_cfg_type;

    _get_LPAmpCfg_Type_with_HSTIA(
        &lp_amp_cfg_type,
        lpdac_cfg
    );
    _get_HSTIACfg_Type(
        &hstia_cfg_type,
        hstia_cfg,
        bTRUE
    );
    memcpy(&sw_matrix_cfg, electrode_routing, sizeof(SWMatrixCfg_Type));
    _get_DSPCfg_Type(
        &dsp_cfg_type, 
        _TIA_SELECTION_HSTIA,
        ADCRate,
        WGClkEnable,
        dsp_cfg
    );

    AD5940_LPAMPCfgS(&lp_amp_cfg_type);
    AD5940_HSTIACfgS(&hstia_cfg_type);
    AD5940_SWMatrixCfgS(&sw_matrix_cfg);
    AD5940_DSPCfgS(&dsp_cfg_type);

    return AD5940ERR_OK;
}
