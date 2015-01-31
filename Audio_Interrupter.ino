// Audio interrupter
// Digital Pin 11 is TX
// Analog pin 0 is input

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

bool NewValue;
int8_t Value;

#define ADCPS_128 ( ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ) )
#define ADCPS_16 ( ( 1 << ADPS1 ) | ( 1 << ADPS0 ) )

void setup ()
{
  
  // Set the PWM Frequencey to 32KHz
  TCCR2B = TCCR2B & 0b11111000 | 0x01;
  
  ADMUX = 0;                  // Set up analog input to pin 0
  ADMUX |= ( 1 << REFS0 );    // Use AVcc as vref
  ADMUX |= ( 1 << ADLAR );    // Left-justify for 8-bit res
  
  ADCSRA &= ~ADCPS_128;       // Set ADC pre-scaler for 2 MHz
  ADCSRA |= ADCPS_16;
  
  ADCSRB = 0;
  
  ADCSRA |= ( 1 << ADEN );    // Enable ADC
  ADCSRA |= ( 1 << ADIE );    // Enable ADC interrupts
  
  ADCSRA |= ( 1 << ADSC );    // Start ADC conversion
  
  // Init status and value
  NewValue = false;
  Value = 0;
  
  pinMode ( 11, OUTPUT );
  
  sei ();
  
};

void loop ()
{
  
  while ( true )
  {
    
    if ( NewValue )
    {
      
      NewValue = false;
      analogWrite ( 11, Value );
      
    }
    
  }
  
};

ISR ( ADC_vect )
{
  
  NewValue = true;
  
  Value = ADCH;
  
  ADCSRA |= ( 1 << ADSC );
  
};


