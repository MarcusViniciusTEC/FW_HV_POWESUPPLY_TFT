#ifndef _HMI_MENU_CFG_H_
#define _HMI_MENU_CFG_H_


#endif

#define HMI_MENU_NUMBER_OF_ITEMS 6
#define HMI_CURSOR ">"

#define vector_hmi_coordinate_retangle_menu_default     \
{                                                       \
    {0,13,127,21},                                      \
    {0,23,127,31},                                      \
    {0,33,127,41},                                      \
    {0,43,127,51},                                      \
    {0,53,127,61}                                       \
}                                       


#define hmi_menu_vector_item_default                    \
{                                                       \
    {                                                   \
        .id = HMI_MENU_ID_CONSTANT_CURRENT,             \
        .string = "1.CONSTANT CURRENT"                  \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_CONSTANT_VOLTAGE,             \
        .string ="2.CONSTANT VOLTAGE"                   \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_CONSTANT_RESISTENCE,          \
        .string ="3.CONSTANT RESITENCE"                 \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_DYNAMIC_MODE,                 \
        .string ="4.DYNAMIC MODE"                       \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_PULSE_MODE,                   \
        .string = "5.PULSE MODE"                        \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_SETTINGS,                     \
        .string = "6.SETTINGS"                          \
    }                                                   \
}