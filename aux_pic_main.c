#define _XTAL_FREQ 16000000
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "config_bits.h"
#include "increment_helper.h"
#include "pl_tone_t_enum.h"
#include "aid_t_enum.h"
#include "mute_t_enum.h"

#define I2C_ADDRESS 0x39
#define LUT_LEN 256
#define MP3_PIN_HOLD_TIME 1000

const uint8_t lut[LUT_LEN] =
{
128,131,134,137,140,143,146,149,152,156,159,162,165,168,171,174,
176,179,182,185,188,191,193,196,199,201,204,206,209,211,213,216,
218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,245,
246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255,
255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247,
246,245,243,242,240,239,237,236,234,232,230,228,226,224,222,220,
218,216,213,211,209,206,204,201,199,196,193,191,188,185,182,179,
176,174,171,168,165,162,159,156,152,149,146,143,140,137,134,131,
128,124,121,118,115,112,109,106,103,99, 96, 93, 90, 87, 84, 81,
79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39,
37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10,
9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8,
9, 10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35,
37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76,
79, 81, 84, 87, 90, 93, 96, 99, 103,106,109,112,115,118,121,124
};


volatile uint32_t increment = 0;
volatile uint16_t mp3_pin_counter = 0;
// ISR sets this to 1 when the hold time has elapsed. We then pull the
// output pins back low.
volatile bool pin_hold_flag = 0;


void mute_init(void)
{
    PORTCbits.RC5 = 0;
    TRISCbits.TRISC5 = 0;
    PORTCbits.RC6 = 0;
    TRISCbits.TRISC6 = 0;
}

void mp3_init(void)
{
    PORTAbits.RA0 = 0;   // Voice ID pin of mp3 module
    TRISAbits.TRISA0 = 0;
    PORTAbits.RA1 = 0;   // CW ID pin of mp3 module
    TRISAbits.TRISA1 = 0;

    // Remaining pins are reserved for future use.
    PORTAbits.RA2 = 0;
    TRISAbits.TRISA2 = 0;
    PORTAbits.RA3 = 0;
    TRISAbits.TRISA3 = 0;
    PORTAbits.RA4 = 0;
    TRISAbits.TRISA4 = 0;
    PORTBbits.RB1 = 0;
    TRISBbits.TRISB1 = 0;
    PORTBbits.RB2 = 0;
    TRISBbits.TRISB2 = 0;
    PORTBbits.RB3 = 0;
    TRISBbits.TRISB3 = 0;
    PORTBbits.RB4 = 0;
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB5 = 0;
    TRISBbits.TRISB5 = 0;
}

void i2c_init(void)
{
    TRISCbits.TRISC3 = 1; // SCL
    TRISCbits.TRISC4 = 1; // SDA
    
    SSPADD = (I2C_ADDRESS << 1);
    SSPCONbits.CKP = 1;
    SSPCONbits.SSPM = 0b0110;
    SSPCONbits.SSPEN = 1;
}

void pwm_init(void)
{
    // PWM setup according to p. 131
    TRISCbits.TRISC2 = 1;       // Make sure to turn this back on once we use it
    PR2 = 0xff;                 // One period is 64 us
    CCP1CONbits.CCP1M = 0b1100; // PWM mode
    CCP1CONbits.DC1B = 0;       // PWM high bits
    CCPR1L = 0;                 // PWM low bits

    PIR1bits.TMR2IF = 0;
    T2CONbits.T2CKPS = 0;
    T2CONbits.TMR2ON = 1;
}

void handle_pl_command(pl_tone_t byte)
{
    if(byte == PL_NONE)
    {
        // Turn off output completely as we don't even want a
        // square wave with fixed duty cycle.
        TRISCbits.TRISC2 = 1;
    }
    else 
    {
            TRISCbits.TRISC2 = 0;
            increment = get_increment(byte);
    }
}

void handle_aid_command(aid_t byte)
{
    switch(byte)
    {
        case AID_VOICE:
            PORTAbits.RA0 = 1;
            break;
        case AID_CW:
            PORTAbits.RA1 = 1;
            break;
        case AID_RES_3:
            PORTAbits.RA2 = 1;
            break;
        case AID_RES_4:
            PORTAbits.RA3 = 1;
            break;
        case AID_RES_5:
            PORTAbits.RA4 = 1;
            break;
        case AID_RES_6:
            PORTBbits.RB1 = 1;
            break;
        case AID_RES_7:
            PORTBbits.RB2 = 1;
            break;
        case AID_RES_8:
            PORTBbits.RB3 = 1;
            break;
        case AID_RES_9:
            PORTBbits.RB4 = 1;
            break;
        case AID_RES_10:
            PORTBbits.RB5 = 1;
            break;
    }
    mp3_pin_counter = MP3_PIN_HOLD_TIME;
}

void handle_mute_command(mute_t byte)
{
    switch(byte)
    {
        case AFO_1_LOW:
            PORTCbits.RC6 = 0;
            break;
        case AFO_1_HIGH:
            PORTCbits.RC6 = 1;
            break;
        case AFO_2_LOW:
            PORTCbits.RC5 = 0;
            break;
        case AFO_2_HIGH:
            PORTCbits.RC5 = 1;
            break;
    }
}

inline is_healthy(void)
{
    return !SSPCONbits.WCOL && !SSPCONbits.SSPOV;
}

int main(int argc, char** argv)
{
    // Run internal oscillator at 16 MHz (p. 91). This has no effect when
    // operating with an external crystal, but we keep it in here just in case.
    OSCCONbits.IRCF = 0b11;

    mp3_init();
    mute_init();
    i2c_init();
    pwm_init();

    // Interrupt setup
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.TMR2IE = 1;

    PIR1bits.TMR2IF = 0;
    PIR1bits.SSPIF = 0;
        
    while(1)
    {
        if(PIR1bits.SSPIF)
        {
            pl_tone_t byte = SSPBUF;
            if(SSPSTATbits.D_nA)
            {
                // All PL-commands have a 0 MSB
                if((byte & 0b10000000) == 0)
                {
                    handle_pl_command(byte);
                }
                // All momentary commands are 0b10xxxxxx
                else if((byte & 0b11000000) == 0b10000000)
                {
                    handle_aid_command(byte);
                }
                // All mute commands are 0b11xxxxxx
                else if((byte & 0b11000000) == 0b11000000)
                {
                    handle_mute_command(byte);
                }
            }
            SSPSTAT = 0;
            PIR1bits.SSPIF = 0;
        }
        if(pin_hold_flag)
        {
            pin_hold_flag = 0;
            PORTAbits.RA0 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA2 = 0;
            PORTAbits.RA3 = 0;
            PORTAbits.RA4 = 0;
            PORTBbits.RB1 = 0;
            PORTBbits.RB2 = 0;
            PORTBbits.RB3 = 0;
            PORTBbits.RB4 = 0;
            PORTBbits.RB5 = 0;
        }
        if(is_healthy()) CLRWDT();
    }

    return (EXIT_SUCCESS);
}

void interrupt isr(void)
{
    static uint32_t accumulator = 0;
    if(PIR1bits.TMR2IF)
    {
        //CCP1CONbits.DC1B = lut[(accumulator >> 16) & 0b11]; // Use with 10-bit LUT
        CCPR1L = lut[(accumulator >> 8) & 0xff];

        accumulator += increment;

        if(mp3_pin_counter)
        {
            mp3_pin_counter--;
            // Set the flag when we drop down to 0.
            if(!mp3_pin_counter) pin_hold_flag = 1;
        }
        PIR1bits.TMR2IF = 0;
    }
}
