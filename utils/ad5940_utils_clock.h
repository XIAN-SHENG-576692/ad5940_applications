#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

typedef struct 
{
    uint32_t ADCRate;       // See @ref ADCRATE_Const.
    float AdcClkFreq;       // ADC clock frequency value (in Hz). This term is referenced throughout the datasheet.
    float SysClkFreq;       // system clock frequency value (in Hz). This term is referenced throughout the datasheet.
    float RatioSys2AdcClk;  // SysClkFreq / AdcClkFreq
} 
AD5940_ClockConfig;

#ifdef __cplusplus
}
#endif
