
#include "adc.h"
#include "adc_types.h"
#include "ADS1015_ADS1115.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#define NUMBER_OF_MODULES_ADC 1

/***********************************************************************************/

ADS1xx5_I2C adc[NUMBER_OF_MODULES_ADC];

/***********************************************************************************/

void adc_thread(void const *pvParameters);

static median_filter_t mendian_filter[ADC_NUMBER_OF_CHANELS] = {0};

static adc_ctrl_t adc_ctrl[ADC_NUMBER_OF_CHANELS] = {0};


/***********************************************************************************/

void adc_init(void);
void ADC_ADS1115_init(void);
uint32_t ADC_ADS1115_get_raw(adc_channels_t channel);



/***********************************************************************************/

void adc_init(void)
{
    ADC_ADS1115_init();

    adc_ctrl[ADC_CH_VOLTAGE].calc_status = CALC_NOT_READY;

    TaskHandle_t xHandle = NULL;
    if(xTaskCreate((TaskFunction_t)adc_thread,         
                    "ADC",                         
                    256	,                                   
                    NULL,                             
                    osPriorityRealtime ,                        
                    &xHandle )!= pdPASS)                     
                    {

                    }
                    else
                    {
                        HAL_GPIO_WritePin(LED_BOARD_GPIO_Port, LED_BOARD_Pin, 1);
                    }
}

/***********************************************************************************/

void ADC_ADS1115_init(void)
{
    for (uint8_t index = 0; index < NUMBER_OF_MODULES_ADC; index++)
    {
        ADS1115(&adc[index], &hi2c1, ADS_ADDR_GND);
        ADSsetGain(&adc[index], GAIN_ONE);
    }
}

/***********************************************************************************/


uint32_t ADC_ADS1115_get_raw(adc_channels_t channel)
{
    if(adc_ctrl[channel].calc_status == CALC_READY)
    {
       return adc_ctrl[ADC_CH_VOLTAGE].raw_value;
       adc_ctrl[channel].calc_status = CALC_NOT_READY;
    }

}



/***********************************************************************************/

uint32_t get_median_filter(median_filter_t *f, uint16_t new_sample, adc_channels_t channel)
{
    adc_ctrl[channel].calc_status = CALC_NOT_READY;

    f->window[f->index] = new_sample;

    f->index++;
    if (f->index >= MEDIAN_SIZE)
        f->index = 0;

    uint16_t temp[MEDIAN_SIZE];
    for (uint8_t i = 0; i < MEDIAN_SIZE; i++)
        temp[i] = f->window[i];

    for (uint8_t i = 0; i < MEDIAN_SIZE - 1; i++)
        for (uint8_t j = i + 1; j < MEDIAN_SIZE; j++)
            if (temp[j] < temp[i]) {
                uint16_t t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
    
    adc_ctrl[channel].calc_status = CALC_READY;
    return temp[MEDIAN_SIZE / 2];
    
}

/***********************************************************************************/

void adc_thread(void const *pvParameters)
{
    for(;;)
    {
        adc_ctrl[ADC_CH_VOLTAGE].raw_value = get_median_filter(&mendian_filter[ADC_CH_VOLTAGE],  ADSreadADC_SingleEnded(&adc[0], ADC_CH_VOLTAGE), ADC_CH_VOLTAGE);
        vTaskDelay(100);
    }
    
}

/***********************************************************************************/