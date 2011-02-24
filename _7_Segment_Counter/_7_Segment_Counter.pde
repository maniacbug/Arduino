const byte ledCharSet[] =
{
  // 00-0F: Hex digits
  B01111110, B00110000, B01101101, B01111001,	// 0123
  B00110011, B01011011, B01011111, B01110000,	// 4567
  B01111111, B01111011, B01110111, B00011111,	// 89AB
  B01001110, B00111101, B01001111, B01000111	// CDEF
};

// LED segment pins A-G are connected to sequential Arduino pins
const int G_segment_pin = 3;
const int num_segments = 7;

const uint8_t begin_value = 0x0;
const uint8_t end_value = 0x10;
uint8_t current_value = 0;

void setup(void)
{
  int pin = G_segment_pin;
  while ( pin < G_segment_pin + num_segments )
  {
    pinMode( pin, OUTPUT );
    digitalWrite( pin, 1 );
    ++pin;
  }
}

void loop(void)
{
  // One bit in 'pins_byte' represents a single segment
  // common anode display, so flip all the font bits
  uint8_t pins_byte = ~ledCharSet[current_value];    
  
  // Display one bit of the pins_byte on each consecutive segment
  int pin = G_segment_pin;
  while ( pin < G_segment_pin + num_segments )
  {
    digitalWrite( pin, pins_byte & 1 );
    pins_byte >>= 1;
    ++pin;
  }
  
  // Next value, and loop around if needed
  if (++current_value >= end_value)
    current_value = begin_value;
    
  // See you in half a sec!
  delay(500);
}
