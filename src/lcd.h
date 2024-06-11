#ifndef _LCD_H
#define _LCD_H

#include "main.h"



extern int bits;





void send_instruction(unsigned char instruction){

    PORTB = 0;
    PORTD = instruction;
    flip_enable();
}

void lcd_init_8bit(){
    
    bits = 8;

    send_instruction(0b00000001); // clear display
    send_instruction(0b00111000); // Set 8-bit mode; 2 line display; 5x8 font
    send_instruction(0b00001100); // Display on; cursor on; cursor blinking;
    send_instruction(0b00000110); // Address increment direction;
   
    // send_instruction(0b00111000);  // Set 8-bit mode; 2 line display; 5x8 font   001 _ _ _ 00
    // send_instruction(0b00000001);  // clear display
    // send_instruction(0b00000010);  // return home
    // send_instruction(0b00001100);  // display on; cursor

}

void send_letter_8bit(unsigned char x){

    PORTB = 0b00000001;
    PORTD = x;
    flip_enable();


}

void lcd_send_string(char *s) {
    while (*s) {
        if (bits == 4) {
            // send_letter_4bit(*s); // If using 4-bit mode (uncomment and implement if needed)
        } else {
            send_letter_8bit(*s);
        }
        _delay_us(50); // Small delay to ensure LCD processes each character
        s++;
    }
}


void lcd_clear(){

    send_instruction(0b00000001);
    flip_enable();

}

void lcd_pos(unsigned char r, unsigned char c)
{
    unsigned char n = (r-1) * 40 + (c-1);
    send_instruction(0b00000010);
    while (n--)
    {
        send_instruction(0b00010100);
    }
}










#endif