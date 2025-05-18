#include "ad5940_electrochemical_ca_function.h"

#include "ad5940_utils.h"
#include "ad5940_electrochemical_utils.h"

static AD5940Err _write_sequence_commands(
    const AD5940_ClockConfig *const clock_cfg,
    const DFTCfg_Type *const dft,
    const uint32_t ADCAvgNum,
    const uint32_t ADCSinc2Osr,
    const uint32_t ADCSinc3Osr,
    const BoolFlag BpNotch,
    const uint32_t DataType
)
{
    AD5940Err error = AD5940ERR_OK;
    
    uint32_t sequence_address = 0x00;

    error = AD5940_ELECTROCHEMICAL_write_sequence_commands_config(
        clock_cfg,
        dft,
        ADCAvgNum,
        ADCSinc2Osr,
        ADCSinc3Osr,
        BpNotch,
        1,
        DataType,
        &sequence_address
    );
    if(error != AD5940ERR_OK) return AD5940ERR_PARA;

    return AD5940ERR_OK;
}

static AD5940Err _start_wakeup_timer_sequence(
    const AD5940_ELECTROCHEMICAL_CA_PARAMETERS *parameters,
    const uint32_t FifoSrc,
    const uint16_t FifoThresh,
    const float LFOSCClkFreq
)
{
    /* Configure FIFO and Sequencer for normal Amperometric Measurement */
    AD5940_FIFOThrshSet((uint32_t) FifoThresh);
    AD5940_FIFOCtrlS(FifoSrc, bTRUE);

    AD5940_SEQCtrlS(bTRUE);

    SEQInfo_Type *ADC_seq_info;
    AD5940_ELECTROCHEMICAL_UTILITY_get_ADC_seq_info(
        &ADC_seq_info
    );

    /* Configure Wakeup Timer*/
    WUPTCfg_Type wupt_cfg;
    wupt_cfg.WuptEn = bTRUE;
    wupt_cfg.WuptEndSeq = WUPTENDSEQ_A;
    wupt_cfg.WuptOrder[0] = ADC_seq_info->SeqId;
    wupt_cfg.SeqxSleepTime[ADC_seq_info->SeqId] = 1; /* The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock. */
    wupt_cfg.SeqxWakeupTime[ADC_seq_info->SeqId] = (uint32_t)(LFOSCClkFreq * parameters->t_interval) - 1;
    AD5940_WUPTCfg(&wupt_cfg);

    return AD5940ERR_OK;
}

/**
 * This function is based on the example in the AppCHRONOAMPInit() function found in
 * ad5940-examples/examples/AD5940_ChronoAmperometric/ChronoAmperometric.c.
 */
AD5940Err AD5940_ELECTROCHEMICAL_CA_start(
    const AD5940_ELECTROCHEMICAL_CA_CONFIG *const config
)
{
    AD5940Err error = AD5940ERR_OK;

    error = AD5940_ELECTROCHEMICAL_CA_PARAMETERS_check(config->parameters);
    if(error != AD5940ERR_OK) return error;

    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    /**
     * Before the application begins, INT are used for configuring parameters.
     * Therefore, they should not be used during the configuration process itself.
     */
    AD5940_clear_GPIO_and_INT_flag();

    switch (config->path_type)
    {
    case 0:
        error = AD5940_ELECTROCHEMICAL_config_afe_lpdac_lptia(
            config->path.lpdac_to_lptia->afe_ref_cfg,
            config->parameters->e_dc
        );
        if(error != AD5940ERR_OK) return error;

        error = _write_sequence_commands(
            config->run->clock_cfg,
            &(config->path.lpdac_to_lptia->dsp_cfg->DftCfg),
            config->path.lpdac_to_lptia->dsp_cfg->ADCFilterCfg.ADCAvgNum,
            config->path.lpdac_to_lptia->dsp_cfg->ADCFilterCfg.ADCSinc2Osr,
            config->path.lpdac_to_lptia->dsp_cfg->ADCFilterCfg.ADCSinc3Osr,
            config->path.lpdac_to_lptia->dsp_cfg->ADCFilterCfg.BpNotch,
            config->run->DataType
        );
        if(error != AD5940ERR_OK) return error;

        error = AD5940_ELECTROCHEMICAL_config_lpdac_lptia_adc(
            config->path.lpdac_to_lptia->lpdac_cfg,
            config->path.lpdac_to_lptia->lptia_cfg,
            config->path.lpdac_to_lptia->dsp_cfg,
            config->run->clock_cfg->ADCRate,
            bFALSE
        );
        if(error != AD5940ERR_OK) return error;
        break;
    
    case 1:
        error = AD5940_ELECTROCHEMICAL_config_afe_lpdac_hstia(
            config->path.lpdac_to_hstia->afe_ref_cfg,
            config->parameters->e_dc
        );
        if(error != AD5940ERR_OK) return error;

        error = _write_sequence_commands(
            config->run->clock_cfg,
            &(config->path.lpdac_to_hstia->dsp_cfg->DftCfg),
            config->path.lpdac_to_hstia->dsp_cfg->ADCFilterCfg.ADCAvgNum,
            config->path.lpdac_to_hstia->dsp_cfg->ADCFilterCfg.ADCSinc2Osr,
            config->path.lpdac_to_hstia->dsp_cfg->ADCFilterCfg.ADCSinc3Osr,
            config->path.lpdac_to_hstia->dsp_cfg->ADCFilterCfg.BpNotch,
            config->run->DataType
        );
        if(error != AD5940ERR_OK) return error;

        error = AD5940_ELECTROCHEMICAL_config_lpdac_hstia_adc(
            config->path.lpdac_to_hstia->lpdac_cfg,
            config->path.lpdac_to_hstia->hstia_cfg,
            config->path.lpdac_to_hstia->dsp_cfg,
            config->path.lpdac_to_hstia->electrode_routing,
            config->run->clock_cfg->ADCRate,
            bFALSE
        );
        if(error != AD5940ERR_OK) return error;
        break;
    
    case 2:
        // TODO
        return AD5940ERR_PARA;
    
    default:
        return AD5940ERR_PARA;
    }

    // Ensure it is cleared as ad5940.c relies on the INTC flag as well.
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

    AGPIOCfg_Type agpio_cfg;
    memcpy(&agpio_cfg, config->run->agpio_cfg, sizeof(AGPIOCfg_Type));
    AD5940_set_INTCCfg_by_AGPIOCfg_Type(&agpio_cfg, AFEINTSRC_DATAFIFOTHRESH);
    AD5940_AGPIOCfg(&agpio_cfg);

    error = _start_wakeup_timer_sequence(
        config->parameters,
        config->run->FifoSrc,
        config->run->FifoThresh,
        config->run->LFOSCClkFreq
    );
    if(error != AD5940ERR_OK) return error;

    return error;
}
