/* 
 * File:   Gesamt_FW_V2.c
 * Author: Florian
 *
 * Created on 22. Dezember 2015, 20:25
 */

// PIC16F1789 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include <math.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


#include <stdio.h>
#include <stdlib.h>
#include "Variables_V2.h"
#include "Prototypes_V2.h"

void main (void)
{
    init();
    while (1)
    {
        TMR0IE = 1;
        hold = 0;                   // button is not pressed for longer than 200 ms
        
        if (sleep == 1)             // if PIC should sleep
        {
            TMR0IE = 0;
            SLEEP();                // PIC sleeps until an interrupt occurs
        }
        
        while (button_colder == 0 || bluetooth_colder == 1)  // while button colder is pressed
        {
            colder();               // subroutine "colder" in "button_subroutines.c"
            TMR10s = 0;
            TMR0IE = 1;
        }
        
        while (button_warmer == 0 || bluetooth_warmer)  // while button warmer is pressed
        {
            warmer();               // subroutine "warmer" in "button_subroutines.c"
            TMR10s = 0;
            TMR0IE = 1;
        } 
    }
}

