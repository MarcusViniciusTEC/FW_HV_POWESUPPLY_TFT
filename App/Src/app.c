

#include "adc.h"
#include "adc.h"

uint32_t app_get_centivolts(adc_channels_t channel)
{
    uint32_t value = 0;
    switch (adc_get_res_divider_status())
    {
    case ADC_RESISTOR_RESET_DIVIDER:
        value = (uint32_t)ADC_ADS1115_get_raw(channel)*2.727272; // NEEDS CALIBRATION;
        break;
    case ADC_RESISTOR_SET_DIVIDER:
        value = (uint32_t)ADC_ADS1115_get_raw(channel)*2.7272; // NEEDS CALIBRATION;
        break;
    default:
        break;
    }
   return value;
}

uint32_t app_get_miliamperes(adc_channels_t channel)
{
    uint32_t miliamps = (ADC_ADS1115_get_raw(channel)-389)*0.17285;

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