/* 
 * File:   button_subroutines.c
 * Author: Florian
 *
 * Created on 20. Oktober 2015, 20:44
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Variables_V2.h"
#include "Prototypes_V2.h"

// with min temp 15°C and max temp 45°C 
// maximum differnce is 30°C
// with min step 0 and max step 150
// maximum number of steps are 150
// for 1°C --> 5 steps

void colder (void)
{
    TMR0IE = 0;
    if (set_temperature_liquid > temperature_liquid_minimum)
    {
        set_temperature_liquid--;  
        send_out_set_temperature_liquid();      // 0 Nachkommastellen, in °C
    }
    
    for (loop_3 = 0; loop_3 < 5; loop_3++)                  
    {
        if (blue_value_temperature < 150 && red_value_temperature > 0)
        {
            blue_value_temperature++;                   
            red_value_temperature--;        
        }      
        count_leds_temperature();
        delay_50ms();                
    }
    
    control_Peltier();
    
    if (button_colder == 0 && hold == 0)        // if button_colder is pressed but NOT hold   
    {
        delay_200ms();                          // wait 200 ms
        hold = 1;                               // now button_colder is hold
    }    
}

void warmer (void)
{
    TMR0IE = 0;
    if (set_temperature_liquid < temperature_liquid_maximum)
    {
        set_temperature_liquid++;
        send_out_set_temperature_liquid();      // 0 Nachkommastellen, in °C
    }
    
    for (loop_3 = 0; loop_3 < 5; loop_3++)
    {
        if (blue_value_temperature > 0 && red_value_temperature < 150)
        {
            blue_value_temperature--;
            red_value_temperature++;    
        } 
        count_leds_temperature();
        delay_50ms();
    }
    
    control_Peltier();
    
    if (button_warmer == 0 && hold == 0)        // if button_warmer is pressed but NOT hold
    {
        delay_200ms();                          // wait 200 ms
        hold = 1;                               // now button_warmer is hold
    }      
}