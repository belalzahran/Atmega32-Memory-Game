
#include "main.h"
#include "lcd.h"
#include "keypad.h"
#include "avr.h"




char buf[20];

int buzzerPin = 5;

int blueLed = 0;
int yellowLed = 1;
int whiteLed = 2;
int redLed = 3;
int greenLed = 4;

int blueButton = 7;
int yellowButton = 6;
int whiteButton = 5;
int redButton = 4;
int greenButton = 3;

int gameScore = 0;
int gameLost = 0;
int pattern[20];
int patternLength = 0;
int demoDelay = 500;
int difficulty = -1;

void displayDemo();
void demoStep(int ledNumber, int duration);
void addStep();
int wasPatternReplicated();





int main() {

    set_input_output_pins();
    lcd_init_8bit();
    starting_msg();

    


    while(1)
    {

        lcd_send_string("Press Red Button");
        lcd_pos(2,2);
        lcd_send_string("to Start Game");

        int gameStarted = -1;
        while(gameStarted == -1){
            if (PINA & (1 << redButton)){
                gameStarted = 1;
            }
        }

        srand(time(NULL));

        lcd_clear();

        lcd_pos(1,3);
        lcd_send_string("Choose Level");
        lcd_pos(2,1);
        lcd_send_string("Easy:G -> Hard:B");

        int pressedDifficultyButton = -1;

        while (pressedDifficultyButton == -1) 
        {
            if (PINA & (1 << blueButton)) {
                pressedDifficultyButton = blueLed;
                demoDelay = 50;
                difficulty = 5;
            } else if (PINA & (1 << yellowButton)) {
                pressedDifficultyButton = yellowLed;
                demoDelay = 100;
                difficulty = 4;
            } else if (PINA & (1 << whiteButton)) {
                pressedDifficultyButton = whiteLed;
                demoDelay = 200;
                difficulty = 3;
            } else if (PINA & (1 << redButton)) {
                pressedDifficultyButton = redLed;
                demoDelay = 300;
                difficulty = 2;
            } else if (PINA & (1 << greenButton)) {
                pressedDifficultyButton = greenLed;
                demoDelay = 500;
                difficulty = 1;
            }
            _delay_ms(50); // Debounce delay
        }

        demoStep(pressedDifficultyButton, 100);
    
        lcd_clear();
        lcd_send_string("Game Starting...");
        _delay_ms(1000);
        

        lcd_clear();
        lcd_pos(1,5);
        lcd_send_string("Score: ");
        lcd_send_integer(gameScore);
        lcd_pos(2,5);
        lcd_send_string("Level: ");
        lcd_send_integer(difficulty);
        _delay_ms(2000);

        

        
        while (!gameLost && gameScore < 40)
        {
            addStep();
            displayDemo();

            if (wasPatternReplicated())
            {
                gameScore++;
                lcd_pos(1,12);
                lcd_send_string("  ");
                lcd_pos(1,12);
                lcd_send_integer(gameScore);

            }
            else{
                gameLost = 1;
            }
            
            _delay_ms(1000);
        }

        lcd_clear();

        lcd_pos(1,4);
        lcd_send_string("Game Over");
        lcd_pos(2,2);
        lcd_send_string("Final Score:");
        lcd_send_integer(gameScore);

        _delay_ms(10000);
        lcd_clear();

        gameLost = 0;
        gameScore = 0;
        patternLength = 0;
        pressedDifficultyButton = -1;
        gameStarted = -1;


    }
   

    return 0;
}


int wasPatternReplicated() {
    for (int i = 0; i < patternLength; i++) 
    {
        int expectedButton = pattern[i];
        int pressedButton = -1;

 
        while (pressedButton == -1) {
            if (PINA & (1 << blueButton)) {
                pressedButton = blueLed;
            } else if (PINA & (1 << yellowButton)) {
                pressedButton = yellowLed;
            } else if (PINA & (1 << whiteButton)) {
                pressedButton = whiteLed;
            } else if (PINA & (1 << redButton)) {
                pressedButton = redLed;
            } else if (PINA & (1 << greenButton)) {
                pressedButton = greenLed;
            }
            _delay_ms(50); 
        }

        demoStep(pressedButton, 100);

        if (pressedButton != expectedButton) {
            PORTC = 0b00011111;
            _delay_ms(200);
            PORTC = 0;
            _delay_ms(200);
            PORTC = 0b00011111;
            _delay_ms(200);
            PORTC = 0;
            _delay_ms(200);
            PORTC = 0b00011111;
            _delay_ms(200);
            PORTC = 0;
            _delay_ms(200);
            PORTC = 0b00011111;
            _delay_ms(200);
            PORTC = 0;
            _delay_ms(200);
            PORTC = 0b00011111;
            _delay_ms(200);
            PORTC = 0;
       
            return 0;
        }

        while (PINA & (1 << pressedButton)) {
            _delay_ms(100);
        }
    }

    return 1; 
}

void addStep() 
{
    if (patternLength < 20) {
        int newStep = rand() % 5;  // Generate a random number between 0 and 4
        pattern[patternLength] = newStep;
        patternLength++;
    }
}

void displayDemo(){
    for (int i = 0; i < patternLength; i++){
        demoStep(pattern[i], demoDelay);
    }
}

void demoStep(int ledNumber, int duration){
    int buzzerPitch = 0;

    switch (ledNumber){
        case 0:
            buzzerPitch = 1000;
            break;
        case 1:
            buzzerPitch = 1250;
            break;
        case 2:
            buzzerPitch = 1500;
            break;
        case 3: 
            buzzerPitch = 1750;
            break;
        case 4:
            buzzerPitch = 2000;
            break;
        default:
            break;
    }

    PORTC |= (1 << ledNumber);
    set_pwm_pitch(buzzerPitch);
    delay_ms(duration);
    set_pwm_pitch(0);
    PORTC &= ~(1 << ledNumber);
    delay_ms(duration/2);


}









