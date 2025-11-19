#include "hmi_menu.h"
#include "hmi_menu_types.h"

#include <stdbool.h>
#include <stdio.h>

#include "st7789.h"
#include "fonts.h"
#include "hmi.h"

#include "main.h"
#include "ADS1015_ADS1115.h"

#include "hmi_dashboard_types.h"

ADS1xx5_I2C i2c_adc;

/***********************************************************************************/

static hmi_edit_t hmi_edit[NUMBER_OF_MODES] = {0};
static hmi_dashboard_ctrl_t hmi_dashboard_ctrl = {0};
static hmi_cursor_cordinate_t hmi_cursor_cordinate[5] = cursor_cordinate_default;
hmi_dashboard_title_t hmi_dashboard_str[3] = hmi_dasboard_vector_tittle_default;

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void);
static void hmi_dashboard_show_power(void);
static void hmi_dashboard_show_current(void);
static void hmi_dashboard_show_temp(void);
static void hmi_dashboard_show_time(void);
static void hmi_dashboard_show_resistence(void);
static void hmi_dashboard_show_out_status(void);
static void hmi_dashboard_show_target(void);
static void hmi_dashboard_show_cursor(void);
static void hmi_dashboard_increment_digit(void);
static void hmi_dashboard_decrement_digit(void);
static void hmi_dashboard_increment_field(void);
static void hmi_dashboard_decrement_field(void);
static void hmi_dashboard_out_state_toggle(void);
static void hmi_dashboard_exit(void);
hmi_out_state_t hmi_dashboard_get_out_state(void);
hmi_dashboard_mode_t hmi_dashboard_get_mode(void);

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


static void hmi_dashboard_show_current(void)
{
    ST7789_WriteString(5, 30, "Read:",Font_11x18, LIGHTBLUE, BLACK);
    ST7789_WriteString(5, 50, "00.000 A",Font_16x26, GREEN, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void)
{
    ST7789_WriteString(5, 80, "23.100 V", Font_16x26, RED, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_power(void)
{
    ST7789_WriteString(5, 110, "00.000 W", Font_16x26, YELLOW, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_resistence(void)
{
    // char sz_string[20] = {0};
    // snprintf(sz_string, sizeof(sz_string), "%d%d.%d%d%d",
    // (ADSreadADC_SingleEnded(&i2c_adc, 0)/10000),
    // (ADSreadADC_SingleEnded(&i2c_adc, 0)/1000) % 10,
    // (ADSreadADC_SingleEnded(&i2c_adc, 0)/100) % 10,
    // (ADSreadADC_SingleEnded(&i2c_adc, 0)/10) % 10,
    // (ADSreadADC_SingleEnded(&i2c_adc, 0) % 10));
    // //ST7789_WriteString(5, 140, "00.000 R",Font_16x26, GRAY, BLACK);   
    // ST7789_WriteString(5, 140, sz_string,Font_16x26, GRAY, BLACK);
}

/***********************************************************************************/

static void hmi_dashboard_show_temp(void)
{
    ST7789_WriteString(143, 110, "Temp: 30.5'C",Font_11x18, MAGENTA, BLACK);
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

static void hmi_dashboard_show_target(void)
{
    // ST7789_WriteString(143, 30, "SET:",Font_11x18, LIGHTBLUE, BLACK);
    // char sz_string[20] = {0};
    // snprintf(sz_string, sizeof(sz_string), "%u%u.%u%u %s",
    // hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT],
    // hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_SECOND_DIGIT],
    // hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_THIRD_DIGIT],
    // hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FOURTH_DIGIT],
    // hmi_dashboard_str[hmi_dashboard_ctrl.mode].symbol);
    // ST7789_WriteString(143, 50, sz_string, Font_16x26, hmi_dashboard_str[hmi_dashboard_ctrl.mode].color, BLACK);


    uint32_t value = 0;

    value  += hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT]*100;
    value  += hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_SECOND_DIGIT]*10;
    value  += hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_THIRD_DIGIT];

    ST7789_WriteString(143, 30, "SET:",Font_11x18, LIGHTBLUE, BLACK);
    char sz_string[20] = {0};
    snprintf(sz_string, sizeof(sz_string), "%d.%u", value, hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FOURTH_DIGIT]);

    ST7789_WriteString(143, 50, sz_string, Font_16x26, hmi_dashboard_str[hmi_dashboard_ctrl.mode].color, BLACK);
    

}

/***********************************************************************************/

static void hmi_dashboard_show_cursor(void)
{
    hmi_cursor_cordinate_t cursor;
    cursor = hmi_cursor_cordinate[hmi_dashboard_ctrl.index_field];
    ST7789_WriteString(cursor.x, cursor.y, "^",Font_11x18, GREEN, BLACK);

    if(hmi_dashboard_ctrl.index_field !=  hmi_dashboard_ctrl.last_index_field)
    {
        cursor = hmi_cursor_cordinate[hmi_dashboard_ctrl.last_index_field];
        ST7789_WriteString(cursor.x, cursor.y, " ",Font_11x18, GREEN, BLACK);

        hmi_dashboard_ctrl.last_index_field = hmi_dashboard_ctrl.index_field;
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
    if(hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT] >= 5)
    {
        hmi_edit[hmi_dashboard_ctrl.mode].value[INDEX_FIRST_DIGIT] = 5;  
        for(uint8_t index = INDEX_SECOND_DIGIT; index < NUMBER_OF_INDEX_DIGITS; index++)
        {
            hmi_edit[hmi_dashboard_ctrl.mode].value[index] =   MIN_DIGIT; 
        }   
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

void hmi_dashboard_init(void)
{
    hmi_dashboard_ctrl.index_field = INDEX_FIRST_DIGIT;
    hmi_dashboard_ctrl.last_index_field = hmi_dashboard_ctrl.index_field;
    hmi_dashboard_ctrl.out_state = OUT_OFF;

    // ADS1115(&i2c_adc, &hi2c1, ADS_ADDR_GND); // Or ADS1015(&i2c, &hi2c1, ADS_ADDR_GND);
    // ADSsetGain(&i2c_adc, GAIN_SIXTEEN);
}

/***********************************************************************************/

void hmi_dashboard_show_screen(void)
{
    ST7789_Fill_Color(BLACK);
    
    hmi_dashboard_ctrl.mode = (hmi_dashboard_mode_t)hmi_get_screen()-2;

    ST7789_WriteString(10, 1, "DASHBOARD", Font_16x26, YELLOW, BLACK);
    ST7789_DrawLine(0, 27, 318, 27, WHITE);
    ST7789_DrawLine(0, 180, 318, 180, WHITE);
    ST7789_WriteString(12, 200, "ABL Solutions Ind", Font_16x26, BLUE, BLACK);
    ST7789_DrawRectangle(0, 0, 318, 239, WHITE);
    ST7789_DrawLine(137, 27, 137, 180, WHITE);
    ST7789_DrawLine(137, 145, 318, 145, WHITE);

    hmi_dashboard_show_voltage();
    hmi_dashboard_show_power();
    hmi_dashboard_show_current();
   // hmi_dashboard_show_temp();
    //hmi_dashboard_show_time();
    hmi_dashboard_show_out_status();
    hmi_dashboard_show_target();
   // hmi_dashboard_show_resistence();
    hmi_dashboard_show_cursor();
}

/***********************************************************************************/

void hmi_dashboard_update_data(void)
{
    //hmi_dashboard_show_resistence();
   //hmi_dashboard_show_voltage();
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
    switch (button_id)
    {
    case BUTTON_LEFT_ID:
        hmi_dashboard_decrement_field();
        break;
    case BUTTON_RIGHT_ID:
        hmi_dashboard_increment_field();
        break;
    case BUTTON_OUT_STATE_ID:
        hmi_dashboard_out_state_toggle();
        hmi_dashboard_show_out_status();
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

    hmi_dashboard_show_cursor();

}

/***********************************************************************************/

void hmi_dashboard_update_encoder(enc_state_t enc_state)
{
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
    hmi_dashboard_show_target();
}

/***********************************************************************************/