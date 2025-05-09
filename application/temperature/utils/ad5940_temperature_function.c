#include "ad5940_temperature.h"

#include "ad5940.h"
#include "ad5940_utils.h"

static void _get_SEQCfg_Type(
    SEQCfg_Type *const type, 
    BoolFlag enable
)
{
    type->SeqMemSize = SEQMEMSIZE_2KB;  /* 2kB SRAM is used for sequencer, others for data FIFO */
    type->SeqBreakEn = bFALSE;
    type->SeqIgnoreEn = bFALSE;
    type->SeqCntCRCClr = bTRUE;
    type->SeqEnable = enable;
    type->SeqWrTimer = 0;
    return;
}

static void _get_FIFOCfg_Type(
    FIFOCfg_Type *const type, 
    BoolFlag enable
)
{
    type->FIFOEn = enable;
    type->FIFOMode = FIFOMODE_FIFO;
    type->FIFOSize = FIFOSIZE_4KB;
    return;
}

static void _ad5940_analog_config(
    const AD5940_TEMPERATURE_ANALOG_CONFIG *const analog,
    const AD5940_ClockConfig *const clock
)
{
    AFERefCfg_Type aferef_cfg;
    ADCBaseCfg_Type adc_base;
    ADCFilterCfg_Type adc_filter;
    //init ad5940 for temperature measurement.
    AD5940_AFECtrlS(AFECTRL_ALL, bFALSE);  /* Init all to disable state */
    aferef_cfg.HpBandgapEn = bTRUE;
    aferef_cfg.Hp1V1BuffEn = bTRUE;
    aferef_cfg.Hp1V8BuffEn = bTRUE;       /* The High speed buffers are automatically turned off during hibernate */
    aferef_cfg.Disc1V1Cap = bFALSE;
    aferef_cfg.Disc1V8Cap = bFALSE;
    aferef_cfg.Hp1V8ThemBuff = bFALSE;
    aferef_cfg.Hp1V8Ilimit = bFALSE;
    aferef_cfg.Lp1V1BuffEn = bFALSE;
    aferef_cfg.Lp1V8BuffEn = bFALSE;
    /* LP reference control - turn off them to save power*/
    aferef_cfg.LpBandgapEn = bFALSE;
    aferef_cfg.LpRefBufEn = bFALSE;
    aferef_cfg.LpRefBoostEn = bFALSE;
    AD5940_REFCfgS(&aferef_cfg);	
    /* Initialize ADC basic function */
    adc_base.ADCMuxP = ADCMUXP_TEMPP;
    adc_base.ADCMuxN = ADCMUXN_TEMPN;
    adc_base.ADCPga = analog->ADCPga;
    AD5940_ADCBaseCfgS(&adc_base);
    /* Initialize ADC filters ADCRawData-->SINC3-->SINC2+NOTCH */
    adc_filter.ADCSinc2Osr = analog->ADCSinc2Osr;
    adc_filter.ADCSinc3Osr = analog->ADCSinc3Osr;
    adc_filter.ADCAvgNum = analog->ADCAvgNum;    /* Don't care about it. Average function is only used for DFT */
    adc_filter.ADCRate = clock->ADCRate;         /* If ADC clock is 32MHz, then set it to ADCRATE_1P6MHZ. Default is 16MHz, use ADCRATE_800KHZ. */
    adc_filter.BpNotch = analog->BpNotch;
    adc_filter.BpSinc3 = analog->BpSinc3;
    adc_filter.Sinc2NotchEnable = analog->Sinc2NotchEnable;
    AD5940_ADCFilterCfgS(&adc_filter);
    AD5940_AFECtrlS(AFECTRL_TEMPSPWR, bTRUE);   /* Turn on temperature sensor power */
    return;
}

static SEQInfo_Type _temperature_seq_info = {
    .SeqId = SEQID_0,       // use SEQ0 to run this sequence
    .WriteSRAM = bTRUE,     // we need to write this sequence to AD5940 SRAM.
};

static AD5940Err _write_temperature_sequence_commands(
	const uint32_t start_address,
    uint32_t *const sequence_length,
    const AD5940_TEMPERATURE_ANALOG_CONFIG *const analog,
    const AD5940_ClockConfig *const clock
)
{
    AD5940Err error;

    uint32_t const *pSeqCmd;
    uint32_t seq_len;
    ClksCalInfo_Type clks_cal;
    uint32_t WaitClks;
    clks_cal.DataType = analog->DataType;
    clks_cal.DataCount = 1;             /* Sample one data when wakeup */
    clks_cal.ADCSinc2Osr = analog->ADCSinc2Osr;
    clks_cal.ADCSinc3Osr = analog->ADCSinc3Osr;
    clks_cal.ADCAvgNum = analog->ADCAvgNum;
    clks_cal.RatioSys2AdcClk = clock->RatioSys2AdcClk; /* Assume ADC clock is same as system clock */
    AD5940_ClksCalculate(&clks_cal, &WaitClks);

    //generate sequence to measure temperature sensor output
    AD5940_SEQGenCtrl(bTRUE); //from now on, record all register operations rather than write them to AD5940 through SPI.

    AD5940_AFECtrlS(AFECTRL_ADCPWR, bTRUE); /* Turn ON ADC power */
    AD5940_SEQGenInsert(SEQ_WAIT(16*50));   /* wait another 50us for ADC to settle. */
    AD5940_AFECtrlS(AFECTRL_TEMPCNV|AFECTRL_ADCCNV, bTRUE);  /* Start ADC convert */
    AD5940_SEQGenInsert(SEQ_WAIT(WaitClks));
    AD5940_AFECtrlS(AFECTRL_ADCPWR|AFECTRL_TEMPCNV, bFALSE);    /* Stop ADC */
    AD5940_SEQGenInsert(SEQ_WAIT(20));			/* Add some delay before put AD5940 to hibernate, needs some clock to move data to FIFO. */
    AD5940_EnterSleepS();/* Goto hibernate */

    AD5940_SEQGenCtrl(bFALSE);  /* stop sequence generator */
    error = AD5940_SEQGenFetchSeq(&pSeqCmd, &seq_len);

    if(error != AD5940ERR_OK) return error;

    *sequence_length = seq_len;
    _temperature_seq_info.SeqRamAddr = start_address;
    _temperature_seq_info.pSeqCmd = pSeqCmd;
    _temperature_seq_info.SeqLen = seq_len;
    AD5940_SEQInfoCfg(&_temperature_seq_info);

    return error;
}

static AD5940Err _write_sequence_commands(
    const AD5940_TEMPERATURE_ANALOG_CONFIG *const config,
    const AD5940_ClockConfig *const clock
)
{
    AD5940Err error = AD5940ERR_OK;

    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    SEQCfg_Type seq_cfg;
    FIFOCfg_Type fifo_cfg;
    
    /* Configure sequencer and stop it */
    _get_SEQCfg_Type(&seq_cfg, bFALSE);
    AD5940_SEQCfg(&seq_cfg);

    /* Reconfigure FIFO */
    _get_FIFOCfg_Type(&fifo_cfg, bFALSE);
    AD5940_FIFOCfg(&fifo_cfg);

    AD5940_clear_GPIO_and_INT_flag();
    
    AD5940_clear_sequence_generator_buffer();

	uint32_t sequence_address = 0x00000000;
    uint32_t sequence_commands_length = 0;
    error = _write_temperature_sequence_commands(
        sequence_address,
        &sequence_commands_length,
        config,
        clock
    );
    if(error != AD5940ERR_OK) return AD5940ERR_PARA;
    sequence_address += sequence_commands_length;

    return error;
}

static AD5940Err _start_wakeup_timer_sequence(
    const uint32_t FIFO_thresh, 
    const uint16_t ADC_sample_interval,
    const uint32_t FifoSrc, 
    const float LFOSC_frequency
)
{
    /* Configure FIFO and Sequencer for normal Amperometric Measurement */
    AD5940_FIFOThrshSet(FIFO_thresh);
    AD5940_FIFOCtrlS(FifoSrc, bTRUE);

    AD5940_SEQCtrlS(bTRUE);

    /* Configure Wakeup Timer*/
    WUPTCfg_Type wupt_cfg;
    wupt_cfg.WuptEn = bTRUE;
    wupt_cfg.WuptEndSeq = WUPTENDSEQ_A;
    wupt_cfg.WuptOrder[0] = _temperature_seq_info.SeqId;
    wupt_cfg.SeqxSleepTime[_temperature_seq_info.SeqId] = 1; /* The minimum value is 1. Do not set it to zero. Set it to 1 will spend 2 32kHz clock. */
    wupt_cfg.SeqxWakeupTime[_temperature_seq_info.SeqId] = (uint32_t)(LFOSC_frequency * ADC_sample_interval * 1E-3F) - 1;
    AD5940_WUPTCfg(&wupt_cfg);

    return AD5940ERR_OK;
}

AD5940Err AD5940_TEMPERATURE_start(
    const AD5940_TEMPERATURE_START_CONFIG *const config
)
{
    AD5940Err error = AD5940ERR_OK;

    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_WriteReg(REG_AFE_TEMPSENS, config->TEMPSENS);
    
    AD5940_clear_GPIO_and_INT_flag();

    _ad5940_analog_config(
        config->analog,
        config->clock
    );
    _write_sequence_commands(
        config->analog,
        config->clock
    );

    // Ensure it is cleared as ad5940.c relies on the INTC flag as well.
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);

    AGPIOCfg_Type agpio_cfg;
    memcpy(&agpio_cfg, config->agpio_cfg, sizeof(AGPIOCfg_Type));
    AD5940_set_INTCCfg_by_AGPIOCfg_Type(&agpio_cfg, AFEINTSRC_DATAFIFOTHRESH);
    AD5940_AGPIOCfg(&agpio_cfg);

    error = _start_wakeup_timer_sequence(
        config->FIFO_thresh,
        config->ADC_sample_interval,
        config->analog->FifoSrc,
        config->LFOSC_frequency
    );
    if(error) return error;
	
    return AD5940ERR_OK;
}

AD5940Err AD5940_TEMPERATURE_stop(
    uint32_t* MCU_FIFO_buffer, 
    const uint16_t MCU_FIFO_buffer_max_length,
    uint16_t *const AD5940_FIFO_count
)
{
    return AD5940_TEMPERATURE_interrupt(
        MCU_FIFO_buffer,
        MCU_FIFO_buffer_max_length,
        0,
        AD5940_FIFO_count
    );
}

AD5940Err AD5940_TEMPERATURE_interrupt(
    uint32_t* MCU_FIFO_buffer, 
    const uint16_t MCU_FIFO_buffer_max_length,
    const uint16_t AD5940_FIFO_new_thresh,
    uint16_t* AD5940_FIFO_count
)
{
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_SleepKeyCtrlS(SLPKEY_LOCK);  /* We need time to read data from FIFO, so, do not let AD5940 goes to hibernate automatically */

    *AD5940_FIFO_count = AD5940_FIFOGetCnt();
    if(*AD5940_FIFO_count > MCU_FIFO_buffer_max_length) return AD5940ERR_BUFF;
    AD5940_FIFORd(MCU_FIFO_buffer, *AD5940_FIFO_count);

    // Enable AFE to enter sleep mode.
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Unlock so sequencer can put AD5940 to sleep */

    AD5940_INTCClrFlag(AFEINTSRC_DATAFIFOTHRESH);
    if(AD5940_FIFO_new_thresh > 0)
    {
        AD5940_reset_fifocon();
        AD5940_FIFOThrshSet(AD5940_FIFO_new_thresh);
        AD5940_EnterSleepS();
    }
    else
    {
        AD5940_WriteReg(REG_AFE_TEMPSENS, 0x0);
        AD5940_shutdown_afe_lploop_hsloop_dsp();
    }

    return AD5940ERR_OK;
}
