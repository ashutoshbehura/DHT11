    //
//    FILE: dht.h
//  AUTHOR: Ashutosh Behura & Harsh
// VERSION: 0.1
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: 
//
// HISTORY:
// see MyDHT11.cpp file
//

#ifndef dht_h
#define dht_h

#if ARDUINO < 100
#include <WProgram.h>
#include <pins_arduino.h>  // fix for broken pre 1.0 version - TODO TEST
#else
#include <Arduino.h>
#endif

#define DHT_LIB_VERSION "0.1.22"

#define DHTLIB_OK                   0
#define DHTLIB_ERROR_CHECKSUM       -1
#define DHTLIB_ERROR_TIMEOUT        -2

// max timeout is 100 usec.
// For a 16 Mhz proc 100 usec is 1600 clock cycles
// loops using DHTLIB_TIMEOUT use at least 4 clock cycli
// so 100 us takes max 400 loops
// so by dividing F_CPU by 40000 we "fail" as fast as possible
#ifndef F_CPU
#define DHTLIB_TIMEOUT 1000  // ahould be approx. clock/40000
#else
#define DHTLIB_TIMEOUT (F_CPU/40000)
#endif

class dht
{
public:
    dht() {};
    // return values:
    // DHTLIB_OK
    // DHTLIB_ERROR_CHECKSUM
    // DHTLIB_ERROR_TIMEOUT
    // DHTLIB_ERROR_CONNECT
    // DHTLIB_ERROR_ACK_L
    // DHTLIB_ERROR_ACK_H
    int read11(uint8_t pin);
    byte humidity;
    byte temperature;

private:
    uint8_t bits[5];  // buffer to receive data
    int8_t read(uint8_t pin);
	int confirm(int pin, int us, byte level);
	byte bits2byte(byte data[8]);
	int sample(int pin, byte data[40]);
	int parse(byte data[40], byte* ptemperature, byte* phumidity) ;
	int read(int pin, byte* ptemperature, byte* phumidity, byte pdata[40]) ;
	
};
#endif
//
// END OF FILE
//
