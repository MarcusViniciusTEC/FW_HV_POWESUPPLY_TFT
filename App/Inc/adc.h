#ifndef _ADC_H_
#define _ADC_H_


#include "adc_types.h"
#include "stdint.h"
#include "main.h"

/***********************************************************************************/

void adc_init(void);
uint32_t ADC_ADS1115_get_raw(adc_channels_t channel);

/***********************************************************************************/

#endif
