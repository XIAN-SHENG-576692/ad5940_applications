#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_utils.h"

/**
 * Configuration structure for temperature measurement using the AD5940.
 */
typedef struct
{
    uint32_t ADCSinc2Osr;       /**< ADC SINC2 oversampling rate. Refer to @ref ADCSINC2OSR_Const. */
    uint32_t ADCSinc3Osr;       /**< ADC SINC3 oversampling rate. Refer to @ref ADCSINC3OSR_Const. */
    uint32_t ADCAvgNum;         /**< ADC averaging number. Refer to @ref ADCAVGNUM_Const. */
    uint32_t ADCPga;            /**< ADC Programmable Gain Amplifier (PGA) setting. Refer to @ref ADCPGA_Const. */

    uint32_t DataType;          /**< Data type configuration. Refer to @ref DATATYPE_Const. */
    uint32_t FifoSrc;           /**< FIFO source configuration. Refer to @ref FIFOSRC_Const. */

    BoolFlag BpNotch;           /**< Enable or disable bypass for the notch filter. */
    BoolFlag BpSinc3;           /**< Enable or disable bypass for the SINC3 filter. */
    BoolFlag Sinc2NotchEnable;  /**< Enable or disable the SINC2-notch filter. */
}
AD5940_TEMPERATURE_ANALOG_CONFIG;

typedef struct
{
    const AGPIOCfg_Type *agpio_cfg;         /**< Pointer to GPIO configuration.
                                                 - Refer to datasheet pages 112 and 122.
                                                 - Configure GPIO for interrupts based on PCB design. */
    const AD5940_ClockConfig *clock_cfg;    /**< Pointer to clock configuration.
                                                 Obtainable via
                                                 @ref AD5940_set_active_power in utility/ad5940_utility_power.h. */
    float LFOSC_frequency;                  /**< Low-frequency oscillator frequency, used for internal timing.
                                                 Obtainable via @ref AD5940_LFOSCMeasure in library/ad5940.h.*/
    uint16_t FIFO_thresh;                   /**< FIFO threshold value. Interrupt is triggered when this threshold is reached. */
}
AD5940_TEMPERATURE_RUN_CONFIG;

typedef struct
{
    float sampling_interval;    /**< Sampling interval in microseconds. */
    uint32_t TEMPSENS;          /**< Temperature sensor configuration. Refer to page 57 of the datasheet:
                                     ```
                                     For improved accuracy, configure the temperature sensor in chop
                                     mode via TEMPCON0, Bits[3:1]. If chopping is selected, the user
                                     must ensure an even number of ADC conversions take place on the
                                     temperature sensor channel and that these results are averaged.
                                     ```
                                     See TEMPCON0 configuration details on page 61 of the datasheet. */
}
AD5940_TEMPERATURE_PARAMETERS;

/**
 * @brief Configuration structure for starting the temperature measurement on the AD5940.
 */
typedef struct
{

    const AD5940_TEMPERATURE_PARAMETERS *parameters;
    const AD5940_TEMPERATURE_ANALOG_CONFIG *analog_cfg;     /**< Analog configuration structure. 
                                                                 Refer to @ref AD5940_TEMPERATURE_ANALOG_CONFIG. */
    const AD5940_TEMPERATURE_RUN_CONFIG *run_cfg;           /**< Execution and timing configuration */
}
AD5940_TEMPERATURE_START_CONFIG;

#ifdef __cplusplus
}
#endif
