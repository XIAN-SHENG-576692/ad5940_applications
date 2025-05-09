#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940_electrochemical_utils.h"

typedef struct 
{

    float LFOSC_frequency;              /**< Low-frequency oscillator frequency, used for internal timing. 
                                             Obtainable via @ref AD5940_LFOSCMeasure in library/ad5940.h.*/
                                                        
    const AD5940_ClockConfig *clock;    /**< Pointer to clock configuration.
                                             Obtainable via 
                                             @ref AD5940_set_active_power 
                                             in utility/ad5940_utility_power.h. */

    const AGPIOCfg_Type *agpio_cfg;     /**< Pointer to GPIO configuration. 
                                             - Refer to datasheet pages 112 and 122.
                                             - Configure GPIO for interrupts based on PCB design. */
    uint32_t DataType;                  /**< Data type configuration. @ref DATATYPE_Const. */
    uint32_t FifoSrc;                   /**< FIFO source configuration. @ref FIFOSRC_Const*/
}
AD5940_ELECTROCHEMICAL_RUN_CONFIG;

#ifdef __cplusplus
}
#endif
