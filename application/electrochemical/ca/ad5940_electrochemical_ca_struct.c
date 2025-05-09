#include "ad5940_electrochemical_ca_struct.h"

AD5940Err AD5940_ELECTROCHEMICAL_CA_PARAMETERS_check(
    const AD5940_ELECTROCHEMICAL_CA_PARAMETERS *const parameters
)
{
    if(parameters->e_dc == 0) return AD5940ERR_PARA;
    if(parameters->t_interval <= 0) return AD5940ERR_PARA;
    return AD5940ERR_OK;
}
