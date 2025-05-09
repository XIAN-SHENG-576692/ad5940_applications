#include "ad5940_electrochemical_dpv_struct.h"

AD5940Err AD5940_ELECTROCHEMICAL_DPV_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_DPV_PARAMETERS *const parameters
)
{
    AD5940Err err;
    float t_interval;
    if(parameters->e_step <= 0) return AD5940ERR_PARA;
    if(parameters->e_pulse <= 0) return AD5940ERR_PARA;
    if(parameters->t_pulse <= 0) return AD5940ERR_PARA;
    if(parameters->scan_rate <= 0) return AD5940ERR_PARA;
    err = AD5940_ELECTROCHEMICAL_DPV_get_t_interval(
        parameters,
        &t_interval
    );
    if(err) return err;
    if(parameters->t_pulse >= t_interval) return AD5940ERR_PARA;
    if(!AD5940_ELECTROCHEMICAL_DPV_is_INVERSION_OPTION(parameters->inversion_option)) return AD5940ERR_PARA;
    return AD5940ERR_OK;
}

AD5940Err AD5940_ELECTROCHEMICAL_DPV_get_t_interval(
    const AD5940_ELECTROCHEMICAL_DPV_PARAMETERS *const parameters,
    float *const t_interval
)
{
    if(parameters->e_step <= 0) return AD5940ERR_PARA;
    if(parameters->scan_rate <= 0) return AD5940ERR_PARA;
    *t_interval = parameters->e_step / parameters->scan_rate;
    return AD5940ERR_OK;
}
