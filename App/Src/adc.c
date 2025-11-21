
#include "adc.h"
#include "adc_types.h"
#include "ADS1015_ADS1115.h"


#define NUMBER_OF_MODULES_ADC  1

/***********************************************************************************/

ADS1xx5_I2C adc[NUMBER_OF_MODULES_ADC];

/***********************************************************************************/


/***********************************************************************************/

void ADC_ADS1115_init(void);
uint32_t ADC_ADS1115_get_raw(adc_channels_t channel);

/***********************************************************************************/

void ADC_ADS1115_init(void)
{
    for(uint8_t index  = 0; index < NUMBER_OF_MODULES_ADC; index++)
    {
        ADS1115(&adc[index], &hi2c1, ADS_ADDR_GND);
        ADSsetGain(&adc[index], GAIN_ONE);
    }
}

/***********************************************************************************/

uint32_t ADC_ADS1115_get_raw(adc_channels_t channel)
{

    HASH_AlgoMode_HASH
    uint32_t adc_filtered = 0;

    for(uint16_t samples = 0; samples < 15; samples++)
    {
        adc_filtered += ADSreadADC_SingleEnded(&adc[0], channel);
    }
    return (adc_filtered/15);

    
}