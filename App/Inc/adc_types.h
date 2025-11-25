#ifndef _ADC_TYPES_H_
#define _ADC_TYPES_H_


#define MEDIAN_SIZE 5
#define NUMBER_OF_SAMPLES 50

#include "stdint.h"


typedef struct {
    uint16_t window[MEDIAN_SIZE];
    uint8_t index;
} median_filter_t;


typedef struct {
    uint32_t raw_value;
} adc_ctrl_t;


typedef enum
{
    ADC_CH_CURRENT_OUT = 0U,
    ADC_CH_VOLTAGE,
    ADC_CH_CURRENT_IN,
    ADC_NUMBER_OF_CHANELS,
}adc_channels_t;

#endif
