 #pragma once

 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "ad5940.h"
 
/**
 * @brief Calculates the required 12-bit and 6-bit LPDAC data based on the input potential.
 *
 * @param[in]  potential          Target output potential (in volts).
 * @param[out] lpdac_data_6_bits  Pointer to store the calculated LPDAC data.
 * @param[out] lpdac_data_12_bits Pointer to store the calculated LPDAC data.
 *
 * @return AD5940Err              Returns an error code. Returns `AD5940_SUCCESS` if successful.
 *
 * @note This function converts the specified `potential` into corresponding digital 
 *       values for the Low Power Digital-to-Analog Converter (LPDAC) of the AD5940.
 *       The computed values are stored in `lpdac_dat_6_bits` and `lpdac_dat_12_bits`.
 * 
 *       Before calling this function, ensure that:
 *       - `LpDacVbiasMux` is set to `LPDACVBIAS_12BIT`
 *       - `LpDacVzeroMux` is set to `LPDACVZERO_6BIT`
 *
 *       Additionally, the provided pointers must be valid to prevent undefined behavior.
 */
AD5940Err AD5940_ELECTROCHEMICAL_calculate_lpdac_dat_6_12_bits_by_potential(
    const float potential, 
    uint16_t *const lpdac_dat_6_bits,
    uint16_t *const lpdac_dat_12_bits
);

/**
 * @brief Calculates the required 12-bit and 6-bit LPDAC data based on the input potential.
 *
 * @param[in]  potential          Target output potential (in volts).
 * @param[out] lpdac_data_bits    Pointer to store the calculated LPDAC data.
 *
 * @return AD5940Err              Returns an error code. Returns `AD5940_SUCCESS` if successful.
 *
 * @note This function converts the specified `potential` into corresponding digital 
 *       values for the Low Power Digital-to-Analog Converter (LPDAC) of the AD5940.
 *       The computed values are stored in `lpdac_dat_bits`.
 * 
 *       Before calling this function, ensure that:
 *       - `LpDacVbiasMux` is set to `LPDACVBIAS_12BIT`
 *       - `LpDacVzeroMux` is set to `LPDACVZERO_6BIT`
 *
 *       Additionally, the provided pointers must be valid to prevent undefined behavior.
 */
AD5940Err AD5940_ELECTROCHEMICAL_calculate_lpdac_dat_bits_by_potential(
    const float potential, 
    uint32_t *const lpdac_dat_bits
);

AD5940Err AD5940_ELECTROCHEMICAL_calculate_hsdac_dat_bits_by_potential(
    const float potential, 
    const uint32_t EXCTBUFGAIN, 
    const uint32_t HSDACGAIN, 
    uint32_t *const hsdac_dat_bits
);
 
 #ifdef __cplusplus
 }
 #endif
 