#include "adc.h"
#include "adc.h"
#include "hmi_dashboard.h"
#include "dac.h"
#include "dac_types.h"


#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


void app_thread(void const *pvParameters);
void app_init(void);

/***********************************************************************************/

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

/***********************************************************************************/

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

uint16_t app_get_temperature(adc_channels_temp_t ch)
{
    float v = 0;
    float variable = 0;
    float temp = 0;
    variable = (3.3/ 30000.0);

    uint32_t adc = 0;
    adc = ADC_ADS1115_GET_Raw_temp(ch);

    v = adc *variable;

    float r = (3.3f / v - 1.0) * 9800.0;  

    float beta = 3950.0;
    float t0 = 298.15; 
    float r0 = 10000.0;

    float tempK = 1.0 / (1.0/t0 + (1.0f/beta) * logf(r / r0));
    temp = tempK - 273.15;
    return temp*100;
}

/***********************************************************************************/

uint32_t app_get_centiwatts(void)
{
    return app_get_miliamperes(ADC_CH_CURRENT_OUT)* app_get_centivolts(ADC_CH_VOLTAGE);
}

/***********************************************************************************/

void app_init(void)
{
    TaskHandle_t xHandle = NULL;
    if (xTaskCreate((TaskFunction_t)app_thread,
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

void app_thread(void const *pvParameters)
{
    for(;;)
    {
        uint32_t vset = hmi_dashboard_get_target_voltage();
        uint32_t iset = hmi_dashboard_get_target_current();
        uint16_t dac_value_voltage = (uint16_t)((vset*0.00095 / 3.300) * 4095.0f);

        uint16_t dac_value_current = (uint16_t)((iset*0.000026 / 3.3f) * 4095.0f);

       DAC_MCP4725_set(DAC_VOLTAGE, dac_value_voltage);
       DAC_MCP4725_set(DAC_CURRENT, dac_value_current);

        vTaskDelay(100);
    }
}