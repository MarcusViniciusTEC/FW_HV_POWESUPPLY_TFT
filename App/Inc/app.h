

#ifndef __APP_H_
#define __APP_H_


#include "adc_types.h"

uint32_t app_get_centivolts(adc_channels_t channel);
uint32_t app_get_miliamperes(adc_channels_t channel);
uint16_t app_get_temperature(adc_channels_temp_t ch);
uint32_t app_get_centiwatts(void);
void app_init(void);

#endif