/* 
 * File:   Variables.h
 * Author: Florian
 *
 * Created on 14. Oktober 2015, 15:29
 */

// Variables.h is a Header File
// which gets included in all other c. Files
// to load Variables in it

#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define _XTAL_FREQ              32000000
#define button_power            RC4
#define button_show             RC5
#define button_colder           RD2
#define button_warmer           RD3
#define led_data_accumulator    LATCbits.LATC0
#define led_data_temperature    LATAbits.LATA6
#define led_data_logo           LATCbits.LATC3
#define IN1                     LATAbits.LATA7    
#define IS1                     RE0   
#define INH1                    CCPR1L     
#define IN2                     LATAbits.LATA4      
#define IS2                     RE2      
#define INH2                    CCPR2L      
#define hello_world             LATDbits.LATD7
#define Luefter                 LATBbits.LATB3
#define Pumpe                   LATBbits.LATB5
#define level_shift             LATDbits.LATD6
    
    
//              *************WS2812*************    

unsigned char k = 0;                            // variable for sending bits to LEDs
unsigned char red_value_accumulator = 0;        // value for red leds accumulator
unsigned char green_value_accumulator = 100;    // value for green leds accumulator
unsigned char blue_value_accumulator = 75;      // value for blue leds accumulator
unsigned char red_value_temperature = 75;       // value for red leds temperature
unsigned char green_value_temperature = 0;      // value for green leds temperature
unsigned char blue_value_temperature = 75;      // value for blue leds temperature 
unsigned char red_value_logo = 75;             // value for red leds logo
unsigned char green_value_logo = 75;           // value for green leds logo
unsigned char blue_value_logo = 75;            // value for blue leds logo
unsigned char number_leds_temperature = 10;
unsigned char number_leds_logo = 4;             // 4 LEDs for logo lightning
unsigned char number_leds_accumulator = 4;
unsigned char active_leds = 4;                  // number of active LEDs (ON)
unsigned char inactive_leds = 0;                // number of inactive LEDs (OFF)
// flash_accumulator is just taken into account, if accumulator level relative < 10%
// if flash_accumulator == 0 -> all accumulator LEDs should be inactive
// if flash_accumulator == 1 -> first accumulator LED should flash, all other accumulator LEDs should be inactive
unsigned char flash_accumulator = 0; 
// flash_temperature is just taken into account, if measured temperature is out of tolerance (+-1°C to set temp)
// if flash_temperature == 0 -> all temperature LEDs should be inactive
// if flash_temperature == 1 -> all active LEDs (depends on set_temperature) should be active
unsigned char flash_temperature = 0;            
unsigned int  TMR1s = 0;                // Software Timer to count 1 second
unsigned char var = 0;                  // variable for "for routine" do change decimal to binary to send out data

//              *************DS2740*************

unsigned char ACR[16];      // 15 variables of ACR (Accumulated Current Register); from ACR0 to ACR15
unsigned char CR[16];       // 15 variables of CR (Current Register); from CR0 to CR15

unsigned char loop = 0;     // variable for-routine
unsigned char loop_2 = 0;   // variable for-routine
unsigned char loop_3 = 0;   // variable for-routine

// start_communication is "1", if DS2740 answers to Reset Pulse of PIC
// start_communication is "0", if DS2740 does not answer to Reset Pulse of PIC
unsigned char start_communication = 0;  

unsigned int  TMR5s = 0;                                    // Software Timer to count 5 seconds
unsigned int  current_register_format = 0;                  // describes the Current Register in dec
unsigned int  current_accumulator_format = 22073;           // describes the Current Accumulator Register
unsigned int  maximum_current_accumulator_format = 22073;   // 100% = 9.198 Ah = 22073 * 416,7 µAh

unsigned int  accumulator_level_relative = 100;             // relative accumulator level (max 100%)           
unsigned short long  accumulator_level_absolute = 9198000;  // absolute accumulator level (max.9.198 Ah)
unsigned short long  current_level = 0;                     // current level 

unsigned char first_time_discharge = 1;
unsigned char first_time_charge = 1;
unsigned char curve_charge_relative = 120;
unsigned short long curve_charge_absolute = 11037600;
unsigned char curve_discharge_relative = 100;
unsigned short long curve_discharge_absolute = 9198000;
unsigned char curve_user_relative = 100;

//              *************ADC_conversion & temperature liquid*************

unsigned char set_temperature_liquid = 30;      // represents temperature, which is set by user in °C
float temperature_liquid_sensor1 = 20;  // represents temperature, which is measured in the liquid in °C   
float temperature_liquid_sensor2 = 20;  // represents temperature, which is measured in the liquid in °C
unsigned char temperature_liquid_relative = 50; // represents relative temperature of liquid in %
unsigned char temperature_liquid_maximum  = 45; // represents maxmimum temperature, which is allowed in liquid
unsigned char temperature_liquid_minimum  = 15; // represents minimum temperature, which is allowed in liquid
float temperature_liquid_absolute = 20;
float voltage = 0;                      // represents voltage, which is measured at PTC with ADC
float voltage_peltier_element_1 = 0;
float voltage_peltier_element_2 = 0;
unsigned int current_pumpe = 0;
unsigned int is1 = 0;
unsigned int is2 = 0;


//              *************button_subroutines*************

// hold is "1", if user presses the button for longer than 200ms
// hold is "0", if user does net press the button for longer than 200ms
unsigned char hold = 0;     

//              *************Peltier_controlling*************

// first_time_heat is "1", if Peltier element gets reversed for the first time to heat
// first_time_heat is "0", if Peltier element is already in the heat mode
unsigned char first_time_heat = 1;
// first_time_cool is "1", if Peltier element gets reversed for the first time to cool
// first_time_cool is "0", if Peltier element is already in the cool mode
unsigned char first_time_cool = 1;
unsigned char pwm_value = 0;                // represents the pwm_value to control the INH of motorcontroller
unsigned int  TMR_before_reverse = 0;       // is the counter for the time before reverse
unsigned char time_before_reverse = 10;     // is the time in s, which is count before reverse polarity of Peltier
unsigned char reverse_time_over = 0;        // == 1 if reverse time is over
unsigned char time_0to100_percent = 20;     // is the time in s, in which peltier goes from 0-100%
unsigned char pwm_max = 100;
unsigned char step_0to100_percent = 0;

//              *************Bluetooth_communication*************

unsigned char i = 1;                        // variable for routine

//              *************temperature_measurment*************

float outside_temperature = 0;



unsigned char z = 0;
unsigned char x = 0;

unsigned char sleep = 1;                    // "1", if system is in sleep mode; "0" if not

unsigned int TMR10s = 0;

unsigned char leds_zero = 0;                // "1", if leds are zero, "0" if not

unsigned char var1 = 0;
unsigned char var2 = 0;
unsigned char var3 = 0;

unsigned char bluetooth_warmer = 0;
unsigned char bluetooth_colder = 0;

unsigned char logo_leds_on = 0;


#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */

