#include "ad5940_electrochemical_interrupt.h"

#include "ad5940_utils.h"

AD5940Err AD5940_ELECTROCHEMICAL_interrupt(
    const uint16_t MCU_FIFO_buffer_max_length,
    const int32_t AD5940_FIFO_new_thresh,
    uint32_t* MCU_FIFO_buffer, 
    uint16_t* MCU_FIFO_count
)
{
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_SleepKeyCtrlS(SLPKEY_LOCK);  /* We need time to read data from FIFO, so, do not let AD5940 goes to hibernate automatically */

    *MCU_FIFO_count = AD5940_FIFOGetCnt();
    if(*MCU_FIFO_count > MCU_FIFO_buffer_max_length) return AD5940ERR_BUFF;
    AD5940_FIFORd(MCU_FIFO_buffer, *MCU_FIFO_count);

    // Refer to page 107 of the datasheet
    // Enable AFE to enter sleep mode.
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Unlock so sequencer can put AD5940 to sleep */

    AD5940_INTCClrFlag(AFEINTSRC_DATAFIFOTHRESH);
    if(AD5940_FIFO_new_thresh == 0)
    {
        AD5940_WriteReg(REG_AFE_TEMPSENS, 0x0);
        AD5940_shutdown_afe_lploop_hsloop_dsp();
    }
    else
    {
        AD5940_reset_fifocon();
        if(AD5940_FIFO_new_thresh > 0)
        {
            AD5940_FIFOThrshSet(AD5940_FIFO_new_thresh);
        }
    }

    return AD5940ERR_OK;
}
