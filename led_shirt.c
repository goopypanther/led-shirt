/* LED Shirt
By Jeremy Ruhland
Programmed for Atmega168

Funct.	Pin Port Register
------	--- ---- --------
Red	13, PB1, OC1A
Green	14, PB2, OC1B
Blue	15, PB3, OC2A
Button	32, PD2, INT0

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static volatile uint8_t delay = 5; // delay time (ms)

int main(void) {
 DDRB |= 1<<PB1 | 1<<PB2 | 1<<PB3; // PB1, PB2, PB3 output
 PORTD &= ~(1<<PD2); // Pullup resistor off on PD2

 //Timer 1 setup (16-bit)
 TCCR1A |= 1<<COM1A1 | 1<<COM1B1 | 1<<WGM10; // Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM, Fast PWM, 8-bit
 TCCR1B |= 1<<WGM12 | 1<<CS11; //Fast PWM, 8-bit, clkI/O/8 (from prescaler)
 
 //Timer 2 setup (8-bit)
 TCCR2A |= 1<<COM2A1 | 1<<WGM21 | 1<<WGM20; // Clear OC2A on compare match, set OC2A at BOTTOM, Fast PWM
 TCCR2B |= 1<<CS21; // clkI/O/8 (from prescaler)

 //Interrupt setup
 EICRA |= 1<<ISC01 | 1<<ISC00; // The rising edge of INT0 generates an interrupt request.
 EIMSK |= 1<<INT0; // Enable INT0 on PB2
 sei(); // Enable interrupts

 volatile uint8_t i = 0;

 while(1) { // Loop forever

  // fade from blue to violet (red pin)
  for (i = 0; i < 256; i++) { 
  OCR1A = i; // Set Output Compair Register 1 A
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from violet to red (blue pin)
  for (i = 255; i > 0; i--) { 
  OCR2A = i; // Set Output Compair Register 2 A
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from red to yellow (green pin)
  for (i = 0; i < 256; i++) { 
  OCR1B = i; // Set Output Compair Register 1 B
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from yellow to green (red pin)
  for (i = 255; i > 0; i--) { 
  OCR1A = i; // Set Output Compair Register 1 A
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from green to teal (blue pin)
  for (i = 0; i < 256; i++) { 
  OCR2A = i; // Set Output Compair Register 2 A
   _delay_ms(delay); // Delay for a few ms
  } 

  // fade from teal to blue (green pin)
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
