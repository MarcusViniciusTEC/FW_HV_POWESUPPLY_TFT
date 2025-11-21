#ifndef _ADC_TYPES_H_
#define _ADC_TYPES_H_


#define MEDIAN_SIZE 5

#include "stdint.h"

typedef enum
{
    CALC_READY = 0U,
    CALC_NOT_READY
}adc_median_cal_status_t;

typedef struct {
    uint16_t window[MEDIAN_SIZE];
    uint8_t index;
} median_filter_t;


typedef struct {
    adc_median_cal_status_t calc_status;
    uint32_t raw_value;


} adc_ctrl_t;


typedef enum
{
    ADC_CH_CURRENT_IN = 0U,
    ADC_CH_VOLTAGE,
    ADC_CH_CURRENT_OUT,
    ADC_NUMBER_OF_CHANELS,
}adc_channels_t;

#endif
