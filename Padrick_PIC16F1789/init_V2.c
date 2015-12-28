/* 
 * File:   init_V2.c
 * Author: Florian
 *
 * Created on 22. Dezember 2015, 20:32
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Variables_V2.h"
#include "Prototypes_V2.h"

void init (void)
{
    OSCCON = 0b11111000;        // 32 MHz HF
    
    PORTA  = 0b10101000;        // IN1 (RA7) high side active; IN2 (RA4) low side active; RA3 = "1"
    PORTB  = 0b00000000;         
    PORTC  = 0b00000000;        // INH1 (RC2) & INH2 (RC1) are "0" -> Standby Mode BTN7970
    PORTD  = 0b00000000;                   
    PORTE  = 0b00000000;
    
    TRISA  = 0b00100111;
    TRISB  = 0b00010011;
    TRISC  = 0b10110000;
    TRISD  = 0b00101111;
    TRISE  = 0b00000111;
    
    ANSELA = 0b00100111;
    ANSELB = 0b00010001;
    ANSELC = 0b00000000;
    ANSELD = 0b00000011;
    ANSELE = 0b00000111;
        
    // Alternative PIN function
    
    CCP1SEL = 0;                // CCP1 is on RC2
    CCP2SEL = 0;                // CCP2 is on RC1
    
    // PWM init - PWM frequency is 19802 Hz for Peltier Element
    
    TRISCbits.TRISC2 = 1;       // Step 1 - page 297
    TRISCbits.TRISC1 = 1;       // Step 1 - page 297
    PR2     = 99;               // Step 2 - page 297; calculation for PR2 in Equation 26-1
    CCP1CON = 0b00001111;       // Step 3 - page 297; PWM mode
    CCP2CON = 0b00001111;       // Step 3 - page 297; PWM mode
    CCPR1L  = 0;                // Step 4 - page 297
    CCPR2L  = 0;                // Step 4 - page 297
    TMR2IF  = 0;                // Step 5 - page 297; clear TMR2IF
    T2CON   = 0b00000101;       // Step 5 - page 297; Prescaler is 4; Timer is ON
    while (TMR2IF == 0);        // Step 6 - page 297; wait until Timer overflows
    TRISCbits.TRISC2 = 0;       // Step 6 - page 297
    TRISCbits.TRISC1 = 0;       // Step 6 - page 297   
    
    // Fixed Voltage Reference
    
    FVRCON  = 0b11001111;       // FVR = 4,096V at ADC & DAC

    // Analog to Digital Converter
    
    ADCON0  = 0b10000001;       // ADRES 12-bit resultat; AN0 choosed; ADC is enabled but not in progress
    ADCON1 = 0b11110011;        // 2's complement format; FRC; Vref- is VSS; Vref+ is FVR Buffer 1
    ADCON2 = 0b00001111;        // Trigger disabled; Single ended ADC
    
    // Digital Analog Converter 1 for Comparator 1 module
    
    DAC1CON0 = 0b10001000;      // DAC1 is enabled; no external OUT; FVR Buffer 2 (4,096V) and Vss
    DAC1CON1 = 176;             // to achieve 2,816V on Output
    
    // Comparator 1
    
    CM1CON0 = 0b10000100;       // C1 is enabled; if (IN+ > IN-) -> OUT = 1; Cout is internal
    CM1CON1 = 0b10101100;       // CxIF is set upon positive going edge on OUT; DAC1_out = IN+; C1IN4- = IN-
    C1IF = 0;
    C1IE = 1;                   // C1 interrupt enable bit
    
    // Interrupt On Change

    IOCCNbits.IOCCN4 = 1;       // interrupt enabled for e negative going edge on RC4
    IOCCFbits.IOCCF4 = 0;       // clear IOCCF4
    
    IOCCNbits.IOCCN5 = 1;       // interrupt enabled for e negative going edge on RC5
    IOCCFbits.IOCCF5 = 0;       // clear IOCCF5
    
    IOCIE = 1;                  // Interrupt on Change Interrupt Enable bit; if Interrupt occurs -> isr
    PEIE = 1;                   // peripheral interrupt enable bit -> for further description -> page 92 in datasheet
    GIE = 1;                    // global interrupt enable bit -> for further description -> page 92 in datasheet
    
    // Timer 1 for "Software_Timer.c"
    
    T1CON = 0b01000000;         // TMR1 clock source is FOSC; 1:1 Prescaler; TMR1 disabled
    TMR1IF = 0;                 // clear TMR1IF
    TMR1 = 0;                   // overwrite TMR1 with zero
    TMR1ON = 1;                 // TMR1 enabled
    
    // Timer 0
    
    TMR0CS = 0;                 // Internal instruction cycle clock (Fosc/4)
    OPTION_REGbits.PSA = 0;     // Prescaler is assigned to the Timer0 module
    OPTION_REGbits.PS = 0b011;  // Prescaler 1:16
    TMR0 = 0;                   // overwrite TMR0 with zero
    TMR0IF = 0;                 // clear TMR0IF       
    TMR0IE = 1;                 // Timer0 interrupt enable bit = "1"
    
    // Bluetooth (EUSART)
    
    SPBRG = 51;                 // Step 1 - page 347; initialize SPBRG, BRGH, BRG16 to achieve 9600 baud
    BRGH = 0;                   // Step 1 - page 347; initialize SPBRG, BRGH, BRG16 to achieve 9600 baud
    BRG16 = 0;                  // Step 1 - page 347; initialize SPBRG, BRGH, BRG16 to achieve 9600 baud
    
    SYNC = 0;                   // Step 2 - page 347; clear SYNC bit
    SPEN = 1;                   // Step 2 - page 347; set SPEN bit
    TXEN = 1;                   // Step 5 - page 347; set TXEN to enable transmission
    
    RCIE = 1;                   // Step 4 - page 351; interrupt enable bit reception
    CREN = 1;                   // Step 7 - page 351; enable reception by setting the CREN bit
                
    // determine variables for full charged accumulator 9.198 Ah = 22073*312,5µAh --> 22073 = 0b0101011000111001;
    
    for (loop = 0; loop < 16; loop++)
    {
        ACR[loop] = 0;
        CR[loop] = 0;
    }
 
    ACR[0]  = 1;
    ACR[3] = 1;
    ACR[4] = 1;
    ACR[5] = 1;
    ACR[9] = 1;
    ACR[10] = 1;
    ACR[12] = 1;
    ACR[14] = 1;

    // init DS2740
    
    Initialization_DS2740();            // Reset Pulse Master + Presence Pulse Slave
    if (start_communication == 1)
    {
        command_adress_0xCC();          // command adress; Master TX: 0xCC
        function_command_write_Data();  // function command to write Data; Master TX: 0x6C
        select_memory_adress_0x10();    // select memory adress 0x10 (Accumulated Current Register MSB)
        overwrite_memory_adress_0x10(); // overwrite Accumulated Current Register MSB-reg and then LSB-reg       
    }
    
    read_EEPROM_logo_leds_on();
    
    step_0to100_percent = pwm_max/time_0to100_percent;
    system_low_power_consumption();
}
