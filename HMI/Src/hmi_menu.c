
#include "hmi_menu.h"
#include "hmi_menu_types.h"

#include <stdbool.h>
#include <stdio.h>

#include "st7789.h"
#include "fonts.h"
#include "hmi.h"

/***********************************************************************************/

static const hmi_menu_data_t hmi_menu_item_vector[HMI_MENU_NUMBER_OF_ITEMS] = hmi_menu_vector_item_default;

static hmi_menu_ctrl_t hmi_menu_ctrl = {0};

/***********************************************************************************/

static void hmi_menu_show_menu_itens(void);
static void hmi_menu_show_menu_cursor(void);
static void hmi_menu_increment_item_index(void);
static void hmi_menu_decrement_item_index(void);



/***********************************************************************************/

void hmi_menu_init(void)
{
    hmi_menu_ctrl.submenu_state = SUBMENU_OFF;
    
}

/***********************************************************************************/

static void hmi_menu_increment_item_index(void)
{
    
    hmi_menu_ctrl.item_index++;
    if(hmi_menu_ctrl.item_index > HMI_MENU_ID_SETTINGS)
    {
        hmi_menu_ctrl.item_index = HMI_MENU_ID_SETTINGS;
    }
}

/***********************************************************************************/

static void hmi_menu_decrement_item_index(void)
{
    hmi_menu_ctrl.item_index--;
    if(hmi_menu_ctrl.item_index > HMI_MENU_ID_SETTINGS)
    {
        hmi_menu_ctrl.item_index = HMI_MENU_ID_CONSTANT_CURRENT;
    }
}

/***********************************************************************************/
static void hmi_menu_show_menu_cursor(void)
{
    ST7789_WriteString(10, (60 + (hmi_menu_ctrl.item_index*20)), HMI_CURSOR, Font_11x18, YELLOW, BLACK);
    if(hmi_menu_ctrl.item_index > HMI_MENU_ID_CONSTANT_CURRENT)
    {
        ST7789_WriteString(10, (40 + (hmi_menu_ctrl.item_index*20)), " ", Font_11x18, YELLOW, BLACK);
    }
    if(hmi_menu_ctrl.item_index <= HMI_MENU_ID_PULSE_MODE)
    {
        ST7789_WriteString(10, (80 + (hmi_menu_ctrl.item_index*20)), " ", Font_11x18, YELLOW, BLACK);
    }
}

/***********************************************************************************/

static void hmi_menu_show_menu_itens(void)
{
    ST7789_WriteString(3, 3, "  ELETRONIC LOAD", Font_16x26, RED, BLACK);
    ST7789_DrawLine(0, 27, 318, 27, WHITE);
    ST7789_DrawRectangle(0, 0, 318, 239, WHITE);
    ST7789_WriteString(3, 30, "MENU: ", Font_16x26, YELLOW, BLACK);

    for (uint8_t index = 0; index < HMI_NUMBER_ID_OF_MENU_ITEM; index++)
    {
        ST7789_WriteString(35, (60+(index*20)), hmi_menu_item_vector[index].string, Font_11x18, WHITE, BLACK);
    }
    ST7789_DrawLine(0, 180, 318, 180, WHITE);
    ST7789_WriteString(12, 200, "ABL Solutions Ind", Font_16x26, BLUE, BLACK);
}

/***********************************************************************************/

void hmi_menu_show_screen(void)
{
    ST7789_Fill_Color(BLACK);
    hmi_menu_show_menu_itens();
    hmi_menu_show_menu_cursor();
}

/***********************************************************************************/

void hmi_menu_update_data(void)
{
    
}

/***********************************************************************************/

void hmi_menu_update_button(button_id_t button_id, button_press_type_t button_press_type)
{
    switch (button_id)
    {
    case BUTTON_LEFT_ID:
        break;
    case BUTTON_RIGHT_ID:

        break;
    case BUTTON_ENC_ID:
        switch (button_press_type)
        {
        case BUTTON_SHORT_PRESS:
            switch (hmi_menu_item_vector[hmi_menu_ctrl.item_index].id)
            {
            case HMI_MENU_ID_CONSTANT_CURRENT:
                break;
            case HMI_MENU_ID_CONSTANT_VOLTAGE:
                break;
            case HMI_MENU_ID_CONSTANT_RESISTENCE:
                break;
            case HMI_MENU_ID_DYNAMIC_MODE:
                break;
            case HMI_MENU_ID_PULSE_MODE:
                break;
            case HMI_MENU_ID_SETTINGS:
                break;
            default:
                break;
            }
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
}

/***********************************************************************************/

void hmi_menu_update_encoder(enc_state_t enc_state)
{
    switch (enc_state)
    {
    case ENC_STATE_CCW:
        hmi_menu_increment_item_index();
        break;
    case ENC_STATE_CW:
        hmi_menu_decrement_item_index();
        break;
    default:
        break;
    }

    hmi_menu_show_menu_cursor();
}

/***********************************************************************************/