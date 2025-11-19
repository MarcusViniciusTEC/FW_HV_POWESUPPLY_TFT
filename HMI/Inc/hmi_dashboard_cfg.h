#ifndef __HMI_CONST_CURRENT_CFG_H
#define __HMI_CONST_CURRENT_CFG_H


#define MIN_DIGIT 0
#define MAX_DIGIT 9

#define cursor_cordinate_default    \
{                                   \
    {                               \
        .x = 143,                   \
        .y = 75,                    \   
        .last_x = 0                 \
    }                               \
    ,                               \
    {                               \
        .x = 163,                   \
        .y = 75,                    \  
        .last_x = 0                 \ 
    }                               \
    ,                               \
    {                               \
        .x =193,                    \
        .y = 75,                    \ 
        .last_x = 0                 \  
    }                               \
    ,                               \
    {                               \
        .x = 210,                   \
        .y = 75,                    \   
        .last_x = 0                 \
    }                               \
    ,                               \
    {                               \
        .x = 225,                   \
        .y = 75,                    \  
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