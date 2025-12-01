#include "hmi_menu.h"
#include "hmi_menu_types.h"

#include <stdbool.h>
#include <stdio.h>

#include "st7789.h"
#include "fonts.h"
#include "hmi.h"

#include "main.h"
#include "adc.h"
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "hmi_dashboard_types.h"


/***********************************************************************************/

static hmi_edit_t hmi_edit[NUMBER_OF_MODES] = {0};
static hmi_dashboard_ctrl_t hmi_dashboard_ctrl = {0};
static hmi_cursor_cordinate_t hmi_voltage_cursor_cordinate[5] = cursor_voltage_cordinate_default;
static hmi_cursor_cordinate_t hmi_current_cursor_cordinate[5] = cursor_current_cordinate_default;
hmi_dashboard_title_t hmi_dashboard_str[3] = hmi_dasboard_vector_tittle_default;

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void);
static void hmi_dashboard_show_power(void);
static void hmi_dashboard_show_current(void);
static void hmi_dashboard_show_temp(void);
static void hmi_dashboard_show_time(void);
static void hmi_dashboard_show_out_status(void);
static void hmi_dashboard_show_target_voltage(void);
static void hmi_dashboard_show_target_current(void);
static void hmi_dashboard_show_cursor(void);
static void hmi_dashboard_show_current_in(void);
static void hmi_dashboard_increment_digit(void);
static void hmi_dashboard_decrement_digit(void);
static void hmi_dashboard_increment_field(void);
static void hmi_dashboard_decrement_field(void);
static void hmi_dashboard_out_state_toggle(void);
static void hmi_dashboard_exit(void);
static void hmi_dashboard_toggle_mode(void);
static void hmi_dashboard_clear_cursor_last_mode(void);
uint32_t hmi_dashboard_get_target_current(void);
uint32_t hmi_dashboard_get_target_voltage(void);
hmi_out_state_t hmi_dashboard_get_out_state(void);
hmi_dashboard_mode_t hmi_dashboard_get_mode(void);

/***********************************************************************************/

uint32_t hmi_dashboard_get_target_current(void)
{
    uint32_t t_current = 0;

    t_current += hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_FIRST_DIGIT    ]*1000;
    t_current += hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_SECOND_DIGIT   ]*100;
    t_current += hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_THIRD_DIGIT    ]*10;
    t_current += hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_FOURTH_DIGIT   ];
    return t_current;
}

/***********************************************************************************/

uint32_t hmi_dashboard_get_target_voltage(void)
{   
    uint32_t t_voltage = 0;

    t_voltage += hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_FIRST_DIGIT    ]*1000;
    t_voltage += hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_SECOND_DIGIT   ]*100;
    t_voltage += hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_THIRD_DIGIT    ]*10;
    t_voltage += hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_FOURTH_DIGIT   ];
    return t_voltage;
}

/***********************************************************************************/

hmi_dashboard_mode_t hmi_dashboard_get_mode(void)
{
    return hmi_dashboard_ctrl.mode;
}

/***********************************************************************************/

hmi_out_state_t hmi_dashboard_get_out_state(void)
{
    return hmi_dashboard_ctrl.out_state;
}

/***********************************************************************************/

static void hmi_dashboard_show_current_in(void)
{
    ST7789_WriteString(10, 190, "IN:4.00 A", Font_11x18, BLUE, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_current(void)
{
    ST7789_WriteString(5, 30, "Read:",Font_11x18, LIGHTBLUE, BLACK);

    uint32_t current = app_get_miliamperes(ADC_CH_CURRENT_OUT);
    char sz_string[28] = {0};

    snprintf(sz_string, sizeof(sz_string), "%02d.%03d",
    (int)(current / 1000),
    (int)(current % 1000));

    ST7789_WriteString(5, 80, sz_string, Font_16x26, YELLOW, BLACK);
    ST7789_WriteString(119, 80, "A", Font_16x26, YELLOW, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void)
{
    uint32_t voltage = app_get_centivolts(ADC_CH_VOLTAGE);
    char sz_string[28] = {0};
    snprintf(sz_string, sizeof(sz_string), "%03d.%02d",
             (int)(voltage / 100),
             (int)(voltage % 100));

    ST7789_WriteString(5, 50, sz_string, Font_16x26, GREEN, BLACK);
    ST7789_WriteString(119, 50, "V", Font_16x26, GREEN, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_power(void)
{
    uint32_t power = app_get_centiwatts();
    char sz_string[28] = {0};
    snprintf(sz_string, sizeof(sz_string), "%03d.%02d",
             (int)(power / 100000),
             (int)(power % 100));

    ST7789_WriteString(5, 110, sz_string, Font_16x26, CYAN, BLACK);
    ST7789_WriteString(119, 110, "W", Font_16x26, CYAN, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_temp(void)
{
    uint32_t temp_tranformer = app_get_temperature(ADC_TEMP_CH_TRANSFORMER);
    uint32_t temp_heatsink = app_get_temperature(ADC_TEMP_CH_HEATSINK);
    char sz_string[28] = {0};
    snprintf(sz_string, sizeof(sz_string), "SINK:%02d.%01d'C",
             (int)(temp_tranformer / 100),
             (int)(temp_tranformer % 10));
    ST7789_WriteString(185, 190, sz_string, Font_11x18, CYAN, BLACK);

    snprintf(sz_string, sizeof(sz_string), "TRFO:%02d.%01d'C",
             (int)(temp_heatsink/ 100),
             (int)(temp_heatsink % 10));
    ST7789_WriteString(185, 210, sz_string, Font_11x18, CYAN, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_time(void)
{
    ST7789_WriteString(143, 130, "Time: 10:23:12",Font_11x18, MAGENTA, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_out_state_toggle(void)
{
    hmi_dashboard_ctrl.out_state =! hmi_dashboard_ctrl.out_state;
}

/***********************************************************************************/

static void hmi_dashboard_show_out_status(void)
{
    ST7789_WriteString(143, 150, "OUT:",Font_16x26, MAGENTA, BLACK);
    switch (hmi_dashboard_ctrl.out_state)
    {
    case OUT_ON:
        ST7789_WriteString(215, 150, "ON! ",Font_16x26, GREEN, BLACK);
        break;
    case OUT_OFF:
        ST7789_WriteString(215, 150, "OFF!",Font_16x26, RED, BLACK);
        break;
    default:
        break;
    }
}

/***********************************************************************************/

static void hmi_dashboard_show_target_voltage(void)
{
    ST7789_WriteString(143, 30, "SET:",Font_11x18, LIGHTBLUE, BLACK);
    char sz_string[24] = {0};
    snprintf(sz_string, sizeof(sz_string), "%u%u%u.%u",
    hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_FIRST_DIGIT],
    hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_SECOND_DIGIT],
    hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_THIRD_DIGIT],
    hmi_edit[MODE_CONSTANT_VOLTAGE].value[INDEX_FOURTH_DIGIT]);

    ST7789_WriteString(143, 50, sz_string, Font_16x26, GREEN, BLACK);
    ST7789_WriteString(250, 50, "V", Font_16x26, GREEN, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_target_current(void)
{
    char sz_string[24] = {0};
    snprintf(sz_string, sizeof(sz_string), "%u.%u%u%u",
    hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_FIRST_DIGIT],
    hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_SECOND_DIGIT],
    hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_THIRD_DIGIT],
    hmi_edit[MODE_CONSTANT_CURRENT].value[INDEX_FOURTH_DIGIT]);

    ST7789_WriteString(143, 80, sz_string, Font_16x26, YELLOW, BLACK);
    ST7789_WriteString(250, 80, "A", Font_16x26, GREEN, BLACK);
    
}

/***********************************************************************************/

static void hmi_dashboard_show_cursor(void)
{

    hmi_cursor_cordinate_t cursor;
    switch (hmi_dashboard_ctrl.mode)
    {
    case MODE_CONSTANT_VOLTAGE:
        ST7789_WriteString(300, 85, " ", Font_11x18, GREEN, BLACK);
        ST7789_WriteString(300, 55, "<", Font_11x18, GREEN, BLACK);

        cursor = hmi_voltage_cursor_cordinate[hmi_dashboard_ctrl.index_field];
        ST7789_WriteString(cursor.x, cursor.y, "^", Font_11x18, GREEN, BLACK);

        if (hmi_dashboard_ctrl.index_field != hmi_dashboard_ctrl.last_index_field)
        {
            cursor = hmi_voltage_cursor_cordinate[hmi_dashboard_ctrl.last_index_field];
            ST7789_WriteString(cursor.x, cursor.y, " ", Font_11x18, GREEN, BLACK);
            hmi_dashboard_ctrl.last_index_field = hmi_dashboard_ctrl.index_field;
        }

        break;
    case MODE_CONSTANT_CURRENT:
        ST7789_WriteString(300, 55, " ", Font_11x18, GREEN, BLACK);
        ST7789_WriteString(300, 85, "<", Font_11x18, GREEN, BLACK);

        cursor = hmi_current_cursor_cordinate[hmi_dashboard_ctrl.index_field];
        ST7789_WriteString(cursor.x, cursor.y, "^", Font_11x18, GREEN, BLACK);

        if (hmi_dashboard_ctrl.index_field != hmi_dashboard_ctrl.last_index_field)
        {
            cursor = hmi_current_cursor_cordinate[hmi_dashboard_ctrl.last_index_field];
            ST7789_WriteString(cursor.x, cursor.y, " ", Font_11x18, GREEN, BLACK);
            hmi_dashboard_ctrl.last_index_field = hmi_dashboard_ctrl.index_field;
        }
        break;

    default:
        break;
    }
}

static void hmi_dashboard_clear_cursor_last_mode(void)
{
    
     hmi_cursor_cordinate_t cursor;

    switch (hmi_dashboard_ctrl.mode)
    {
    case MODE_CONSTANT_CURRENT:
        for (uint8_t index = 0; index < 5; index++)
        {
            cursor = hmi_voltage_cursor_cordinate[index];
            ST7789_WriteString(cursor.x, cursor.y, " ", Font_11x18, GREEN, BLACK);
        }
        break;
    case MODE_CONSTANT_VOLTAGE:
        for (uint8_t index = 0; index < 5; index++)
        {
            cursor = hmi_current_cursor_cordinate[index];
            ST7789_WriteString(cursor.x, cursor.y, " ", Font_11x18, GREEN, BLACK);
        }
        break;

    default:
        break;
    }

}

/***********************************************************************************/
static void hmi_dashboard_increment_field(void)
{
    hmi_dashboard_ctrl.index_field++;
    if(hmi_dashboard_ctrl.index_field > INDEX_THIRD_DIGIT)
    {
        hmi_dashboard_ctrl.index_field =  INDEX_FOURTH_DIGIT;
    }
}

/***********************************************************************************/

static void hmi_dashboard_decrement_field(void)
{
    hmi_dashboard_ctrl.index_field--;
    if(hmi_dashboard_ctrl.index_field > INDEX_FOURTH_DIGIT)
    {
        hmi_dashboard_ctrl.index_field = INDEX_FIRST_DIGIT;
    }
}

/***********************************************************************************/

static void hmi_dashboard_increment_digit(void)
{
    hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field]++;
    if(hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] > MAX_DIGIT)
    {
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] = MIN_DIGIT;
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field - 1] = hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field -1] +1;  
        if(hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field -1] > MAX_DIGIT)
        {
            hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field-1] = MIN_DIGIT;
            hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field - 2] = hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field -2] +1;   
        }
    }

    switch (hmi_dashboard_ctrl.mode)
    {
    case MODE_CONSTANT_VOLTAGE:
        if (hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT] >= 5)
        {
            hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT] = 5;
            for (uint8_t index = INDEX_SECOND_DIGIT; index < NUMBER_OF_INDEX_DIGITS; index++)
            {
                hmi_edit[hmi_dashboard_ctrl.mode].value[index] = MIN_DIGIT;
            }
        }
        break;
    case MODE_CONSTANT_CURRENT:
        if (hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT] >= 2)
        {
            hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT] = 2;
            for (uint8_t index = INDEX_SECOND_DIGIT; index < NUMBER_OF_INDEX_DIGITS; index++)
            {
                hmi_edit[hmi_dashboard_ctrl.mode].value[index] = MIN_DIGIT;
            }
        }
        break;
    default:
        break;
    }
}

/***********************************************************************************/

static void hmi_dashboard_decrement_digit(void)
{
    hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field]--;
    if(hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] > MAX_DIGIT && hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field - 1] > 0)
    {
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] = MAX_DIGIT;
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field-1] = hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field-1] - 1;
    }
    if(hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] > MAX_DIGIT && hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field - 2] > 0)
    {
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field - 2] = hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field - 2] -1;
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] = MAX_DIGIT;
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field-1] = MAX_DIGIT;
    }
    if(hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] > MAX_DIGIT)
    {
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] = MIN_DIGIT;
    }    
    if(hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] > 4)
    {
        hmi_edit[hmi_dashboard_ctrl.mode].value[hmi_dashboard_ctrl.index_field] = 4;
    }
}

/***********************************************************************************/

void hmi_dashboard_toggle_mode(void)
{
    switch (hmi_dashboard_ctrl.mode)
    {
    case MODE_CONSTANT_CURRENT:
        hmi_dashboard_ctrl.mode = MODE_CONSTANT_VOLTAGE;
        break;
    case MODE_CONSTANT_VOLTAGE:
        hmi_dashboard_ctrl.mode = MODE_CONSTANT_CURRENT;
        break;
    default:
        break;
    }
    hmi_dashboard_clear_cursor_last_mode();
}

/***********************************************************************************/

void hmi_dashboard_init(void)
{
    hmi_dashboard_ctrl.index_field = INDEX_FIRST_DIGIT;
    hmi_dashboard_ctrl.last_index_field = hmi_dashboard_ctrl.index_field;
    hmi_dashboard_ctrl.out_state = OUT_OFF;
    hmi_dashboard_ctrl.display_update = DISPLAY_UPDATING_DATA;

    hmi_dashboard_ctrl.mode = MODE_CONSTANT_CURRENT;
}

/***********************************************************************************/

void hmi_dashboard_show_screen(void)
{
    ST7789_Fill_Color(BLACK);
    ST7789_WriteString(10, 1, "-----DASHBOARD----", Font_16x26, YELLOW, BLACK);
    
    ST7789_DrawLine(0, 27, 318, 27, WHITE);
    ST7789_DrawLine(0, 180, 318, 180, WHITE);
    ST7789_DrawRectangle(0, 0, 318, 239, WHITE);
    ST7789_DrawLine(137, 27, 137, 180, WHITE);
    ST7789_DrawLine(137, 145, 318, 145, WHITE);

    hmi_dashboard_show_out_status();
    hmi_dashboard_show_target_voltage();
    hmi_dashboard_show_target_current();
    hmi_dashboard_show_current_in();
    hmi_dashboard_show_cursor();
    hmi_dashboard_show_temp();
}

/***********************************************************************************/

void hmi_dashboard_update_data(void)
{
    switch (hmi_dashboard_ctrl.display_update)
    {
    case DISPLAY_NOT_UPDTATING_EVENT:
        break;
    case DISPLAY_UPDATING_EVENT:
        hmi_dashboard_show_out_status();
        hmi_dashboard_show_target_voltage();
        hmi_dashboard_show_target_current();
        hmi_dashboard_show_cursor();
        hmi_dashboard_ctrl.display_update = DISPLAY_UPDATING_DATA;
        break;
    case DISPLAY_UPDATING_DATA:
        hmi_dashboard_show_voltage();
        hmi_dashboard_show_current();
        hmi_dashboard_show_power();
        hmi_dashboard_show_temp();
    default:
        break;
    }
}

/***********************************************************************************/

void hmi_dashboard_exit(void)
{
    hmi_dashboard_ctrl.index_field = INDEX_FIRST_DIGIT;
    hmi_dashboard_ctrl.last_index_field = hmi_dashboard_ctrl.index_field;
    hmi_dashboard_ctrl.out_state = OUT_OFF;
}

/***********************************************************************************/

void hmi_dashboard_update_button(button_id_t button_id, button_press_type_t button_press_type)
{
    hmi_dashboard_ctrl.display_update = DISPLAY_NOT_UPDTATING_EVENT;

    switch (button_id)
    {
    case BUTTON_LEFT_ID:
        hmi_dashboard_decrement_field();
        break;
    case BUTTON_RIGHT_ID:
        hmi_dashboard_increment_field();
        break;
    case BUTTON_OUT_STATE_ID:
        hmi_dashboard_toggle_mode();
        break;
    case BUTTON_ENC_ID:
        switch (button_press_type)
        {
        case BUTTON_SHORT_PRESS:
            hmi_dashboard_exit();
            hmi_set_screen(HMI_ID_SCREEN_MENU);
            break;
        case BUTTON_LONG_PRESS:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    hmi_dashboard_ctrl.display_update = DISPLAY_UPDATING_EVENT;
}

/***********************************************************************************/

void hmi_dashboard_update_encoder(enc_state_t enc_state)
{
    hmi_dashboard_ctrl.display_update = DISPLAY_NOT_UPDTATING_EVENT;

    switch (enc_state)
    {
    case ENC_STATE_CCW:
        hmi_dashboard_increment_digit();
        break;
    case ENC_STATE_CW:
        hmi_dashboard_decrement_digit();
        break;
    default:
        break;
    }
    hmi_dashboard_ctrl.display_update = DISPLAY_UPDATING_EVENT;
}

/***********************************************************************************/