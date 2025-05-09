#include "ad5940_utils_adc.h"

static const uint32_t dft_table[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
static const uint32_t sinc2osr_table[] = {22,44,89,178,267,533,640,667,800,889,1067,1333,0};
static const uint32_t sinc3osr_table[] = {5,4,2,0};

AD5940Err AD5940_get_calibration_frequency(
    const float adc_clock_frequency,
    const uint32_t Sample_period,
    const uint32_t DFTNUM_Const,
    const uint32_t ADCSINC2OSR_Const,
    const uint32_t ADCSINC3OSR_Const,
    float *const calibration_frequency
)
{
    AD5940Err error = AD5940ERR_OK;

    uint16_t DFTNUM;
    uint16_t ADCSinc2Osr;
    uint16_t ADCSinc3Osr;
    error = AD5940_map_DFTNUM(
        DFTNUM_Const, 
        &DFTNUM
    );
    if(error != AD5940ERR_OK) return error;
    error = AD5940_map_ADCSinc2Osr(
        ADCSINC2OSR_Const, 
        &ADCSinc2Osr
    );
    if(error != AD5940ERR_OK) return error;
    error = AD5940_map_ADCSinc3Osr(
        ADCSINC3OSR_Const, 
        &ADCSinc3Osr
    );
    if(error != AD5940ERR_OK) return error;

    *calibration_frequency = adc_clock_frequency / DFTNUM / ADCSinc2Osr / ADCSinc3Osr * Sample_period;
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_DFTNUM(
    const uint32_t DFTNUM_Const, 
    uint16_t *const DFTNUM
)
{
    if(DFTNUM_Const > 12) return AD5940ERR_PARA;
    *DFTNUM = dft_table[DFTNUM_Const];
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_ADCSinc2Osr(
    const uint32_t ADCSINC2OSR_Const, 
    uint16_t *const ADCSinc2Osr
)
{
    if(!IS_ADCSINC2OSR(ADCSINC2OSR_Const)) return AD5940ERR_PARA;
    *ADCSinc2Osr = sinc2osr_table[ADCSINC2OSR_Const];
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_ADCSinc3Osr(
    const uint32_t ADCSINC3OSR_Const, 
    uint16_t *const ADCSinc3Osr
)
{
    if(!IS_ADCSINC3OSR(ADCSINC3OSR_Const)) return AD5940ERR_PARA;
    *ADCSinc3Osr = sinc3osr_table[ADCSINC3OSR_Const];
    return AD5940ERR_OK;
}

AD5940Err AD5940_map_ADCPGA(
    const uint32_t ADCPGA_const,
    float *const ADCPga_float
)
{
    switch (ADCPGA_const)
    {
    case ADCPGA_1:
        *ADCPga_float = 1.0f;
        return AD5940ERR_OK;
    
    case ADCPGA_1P5:
        *ADCPga_float = 1.5f;
        return AD5940ERR_OK;
    
    case ADCPGA_2:
        *ADCPga_float = 2.0f;
        return AD5940ERR_OK;
    
    case ADCPGA_4:
        *ADCPga_float = 4.0f;
        return AD5940ERR_OK;
    
    case ADCPGA_9:
        *ADCPga_float = 9.0f;
        return AD5940ERR_OK;

    default:
        break;
    }
    return AD5940ERR_PARA;
}

AD5940Err AD5940_convert_adc_to_current(
    const uint32_t adc_data,
    const fImpPol_Type *const RtiaCalValue,
    const uint32_t ADCPga,
    const float VRef1p82,
    int32_t *const current
)
{
    if(!IS_ADCPGA(ADCPga)) return AD5940ERR_PARA;
    *current = (int32_t) (
        (
            AD5940_ADCCode2Volt(
                adc_data & 0xFFFF, 
                ADCPga, 
                VRef1p82
            ) 
            / RtiaCalValue->Magnitude
        )
        * 1e9f
    );
    return AD5940ERR_OK;
}

AD5940Err AD5940_convert_adc_to_temperature(
    const uint32_t adc_data, 
    const uint32_t ADCPGA_Const,
    int32_t *const temperature
)
{
    #define K 8.13f
    float adcpga_float;
    AD5940Err error = AD5940_map_ADCPGA(
        ADCPGA_Const,
        &adcpga_float
    );
    if(error != AD5940ERR_OK) return AD5940ERR_PARA;
    *temperature = adc_data & 0xffff;
    *temperature -= 0x8000;	// data from SINC2 is added 0x8000, while data from register TEMPSENSDAT has no 0x8000 offset.
    *temperature = (*temperature / K / adcpga_float - 273.15f) * 1e6f;
    return AD5940ERR_OK;
}
