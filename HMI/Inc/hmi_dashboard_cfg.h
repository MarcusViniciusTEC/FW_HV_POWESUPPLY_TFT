#ifndef __HMI_CONST_CURRENT_CFG_H
#define __HMI_CONST_CURRENT_CFG_H


#define MIN_DIGIT 0
#define MAX_DIGIT 9

#define cursor_voltage_cordinate_default    \
{                                   \
    {                               \
        .x = 144,                   \
        .y = 110,                    \   
        .last_x = 0                 \
    }                               \
    ,                               \
    {                               \
        .x = 163,                   \
        .y = 110,                    \  
        .last_x = 0                 \ 
    }                               \
    ,                               \
    {                               \
        .x =183,                    \
        .y = 110,                    \ 
        .last_x = 0                 \  
    }                               \
    ,                               \
    {                               \
        .x = 210,                   \
        .y = 110,                    \   
        .last_x = 0                 \
    }                               \
    ,                               \
    {                               \
        .x = 225,                   \
        .y = 110,                    \  
        .last_x = 0                 \ 
    }                               \
}

#define cursor_current_cordinate_default    \
{                                   \
    {                               \
        .x = 144,                   \
        .y = 110,                    \   
        .last_x = 0                 \
    }                               \
    ,                               \
    {                               \
        .x = 178,                   \
        .y = 110,                    \  
        .last_x = 0                 \ 
    }                               \
    ,                               \
    {                               \
        .x =192,                    \
        .y = 110,                    \ 
        .last_x = 0                 \  
    }                               \
    ,                               \
    {                               \
        .x = 213,                   \
        .y = 110,                    \   
        .last_x = 0                 \
    }                               \
    ,                               \
    {                               \
        .x = 225,                   \
        .y = 110,                    \  
        .last_x = 0                 \ 
    }                               \
}

#define hmi_dasboard_vector_tittle_default  \
{                                           \
    {                                       \
        .tittle = " CONSTANT CURRENT",      \
        .symbol = "A",                      \
        .color  =  GREEN                    \
    }                                       \
    ,                                       \
    {                                       \
        .tittle = " CONSTANT VOLTAGE",      \
        .symbol = "V",                      \ 
        .color  = RED                       \
    }                                       \
    ,                                       \
    {                                       \
        .tittle = "CONSTANT RESISTENCE",    \
        .symbol = "R",                      \
        .color  =  GRAY                     \
    }                                       \
}

#endif