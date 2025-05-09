#include "ad5940_utils_fifo.h"

void AD5940_reset_fifocon(void)
{
    uint32_t fifocon = 0;
    fifocon = AD5940_ReadReg(REG_AFE_FIFOCON);
    AD5940_WriteReg(REG_AFE_FIFOCON, 0);        /* Disable FIFO before changing memory configuration */
    AD5940_WriteReg(REG_AFE_FIFOCON, fifocon);  /* restore FIFO configuration */
}
