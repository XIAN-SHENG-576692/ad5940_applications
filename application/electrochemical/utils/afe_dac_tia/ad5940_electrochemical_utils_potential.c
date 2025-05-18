#include "ad5940_electrochemical_utils_potential.h"

#include "ad5940_utils.h"

AD5940Err AD5940_ELECTROCHEMICAL_calculate_lpdac_dat_6_12_bits_by_potential(
    const float potential, 
    uint16_t *const lpdac_dat_6_bits,
    uint16_t *const lpdac_dat_12_bits
)
{
    AD5940Err error;
    *lpdac_dat_6_bits = 0x20;
    float base_voltage;
    error = AD5940_convert_lpdac_dat_6_bits_to_voltage(
        *lpdac_dat_6_bits,
        &base_voltage
    );
    if(error) return error;

    error = AD5940_convert_voltage_to_lpdac_dat_12_bits(
        (base_voltage - potential),
        AD5940_LPDAC_ROUND_NEAREST,
        lpdac_dat_12_bits
    );
    if(error) return error;

    return AD5940ERR_OK;
}

AD5940Err AD5940_ELECTROCHEMICAL_calculate_lpdac_dat_bits_by_potential(
    const float potential, 
    uint32_t *const lpdac_dat_bits
)
{
    AD5940Err error;
    uint16_t lpdac_dat_6_bits;
    uint16_t lpdac_dat_12_bits;
    error = AD5940_ELECTROCHEMICAL_calculate_lpdac_dat_6_12_bits_by_potential(
        potential,
        &lpdac_dat_6_bits,
        &lpdac_dat_12_bits
    );
    if(error) return error;

    error = AD5940_combine_lpdac_dat_bits(
        lpdac_dat_6_bits,
        lpdac_dat_12_bits,
        lpdac_dat_bits
    );
    if(error) return error;

    return AD5940ERR_OK;
}

AD5940Err AD5940_ELECTROCHEMICAL_calculate_hsdac_dat_bits_by_potential(
    const float potential, 
    const uint32_t EXCTBUFGAIN, 
    const uint32_t HSDACGAIN, 
    uint32_t *const hsdac_dat_bits
)
{
    return AD5940_convert_voltage_to_hsdac_dat_bits(
        -potential,
        AD5940_HSDAC_ROUND_NEAREST,
        EXCTBUFGAIN,
        HSDACGAIN,
        hsdac_dat_bits
    );
}
