#ifndef _HMI_MENU_H_
#define _HMI_MENU_H_

#include "buttons_types.h"
#include "encoder_types.h"

void hmi_menu_init(void);
void hmi_menu_show_screen(void);
void hmi_menu_update_data(void);
void hmi_menu_update_button(button_id_t button_id, button_press_type_t button_press_type);
void hmi_menu_update_encoder(enc_state_t enc_state);

#endif