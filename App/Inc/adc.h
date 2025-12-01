#ifndef _ADC_H_
#define _ADC_H_


#include "adc_types.h"
#include "stdint.h"
#include "main.h"

/***********************************************************************************/

void adc_init(void);
adc_res_divider_stauts_t adc_get_res_divider_status(void);
uint32_t ADC_ADS1115_get_raw(adc_channels_t channel);
uint32_t ADC_ADS1115_GET_Raw_temp(adc_channels_temp_t temp_channel);

/***********************************************************************************/

#endif
