#ifndef _ADC_H_
#define _ADC_H_


#include "main.h"
#include "dac_types.h"

void DAC_MCP4725_init(void);
void DAC_MCP4725_set(dac_channels_t channel, uint16_t value);


#endif
