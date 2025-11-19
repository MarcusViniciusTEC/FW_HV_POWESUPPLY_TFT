
#include "buttons.h"

/***********************************************************************************/

void read_buttons_state(button_data_t *button_event)
{
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(button_event->cfg.GPIO, button_event->cfg.pin);
    uint32_t current_time = HAL_GetTick();

    if(button_event->state == BUTTON_STATE_START)
    {
        if(pin_state == GPIO_PIN_RESET)
        {
           button_event->state =  BUTTON_STATE_PRESSED;
           button_event->start_time = current_time;

            if(button_event->button_handler)
            {
                button_event->button_handler(button_event->id, BUTTON_SHORT_PRESS);
            }
        }
    }
    if(button_event->state ==  BUTTON_STATE_PRESSED)
    {
        if(pin_state == GPIO_PIN_RESET)
        {
            if(current_time > (button_event->start_time + 1000))
            {
            if(button_event->button_handler)
                {
                    button_event->button_handler(button_event->id, BUTTON_LONG_PRESS);
                }
                button_event->state = BUTTON_STATE_WAITRELEASE;
            }
        }
        else
        {
            button_event->state = BUTTON_STATE_START;              
        }
    }
    if(button_event->state == BUTTON_STATE_WAITRELEASE )
    {
        button_event->state = BUTTON_STATE_START;
    }
}
