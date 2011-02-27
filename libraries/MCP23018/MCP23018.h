#include <WProgram.h>

/**
 * Driver for Microchip MCP23018 Port Extender
 *
 * See @link ww1.microchip.com/downloads/en/DeviceDoc/22103a.pdf Datasheet @endlink
 *
 * Create one instance of this class for each chip in your circuit.
 *
 * @warning This class is pretty incomplete.  It only drives a small subset of the
 * chip's overall functionality.
 */
class MCP23018
{
protected:
	/**
	 * The 7-bit address of the device we're driving on the I2C bus.
	 */
	uint8_t i2c_address;

	/**{*/
	/**
	 * Internals.
	 *
	 * Protected methods that address the chip directly.
	 */

	/**
	 * Write an entire control register
	 *
	 * @param address The address of the particular register
	 * @param data The data to write into the particular register
	 */
	void writeToRegister(uint8_t address, uint8_t data);

	void writePairToRegister(uint8_t address, uint8_t first, uint8_t second);

	/**
	 * Read an entire control register
	 *
	 * @param address The address of the particular register
	 * @return The data read from the particular register
	 */
	uint8_t readFromRegister(uint8_t address);

	/**
	 * Set a single bit in a register
	 *
	 * This loads all the values from the device first, sets the correct bit
	 * an pushes them back.
	 *
	 * @param address_bit The address and bit number (0-7)
	 * @param bitState Whether to set the bit (true) or clear it (false)
	 */
	void setBitInRegister(const uint8_t address_bit[], bool bitState);

	/**
	 * Set a single bit in a register
	 *
	 * This loads all the values from the device first, sets the correct bit
	 * an pushes them back.
	 *
	 * For example, to set the bottom 4 bits to B1010, send B1010 as data and
	 * mask as B1111.
	 *
	 * @param address The address of the register
	 * @param data The data that should be set into the bits
	 * @param mask 1's for all bits that should be set.
	 */
	void setBitGroupInRegister(const uint8_t address, const uint8_t data, const uint8_t mask);
	/**}*/

public:
	/**
	 * Constructor
	 *
	 * @param _address the low 3 bits of the address for the chip being
	 * addressed.  This should correspond to the voltage on the ADDR pin.
	 */
	MCP23018(uint8_t _address);

	/**
	 * Setup & prepare
	 *
	 * Call this function from within the setup() function of your
	 * sketch, AFTER Wire.begin().
	 *
	 * @warning This currently sets both ports to be outputs, which is
	 * not very general-purpose.
	 */
	void begin(void);

	/**
	 * Set the pull-up resistors
	 *
	 * The MCP23018 has open-drain inputs, which means if you want to use the
	 * outputs to drive logic inputs on some other device, you need to engage
	 * the pullup resistors.
	 *
	 * @param _a One bit for each line on port 'A', send 1 to enable the pullup, 
	 * 0 to disable
	 * @param _b One bit for each line on port 'B', send 1 to enable the pullup, 
	 * 0 to disable
	 */	
	void SetPullups(uint8_t _a, uint8_t _b);

	/**
	 * Set the outputs on Port A
	 *
	 * @param _data Bits to set
	 */	 
	void SetPortA(uint8_t _data);
	
	/**
	 * Set the outputs on Port B
	 *
	 * @param _data Bits to set
	 */	 
	void SetPortB(uint8_t _data);
	
	void SetPorts(uint8_t _a, uint8_t _b);
	
	uint8_t GetPortA(void);
	uint8_t GetPortB(void);
	uint8_t GetLatchPortA(void);
	uint8_t GetLatchPortB(void);
	void SetPortB_bits(uint8_t _data, uint8_t _mask);
};

	
