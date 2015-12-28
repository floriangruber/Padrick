/* 
 * File:   Interrupt.c
 * Author: Florian
 *
 * Created on 26. Dezember 2015, 14:09
 */

#include <stdio.h>
#include <stdlib.h>
#include "Prototypes_V2.h"
#include "Variables_V2.h"
#include <xc.h>

void system_low_power_consumption (void)
{
    PORTA  = 0b10000000;
    TRISA  = 0b00100111;
    ANSELA = 0b00011111;
    
    PORTB  = 0b00000010;
    TRISB  = 0b00010011;
    ANSELB = 0b00000000;
    
    PORTC  = 0b00110000;
    TRISC  = 0b10110000;
    ANSELC = 0b00000000;
    
    PORTD  = 0b00111100;
    TRISD  = 0b00101111;
    ANSELD = 0b00000000;
    
    PORTE  = 0b00001000;
    TRISE  = 0b00001111;
    ANSELE = 0b00000000;
}

void system_normal_power_consumption (void)
{
    OSCCON = 0b11111000;        // 32 MHz
    PORTA  = 0b10101000;        // IN1 (RA7) high side active; IN2 (RA4) low side active; RA3 = "1"
    PORTB  = 0b00000000;         
    PORTC  = 0b00000000;        // INH1 (RC2) & INH2 (RC1) are "0" -> Standby Mode BTN7970
    PORTD  = 0b01000000;                   
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

    all_leds_zero();
        
    // Alternative PIN function
    
    CCP1SEL = 0;                // CCP1 is on RC2
    CCP2SEL = 0;                // CCP2 is on RC1
    
    // PWM init - PWM frequency is 20 kHz for Peltier Element
    
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
    
    // Bluetooth (EUSART)
    
    SPBRG = 51;                 // Step 1 - page 347; initialize SPBRG, BRGH, BRG16 to achieve 9600 baud
    BRGH = 0;                   // Step 1 - page 347; initialize SPBRG, BRGH, BRG16 to achieve 9600 baud
    BRG16 = 0;                  // Step 1 - page 347; initialize SPBRG, BRGH, BRG16 to achieve 9600 baud
    
    SYNC = 0;                   // Step 2 - page 347; clear SYNC bit
    SPEN = 1;                   // Step 2 - page 347; set SPEN bit
    TXEN = 1;                   // Step 5 - page 347; set TXEN to enable transmission
    
    RCIE = 1;                   // Step 4 - page 351; interrupt enable bit reception
    CREN = 1;                   // Step 7 - page 351; enable reception by setting the CREN bit

    __delay_ms(2);              // wait 2 ms
    if (logo_leds_on == 1)      // if user wants ...
    {
        update_leds_logo();     // update LEDs for logo lightning
    }
    count_leds_accumulator();   // update accumulator leds
    count_leds_temperature();   // update temperature leds
    
}

void restart_system (void)
{
    set_temperature_liquid = 30;        // set temperature liquid at beginning 
    red_value_temperature = 75;         // value for red leds temperature
    green_value_temperature = 0;        // value for green leds temperature
    blue_value_temperature = 75;        // value for blue leds temperature 
    flash_accumulator = 0;                
    flash_temperature = 0;
    first_time_discharge = 1;
    first_time_charge = 1;
    reverse_time_over = 0;
    pwm_value = 0;
    first_time_heat = 1;
    first_time_cool = 1;
    TMR_before_reverse = 0;
    system_normal_power_consumption(); 
    TMR10s = 0;
    leds_zero = 0;
}

void power_button_debounce (void)
{
    __delay_ms(10);
    while (button_power == 0);
    __delay_ms(10);
    while (button_power == 0);
    __delay_ms(10);
    while (button_power == 0);
    __delay_ms(10);
    while (button_power == 0);    
}

void show_button_debounce (void)
{
    __delay_ms(10);
    while (button_show == 0);
    __delay_ms(10);
    while (button_show == 0);
    __delay_ms(10);
    while (button_show == 0);
    __delay_ms(10);
    while (button_show == 0);    
}

interrupt void isr (void)                   // an interrupt was set
{
    if (RCIF == 1)                          // data vie bluetooth received
    {
        RCIF = 0;                           // clear RCIF; GIE is cleared by hardware
        
        // if RCREG == ASCII 'a' AND PIC doesn't sleep already -> 
        // shut down system and go into sleep mode
        
        if (RCREG == 'a' && sleep == 0)     
        {
            sleep = 1;                      // sleep = "1"; PIC goes to sleep mode
            IOCCNbits.IOCCN5 = 0;           // IOCCF5 should not wake up PIC (show button)
            system_low_power_consumption(); // initialize system for lowest possible power consumption  
        }
        
        // if RCREG == ASCII 'a' AND PIC doesn't sleep already -> 
        // show accumulator and temperature LED's for 10 seconds
        
        if (RCREG == 'b' && sleep == 0)     
        {
            count_leds_accumulator();       // activate accumulator LEDs ...
            count_leds_temperature();       // activate temperature LEDs ...
            TMR10s = 0;                     // ... for 10 seconds
            leds_zero = 0;                  // leds_zero = "0"
        }
        
        //if RCREG == ASCII 'O' AND PIC doesn't sleep already 
        
        if (RCREG == 'O' && sleep == 0)
        {
            if (logo_leds_on == 1)          // if logo LED's are ON -> shut OFF logo LED's
            {
                logo_leds_on = 0;
                red_value_logo = 0;
                green_value_logo = 0;
                blue_value_logo = 0;
                update_leds_logo();     
            }
            else                            // if logo LED's are OFF -> shut ON logo LED's
            {
                logo_leds_on = 1;
                red_value_logo = 75;
                green_value_logo = 75;
                blue_value_logo = 75;
                update_leds_logo();    
            }
        }
        
        // if RCREG == ASCII '+' -> temperature warmer
        // until RCREG == ASCII 'E'
        
        if (RCREG == '+')                   
        {
            bluetooth_warmer = 1;           // bluetooth_warmer = "1" -> temperature warmer
        }
        
        // if RCREG == ASCII '-' -> temperature colder
        // until RCREG == ASCII 'E'
        
        if (RCREG == '-')                   
        {
            bluetooth_colder = 1;           // bluetooth_colder = "1" -> temperature colder
        }
        
        // if RCREG == ASCII 'E' -> end of temperature configuration
        
        if (RCREG == 'E')                   
        {
            bluetooth_warmer = 0;           // bluetooth_warmer = "0"
            bluetooth_colder = 0;           // bluetooth_colder = "0"
        }
    }
    
    if (IOCCFbits.IOCCF4 == 1)              // power_button pressed
    {
        IOCCFbits.IOCCF4 = 0;               // clear IOCCF4 (interrupt flag); GIE is cleared by hardware
        power_button_debounce();            // debounce power button
        IOCCFbits.IOCCF4 = 0;
        if (sleep == 1)                     // if PIC "was" in sleep mode
        {
            sleep = 0;                      // PIC goes into normal mode
            IOCCNbits.IOCCN5 = 1;           // if interrupt on show_button occurs, PIC goes to isr
            restart_system();
        }
        else                                // if PIC "was" in normal mode
        {
            sleep = 1;                      // PIC goes into sleep mode
            IOCCNbits.IOCCN5 = 0;           // IOCCF5 should not wake up PIC (show button)
            system_low_power_consumption(); // initialize system for lowest possible power consumption 
        }
    }
    
    if (IOCCFbits.IOCCF5 == 1)              // show button pressed
    {
        IOCCFbits.IOCCF5 = 0;               // clear IOCCF5 (interrupt flag); GIE is cleared by hardware
        show_button_debounce();             // debounce show button
        IOCCFbits.IOCCF5 = 0;
        count_leds_accumulator();           // activate accumulator LED's ...
        count_leds_temperature();           // activate temperature LED's ...
        TMR10s = 0;                         // ... for at least 10 seconds
        leds_zero = 0;                      // leds_zero = "0"
    }
    
    if (TMR0IF == 1)
    {
        TMR0IF = 0;             // clear TMR0IF
        TMR0 = 6;               // set TMR0 to 6
        TMR1s++;                // increase TMR5s
        TMR5s++;                // increase TMR1s
        TMR10s++;               // increase TMR10s
        TMR_before_reverse++;   // increase TMR_before_reverse
        
        if (TMR1s > 2000)
        {
            TMR1s = 0;
            control_Peltier();
            DS2740_communication();
            read_every_ADC();
            send_out_data();
        }
        
        if (TMR10s > 20000)             // 10 seconds over
        {
            TMR10s = 0;
            leds_zero = 1;
            all_leds_zero();
        } 
    }
    
    if (C1IF == 1)              // if Comparator 1 interrupt flag is set -> voltage supply under +10V
    {
        C1IF = 0;               // clear Comparator 1 interrupt flag
        level_shift = 0;        // shut OFF all LED's
        INH1 = 0;               // BTN1 in Standby Mode
        INH2 = 0;               // BTN2 in Standby Mode
        Pumpe = 0;              // Pumpe is OFF
        Luefter = 0;            // Luefter is OFF
        write_EEPROM_logo_leds_on();    // write "logo_leds_on" into EEPROM
    }
}
