#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Set the LED_PIN as output
    DDRB = DDRB | (1 << DDB5);

    while (1) {
        // Toggle the LED state
        PORTB = PORTB | (1 << PORTB5);

        _delay_ms(2000);

        PORTB = PORTB & ~(1 << PORTB5);
        // Wait for 1 second (1000 milliseconds)
        _delay_ms(2000);
    }

    return 0;
}
