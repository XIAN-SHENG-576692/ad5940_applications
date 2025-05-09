#include "ad5940_electrochemical_cv_struct.h"

AD5940Err AD5940_ELECTROCHEMICAL_CV_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_CV_PARAMETERS *const parameters
)
{
    if(parameters->e_begin > parameters->e_vertex1 && parameters->e_begin > parameters->e_vertex2) return AD5940ERR_PARA;
    if(parameters->e_begin < parameters->e_vertex1 && parameters->e_begin < parameters->e_vertex2) return AD5940ERR_PARA;
    if(parameters->e_step <= 0) return AD5940ERR_PARA;
    if(parameters->scan_rate <= 0) return AD5940ERR_PARA;
    return AD5940ERR_OK;
}

AD5940Err AD5940_ELECTROCHEMICAL_CV_get_t_interval(
    const AD5940_ELECTROCHEMICAL_CV_PARAMETERS *const parameters,
    float *const t_interval
)
{
    if(parameters->e_step <= 0) return AD5940ERR_PARA;
    if(parameters->scan_rate <= 0) return AD5940ERR_PARA;
    *t_interval = parameters->e_step / parameters->scan_rate;
    return AD5940ERR_OK;
}
