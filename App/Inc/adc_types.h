#ifndef _ADC_TYPES_H_
#define _ADC_TYPES_H_


#define MEDIAN_SIZE 5
#define NUMBER_OF_SAMPLES 50

#include "stdint.h"

typedef enum
{
    ADC_TEMP_CH_TRANSFORMER = 0U,
    ADC_TEMP_CH_HEATSINK,
    NUMBER_OF_SENSE_TEMP
}adc_channels_temp_t;

typedef enum
{
    ADC_RESISTOR_SET_DIVIDER = 0U,
    ADC_RESISTOR_RESET_DIVIDER,
}adc_res_divider_stauts_t;

typedef struct {
    uint16_t window[MEDIAN_SIZE];
    uint8_t index;
} median_filter_t;


typedef struct {
    uint32_t raw_value;
    uint32_t raw_value_temp[NUMBER_OF_SENSE_TEMP];
    adc_res_divider_stauts_t resistor_status;
} adc_ctrl_t;


typedef enum
{
    ADC_CH_CURRENT_OUT = 0U,
    ADC_CH_VOLTAGE,
    ADC_CH_CURRENT_IN,
    ADC_CH_TEMP,
    ADC_NUMBER_OF_CHANELS
}adc_channels_t;

#endif
