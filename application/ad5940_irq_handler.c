#include "ad5940_electrochemical_interrupt.h"

#include "ad5940_utils.h"

AD5940Err AD5940_irq_handler(
    const int32_t new_fifo_thresh,
    const uint16_t buffer_max_length,
    uint32_t* buffer, 
    uint16_t* buffer_length
)
{
    /* Wakeup AFE by read register, read 10 times at most */
    if(AD5940_WakeUp(10) > 10) return AD5940ERR_WAKEUP;  /* Wakeup Failed */

    AD5940_SleepKeyCtrlS(SLPKEY_LOCK);  /* We need time to read data from FIFO, so, do not let AD5940 goes to hibernate automatically */

    *buffer_length = AD5940_FIFOGetCnt();
    if(*buffer_length > buffer_max_length) return AD5940ERR_BUFF;
    AD5940_FIFORd(buffer, *buffer_length);

    // Refer to page 107 of the datasheet
    // Enable AFE to enter sleep mode.
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Unlock so sequencer can put AD5940 to sleep */

    AD5940_INTCClrFlag(AFEINTSRC_DATAFIFOTHRESH);
    if(new_fifo_thresh == 0)
    {
        AD5940_shutdown_afe_lploop_hsloop_dsp();
    }
    else
    {
        AD5940_reset_fifocon();
        if(new_fifo_thresh > 0)
        {
            AD5940_FIFOThrshSet(new_fifo_thresh);
        }
    }

    return AD5940ERR_OK;
}
