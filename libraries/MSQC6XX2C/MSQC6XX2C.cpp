/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <MCP23018.h>
#include "MSQC6XX2C.h"

#undef DEBUG
#ifdef DEBUG
#define IF_DEBUG(x) (x)
#else
#define IF_DEBUG(x)
#endif

const byte ledCharSet[] =
{
  // 00-0F: Hex digits
  B01111110, B00110000, B01101101, B01111001,	// 0123
  B00110011, B01011011, B01011111, B01110000,	// 4567
  B01111111, B01111011, B01110111, B00011111,	// 89AB
  B01001110, B00111101, B01001111, B01000111	// CDEF
};

uint8_t latency;

// 50 ticks @ 15.625kHz = 3.2msec delay = 312.5Hz update per digit /4 = 78.125Hz total update cycle
const uint8_t tick_reset = 256-50; //256-80; 

// Initial display state
uint8_t values_init[] = { 0,1,2,3 };

// Single instance that will get updated via interrupts
MSQC6XX2C* instance = NULL;

MSQC6XX2C::MSQC6XX2C(MCP23018* _pex): 
pex(_pex), values_display_current( values_display ), values_display_end( values_display + 4 ), bitval( B1000 )
{
  instance = this;  

  set(values_init);
}

void MSQC6XX2C::begin(void)
{
  // Ports A0-A4 drive transistors for the digits
  // They need to be pulled up to Vcc so that a '1' will
  // Send current out to the resistor's base.
  pex->SetPullups(0xf,0);

  // Default value: Single 'E', signifying an error  
  pex->SetPorts(B1,~ledCharSet[0xE]);

  // Setup interrupts

  // Prescaler = FCPU/1024 = 16MHz/1024 = 15.625kHz
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Enable Overflow Interrupt Enable
  TIMSK2 = 1<<TOIE2;

  //Initialize Counter
  TCNT2 = tick_reset;

  // Enable interrupts
  sei();

  IF_DEBUG(pinMode(13,OUTPUT));
}

void MSQC6XX2C::set(uint8_t* _values)
{
  memcpy(values_display,_values,4);
}

void MSQC6XX2C::update(void)
{
  pex->SetPorts(bitval,~ledCharSet[*values_display_current]);
  bitval >>= 1;
  if ( ++values_display_current >= values_display_end )
  {
    values_display_current = values_display;
    bitval = B1000;
  }
}

#ifdef DEBUG
uint8_t debug_counter = 0xff;
uint8_t debug_led = 0xff;
#endif

void MSQC6XX2C::isr(void)
{
	// Note that you can currently only have ONE of these displays.
	// If we wanted more, we'd have to add some management here to loop
	// through all the displays and update each.

#ifdef DEBUG
	// Debugging consists of flashing an LED every so often, so that we know
	// the isr is firing.
	digitalWrite(13,debug_led);
	
	if (! debug_counter--)
		debug_led = ~debug_led;
#endif
	
	if (instance)
		instance->update();	
}

// Multiplex LED digits on the interrupt
ISR(TIMER2_OVF_vect) {

  //  Aaargh, Wire uses interrupts, and they are disabled now.  :O
  sei();
	
  MSQC6XX2C::isr();

  //Capture the current timer value. This is how much error we
  //have due to interrupt latency and the work in this function
  latency=TCNT2;

  //Reload the timer and correct for latency.
  TCNT2=latency+tick_reset;
}

