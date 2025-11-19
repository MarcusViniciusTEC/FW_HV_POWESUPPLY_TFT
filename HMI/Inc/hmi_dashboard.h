#ifndef __HMI_CONST_CURRENT_H
#define __HMI_CONST_CURRENT_H

#include "buttons_types.h"
#include "encoder_types.h"

void hmi_dashboard_init(void);
void hmi_dashboard_show_screen(void);
void hmi_dashboard_update_data(void);
void hmi_dashboard_update_button(button_id_t button_id, button_press_type_t button_press_type);
void hmi_dashboard_update_encoder(enc_state_t enc_state);


#endif