#ifndef _BUTTONS_TYPES_H_
#define _BUTTONS_TYPES_H_

#include "buttons_cfg.h"

typedef struct
{
    GPIO_TypeDef* GPIO;
    uint16_t pin;
}button_cfg_t;

typedef enum
{
    BUTTON_SHORT_PRESS = 1,
    BUTTON_LONG_PRESS,
    BUTTON_STOPED
}button_press_type_t;

typedef enum 
{
    BUTTON_OUT_STATE_ID = 0U,
    BUTTON_LEFT_ID,
    BUTTON_RIGHT_ID,
    BUTTON_ENC_ID,
    NUMBER_OF_BUTTONS,
}button_id_t;

typedef enum
{
    BUTTON_STATE_START = 0U,
    BUTTON_STATE_PRESSED ,
    BUTTON_STATE_WAITRELEASE

}button_pressed_state_t;

typedef struct 
{
    FlagStatus  state;
    FlagStatus  state_long_press;
    FlagStatus  state_previous;
    uint32_t    current_tick;
    uint32_t    previous_tick;
    uint32_t    long_press_tick;
    uint32_t    press_duration;
}button_update_t;

typedef struct 
{
    button_cfg_t cfg;
    void(*button_handler)(button_id_t, button_press_type_t);
    button_pressed_state_t state;
    button_id_t id;
    FlagStatus pressed_pin_state;
    uint32_t start_time;
}button_data_t;

#endif