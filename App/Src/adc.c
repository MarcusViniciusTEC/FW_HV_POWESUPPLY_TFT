
#include "adc.h"
#include "adc_types.h"
#include "ADS1015_ADS1115.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#define NUMBER_OF_MODULES_ADC 1
#define MODULE_DEFAULT 0

/***********************************************************************************/

ADS1xx5_I2C adc;

/***********************************************************************************/

void adc_thread(void const *pvParameters);
static adc_ctrl_t adc_ctrl[ADC_NUMBER_OF_CHANELS] = {0};

static uint16_t window[ADC_NUMBER_OF_CHANELS][MEDIAN_SIZE] = {0};
static uint8_t index_adc[ADC_NUMBER_OF_CHANELS] = {0};

/***********************************************************************************/

void adc_init(void);
void ADC_ADS1115_init(void);
uint32_t ADC_ADS1115_get_raw(adc_channels_t channel);

/***********************************************************************************/

void adc_init(void)
{
    ADC_ADS1115_init();

    TaskHandle_t xHandle = NULL;
    if (xTaskCreate((TaskFunction_t)adc_thread,
                    "ADC",
                    512,
                    NULL,
                    osPriorityRealtime,
                    &xHandle) != pdPASS)
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
    ADS1115(&adc, &hi2c1, ADS_ADDR_GND);
    ADSsetGain(&adc, GAIN_ONE);
}

/***********************************************************************************/



uint32_t median_filter(uint8_t ch, uint16_t new_sample)
{
    window[ch][index_adc[ch]] = new_sample;

    index_adc[ch]++;
    if (index_adc[ch] >= MEDIAN_SIZE)
        index_adc[ch] = 0;

    // copia os dados para um array temporário
    uint16_t temp[MEDIAN_SIZE];
    for (uint8_t i = 0; i < MEDIAN_SIZE; i++)
        temp[i] = window[ch][i];

    // ordena (bubble sort simplificado)
    for (uint8_t i = 0; i < MEDIAN_SIZE - 1; i++)
        for (uint8_t j = i + 1; j < MEDIAN_SIZE; j++)
            if (temp[j] < temp[i]) {
                uint16_t t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }

    // retorna a mediana
    return temp[MEDIAN_SIZE / 2];

}

uint16_t ema_filter(adc_channels_t channel, uint16_t new_sample)
{
    static uint16_t filtered[ADC_NUMBER_OF_CHANELS] = {0};
    filtered[channel] = filtered[channel] + ((new_sample - filtered[channel]) / 2);
    return filtered[channel];
}

/***********************************************************************************/

uint16_t filter_adc(uint8_t channel)
{
    uint32_t adc_filtered = 0;

    for(uint16_t samples = 0; samples < NUMBER_OF_SAMPLES; samples++)
    {
        adc_filtered += ADSreadADC_SingleEnded(&adc, channel);
    }
    return (adc_filtered/NUMBER_OF_SAMPLES);
}

/***********************************************************************************/

uint32_t ADC_ADS1115_get_raw(adc_channels_t channel)
{
   return ema_filter(channel, adc_ctrl[channel].raw_value); 
}   


/***********************************************************************************/

void adc_thread(void const *pvParameters)
{
    for (;;)
    {
        for(uint8_t channel_index = 0; channel_index < ADC_NUMBER_OF_CHANELS; channel_index++)
        {  
            adc_ctrl[channel_index].raw_value = ADSreadADC_SingleEnded(&adc, channel_index);
        }      
        vTaskDelay(130);
    }
}

/***********************************************************************************/