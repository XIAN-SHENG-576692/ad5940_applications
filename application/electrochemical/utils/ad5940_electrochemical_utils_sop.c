#include "ad5940_electrochemical_utils_sop.h"

#include "ad5940.h"
#include "ad5940_utils.h"

#include "ad5940_electrochemical_utils.h"

static SEQInfo_Type _ADC_seq_info = {
    .SeqId = SEQID_0,
    .WriteSRAM = bTRUE,
};

void AD5940_ELECTROCHEMICAL_UTILITY_get_ADC_seq_info(
    SEQInfo_Type **ADC_seq_info
)
{
    *ADC_seq_info = &_ADC_seq_info;
    return;
}

static void _get_SEQCfg_Type(
    SEQCfg_Type *const type, 
    BoolFlag enable
)
{
    type->SeqMemSize = SEQMEMSIZE_2KB;  /* 2kB SRAM is used for sequencer, others for data FIFO */
    type->SeqBreakEn = bFALSE;
    type->SeqIgnoreEn = bTRUE;
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

static void _get_ClksCalInfo_Type(
    ClksCalInfo_Type *const type,
    const ADCFilterCfg_Type *const adc_filter,
    const DFTCfg_Type *const dft,
    const AD5940_ClockConfig *const clock_cfg,
    uint32_t DataType,
    uint32_t DataCount
)
{
    type->ADCAvgNum = adc_filter->ADCAvgNum;        /* Don't care */
    type->ADCSinc2Osr = adc_filter->ADCSinc2Osr;    /* Don't care */
    type->ADCSinc3Osr = adc_filter->ADCSinc3Osr;
    type->DataType = DataType;
    type->DataCount = DataCount;                   /* Sample one point everytime */
    type->DftSrc = dft->DftSrc;
    type->RatioSys2AdcClk = clock_cfg->RatioSys2AdcClk;
    return;
}

/**
* @brief Generate ADC control sequence and write the commands to SRAM.
* @return return error code.
*/
static AD5940Err _write_ADC_sequence_commands(
	const uint32_t start_address, 
    uint32_t *const sequence_length,
    const ADCFilterCfg_Type *const adc_filter,
    const DFTCfg_Type *const dft,
    const AD5940_ClockConfig *const clock_cfg,
    uint32_t DataType
)
{
	AD5940Err error = AD5940ERR_OK;

	const uint32_t *pSeqCmd;
	uint32_t SeqLen;

	uint32_t WaitClks;
    ClksCalInfo_Type clks_cal;

    _get_ClksCalInfo_Type(
        &clks_cal,
        adc_filter,
        dft,
        clock_cfg,
        DataType,
        1
    );
	AD5940_ClksCalculate(&clks_cal, &WaitClks);

	AD5940_SEQGenCtrl(bTRUE);
    
	AD5940_AFECtrlS(AFECTRL_ADCPWR | AFECTRL_SINC2NOTCH, bTRUE);
	AD5940_SEQGenInsert(SEQ_WAIT(16*250));  /* wait 250us for reference power up */
	AD5940_AFECtrlS(AFECTRL_ADCCNV, bTRUE);  /* Start ADC convert and DFT */
	AD5940_SEQGenInsert(SEQ_WAIT(WaitClks));  /* wait for first data ready */
	AD5940_AFECtrlS(AFECTRL_ADCPWR | AFECTRL_ADCCNV | AFECTRL_SINC2NOTCH, bFALSE);  /* Stop ADC */
	// AD5940_EnterSleepS();/* Goto hibernate */
	/* Sequence end. */
	error = AD5940_SEQGenFetchSeq(&pSeqCmd, &SeqLen);
	AD5940_SEQGenCtrl(bFALSE); /* Stop sequencer generator */

    if(error != AD5940ERR_OK) return error;

    *sequence_length = SeqLen;
    _ADC_seq_info.SeqRamAddr = start_address;
    _ADC_seq_info.pSeqCmd = pSeqCmd;
    _ADC_seq_info.SeqLen = SeqLen;
    AD5940_SEQInfoCfg(&_ADC_seq_info);

	return AD5940ERR_OK;
}

static void _start()
{
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
    
    /**
     * Before the application begins, INT are used for configuring parameters.
     * Therefore, they should not be used during the configuration process itself.
     */
    AD5940_clear_sequence_generator_buffer();
}

AD5940Err AD5940_ELECTROCHEMICAL_write_sequence_commands_config(
    const ADCFilterCfg_Type *const adc_filter,
    const DFTCfg_Type *const dft,
    const AD5940_ClockConfig *const clock_cfg,
    const uint32_t DataType,
    uint32_t *const sequence_address
)
{
    AD5940Err error = AD5940ERR_OK;

    _start();

    *sequence_address = 0x00;
    uint32_t sequence_commands_length = 0;

    error = _write_ADC_sequence_commands(
        *sequence_address,
        &sequence_commands_length,
        adc_filter,
        dft,
        clock_cfg,
        DataType
    );
    if(error != AD5940ERR_OK) return AD5940ERR_PARA;
    *sequence_address += sequence_commands_length;

    return AD5940ERR_OK;
}
