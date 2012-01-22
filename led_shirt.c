/* LED Shirt
By Jeremy Ruhland
Programmed for Attiny25
                ------
               -|*   |- VCC
               -|    |- BTN (PB2)
OC1B (PB4) BLU -|    |- RED (PB1) OC0B
           GND -|    |- GRN (PB0) OC0A
                ------
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static volatile uint8_t delay = 5; // delay time (ms)

int main(void) {
 DDRB |= 1<<PB0 | 1<<PB1 | 1<<PB4; // PB0, PB1, PB4 output
 PORTB &= ~(1<<PB2); // Pullup resistor off on PB2

 //Timer 0 setup
 TCCR0A |= 1<<COM0A1 | 1<<COM0B1 | 1<<WGM00 | 1<<WGM01; // Clear OC0A/OC0B on Compare Match, set OC0A/OC0B at BOTTOM, Fast PWM mode
 TCCR0B |= 1<<CS01; // 8 prescaler
 OCR0A = 0;
 OCR0B = 0;
 
 //Timer 1 setup
 TCCR1 |= 1<<CS12; // 8 prescaler
 GTCCR |= 1<<PWM1B | 1<<COM1B1; // Pulse Width Modulator B Enable, OC1x cleared on compare match. Set when TCNT1 = $00 in the CPU clock cycle after a compare match with OCR1C.
 OCR1A = 0;
 OCR1C = 255; // Resets line to high when timer reaches max value

 //Interrupt setup
 MCUCR |= 1<<ISC01 | 1<<ISC00; // The rising edge of INT0 generates an interrupt request.
 GIMSK |= 1<<INT0; // Enable INT0 on PB2
 sei(); // Enable interrupts

 volatile uint8_t i = 0;

 while(1) { // Loop forever

  // fade from blue to violet
  for (i = 0; i < 256; i++) { 
  OCR0B = i; // Set Output Compair Register 0 B
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from violet to red
  for (i = 255; i > 0; i--) { 
  OCR0A = i; // Set Output Compair Register 0 A
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from red to yellow
  for (i = 0; i < 256; i++) { 
  OCR1B = i; // Set Output Compair Register 1 B
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from yellow to green
  for (i = 255; i > 0; i--) { 
  OCR0B = i; // Set Output Compair Register 0 B
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from green to teal
  for (i = 0; i < 256; i++) { 
  OCR0A = i; // Set Output Compair Register 0 A
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from teal to blue
  for (i = 255; i > 0; i--) { 
  OCR1B = i; // Set Output Compair Register 1 B
   _delay_ms(delay); // Delay for a few ms
  } 
 }
}

// Pushbutton interrupt
SIGNAL(INT0_vect)  {
 cli(); // Disable interrupts
 delay += 5; // Add 5 ms to delay time
 if (delay > 15) {
  delay = 5; // Set delay back to 5 ms
 }
 _delay_ms(5); // Debounce delay for pushbutton switch
 sei(); // Reenable interrupts
}
