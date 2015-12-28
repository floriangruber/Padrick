/* 
 * File:   ADC_conversion.c
 * Author: Florian
 *
 * Created on 23. Dezember 2015, 11:16
 */

#include <stdio.h>
#include <stdlib.h>
#include "Variables_V2.h"
#include <xc.h>

void read_liquid_temperature_sensor1 (void)         // AN1 (RA1)
{
    ADCON0 = 0b00000101;                            // AN1 choosed, ADC enabled
    __delay_ms(1);                                  // wait 1 ms for changing analog input
    GO_nDONE = 1;                                   // start ADC
    while (GO_nDONE == 1);                          // wait until ADC is completed
    voltage = ADRES*0.001;                          // 0.001V because of 4,096V Vref and 12 bit result
    temperature_liquid_sensor1 = 2.21*voltage*voltage-32.79*voltage+93.05;
}

void read_liquid_temperature_sensor2 (void)         // AN2 (RA2)
{
    ADCON0 = 0b00001001;                            // AN2 choosed, ADC enabled
    __delay_ms(1);                                  // wait 1 ms for changing analog input
    GO_nDONE = 1;                                   // start ADC
    while (GO_nDONE == 1);                          // wait until ADC is completed
    voltage = ADRES*0.001;                          // 0.001V because of 4,096V Vref and 12 bit result
    temperature_liquid_sensor2 = 2.21*voltage*voltage-32.79*voltage+93.05;  
}

void read_outside_temperature (void)                // AN0 (RA0)
{
    ADCON0 = 0b00000001;                            // AN0 choosed, ADC enabled
    __delay_ms(1);                                  // wait 1 ms for changing analog input
    GO_nDONE = 1;                                   // start ADC
    while (GO_nDONE == 1);                          // ADC conversion completed
    voltage = ADRES*0.001;                          // 0.001V because of 4,096V Vref and 12 bit result   
    outside_temperature = (voltage-0.5)/0.01;       // with calculation of TC1047A datasheet
}

void read_is1 (void)                                // AN5 (RE0) 
{
    ADCON0 = 0b00010101;                            // AN5 choosed, ADC enabled
    __delay_ms(1);                                  // wait 1 ms for changing analog input
    GO_nDONE = 1;                                   // start ADC
    while (GO_nDONE == 1);                          // ADC conversion completed
    voltage = ADRES*0.001;                          // 0.001V because of 4,096V Vref and 12 bit result 
    is1 = voltage*1000;                             // in mV
}

void read_is2 (void)                                // AN7 (RE2)
{
    ADCON0 = 0b00011101;                            // AN7 choosed, ADC enabled
    __delay_ms(1);                                  // wait 1 ms for changing analog input
    GO_nDONE = 1;                                   // start ADC
    while (GO_nDONE == 1);                          // ADC conversion completed
    voltage = ADRES*0.001;                          // 0.001V because of 4,096V Vref and 12 bit result 
    is2 = voltage*1000;                             // in mV
}

void read_peltier_element_1 (void)                      // AN4 (RA5)
{
    ADCON0 = 0b00010001;                                // AN4 choosed, ADC enabled
    __delay_ms(1);                                      // wait 1 ms for changing analog input
    GO_nDONE = 1;                                       // start ADC
    while (GO_nDONE == 1);                              // ADC conversion completed
    voltage = ADRES*0.001;                              // 0.001V because of 4,096V Vref and 12 bit result 
    voltage_peltier_element_1 = (14700/4700)*voltage;   // 10k & 4k7 voltage divider
}

void read_peltier_element_2 (void)                      // AN6 (RE1)
{
    ADCON0 = 0b00011001;                                // AN6 choosed, ADC enabled
    __delay_ms(1);                                      // wait 1 ms for changing analog input
    GO_nDONE = 1;                                       // start ADC
    while (GO_nDONE == 1);                              // ADC conversion completed
    voltage = ADRES*0.001;                              // 0.001V because of 4,096V Vref and 12 bit result
    voltage_peltier_element_2 = (14700/4700)*voltage;   // 10k & 4k7 voltage divider
}

void read_pumpe (void)                                  // AN11 (RB4)
{
    ADCON0 = 0b00101101;                                // AN11 choosed, ADC enabled
    __delay_ms(1);                                      // wait 1 ms for changing analog input
    GO_nDONE = 1;                                       // start ADC
    while (GO_nDONE == 1);                              // ADC conversion completed
    voltage = ADRES*0.001;                              // 0.001V because of 4,096V Vref and 12 bit result
    current_pumpe = (voltage*10000);                    // I = U/R = (voltage/100mOhm)*1000 -> [] = mA
}

void read_every_ADC (void)
{
    read_outside_temperature();
    read_liquid_temperature_sensor1();
    read_liquid_temperature_sensor2();
    temperature_liquid_absolute = (temperature_liquid_sensor1+temperature_liquid_sensor2)/2;
    read_is1();
    read_is2();
    read_peltier_element_1();
    read_peltier_element_2();
    read_pumpe();
}