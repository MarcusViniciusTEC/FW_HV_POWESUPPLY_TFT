

#include "adc.h"

uint32_t app_get_centivolts(adc_channels_t channel)
{
    return ADC_ADS1115_get_raw(channel)*1.025641026; // NEEDS CALIBRATION;
}

uint32_t app_get_miliamperes(adc_channels_t channel)
{
    return ADC_ADS1115_get_raw(channel)*0.1136; // NEEDS CALIBRATION;
}

uint32_t app_get_centiwatts(void)
{
    return app_get_miliamperes(ADC_CH_CURRENT_OUT)* app_get_centivolts(ADC_CH_VOLTAGE);
}