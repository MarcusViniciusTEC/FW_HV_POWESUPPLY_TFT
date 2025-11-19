#ifndef _ENCODER_TYPES_H_
#define _ENCODER_TYPES_H_


typedef enum
{
    EVENT_RUN = 0U,
    EVENT_STOPED
}enc_event_t;

typedef enum
{
    ENC_STATE_CCW = 0U,
    ENC_STATE_CW, 
    ENC_STATE_STOPED
}enc_state_t;

typedef struct 
{
    enc_event_t event;
    enc_state_t state;
}enc_ctrl_t;

typedef struct 
{
    void(*encoder_event)(enc_state_t);
    enc_state_t enc_state_init;
    enc_event_t enc_event_init;
}encoder_data_t;





#endif