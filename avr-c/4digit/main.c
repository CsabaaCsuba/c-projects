/*
pin order:  Digit1    | UpperMid | UpperLeft  | Digit2      | Digit3 | UpperRight
            LowerLeft | LowerMid | Dot        | LowerRight  | Middle | Digit4


*/

#include <avr/io.h>
#include <util/delay.h>

// Pin definitions (mapped to PORTB and PORTD)
#define DIGIT1  2 //PD2
#define DIGIT2  3 //PD3
#define DIGIT3  4 //PD4
#define DIGIT4  5 //PD5

#define UPPER_MID    6//PD6
#define UPPER_LEFT   7//PD7
#define UPPER_RIGHT  0//PB0
#define LOWER_LEFT   1//PB1
#define LOWER_MID    2//PB2
#define DOT          3//PB3
#define LOWER_RIGHT  4//PB4
#define MIDDLE       5//PB5

// Number patterns for a 7-segment display
uint8_t numbers[][8] = {
    {0, 0, 0, 0, 0, 1, 0, 1}, // 0
    {1, 1, 0, 1, 1, 1, 0, 1}, // 1
    {0, 1, 0, 0, 0, 1, 1, 0}, // 2
    {0, 1, 0, 1, 0, 1, 0, 0}, // 3
    {1, 0, 0, 1, 1, 1, 0, 0}, // 4
    {0, 0, 1, 1, 0, 1, 0, 0}, // 5
    {0, 0, 1, 0, 0, 1, 0, 0}, // 6
    {0, 1, 0, 1, 1, 1, 0, 1}, // 7
    {0, 0, 0, 0, 0, 1, 0, 0}, // 8
    {0, 0, 0, 1, 0, 1, 0, 0}  // 9
};

// Function to initialize the pins as outputs
void init_pins(void) {
    DDRD |= (1 << DIGIT1) | (1 << DIGIT2) | (1 << DIGIT3) | (1 << DIGIT4) | (1 << UPPER_MID) | (1 << UPPER_LEFT);
    DDRB |= (1 << UPPER_RIGHT) | (1 << LOWER_LEFT) | (1 << LOWER_MID) | (1 << DOT) | (1 << LOWER_RIGHT) | (1 << MIDDLE);

    // Set all outputs to LOW initially
    PORTD &= ~((1 << DIGIT1) | (1 << DIGIT2) | (1 << DIGIT3) | (1 << DIGIT4) | (1 << UPPER_MID) | (1 << UPPER_LEFT));
    PORTB &= ~((1 << UPPER_RIGHT) | (1 << LOWER_LEFT) | (1 << LOWER_MID) | (1 << DOT) | (1 << LOWER_RIGHT) | (1 << MIDDLE));
}

// Function to write a digit to the specified place
void write_digit(uint8_t place, uint8_t digit) {
    if (digit > 9) return; // Invalid digit

    // Turn off all digits
    PORTD &= ~((1 << DIGIT1) | (1 << DIGIT2) | (1 << DIGIT3) | (1 << DIGIT4));

    // Write the segments for the digit
    uint8_t* segment_ports[] = {&PORTD, &PORTD, &PORTB, &PORTB, &PORTB, &PORTB, &PORTB, &PORTB};
    uint8_t segment_pins[] = {UPPER_MID, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_MID, DOT, LOWER_RIGHT, MIDDLE};

    for (uint8_t i = 0; i < 8; i++) {
        if (numbers[digit][i]) {
            *segment_ports[i] |= (1 << segment_pins[i]); // Set HIGH
        } else {
            *segment_ports[i] &= ~(1 << segment_pins[i]); // Set LOW
        }
    }

    // Turn on the corresponding digit place
    PORTD |= (1 << place);
    _delay_ms(5); // Small delay for persistence
    PORTD &= ~(1 << place); // Turn off the place
}

void write_number(int number){
    if (number > 9999){ // 4 digit display can't show 5 digit numbers
        return;
    }
    if (number < 10){
        while (1){
            write_digit(DIGIT4, number);
        }
    }

    int digits[4] = {-1, -1, -1, -1};
    int num_part = number;
    for (int i=3; i>=0; i--){
        int next_part = num_part % 10;

        digits[i] = next_part;
        num_part = (int)num_part / 10; //if it's 99, make it 9
        if (num_part < 10 && i > 0){ //if the number reached it's last digit
            digits[i-1] = num_part;
            break;
        }
    }
    while (1){
        write_digit(DIGIT1, digits[0]);
        write_digit(DIGIT2, digits[1]);
        write_digit(DIGIT3, digits[2]);
        write_digit(DIGIT4, digits[3]);
    }

}

// Main function
int main(void) {
    init_pins();

    write_number(5050);
    return 0;
}
