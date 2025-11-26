#ifndef __HMI_CONST_CURRENT_TYPES_H
#define __HMI_CONST_CURRENT_TYPES_H

#include "hmi_dashboard_cfg.h"
#include "stdint.h"

typedef enum
{
    OUT_ON = 0U,
    OUT_OFF
}hmi_out_state_t;

typedef enum
{
    INDEX_FIRST_DIGIT = 0U,
    INDEX_SECOND_DIGIT,
    INDEX_THIRD_DIGIT,
    INDEX_FOURTH_DIGIT,
    NUMBER_OF_INDEX_DIGITS
}hmi_digit_types_t;

typedef enum
{
    MODE_CONSTANT_CURRENT = 0U,
    MODE_CONSTANT_VOLTAGE,
    NUMBER_OF_MODES
}hmi_dashboard_mode_t;

typedef struct
{
    uint8_t value[NUMBER_OF_INDEX_DIGITS];
}hmi_edit_t;

typedef enum
{
    DISPLAY_NOT_UPDTATING_EVENT= 0U,
    DISPLAY_UPDATING_EVENT,
    DISPLAY_UPDATING_DATA
}hmi_dashboard_update_display_stataus_t;

typedef struct 
{
    hmi_dashboard_mode_t mode;
    uint8_t index_field;
    uint8_t last_index_field;
    hmi_out_state_t out_state;
    hmi_dashboard_update_display_stataus_t display_update;
}hmi_dashboard_ctrl_t;

typedef struct 
{
    uint16_t x;
    uint16_t y;
    uint8_t last_x;
}hmi_cursor_cordinate_t;

typedef struct
{
    char *tittle;
    char *symbol;
    uint16_t color;
}hmi_dashboard_title_t;





#endif