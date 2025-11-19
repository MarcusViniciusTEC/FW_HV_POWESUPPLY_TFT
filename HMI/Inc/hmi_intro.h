#ifndef __HMI_INTRO_H
#define __HMI_INTRO_H

#include "buttons_types.h"
#include "encoder_types.h"

void hmi_intro_init(void);
void hmi_intro_show_screen(void);
void hmi_intro_update_data(void);
void hmi_intro_update_button(button_id_t button_id, button_press_type_t button_press_type);
void hmi_intro_update_encoder(enc_state_t enc_state);

#define TIME_EXECUTION_INTRO 2000

#endif
