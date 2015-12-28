/* 
 * File:   Prototypes.h
 * Author: Florian
 *
 * Created on 14. Oktober 2015, 15:38
 */

// Prototypes.h is a Header File
// to include Prototypes in all c.Files
// where they are needed to get no error

#ifndef PROTOTYPES_H
#define	PROTOTYPES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// init System
void init (void);

// WS2812b Prototypes
void flash_led_accumulator  (void);
void load_value_accumulator (void);
void count_leds_accumulator (void);
void flash_leds_temperature (void);
void load_value_temperature (void);
void count_leds_temperature (void);
void update_leds_logo (void);
void all_leds_zero (void);

//Software Timer Prototypes
void delay_10us (void);
void delay_80us (void);
void delay_90us (void);
void delay_150us(void);
void delay_450us(void);
void delay_720us(void);
void delay_50ms(void);
void delay_200ms(void);

//DS2740 Prototypes
void write (unsigned char u);
unsigned char read (void);
void Initialization_DS2740 (void);
void command_adress_0xCC (void);
void function_command_write_Data (void);
void function_command_read_Data (void);
void select_memory_adress_0x0E (void);
void select_memory_adress_0x10 (void);
void overwrite_memory_adress_0x10 (void);
void read_memory_adress_0x0E (void);
void read_data_DS2740 (void);
void DS2740_communication (void);
void overwrite_DS2740 (unsigned int i);

//calculations
void change_bin_to_dec (void);
void calculate_DS2740 (void);

//ADC_conversion
void read_liquid_temperature_sensor1(void);
void read_liquid_temperature_sensor2(void);
void read_outside_temperature(void);
void read_is1(void);
void read_is2(void);
void read_peltier_element_1(void);
void read_peltier_element_2(void);
void read_pumpe(void);
void read_every_ADC (void);

//button_subroutines
void colder (void);
void warmer (void);

//Bluetooth_communication
void send_out_data (void);
void change_dec_to_ascii_send_data (unsigned short long dec, unsigned char komma);
void send_out_set_temperature_liquid (void);

//Peltier_controlling
void control_Peltier (void);
void cool_Peltier (void);
void heat_Peltier (void);

//Interrupt
interrupt void isr (void);
void system_low_power_consumption (void);
void restart_system (void);
void power_button_debounce (void);
void show_button_debounce (void);
void system_normal_power_consumption (void);

// EEPROM
void read_EEPROM_logo_leds_on (void);
void write_EEPROM_logo_leds_on (void);

#ifdef	__cplusplus
}
#endif

#endif	/* PROTOTYPES_H */

