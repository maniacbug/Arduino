/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <Stepper.h>

// How many steps in a revolution
const int steps_per_rev = 200;

// create an instance of the stepper class, specifying
// the number of steps of the motor per revolution 
// and the pins it's attached to
Stepper stepper(steps_per_rev, 4, 5, 7, 8);

// The pins which enable the motor driver.  We turn these
// off when we're not turning the motor, otherwise the power will
// stay on all the time.
const int enable_pins[] = { 3, 6 };

// Pins where the switches are located
const int sw1_pin = 11;
const int sw2_pin = 12;

// Energize or de-energize the motor, HIGH=energize
void set_enable_pins(int value)
{
  digitalWrite(enable_pins[0],value);
  digitalWrite(enable_pins[1],value);  
}

void setup(void)
{
  // Set up enable pins
  pinMode(enable_pins[0],OUTPUT);
  pinMode(enable_pins[1],OUTPUT);
  set_enable_pins(LOW);
  
  // set the speed of the motor to 20 RPMs
  stepper.setSpeed(20);
  
  // set up switch pins
  pinMode(sw1_pin,INPUT);
  pinMode(sw2_pin,INPUT);
}

void loop(void)
{
  // read the pins
  int sw1_reading = digitalRead(sw1_pin);
  int sw2_reading = digitalRead(sw2_pin);
  
  // if only one switch is high, turn the motor the
  // corresponding direction one half revolution.
  if ( sw1_reading && ! sw2_reading )
  {
    set_enable_pins(HIGH);
    stepper.step(-steps_per_rev/2);
    set_enable_pins(LOW);
  }
  if ( sw2_reading && ! sw1_reading )
  {
    set_enable_pins(HIGH);
    stepper.step(steps_per_rev/2);
    set_enable_pins(LOW);
  }
}
