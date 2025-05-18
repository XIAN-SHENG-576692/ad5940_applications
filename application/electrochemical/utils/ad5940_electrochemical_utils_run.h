#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_electrochemical_utils.h"

typedef struct 
{
    const AGPIOCfg_Type *agpio_cfg;         /**< Pointer to GPIO configuration. 
                                                 - Refer to datasheet pages 112 and 122.
                                                 - Configure GPIO for interrupts based on PCB design. */
    const AD5940_ClockConfig *clock_cfg;    /**< Pointer to clock configuration.
                                                 Obtainable via
                                                 @ref AD5940_set_active_power in utility/ad5940_utility_power.h. */
    float LFOSCClkFreq;                     /**< Low-frequency oscillator frequency, used for internal timing.
                                                 Obtainable via @ref AD5940_LFOSCMeasure in library/ad5940.h.*/
    uint32_t DataType;                      /**< Data type configuration. @ref DATATYPE_Const. */
    uint32_t FifoSrc;                       /**< FIFO source configuration. @ref FIFOSRC_Const*/
    uint16_t FifoThresh;                    /**< FIFO threshold value. Interrupt is triggered when this threshold is reached. */
}
AD5940_ELECTROCHEMICAL_RUN_CONFIG;

#ifdef __cplusplus
}
#endif
