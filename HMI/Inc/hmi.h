
#ifndef __HMI_H
#define __HMI_H

#include "buttons_types.h"
#include "encoder_types.h"
#include "hmi_types.h"

void hmi_init(void);
void hmi_set_screen(hmi_screens_id_t hmi_screens_id);
void hmi_showing_update_data(button_id_t button_id, button_press_type_t button_press_type);
hmi_screens_id_t hmi_get_screen(void);


#endif