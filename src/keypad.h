#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "main.h"


int get_keypad_column(){

    DDRC = 0b11111000;
    PORTC = 0b00000111; // gives rows

    switch(PINC){
        case 0b00000110:
            return 1; // 1
        case 0b00000101:
            return 2; // 2
        case 0b00000011:
            return 3; // 3
        default:
            return 0; // 0
    }

}

int get_keypad_row(){

  DDRC = 0b000000111;
  PORTC = 0b00000111;


  switch(PINC){
    case 0b00001111:
    case 0b10001111:
        return 1;
    
    case 0b00010111:
    case 0b10010111:
        return 2;

    case 0b00100111:
    case 0b10100111:
        return 3;

    case 0b01000111:
    case 0b11000111:
        return 4;

    case 0b00000111:
    case 0b10000111:
        return 0;
    default:
        return;
  }

}

int key_is_pressed(){

    if (get_keypad_column() + get_keypad_row() > 1){
        return 1;
    }
    else{
        return 0;
    }
}

char scan_keypad()
{

    char keypad_chars[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};

    int column = get_keypad_column();
    int row = get_keypad_row();

    if (column != 0 && row != 0)
    {

        int index = (((row - 1) * 3) + column) -1;
        
        if (key_press_mutex){
            

            // send_letter_8bit(keypad_chars[index]);
            key_press_mutex = 0;
            return keypad_chars[index];
        }


        
        
    }
    else if (!key_is_pressed())
    {
        key_press_mutex = 1;

    }

    return '\0';
        
}
















#endif 