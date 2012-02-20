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
#include <util/delay.h>

#define FADEDELAY 39 // Millisecond delay between changes to OCRnx, 39ms ~ 1 min per color cycle

int main(void) {
 DDRB |= 1<<PB0 | 1<<PB1 | 1<<PB4; // PB0, PB1, PB4 output
 PORTB &= ~(1<<PB2); // Pullup resistor off on PB2

 //Timer 0 setup
 TCCR0A |= 1<<COM0A1 | 1<<COM0B1 | 1<<WGM00 | 1<<WGM01; // Clear OC0A/OC0B on Compare Match, set OC0A/OC0B at BOTTOM, Fast PWM mode
 TCCR0B |= 1<<CS01; // 8 prescaler
 
 //Timer 1 setup
 TCCR1 |= 1<<CS12; // 8 prescaler
 GTCCR |= 1<<PWM1B | 1<<COM1B1; // Pulse Width Modulator B Enable, OC1x cleared on compare match. Set when TCNT1 = $00 in the CPU clock cycle after a compare match with OCR1C.
 OCR1C = 255; // Resets line to high when timer reaches max value

 OCR1B = 255; // Turn on blue to cleanly enter the color cycle loop 
 _delay_ms(FADEDELAY); // Stay blue for a bit

 for(;;) { // Loop forever

  // fade from blue to violet
  while(OCR0B < 255) { 
   OCR0B++;
   _delay_ms(FADEDELAY);
  }
  _delay_ms(FADEDELAY); // Stay violet for a bit

  // fade from violet to red  
  while(OCR0A >0) { 
   OCR0A--;
   _delay_ms(FADEDELAY);
  }
  _delay_ms(FADEDELAY); // Stay red for a bit

  // fade from red to yellow
  while(OCR1B < 255) { 
   OCR1B++;
   _delay_ms(FADEDELAY);
  }
  _delay_ms(FADEDELAY); // Stay yellow for a bit

  // fade from yellow to green
  while(OCR0B >0) { 
   OCR0B--;
   _delay_ms(FADEDELAY);
  }
  _delay_ms(FADEDELAY); // Stay green for a bit

  // fade from green to teal
  while(OCR0A < 255) { 
   OCR0A++;
   _delay_ms(FADEDELAY);
  }
  _delay_ms(FADEDELAY); // Stay teal for a bit

  // fade from teal to blue
  while(OCR1B >0) { 
   OCR1B--;
   _delay_ms(FADEDELAY);
  }
  _delay_ms(FADEDELAY); // Stay blue for a bit
 }
}
