#ifndef _DAC_TYPES_H_
#define _DAC_TYPES_H_


#include "MCP4725.h"

typedef enum
{
    DAC_CURRENT = 0U,
    DAC_VOLTAGE,
    DAC_NUMBER_OF_CHANELS,
}dac_channels_t;

typedef struct
{
    MCP4725 DAC[DAC_NUMBER_OF_CHANELS];
    dac_channels_t dac_channel;
}DAC_MCP4725_ctrl_t;

#endif
