/* 
 * File:   calculations.c
 * Author: Florian
 *
 * Created on 14. Oktober 2015, 15:51
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include "Variables_V2.h"
#include "Prototypes_V2.h"

void change_bin_to_dec(void)
{
    if (CR[15] == 1)                    // sign bit is "1" --> Zweierkomplement
    {
        for (loop = 0; loop < 15; loop++)        // from bit0 to bit14
        {
            CR[loop] = !CR[loop];             // change bitwise
        }
    }
    
    ACR[15] = 0;
    for (loop = 0; loop < 16; loop++)            // change current accumulator format from binary to decimal
    {
        current_accumulator_format = current_accumulator_format >> 1;
        if (ACR[loop] == 1)
        {
            current_accumulator_format = current_accumulator_format | 0x8000;   // 0x8000 = 0b1000...0
        }
    } 
    
    CR[15] = 0;
    for (loop = 0; loop < 16; loop++)            // change current format from binary to decimal
    {
        current_register_format = current_register_format >> 1;
        if (CR[loop] == 1)
        {
            current_register_format = current_register_format | 0x8000;         // 0x8000 = 0b1000...0
        }
    }
}

void calculate_DS2740(void)
{
    // for 15mOhm Resistor...
    // Current Register Format:     LSB = 104,2  µA
    // Current Accumulator Format:  LSB = 416,7  µAh    ||    Maximum Resolution = +- 13,65 Ah
    
    accumulator_level_relative = current_accumulator_format/(maximum_current_accumulator_format/100);    
    accumulator_level_absolute = current_accumulator_format*416.7;  // in µAh
    current_level = current_register_format*104.2;                  // in µA  
    if (accumulator_level_relative >= 120)
    {
        accumulator_level_relative = 120;
        return;
    }
    if (accumulator_level_relative <= 0)
    {
        accumulator_level_relative = 0;
    }
}
