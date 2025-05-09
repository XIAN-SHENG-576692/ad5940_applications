#include "ad5940_utils_power.h"

AD5940Err AD5940_set_active_power(
    const uint32_t AFEPWR_Const,
    const uint32_t CLKSEL,
    AD5940_ClockConfig *const clockConfig
)
{
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_WriteReg(REG_AFECON_CLKSEL, CLKSEL);

    switch (AFEPWR_Const)
    {
    case AFEPWR_HP:
        AD5940_HPModeEn(bTRUE);
        clockConfig->ADCRate = ADCRATE_1P6MHZ;
        clockConfig->AdcClkFreq = 3.2E7F;
        clockConfig->SysClkFreq = 1.6E7F;
        break;

    case AFEPWR_LP:
        AD5940_HPModeEn(bFALSE);
        clockConfig->ADCRate = ADCRATE_800KHZ;
        clockConfig->AdcClkFreq = 1.6E7F;
        clockConfig->SysClkFreq = 1.6E7F;
        break;

    default:
        return AD5940ERR_PARA;
    }

    clockConfig->RatioSys2AdcClk = clockConfig->SysClkFreq / clockConfig->AdcClkFreq;

    return AD5940ERR_OK;
}

/**
 * This function is based on the example in the AppCHRONOAMPCtrl() function found in
 * ad5940-examples/examples/AD5940_ChronoAmperometric/ChronoAmperometric.c. 
 */
AD5940Err AD5940_shutdown_afe_lploop_hsloop_dsp(void)
{
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_AFECtrlS(AFECTRL_ALL, bFALSE);

    /* Start Wupt right now */
    AD5940_WUPTCtrl(bFALSE);
    /* There is chance this operation will fail because sequencer could put AFE back 
    to hibernate mode just after waking up. Use STOPSYNC is better. */
    AD5940_WUPTCtrl(bFALSE);

    AFERefCfg_Type aferef_cfg;
    memset(&aferef_cfg, 0, sizeof(aferef_cfg));
    AD5940_REFCfgS(&aferef_cfg);
		
    /* Turn off LPloop related blocks which are not controlled automatically by sleep operation */
    LPLoopCfg_Type lp_loop;
    memset(&lp_loop, 0, sizeof(lp_loop));
    AD5940_LPLoopCfgS(&lp_loop);
		
    HSLoopCfg_Type hs_loop;
    memset(&hs_loop, 0, sizeof(hs_loop));
    AD5940_HSLoopCfgS(&hs_loop);
		
    DSPCfg_Type dsp_cfg;
    memset(&dsp_cfg, 0, sizeof(dsp_cfg));
    AD5940_DSPCfgS(&dsp_cfg);

    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK);

    AD5940_ShutDownS();

    return AD5940ERR_OK;
}
