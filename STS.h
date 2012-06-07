/**
 * Copyright (c) 2012 Jose A. J. Berni. All rights reserved.
 *
 * This file is part of STS-Arduino.
 *
 * STS-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * STS-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with STS-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef STS_h
#define STS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <inttypes.h>

#define MAX_FRAME_DATA_SIZE 2500


#define START_BYTE_1				0xc1
#define START_BYTE_2				0xc0

#define HEADER_BYTES				0xc0c1
#define FOOTER_BYTES				0xc2c3c4c5
#define PROTOCOL_VERSION 			0x1000



//Implemented messages
#define MSG_GET_HW_VERSION			0x00000080
#define MSG_GET_SW_VERSION			0x00000090
#define MSG_GET_SERIAL_NUMBER		0x00000100

#define MSG_GET_CORRECTED_SPECTRUM	0x00101000
#define MSG_SET_INTEGRATION_TIME	0x00110010

#define MSG_GET_AVG_SCANS			0x00110510
#define MSG_SET_AVG_SCANS			0x00120010

#define MSG_GET_TEMPERATURE			0X00400001

// Error codes
#define NO_ERROR 0
#define CHECKSUM_FAILURE 1
#define PACKET_EXCEEDS_BYTE_ARRAY_LENGTH 2
#define UNEXPECTED_START_BYTE 3

uint32_t msgType;

class STSMessage {
public:
	
	/**
	 * Default constructor
	 */
	STSMessage();
	/**
	 * Returns Protocol Version of the response
	 */
	uint16_t getProtocolVer();
	void setProctocolVer(uint16_t protocolVer);
	/**
	 * Returns Protocol Version of the response
	 */
	uint16_t getFlags();
	void setFlags(uint16_t protocolVer);
	/**
	 * Returns Protocol Version of the response
	 */
	uint16_t getErrorNumber();
	void setErrorNumber(uint16_t protocolVer);
	/**
	 * Returns Msg Id of the response
	 */
	uint32_t getMsgId();
	void setMsgId(uint32_t apiId);
	
	/**
	 * Returns Regarding ID of the response
	 */
	uint32_t getRegardingId();
	void setRegardingId(uint32_t apiId);
	
	
	/**
	 * Returns the payload length derived from the byte reamining field
	*/
	uint32_t getPayloadLength();
	void setPyalodLenght(uint32_t apiId);
	
	/**
	 * Returns the packet checksum Type: 0 no checksum, 1 MD5 (16 byte)
	 */
	uint8_t getChecksumType();
	void setChecksumType(uint8_t checksum);
		
	
	/**
	 * Returns the length of the packet
	 */
	uint16_t getPacketLength();
	
	/**
	 * Resets the message to default values
	 */
	void reset();
	/**
	 * Initializes the response
	 */
	 
	void init();
	bool isAvailable();
	void setAvailable(bool complete);
	
	/**
	 * Returns true if the response contains errors
	 */
	 
	bool isError();
	
	/**
	 * Returns an error code, or zero, if successful.
	 * Error codes include: CHECKSUM_FAILURE, PACKET_EXCEEDS_BYTE_ARRAY_LENGTH, UNEXPECTED_START_BYTE
	 */
	uint8_t getErrorCode();
	void setErrorCode(uint8_t errorCode);

protected:
	// pointer to frameData
	uint8_t* _frameDataPtr;
	
private:
	uint8_t _msgId;
	uint8_t _msbLength;
	uint8_t _lsbLength;
	uint8_t[] _checksum={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t _frameLength;
	
	bool _complete;
	uint8_t _errorCode;
};


class STSSpectrum {
public:
	STSSpectrum();
	
	
	

}

class STS {
public:
	STS();
	
	
	
	/**
	 * Reads all available serial bytes until a packet is parsed, an error occurs, or the buffer is empty.
	 * 
	 * <p/>
	 * This method should always return quickly since it does not wait for serial data to arrive.
	 * You will want to use this method if you are doing other timely stuff in your loop, where
	 * a delay would cause problems.
	 * NOTE: calling this method resets the current response, so make sure you first consume the
	 * current response
	 */ 
	 
	void readPacket();
	/**
	 * Waits a maximum of <i>timeout</i> milliseconds for a response packet before timing out; returns true if packet is read.
	 * Returns false if timeout or error occurs.
	 */
	bool readPacket(int timeout);
	
	/**
	 * Reads until a packet is received or an error occurs.
	 * Caution: use this carefully since if you don't get a response, your Arduino code will hang on this
	 * call forever!! often it's better to use a timeout: readPacket(int)
	 */
	void readPacketUntilAvailable();
	/**
	 * Starts the serial connection at the supplied baud rate
	 */
	void begin(long baud);
	void getResponse(STSResponse &response);
	/**
	 * Returns a reference to the current response
	 * Note: once readPacket is called again this response will be overwritten!
	 */
	STSResponse& getResponse();
	/**
	 * Sends a STSMessage out the serial port
	 */
	void send(STSMessage &message);
	
	/**
	 * Sends the request corrected spectrum message to the STS.
	 * 
	 */
	void askSpectrum();
	
	/**
	 * Sets the integration time in us.
	 * 
	 */
	void setIntegrationTime(uint16_t integrationTime);

	/**
	 * Specify the serial port.  Only relevant for Arduinos that support multiple serial ports (e.g. Mega)
	 */
	void setSerial(HardwareSerial &serial);
	
private:
	bool available();
	uint8_t read();
	void flush();
	void write(uint8_t val);
	void sendByte(uint8_t b);
	void sendU16(uint16_t b);
	void sendU32(uint32_t b);
	void resetResponse();
	STSResponse _response;
	// current packet position for response.  just a state variable for packet parsing and has no relevance for the response otherwise
	uint8_t _pos;
	// last byte read
	uint8_t b;
	uint8_t _checksumTotal;

	// buffer for incoming RX packets.  holds only the api specific frame data, starting after the api id byte and prior to checksum
	uint8_t _responseFrameData[MAX_FRAME_DATA_SIZE];
	HardwareSerial* _serial;
};

