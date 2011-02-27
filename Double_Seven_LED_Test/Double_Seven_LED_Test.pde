#include <Wire.h>
#include <MCP23018.h>

// Port extender is on address 0
MCP23018 pex(0);

// Font

byte ledCharSet[] =
{
  	// 00-0F: Hex digits
  	B01111110, B00110000, B01101101, B01111001,	// 0123
  	B00110011, B01011011, B01011111, B01110000,	// 4567
  	B01111111, B01111011, B01110111, B00011111,	// 89AB
  	B01001110, B00111101, B01001111, B01000111	// CDEF
};

// Reverse the bits in a single byte
uint8_t reverse(uint8_t _in)
{
  uint8_t result = 0;
  uint8_t bit = B10000000;

  while (bit)
  {
    if ( _in & 1 )
      result |= bit;
      
    _in >>= 1;
    bit >>= 1;
  }

  return result;
}

void setup(void)
{
  Wire.begin();
  pex.begin();
  
  // Set all 1's, turns the display off by default
  pex.SetPorts(0xff,0xff);

  // This font requires some manhandling.
  // Needs to be reversed, because I have the bits wired up exactly
  // backwards.  Leftshift it one to start with because the MSB is never used
  // XOR with ff because this is a common ANODE 7-segment, which means we go LOW
  // to turn it on.

  byte* current = ledCharSet;
  while( current < ledCharSet + sizeof(ledCharSet) )
    *current++ = ~(reverse(*current << 1));  
}

uint8_t value;

void loop()
{
  // Send the current value to the chip, low nibble to 'A', high nibble 'B'
  pex.SetPorts(ledCharSet[value >> 4],ledCharSet[value & 0xf]);
  
  // Next value.  Note that this will wrap around from 0xff back to 0 with no additional
  // work needed because it is an 8-bit value.
  ++value;
      
  // ...and wait
  delay(500);
}
