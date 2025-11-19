#include "encoder.h"
#include "encoder_types.h"
#include "stdbool.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/***********************************************************************************/

 static enc_ctrl_t  enc_ctrl = {0};

/***********************************************************************************/

void encoder_thread(void const *pvParameters);
void encoder_update(void);

/***********************************************************************************/

void encoder_init(void)
{
    enc_ctrl.state = ENC_STATE_STOPED;
    enc_ctrl.event = EVENT_STOPED;
    TaskHandle_t xHandle = NULL;
    xTaskCreate((TaskFunction_t)encoder_thread,     /* Function that implements the task. */
                    "ENCODER",                      /* Text name for the task. */
                    128	,                           /* Stack size in words, not bytes. */
                    NULL,                           /* Parameter passed into the task. */
                    osPriorityNormal,               /* Priority at which the task is created. */
                    &xHandle );                     /* Used to pass out the created task's handle. */
}

/***********************************************************************************/

void encoder_update(void)
{
    static uint32_t current_state = 0;
    static uint32_t last_state = 0;
    current_state = (uint32_t)((TIM2->CNT)/4); 
    if(current_state > last_state)
    {
        enc_ctrl.event = EVENT_RUN;
        enc_ctrl.state = ENC_STATE_CCW;
        last_state = current_state;
    }
    else if(current_state < last_state)
    {
        enc_ctrl.event = EVENT_RUN;
        enc_ctrl.state = ENC_STATE_CW;
        last_state = current_state;
    }
}

/***********************************************************************************/

void encoder_thread(void const *pvParameter)
{
    for(;;)
    {
        encoder_update();
        vTaskDelay(5);
    }
}

/***********************************************************************************/

void read_encoder_state(encoder_data_t *encoder_data)
{
    if(enc_ctrl.event == EVENT_RUN)
    {
        if(enc_ctrl.state == ENC_STATE_CW)
        {
            encoder_data->encoder_event(ENC_STATE_CW);
            enc_ctrl.event = EVENT_STOPED;
        }

        if(enc_ctrl.state == ENC_STATE_CCW)
        {
            encoder_data->encoder_event(ENC_STATE_CCW);
            enc_ctrl.event = EVENT_STOPED;
        }
    }
    else
    {
        enc_ctrl.event =  EVENT_STOPED;
        enc_ctrl.state =  ENC_STATE_STOPED;
    }
}

