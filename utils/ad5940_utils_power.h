#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"
#include "ad5940_utils_clock.h"

/**
 * Configures the active power settings for the AD5940 chip.
 * 
 * @note
 * This function adjusts power and clock settings for optimal operation of 
 * the AD5940, considering both internal oscillators and external crystal options.
 * 
 * References:
 * - Datasheet pages 129 and 132:
 *   - LFOSC: Low-frequency oscillator, used for the sleep/wake timer.
 *   - HFOSC: High-frequency oscillator, powering the HSDAC, HSTIA, and ADC circuits.
 *   - HFXTAL: External crystal oscillator option, mutually exclusive with HFOSC.
 * 
 * - Datasheet page 135:
 *   ```
 *   The ADC can run at 32 MHz, but system clock cannot run at 32 MHz. 
 *   It is required to divide the system clock by 2 first before switching 
 *   the oscillator to 32 MHz. Refer to the SYSCLKDIV bit in the CLKCON0 register.
 *   ```
 *
 * - Refer to page 55 of the datasheet, 
 *   ```
 *   The ADC supports averaging and digital filtering options. 
 *   The user can trade off speed vs. precision by using these options.
 *   The highest ADC update rate is 800 kHz in normal mode and 1.6 MHz 
 *   in high speed mode, with no digital filtering.
 *   ```
 * 
 * This function is implemented based on @ref AD5940_HPModeEn, as defined in `ad5940.h`.
 * 
 * Parameters:
 * @param AFEPWR_Const  A constant defining the AFE (Analog Front End) power settings. See @ref AFEPWR_Const for options.
 * @param CLKSEL        Clock selection parameter. Refer to datasheet page 133 for configuration details.
 * @param clockConfig   Pointer to a configuration structure to store clock settings. See @ref AD5940_ClockConfig.
 * 
 * @return AD5940Err    Error code indicating the success or failure of the operation.
 */
AD5940Err AD5940_set_active_power(
    const uint32_t AFEPWR_Const,
    const uint32_t CLKSEL,
    AD5940_ClockConfig *const clockConfig
);

/**
 * Performs a complete shutdown of the AD5940 chip.
 * 
 * This function not only invokes `AD5940_ShutDownS()` from ad5940.h to disable
 * the core functionalities but also ensures that the DAC, TIA, and ADC are reset to zero,
 * leaving the chip in a fully powered-down and inactive state.
 * 
 * For detailed reference on the chip's shutdown procedure, see page 129 of the datasheet.
 * 
 * @return AD5940Err     Returns an error code that indicates the success or failure
 *                       of the shutdown process.
 */
AD5940Err AD5940_shutdown_afe_lploop_hsloop_dsp(void);

#ifdef __cplusplus
}
#endif
