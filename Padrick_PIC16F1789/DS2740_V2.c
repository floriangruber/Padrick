/* 
 * File:   DS2740.c
 * Author: Florian
 *
 * Created on 14. Oktober 2015, 15:47
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Variables_V2.h"
#include "Prototypes_V2.h"

//              *************DS2740_Subroutines*************



void write (unsigned char u)
{
    if (u == 0)                     // write "0"
    {
        TRISDbits.TRISD5 = 0;       // keep bus line low ...         
        delay_90us();               // ... for 90 탎
        TRISDbits.TRISD5 = 1;       // RA0 is Input ...
        delay_10us();               // for 10 탎
    }
    else                            // write "1"
    {
        TRISDbits.TRISD5 = 0;       // keep bus line low ...  
        delay_10us();               // ... for 10 탎
        TRISDbits.TRISD5 = 1;       // RA0 is Input ...
        delay_90us();               // for 90 탎
    }
}

unsigned char read (void)
{
    TRISDbits.TRISD5 = 0;       // keep bus line low ...  
    __delay_us(2);              // for 2 탎
    TRISDbits.TRISD5 = 1;       // RA0 is Input ...
    __delay_us(4);              // for 4 탎
    if (RD5 == 1)               // DS2740 writes "1"
    {    
        delay_90us();
        return(1);              // return a "1" to the variable
    }
    else                        // DS2740 writes "0"
    {
        delay_90us();
        return(0);              // return a "0" to the variable
    }
}

void Initialization_DS2740 (void)         // 1-Wire initialization sequence
{
    start_communication = 0;
    RD5 = 0;
    TRISDbits.TRISD5 = 0;       // RA0 is Output low ...
    delay_720us();              // ... for 720 탎
    TRISDbits.TRISD5 = 1;       // RA0 is Input
    delay_80us();               // wait 80 탎, now RA0 should be "0" because of Presence Pulse
    if (RD5 == 0)
    {
        start_communication = 1;
    }
    delay_450us();              // wait 450 탎 
}

void command_adress_0xCC (void)           // 0xCC = 0b11001100
{
    write(0);                   // LSB
    write(0);
    write(1);
    write(1);
    write(0);
    write(0);
    write(1);
    write(1);                   // MSB
}

void function_command_write_Data (void)   // 0x6C = 0b01101100
{
    write(0);                   // LSB
    write(0);
    write(1);
    write(1);
    write(0);
    write(1);
    write(1);
    write(0);                   // MSB
}

void function_command_read_Data (void)    // 0x69 = 0b01101001     
{
    write(1);                   // LSB
    write(0);
    write(0);
    write(1);
    write(0);
    write(1);
    write(1);
    write(0);                   // MSB
}

void select_memory_adress_0x0E (void)     // 0x0E = 0b00001110
{
    write(0);                   // LSB
    write(1);
    write(1);
    write(1);
    write(0);
    write(0);
    write(0);
    write(0);                   // MSB   
}

void select_memory_adress_0x10 (void)     // 0x10 = 0b00010000
{
    write(0);                   // LSB
    write(0);
    write(0);
    write(0);
    write(1);
    write(0);
    write(0);
    write(0);                   // MSB
}

void overwrite_memory_adress_0x10 (void)  
{
    for (loop = 8; loop < 15; loop++)    // Accumulated Current Register MSB
    {
        write(ACR[loop]);
    }
    
    for (loop = 0; loop < 8; loop++)     // Accumulated Current Register LSB
    {
        write(ACR[loop]);
    }              
}

void read_memory_adress_0x0E (void)
{
    for (loop = 8; loop < 16; loop++)    // Current Register MSB
    {
        CR[loop] = read();
    }
    
    for (loop = 0; loop < 8; loop++)     // Current Register LSB
    {
        CR[loop] = read();
    }
    
    for (loop = 8; loop < 16; loop++)    // Accumulated Current Register MSB
    {
        ACR[loop] = read();
    }
    
    for (loop = 0; loop < 8; loop++)     // Accumulated Current Register LSB
    {
        ACR[loop] = read();
    }
}

void overwrite_DS2740 (unsigned int i)
{
    for (loop = 0; loop < 15; loop++)
    {
        ACR[loop] = i & 0x0001;
        i = i >> 1;
    }
    ACR[15] = 0;
    overwrite_memory_adress_0x10();
}

void read_data_DS2740 (void)
{
    Initialization_DS2740();            // Reset Pulse Master + Presence Pulse Slave
    if (start_communication == 1)       // if DS2740 starts communication with Master
    {
        command_adress_0xCC();          // command adress; Master TX: 0xCC
        function_command_read_Data();   // function command to read Data; Master RX: 0x69
        select_memory_adress_0x0E();    // select memory adress 0x10 (Accumulated Current Register MSB)
        read_memory_adress_0x0E();      // read Accumulated Current Register MSB-reg and then LSB-register
        change_bin_to_dec();
        calculate_DS2740();
    }           
}

void DS2740_communication (void)
{
    read_data_DS2740();
    if (CR[15] == 1)        //discharge
    {
        if (first_time_discharge == 1)
        {
            first_time_discharge = 0;
            first_time_charge = 1;
            curve_discharge_absolute = (curve_charge_absolute/1.2);
            curve_discharge_relative = (curve_charge_relative/1.2);
            overwrite_DS2740(curve_discharge_absolute/416.7);
            read_data_DS2740();
        }
        curve_discharge_absolute = accumulator_level_absolute;
        curve_discharge_relative = accumulator_level_relative;
        curve_user_relative = (curve_discharge_relative*1.2 - 20);
    }
    else                    // charge
    {
        if (first_time_charge == 1)
        {
            first_time_charge = 0;
            first_time_discharge = 1;
            curve_charge_absolute = (curve_discharge_absolute*1.2);
            curve_charge_relative = (curve_discharge_relative*1.2);
            overwrite_DS2740(curve_charge_absolute/416.7);
            read_data_DS2740();
        }
        curve_charge_absolute = accumulator_level_absolute;
        curve_charge_relative = accumulator_level_relative;
        curve_user_relative = curve_charge_relative - 20;
    }
    if (curve_user_relative <= 0)
    {
        curve_user_relative = 0;
    } 
    if (leds_zero == 0)
    {
        count_leds_accumulator();
    }
}

