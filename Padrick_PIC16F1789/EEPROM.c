/* 
 * File:   EEPROM.c
 * Author: Florian
 *
 * Created on 28. Dezember 2015, 11:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Prototypes_V2.h"
#include "Variables_V2.h"

void write_EEPROM_logo_leds_on (void)
{
    EEADRL = 0;
    EEDATL = logo_leds_on;
    EEPGD = 0;
    CFGS = 0;
    WREN = 1;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    WR = 1;
    WREN = 0;
    while (WR == 1);
    EEIF = 0;
}

void read_EEPROM_logo_leds_on (void)
{
    GIE = 0;
    EEADRL = 0;
    EEPGD = 0;
    CFGS = 0;
    RD = 1;
    logo_leds_on = EEDATL;
    GIE = 1;
}