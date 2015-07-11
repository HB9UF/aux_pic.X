#ifndef AID_T_ENUM_H
#define	AID_T_ENUM_H

typedef enum aid_t {
    AID_CW     = 0b10000001,
    AID_VOICE  = 0b10000010,
    AID_RES_3  = 0b10000011,
    AID_RES_4  = 0b10000100,
    AID_RES_5  = 0b10000101,
    AID_RES_6  = 0b10000110,
    AID_RES_7  = 0b10000111,
    AID_RES_8  = 0b10001000,
    AID_RES_9  = 0b10001001,
    AID_RES_10 = 0b10001010
} aid_t;

#endif
