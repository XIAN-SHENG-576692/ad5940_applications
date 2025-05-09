#include "ad5940_main.h"
#include "ad5940_utils.h"

AD5940Err AD5940_MAIN_init(
    uint32_t *const sequencer_generator_buffer, 
    const uint16_t sequencer_generator_buffer_length
)
{
    AD5940Err error = AD5940ERR_OK;

    error = AD5940_set_sequence_generator_buffer(
        sequencer_generator_buffer, 
        sequencer_generator_buffer_length
    );
    if(error) return error;

    /* Use hardware reset */
    AD5940_HWReset();
    // AD5940_SoftRst();

    /* Platform configuration */
    AD5940_Initialize();

    /* Enable AFE to enter sleep mode. */
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK);

    /* Goto hibernate */
    AD5940_EnterSleepS();

    return AD5940ERR_OK;
}

AD5940Err AD5940_MAIN_reset(void)
{
    AD5940Err error = AD5940ERR_OK;

    error = AD5940_clear_sequence_generator_buffer();
    if(error) return error;

    /* Use hardware reset */
    AD5940_HWReset();
    // AD5940_SoftRst();
    
    AD5940_Delay10us(1E4);

    /* Platform configuration */
    AD5940_Initialize();

    /* Enable AFE to enter sleep mode. */
    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK);

    /* Goto hibernate */
    AD5940_EnterSleepS();

    return AD5940ERR_OK;
}
