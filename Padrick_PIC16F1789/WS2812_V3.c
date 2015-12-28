/* 
 * File:   WS2812.c
 * Author: Florian
 *
 * Created on 14. Oktober 2015, 15:07
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Variables_V2.h"
#include "Prototypes_V2.h"

void all_leds_zero (void)
{
    red_value_accumulator = 0;
    blue_value_accumulator = 0;
    green_value_accumulator = 0;
    
    var1 = red_value_temperature;
    var2 = blue_value_temperature;
    var3 = green_value_temperature;   
    
    red_value_temperature = 0;
    blue_value_temperature = 0;
    green_value_temperature = 0;
    
    active_leds = 0;
    load_value_accumulator();
    load_value_temperature();
    
    red_value_accumulator = 0;
    blue_value_accumulator = 75;
    green_value_accumulator = 100;
    
    red_value_temperature = var1;
    blue_value_temperature = var2;
    green_value_temperature = var3;    
}

/************************************accumulator***************************************************/

void count_leds_accumulator (void)
{
    if (curve_user_relative >= 80)                                         // >= 80.0%
    {
        active_leds = 4;
        load_value_accumulator();
        return;
    }
    if (curve_user_relative < 80 && curve_user_relative >= 60)      // 80.0 - 60.0%
    {
        active_leds = 3;
        load_value_accumulator();
        return;
    }
    if (curve_user_relative < 60 && curve_user_relative >= 40)      // 60.0 - 40.0%
    {
        active_leds = 2; 
        load_value_accumulator();
        return;
    }
    if (curve_user_relative < 40 && curve_user_relative >= 10)      // 40.0 - 10.0%
    {
        active_leds = 1; 
        load_value_accumulator();
        return;
    }
    if (curve_user_relative < 10)
    {
        active_leds = 1;
        flash_led_accumulator();
    }
}

void load_value_accumulator (void)
{  
    flash_accumulator = 0;
    inactive_leds = 4-active_leds;         // maximum LEDs are 4; anzahl_nicht_leds describe inactive LEDs

    for (loop = 0; loop < active_leds; loop++)      // for routine to activate all active_LEDs with specific PWM
    {
        var = green_value_accumulator;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe green LEDs
        {
            k = var & 0b10000000;
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_accumulator = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_accumulator = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_accumulator = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_accumulator = 0;    
                NOP();
            }
            var = var << 1;
        }

        var = red_value_accumulator;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe red LEDs
        {
            k = var & 0b10000000;
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_accumulator = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_accumulator = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_accumulator = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_accumulator = 0;    
                NOP();
            }
            var = var << 1;
        }

        var = blue_value_accumulator;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe blue LEDs
        {
            k = var & 0b10000000;
            
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_accumulator = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_accumulator = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_accumulator = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_accumulator = 0;    
                NOP();
            }
            
            var = var << 1;
        }
    }

    for (loop = 0; loop < inactive_leds; loop++)     // for routine to deactivate all inactive_leds
    {
        for (loop_2 = 0; loop_2 < 24; loop_2++)      // for routine to send 24 Databits with "0" per inactive_led
        {
            led_data_accumulator = 1;      
            NOP();                          // T0H
            led_data_accumulator = 0;
            NOP();                          // T0L
        }
    }
}

void flash_led_accumulator (void)
{
    if (flash_accumulator == 0)             // if flash = 0; signalizes that LED was "1"
    {
        for (loop = 0; loop < 96; loop++)  // all LEDS are zero
        {
            led_data_accumulator = 1;      
            NOP();                          // T0H
            led_data_accumulator = 0;
            NOP();                          // T0L    
        }
    }

    else                                    // if flash = 1; signalizes that LED was "0"
    {
        // flash the first led with a special colour
        green_value_accumulator = 20;
        red_value_accumulator = 150;
        blue_value_accumulator = 0;
        load_value_accumulator();
        // change LED colour to normal colour
        green_value_accumulator = 100;
        red_value_accumulator = 0;
        blue_value_accumulator = 75;
    }
    flash_accumulator =! flash_accumulator; // change flash
}

/************************************temperature***************************************************/

void count_leds_temperature (void)
{
    temperature_liquid_relative = ((set_temperature_liquid-temperature_liquid_minimum)*100)/(temperature_liquid_maximum-temperature_liquid_minimum);
    if (temperature_liquid_relative == 100)
    {
        active_leds = 10;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 100 && temperature_liquid_relative >= 87)
    {
        active_leds = 9;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 87 && temperature_liquid_relative >= 75)
    {
        active_leds = 8;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 75 && temperature_liquid_relative >= 62)
    {
        active_leds = 7;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 62 && temperature_liquid_relative >= 50)
    {
        active_leds = 6;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 50 && temperature_liquid_relative >= 37)
    {
        active_leds = 5;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 37 && temperature_liquid_relative >= 25)
    {
        active_leds = 4;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 25 && temperature_liquid_relative >= 12)
    {
        active_leds = 3;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative < 12 && temperature_liquid_relative > 0)
    {
        active_leds = 2;
        load_value_temperature();
        return;
    }
    if (temperature_liquid_relative == 0)
    {
        active_leds = 1;
        load_value_temperature();
        return;
    }
}

void load_value_temperature (void)
{  
    inactive_leds = 10-active_leds;         // maximum LEDs are 4; anzahl_nicht_leds describe inactive LEDs

    for (loop = 0; loop < active_leds; loop++)      // for routine to activate all active_LEDs with specific PWM
    {
        var = green_value_temperature;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe green LEDs
        {
            k = var & 0b10000000;
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_temperature = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_temperature = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_temperature = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_temperature = 0;    
                NOP();
            }
            var = var << 1;
        }

        var = red_value_temperature;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe red LEDs
        {
            k = var & 0b10000000;
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_temperature = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_temperature = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_temperature = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_temperature = 0;    
                NOP();
            }
            var = var << 1;
        }

        var = blue_value_temperature;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe blue LEDs
        {
            k = var & 0b10000000;
            
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_temperature = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_temperature = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_temperature = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_temperature = 0;    
                NOP();
            }
            
            var = var << 1;
        }
    }

    for (loop = 0; loop < inactive_leds; loop++)     // for routine to deactivate all inactive_leds
    {
        for (loop_2 = 0; loop_2 < 24; loop_2++)      // for routine to send 24 Databits with "0" per inactive_led
        {
            led_data_temperature = 1;      
            NOP();                          // T0H
            led_data_temperature = 0;
            NOP();                          // T0L
        }
    }
}

void flash_leds_temperature (void)
{
    if (flash_temperature == 0)             // if flash = 0; signalizes that LED was "1"
    {
        for (loop = 0; loop < 240; loop++)  // all LEDS are zero
        {
            led_data_temperature = 1;      
            NOP();                          // T0H
            led_data_temperature = 0;
            NOP();                          // T0L    
        }
    }

    else                                    // if flash = 1; signalizes that LED was "0"
    {
        count_leds_temperature();
    }
    flash_temperature =! flash_temperature; // change flash
}

void update_leds_logo (void)
{
    for (loop = 0; loop < number_leds_logo; loop++)      // for routine to activate all active_LEDs with specific PWM
    {
        var = green_value_logo;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe green LEDs
        {
            k = var & 0b10000000;
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_logo = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_logo = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_logo = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_logo = 0;    
                NOP();
            }
            var = var << 1;
        }

        var = red_value_logo;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe red LEDs
        {
            k = var & 0b10000000;
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_logo = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_logo = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_logo = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_logo = 0;    
                NOP();
            }
            var = var << 1;
        }

        var = blue_value_logo;
        for (loop_2 = 0; loop_2 < 8; loop_2++)            // describe blue LEDs
        {
            k = var & 0b10000000;
            
            if (k == 0)                // T0H = 0.35 탎; T0L = 0.9 탎
            {
                led_data_logo = 1;      
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                led_data_logo = 0;
                NOP();
            }
            else                 // T1H = 0.9 탎; T1L = 0.35 탎
            {
                led_data_logo = 1;    
                NOP();                  // one NOP takes 4 steps with OSCCON = 32MHz --> 1 NOP takes 125 ns
                NOP();
                NOP();
                NOP();
                NOP();                  // seven NOPs take 0.875 탎
                led_data_logo = 0;    
                NOP();
            }
            
            var = var << 1;
        }
    }    
}
