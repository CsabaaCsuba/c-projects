#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Set the LED_PIN as output
    DDRB = DDRB | (1 << DDB0);
    //turn off base-led
    DDRB = DDRB | (1 << DDB5);
    PORTB = PORTB &  ~(1 << PORTB5);

    while (1) {
        // Toggle the LED state
        // PORTB = PORTB | (1 << PORTB5);
        PORTB = PORTB | (1 << PORTB0);

        _delay_ms(1000);
        
        PORTB = PORTB & ~(1 << PORTB0);
        _delay_ms(1000);
    }

    return 0;
}
