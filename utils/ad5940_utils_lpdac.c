#include "ad5940_utils_lpdac.h"

#include <stdlib.h>

/** 
 * Default LPDAC resolution(2.5V internal reference).
 * @see page 30 of the datasheet.
*/
#define DAC12BITVOLT_1LSB   (2.2 / 4095.0)            // V
#define DAC6BITVOLT_1LSB    (64.0 * DAC12BITVOLT_1LSB)    // V

AD5940Err AD5940_convert_voltage_to_lpdac_dat_6_bits(
    const float voltage, 
    AD5940_LPDAC_ROUND handler,
    uint16_t *const lpdac_dat_6_bits
)
{
    uint16_t code;
    switch (handler)
    {
    case AD5940_LPDAC_ROUND_CEIL:
        code = (uint16_t) ceilf(voltage / DAC6BITVOLT_1LSB);
        break;
    case AD5940_LPDAC_ROUND_FLOOR:
        code = (uint16_t) floorf(voltage / DAC6BITVOLT_1LSB);
        break;
    case AD5940_LPDAC_ROUND_NEAREST:
        code = (uint16_t) roundf(voltage / DAC6BITVOLT_1LSB);
        break;
    default:
        return AD5940ERR_PARA;
    }
    if(code > 0x3F) return AD5940ERR_PARA;
    *lpdac_dat_6_bits = code;
    return AD5940ERR_OK;
}

AD5940Err AD5940_convert_voltage_to_lpdac_dat_12_bits(
    const float voltage, 
    AD5940_LPDAC_ROUND handler,
    uint16_t *const lpdac_dat_12_bits
)
{
    uint16_t bits;
    switch (handler)
    {
    case AD5940_LPDAC_ROUND_CEIL:
        bits = (uint16_t) ceilf(voltage / DAC12BITVOLT_1LSB);
        break;
    case AD5940_LPDAC_ROUND_FLOOR:
        bits = (uint16_t) floorf(voltage / DAC12BITVOLT_1LSB);
        break;
    case AD5940_LPDAC_ROUND_NEAREST:
        bits = (uint16_t) roundf(voltage / DAC12BITVOLT_1LSB);
        break;
    default:
        return AD5940ERR_PARA;
    }
    if(bits > 0xFFF) return AD5940ERR_PARA;
    *lpdac_dat_12_bits = bits;
    return AD5940ERR_OK;
}

AD5940Err AD5940_convert_lpdac_dat_6_bits_to_voltage(
    const uint16_t lpdac_dat_6_bits,
    float *const voltage
)
{
    if(lpdac_dat_6_bits > 0x3F) return AD5940ERR_PARA;
    *voltage = lpdac_dat_6_bits * DAC6BITVOLT_1LSB;
    return AD5940ERR_OK;
}

AD5940Err AD5940_convert_lpdac_dat_12_bits_to_voltage(
    const uint16_t lpdac_dat_12_bits,
    float *const voltage
)
{
    if(lpdac_dat_12_bits > 0xFFF) return AD5940ERR_PARA;
    *voltage = lpdac_dat_12_bits * DAC12BITVOLT_1LSB;
    return AD5940ERR_OK;
}

AD5940Err AD5940_combine_lpdac_dat_bits(
    const uint16_t lpdac_dat_6_bits,
    const uint16_t lpdac_dat_12_bits,
    uint32_t *const lpdac_dat_bits
)
{
    *lpdac_dat_bits = (((uint32_t) (lpdac_dat_6_bits)) << 12) | ((uint32_t) (lpdac_dat_12_bits));
    return AD5940ERR_OK;
}
