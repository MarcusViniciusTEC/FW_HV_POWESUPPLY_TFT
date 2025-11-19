
#ifndef __HMI_TYPES_H
#define __HMI_TYPES_H

#include "hmi_cfg.h"
#include "buttons_types.h"
#include "encoder_types.h"

typedef enum
{
    HMI_ID_SCREEN_INTRO = 0U,
    HMI_ID_SCREEN_MENU,
    HMI_ID_SCREEN_DASHBOARD,
    HMI_NUMBERS_OF_ID
}hmi_screens_id_t;

typedef struct 
{
    hmi_screens_id_t screen_id_info;
    void(*init)(void);
    void(*show_screen)(void);
    void(*show_data)(void);
    void(*update_data_buttons)(button_id_t button_id, button_press_type_t button_press_type);
    void(*update_data_encoder)(enc_state_t enc_state);

}hmi_screen_info_t;

typedef enum
{
    HMI_SHOWING_SCREEN = 0U,
    HMI_SHOWING_DATA,
    HMI_SHOWING_UPDATE_DATA
}hmi_screens_update_state_t;


typedef struct 
{
    hmi_screens_id_t screen_id;
    hmi_screens_id_t last_screen_id;
    hmi_screens_id_t next_screen_id;
    hmi_screens_update_state_t state;
    
}hmi_ctrl_t;


#endif
