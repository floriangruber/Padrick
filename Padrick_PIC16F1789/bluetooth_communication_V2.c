/* 
 * File:   bluetooth_communication_V2.c
 * Author: Florian
 *
 * Created on 27. Dezember 2015, 15:55
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Prototypes_V2.h"
#include "Variables_V2.h"

void send_out_data (void)
{
    change_dec_to_ascii_send_data (outside_temperature, 0);                 // keine Nachkommastelle, in °C
    change_dec_to_ascii_send_data (temperature_liquid_sensor1*100000, 5);   // 5 Nachkommastellen, in °C
    change_dec_to_ascii_send_data (temperature_liquid_sensor2*100000, 5);   // 5 Nachkommastellen, in °C
    change_dec_to_ascii_send_data (temperature_liquid_absolute*100000, 5);  // 5 Nachkommastellen, in °C           
    change_dec_to_ascii_send_data (is1, 0);                                 // 0 Nachkommastellen, in mV
    change_dec_to_ascii_send_data (is2, 0);                         // 0 Nachkommastellen, in mV
    change_dec_to_ascii_send_data (curve_charge_relative, 0);       // 0 Nachkommastellen, in %
    change_dec_to_ascii_send_data (curve_discharge_relative, 0);    // 0 Nachkommastellen, in %
    change_dec_to_ascii_send_data (curve_user_relative, 0);         // 0 Nachkommastellen, in %
    change_dec_to_ascii_send_data (current_level/1000, 0);          // 0 Nachkommastellen, in mA
    change_dec_to_ascii_send_data (set_temperature_liquid, 0);      // 0 Nachkommastellen, in °C
    TXREG = 13;                 // carriage return
    while (TXIF == 0);
    TXREG = 10;                 // line feed
    while (TXIF == 0);    
}

void send_out_set_temperature_liquid (void)
{
    change_dec_to_ascii_send_data (set_temperature_liquid, 0);      // 0 Nachkommastellen, in °C
    TXREG = 13;                 // carriage return
    while (TXIF == 0);
    TXREG = 10;                 // line feed
    while (TXIF == 0);        
}

void change_dec_to_ascii_send_data (unsigned short long dec, unsigned char komma)
{
    z = 0;
    i = 1;
    x = 0;
    for (z = 0; z < 7; z++)
    {
        var = ((dec*i)/1000000);
        if (z == 7-komma)
        {
            TXREG = ',';
            while (TXIF == 0);
        }
        if (var != 0)
        {
            x = 1;
        }
        if (x == 1)
        {
            TXREG = var+48;
            while (TXIF == 0);
        }
        dec = ((dec*i)%1000000);
        i = 10;
    }
    TXREG = ' ';
    while (TXIF == 0);
}