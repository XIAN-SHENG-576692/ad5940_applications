#include "ad5940_utils_sequence_generator.h"

static uint32_t *_sequence_generator_buffer;
static uint16_t _sequence_generator_buffer_length;

AD5940Err AD5940_set_sequence_generator_buffer(
    uint32_t *const sequence_generator_buffer, 
    const uint16_t sequence_generator_buffer_length
)
{
    if(sequence_generator_buffer == 0) return AD5940ERR_PARA;
    if(sequence_generator_buffer_length == 0) return AD5940ERR_PARA;

    _sequence_generator_buffer = sequence_generator_buffer;
    _sequence_generator_buffer_length = sequence_generator_buffer_length;

    AD5940_SEQGenInit(
        _sequence_generator_buffer, 
        _sequence_generator_buffer_length
    );
    
    return AD5940ERR_OK;
}

AD5940Err AD5940_clear_sequence_generator_buffer(void)
{
    AD5940_SEQGenInit(
        _sequence_generator_buffer, 
        _sequence_generator_buffer_length
    );
    return AD5940ERR_OK;
}

AD5940Err AD5940_get_change_sequence_info_command(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen,
    uint32_t *const sequence_command
)
{
    switch (SEQID)
    {
    case SEQID_0:
        *sequence_command = SEQ_WR(
            REG_AFE_SEQ0INFO,
            (RegAddr << BITP_AFE_SEQ0INFO_ADDR) | (RegLen << BITP_AFE_SEQ0INFO_LEN)
        );
        break;
    case SEQID_1:
        *sequence_command = SEQ_WR(
            REG_AFE_SEQ1INFO,
            (RegAddr << BITP_AFE_SEQ1INFO_ADDR) | (RegLen << BITP_AFE_SEQ1INFO_LEN)
        );
        break;
    case SEQID_2:
        *sequence_command = SEQ_WR(
            REG_AFE_SEQ2INFO,
            (RegAddr << BITP_AFE_SEQ2INFO_ADDR) | (RegLen << BITP_AFE_SEQ2INFO_LEN)
        );
        break;
    case SEQID_3:
        *sequence_command = SEQ_WR(
            REG_AFE_SEQ3INFO,
            (RegAddr << BITP_AFE_SEQ3INFO_ADDR) | (RegLen << BITP_AFE_SEQ3INFO_LEN)
        );
        break;
    default:
        return AD5940ERR_PARA;
    }
    return AD5940ERR_OK;
}

AD5940Err AD5940_write_change_sequence_info_command(
    const uint8_t SEQID,
    const uint16_t RegAddr, 
    const uint16_t RegLen
)
{
    switch (SEQID)
    {
    case SEQID_0:
        AD5940_WriteReg(
            REG_AFE_SEQ0INFO,
            (RegAddr << BITP_AFE_SEQ0INFO_ADDR) | (RegLen << BITP_AFE_SEQ0INFO_LEN)
        );
        break;
    case SEQID_1:
        AD5940_WriteReg(
            REG_AFE_SEQ1INFO,
            (RegAddr << BITP_AFE_SEQ1INFO_ADDR) | (RegLen << BITP_AFE_SEQ1INFO_LEN)
        );
        break;
    case SEQID_2:
        AD5940_WriteReg(
            REG_AFE_SEQ2INFO,
            (RegAddr << BITP_AFE_SEQ2INFO_ADDR) | (RegLen << BITP_AFE_SEQ2INFO_LEN)
        );
        break;
    case SEQID_3:
        AD5940_WriteReg(
            REG_AFE_SEQ3INFO,
            (RegAddr << BITP_AFE_SEQ3INFO_ADDR) | (RegLen << BITP_AFE_SEQ3INFO_LEN)
        );
        break;
    default:
        return AD5940ERR_PARA;
    }
    return AD5940ERR_OK;
}
