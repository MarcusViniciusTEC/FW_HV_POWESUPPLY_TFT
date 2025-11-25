

#include "adc.h"

uint32_t app_get_centivolts(adc_channels_t channel)
{
    return (uint32_t)ADC_ADS1115_get_raw(channel)*1.2152777; // NEEDS CALIBRATION;
}

uint32_t app_get_miliamperes(adc_channels_t channel)
{
    uint32_t miliamps = (ADC_ADS1115_get_raw(channel)-389)*0.090025;

    if(miliamps > 3000)
    {
        miliamps = 0;
    }else
    {
        
    }

    return miliamps;
}

uint32_t app_get_centiwatts(void)
{
    return app_get_miliamperes(ADC_CH_CURRENT_OUT)* app_get_centivolts(ADC_CH_VOLTAGE);
}