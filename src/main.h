#ifndef _MAIN_H
#define _MAIN_H


#define F_CPU 8000000UL

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

extern void lcd_send_string(char *s);
extern void send_letter_8bit(unsigned char x);
extern void lcd_clear();
extern char scan_keypad();
extern void lcd_pos(unsigned char r, unsigned char c);
extern void sleep_700ns(void);


int delay = 10;
int bits = 0;
char prev;
int key_press_mutex = 1;

void delay_us(uint16_t us) {
    while (us--) {
        _delay_us(1); 
    }
}

void delay_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}
void set_pwm_pitch(int pitch) {
    if (pitch == 0) {
     
        PORTC &= ~(1 << 5);
        return;
    }

   
    if (pitch < 50) {
        pitch = 50;  
    } else if (pitch > 2000) {
        pitch = 2000; 
    }


    int period = 1000000 / pitch;  
    int high_time = period / 2;   

 
    for (int i = 0; i < 100; i++) { 
        PORTC |= (1 << 5);  
        delay_us(high_time);
        PORTC &= ~(1 << 5); 
        delay_us(period - high_time);
    }
}


void flip_enable() {
    PORTB |= (1 << 2); // Turn on Enable Pin
    _delay_ms(5);      // Small delay to ensure proper pulse width (1 µs is more than sufficient for 450 ns requirement)
    PORTB &= ~(1 << 2); // Turn off Enable Pin
    _delay_ms(5);      // Small delay to ensure proper pulse width
}


void starting_msg(){

    lcd_pos(1,3);
    lcd_send_string("starting...");
    _delay_ms(1000);
    lcd_clear();

}

void set_input_output_pins(){

  DDRD = 0b11111111;
  DDRB = 0b00000111;



  DDRA = 0b00000000;
  DDRC = 0b00111111; 


  PORTC = 0;
  PORTB = 0;
  PORTD = 0;
  PORTA = 0; 

}

void prompt_for_input(char* prompt, char* buffer){

  
    lcd_send_string(prompt);
    char input;
    int index = 0;

    do{
        input = scan_keypad();
        if (input != '#' && input != '\0')
        {
            buffer[index++] = input;
            send_letter_8bit(input);
        }

    }while(input != '#');

    _delay_ms(500);
     lcd_clear();

    buffer[index] = '\0';

   



}

void lcd_send_integer(int x) {
    char str[10];
    memset(str, 0, sizeof(str)); // Clear the buffer
    sprintf(str, "%d", x);
    lcd_send_string(str);
}

void lcd_send_float(float x) {
    char str[10];
    memset(str, 0, sizeof(str)); // Clear the buffer
    sprintf(str, "%.2f", x);
    lcd_send_string(str);
}

int button_is_pressed(unsigned char pin, unsigned char pinNumber){

    return pin & (1 << pinNumber);
}

char* charToBinaryString(unsigned char num) {
    char* binaryStr = (char*)malloc(9 * sizeof(char)); // Allocate memory for the binary string

    binaryStr[8] = '\0'; // Null-terminate the string

    // Convert the number to binary representation
    for (int i = 7; i >= 0; i--) {
        binaryStr[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    return binaryStr;
}





#endif 
