#ifndef CONFIG_BITS_H
#define CONFIG_BITS_H


// PIC16F726 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config BORV = 19        // Brown-out Reset Voltage selection bit (Brown-out Reset Voltage (VBOR) set to 1.9 V nominal)
#pragma config PLLEN = ON       // INTOSC PLLEN Enable Bit (INTOSC Frequency is 16MHz (32x))

// CONFIG2
#pragma config VCAPEN = DIS     // Voltage Regulator Capacitor Enable bits (All VCAP pin functions are disabled)

#endif