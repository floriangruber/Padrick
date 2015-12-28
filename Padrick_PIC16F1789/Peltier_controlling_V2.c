/* 
 * File:   Peltier_controlling.c
 * Author: Florian
 *
 * Created on 20. Oktober 2015, 14:56
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Variables_V2.h"
#include "Prototypes_V2.h"
    
void control_Peltier (void)
{ 
    // if temperature, which is set from user, is lower than the measured temperature -1 --> cool
    // example: set = 17°; measured = 22° --> set < measured-1 --> cool
    
    if (set_temperature_liquid < (temperature_liquid_absolute - 1))        
    {
        cool_Peltier();
    }
    
    // if temperature, which is set from user, is higher than the measured temperature +1 --> heat
    // example: set = 22°; measured = 17° --> set > measured+1 --> heat
    
    else if (set_temperature_liquid > (temperature_liquid_absolute + 1))
    {
        heat_Peltier();
    }
    
    // if measured temperature is in tolerance (+-1°C of set temperature) --> switch off Peltier
  
    else 
    {
        first_time_cool = 1;
        first_time_heat = 1;        
        INH1 = 0;                   // Motorcontroller 1 in Standby Mode
        INH2 = 0;                   // Motorcontroller 2 in Standby Mode
        pwm_value = 0;              // set pwm_value to zero
        TMR_before_reverse = 0;     // reset TMR_before_reverse
        reverse_time_over = 0;      // reverse time is not over
    }
    
}

void cool_Peltier (void)
{
    if (first_time_cool == 1)       // if Peltier gets reversed for the first time to cool 
    {
        first_time_cool = 0;        
        first_time_heat = 1;

        INH1 = 0;                   // Motorcontroller 1 in Standby Mode
        INH2 = 0;                   // Motorcontroller 2 in Standby Mode
        IN1 = 0;                    // Motorcontroller 1: Low Side active
        IN2 = 1;                    // Motorcontroller 2: High Side active
        
        // reset TMR_before_reverse
        
        TMR_before_reverse = 0;     // reset TMR_before_reverse
        reverse_time_over = 0;      // reverse time is not over
        pwm_value = 5;
    }
    
    // TMR_before_reverse counts in 5 ms steps
    // time_before_reverse_polarity is defined in Variables; [time_before_reverse_polarity] = s
    
    if (TMR_before_reverse > (time_before_reverse/0.005))  
    {
        TMR_before_reverse = 0;         // reset TMR_before_reverse
        reverse_time_over = 1;          // reverse time is over
    }
    
    if (reverse_time_over == 1)         // if reverse time is over
    {
        if (pwm_value <= (pwm_max-step_0to100_percent)) // if pwm_value is less than 100
        {
            // add (100/time_0to100_percent) to pwm_value -> linear slope in %/s
            pwm_value += step_0to100_percent;       
        }

        INH1  = pwm_max;        // Motorcontroller 1 in Power Mode
        INH2  = pwm_value;      // Motorcontroller 2 switches from Power to Standby Mode with pwm_value    
    }
}
    
void heat_Peltier (void)
{
    if (first_time_heat == 1)       // if Peltier gets reversed for the first time to cool 
    {
        first_time_heat = 0;
        first_time_cool = 1;

        INH1 = 0;                   // Motorcontroller 1 in Standby Mode
        INH2 = 0;                   // Motorcontroller 2 in Standby Mode
        IN1 = 1;                    // Motorcontroller 1: High Side active
        IN2 = 0;                    // Motorcontroller 2: Low Side active
        
        // reset TMR_before_reverse

        TMR_before_reverse = 0;     // reset TMR_before_reverse
        reverse_time_over = 0;      // reverse time is not over
        pwm_value = 5;
    }
    
    // TMR_before_reverse counts in 5 ms steps
    // time_before_reverse_polarity is defined in Variables; [time_before_reverse_polarity] = s
    
    if (TMR_before_reverse > (time_before_reverse/0.0005))
    {
        TMR_before_reverse = 0;         // reset TMR_before_reverse
        reverse_time_over = 1;          // reverse time is over  
    }
            
    if (reverse_time_over == 1)         // if reverse time is over
    {
        if (pwm_value <= (pwm_max-step_0to100_percent)) // if pwm_value is less than 100-steps from 0 to 100 percent
        {
            // add (one step from 0 to 100 percent) to pwm_value -> linear slope in %/s
            pwm_value += step_0to100_percent;       
        }

        INH1  = pwm_value;              // Motorcontroller 1 switches from Power to Standby Mode with pwm_value
        INH2  = pwm_max;                // Motorcontroller 2 is in Power Mode      
    }
}
