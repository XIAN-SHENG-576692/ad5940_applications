/**
 * Configures the voltage between V_bias and V_zero.
 * 
 * @note
 * Refer to page 30 of the datasheet for detailed information on setting the voltage
 * between V_bias and V_zero. The default MUX settings used in this file are:
 * - `LPDACVBIAS_12BIT` for V_bias (higher precision: 12 bits)
 * - `LPDACVZERO_6BIT` for V_zero (lower precision: 6 bits)
 * 
 * Since V_bias has higher precision than V_zero, the desired output voltage (V_out)
 * should be set as follows:
 * 1. Configure V_zero using the maximum voltage (V_max).
 * 2. Adjust V_bias to fine-tune the precision of V_out.
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

typedef enum {
    AD5940_LPDAC_ROUND_CEIL,
    AD5940_LPDAC_ROUND_FLOOR,
    AD5940_LPDAC_ROUND_NEAREST,
} 
AD5940_LPDAC_ROUND;

AD5940Err AD5940_convert_voltage_to_lpdac_dat_6_bits(
    const float voltage, 
    AD5940_LPDAC_ROUND handler,
    uint16_t *const lpdac_dat_6_bits
);

AD5940Err AD5940_convert_voltage_to_lpdac_dat_12_bits(
    const float voltage, 
    AD5940_LPDAC_ROUND handler,
    uint16_t *const lpdac_dat_12_bits
);

AD5940Err AD5940_convert_lpdac_dat_6_bits_to_voltage(
    const uint16_t lpdac_dat_6_bits,
    float *const voltage
);

AD5940Err AD5940_convert_lpdac_dat_12_bits_to_voltage(
    const uint16_t lpdac_dat_12_bits,
    float *const voltage
);

AD5940Err AD5940_combine_lpdac_dat_bits(
    const uint16_t lpdac_dat_6_bits,
    const uint16_t lpdac_dat_12_bits,
    uint32_t *const lpdac_dat_bits
);

#ifdef __cplusplus
}
#endif
