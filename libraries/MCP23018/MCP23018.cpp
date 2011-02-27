/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <Wire.h>
#include "MCP23018.h"

#undef DEBUG

// MCP23018 address on I2C bus

const uint8_t I2C_MCP23018 = B0100000;

// MCP23018 registers

const uint8_t IODIRA = 0x0;
const uint8_t IODIRB = 0x1;
const uint8_t IOCON = 0xA;
const uint8_t IOCON_SEQOP[] = { IOCON, 5 };
const uint8_t GPPUA = 0x0C;
const uint8_t GPPUB = 0x0D;
const uint8_t GPIOA = 0x12;
const uint8_t GPIOB = 0x13;
const uint8_t OLATA = 0x14;
const uint8_t OLATB = 0x15;

void MCP23018::writeToRegister(uint8_t address, uint8_t data)
{
	Wire.beginTransmission(i2c_address);
	Wire.send(address);
	Wire.send(data);
	Wire.endTransmission();

#ifdef DEBUG
	Serial.print(">");
	Serial.print(address,HEX);
	Serial.print(">");
	Serial.println(data,HEX);
#endif
	
	readFromRegister(address);
}

void MCP23018::writePairToRegister(uint8_t address, uint8_t first_data, uint8_t second_data)
{
	Wire.beginTransmission(i2c_address);
	Wire.send(address);
	Wire.send(first_data);
	Wire.send(second_data);
	Wire.endTransmission();
	
#ifdef DEBUG
	Serial.print(">");
	Serial.print(address,HEX);
	Serial.print(">");
	Serial.println(data,HEX);
#endif
	
	readFromRegister(address);
}

uint8_t MCP23018::readFromRegister(uint8_t address)
{
	uint8_t received_data = 0;
	
	// Establish connection, select receipt address
	Wire.beginTransmission(i2c_address);
	Wire.send(address);
	Wire.endTransmission();
	
	// Request one data byte
	Wire.requestFrom(i2c_address, (uint8_t)1);
	
#ifdef DEBUG
	Serial.print("<");
	Serial.print(address,HEX);
	Serial.print("<");
#endif
	
	// Fill variables when ready
	if(Wire.available())
	{
		received_data = Wire.receive();
#ifdef DEBUG
		Serial.println(received_data ,HEX);
#endif
}
	
#ifdef DEBUG
	else
	  Serial.println("?");
#endif
	
	return received_data;
}

void MCP23018::setBitInRegister(const uint8_t address_bit[], bool bitState)
{
	uint8_t temp;
	uint8_t address = address_bit[0];
	uint8_t bit = address_bit[1];

	// Use bitState to decide which masking to use (to 1 or to 0)
	if(bitState)
		temp = readFromRegister(address) | (1 << bit);
	else
		temp = readFromRegister(address) & ~(1 << bit);
	
	writeToRegister(address, temp);
}

void MCP23018::setBitGroupInRegister(const uint8_t address, const uint8_t data, const uint8_t mask)
{
#ifdef DEBUG
    Serial.print("()setBitGroupInRegister(");
    Serial.print(address,HEX);
    Serial.print(",");
    Serial.print(data,HEX);
    Serial.print(",");
    Serial.print(mask,HEX);
    Serial.println(")");
#endif
	
	uint8_t temp = readFromRegister(address) & ( mask ^ 0xff) | ( data & mask );	
	writeToRegister(address, temp);
}

MCP23018::MCP23018(uint8_t _address)
{
	i2c_address = ( _address & B111 ) | I2C_MCP23018;
}

void MCP23018::begin(void)
{
	// Set all pins to outputs
	writePairToRegister(IODIRA,0,0);
}

void MCP23018::SetPullups(uint8_t _a, uint8_t _b)
{
	writePairToRegister(GPPUA,_a,_b);  
}

void MCP23018::SetPortA(uint8_t _data)
{
	writeToRegister(GPIOA,_data);
}

void MCP23018::SetPortB(uint8_t _data)
{
	writeToRegister(GPIOB,_data);
}

void MCP23018::SetPorts(uint8_t _a, uint8_t _b)
{
	writePairToRegister(GPIOA,_a,_b);
}

uint8_t MCP23018::GetPortA(void)
{
	return readFromRegister(GPIOA);
}

uint8_t MCP23018::GetPortB(void)
{
	return readFromRegister(GPIOB);
}

uint8_t MCP23018::GetLatchPortA(void)
{
	return readFromRegister(OLATA);
}

uint8_t MCP23018::GetLatchPortB(void)
{
	return readFromRegister(OLATB);
}

void MCP23018::SetPortB_bits(uint8_t _data, uint8_t _mask)
{
	setBitGroupInRegister(OLATB,_data,_mask);
}