/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <WProgram.h>

class MCP23018;

/**
 * Driver for Everlight MSQC6XX2C 4-digit 7-segment LED display
 *
 * See <a href="http://download.siliconexpert.com/pdfs/2007/07/24/semi_b/manual/ele/display/msqc6x12c.pdf">Datasheet</a>
 *
 * This uses a MCP23018 port extender to drive the pins.  Working with other similiar models or shift
 * registers would require changes to abstract out the pin-writing interface.
 *
 * This uses the Timer2 interrupt to handle the multiplexing, changing a digit every 5.12msec. 
 *
 * @warning This has only been tested on 6912C, but should work on the other
 * common-anode members of this family.  Common-cathode models probably will require changes.
 *
 * @warning Timing is hard-coded for 16MHz units.  Using it on uC's with other speeds will require
 * changes.
 *
 * @warning Only one of these displays may be driven at any time.  If more are needed, this driver
 * will require changes to keep track of all displays and update each of them.
 */

class MSQC6XX2C
{
public:
  /**
   * Constructor.
   *
   * @param _pex Pointer to the port extender which is connected to the actual
   * unit.
   */
  MSQC6XX2C(MCP23018* _pex);

  /**
   * Prepare and setup the device
   *
   * @warning Be sure to begin the port extender first
   */
  void begin(void);

  /**
   * Set the displayed values
   *
   * Updates the display to the @a _values sent.  For example, to display
   * 1234:
   * @code
   *	uint8_t showvalues[4] = { 1, 2, 3, 4 };
   *    display.set( showvalues );
   * @endcode
   *
   * @param _values Pointer to FOUR uint's which will be displayed left-to-right.
   * Each value must be the exact integer to display.  
   */
  void set(uint8_t* _values);

  /**
   * Service interrupts for MSQC6XX2C devices
   *
   * This is something of a work-around.  I would prefer to declare the ISR to
   * be a friend of this class, but that seems not to work.  Instead, this will
   * be the one public interface into this class that the ISR can call. Then
   * any internals can be called by isr().
   *
   * @warning Should only be called internally by the ISR.
   */
  inline static void isr(void);

protected:
  /**
   * Update the next digit to the display
   *
   * Called by the isr()
   */ 
  inline void update(void);
	
private:
  MCP23018* pex; /**< Port extender used to interface with the unit */
  uint8_t values_display[4]; /**< Display buffer, which is being currently displayed */
  uint8_t* values_display_current; /**< The particular digit which will be displayed next */
  uint8_t* values_display_end; /**< The point just beyond the last valid digit in the display buffer */
  uint8_t bitval; /**< Which digit will be displayed next. B1000 is left-most, B1 is right-most */
};

