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
#include "My_DHT_Using_Opp.h"


int dht::confirm(int pin, int us, byte level) {
    // wait one more count to ensure.
    int cnt = us / 10 + 1;
    bool ok = false;
    for (int i = 0; i < cnt; i++) {
        if (digitalRead(pin) != level) {
            ok = true;
            break;
        }
        delayMicroseconds(10);
    }
    if (!ok) {
        return -1;
    }
    return 0;
}

byte dht::bits2byte(byte data[8]) {
    byte v = 0;
    for (int i = 0; i < 8; i++) {
        v += data[i] << (7 - i);
    }
    return v;
}

int dht::sample(int pin, byte data[40]) {
    // empty output data.
    memset(data, 0, 40);

    // notify DHT11 to start: 
    //    1. PULL LOW 20ms.
    //    2. PULL HIGH 20-40us.
    //    3. SET TO INPUT.
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(20);
    digitalWrite(pin, HIGH);
    delayMicroseconds(30);
    pinMode(pin, INPUT);

    // DHT11 starting:
    //    1. PULL LOW 80us
    //    2. PULL HIGH 80us
    if (confirm(pin, 80, LOW)) {
        return DHTLIB_ERROR_TIMEOUT;
    }
    if (confirm(pin, 80, HIGH)) {
        return DHTLIB_ERROR_TIMEOUT;
    }

    // DHT11 data transmite:
    //    1. 1bit start, PULL LOW 50us
    //    2. PULL HIGH 26-28us, bit(0)
    //    3. PULL HIGH 70us, bit(1)
    for (int j = 0; j < 40; j++) {
        if (confirm(pin, 50, LOW)) {
            return DHTLIB_ERROR_TIMEOUT;
        }

        // read a bit, should never call method,
        // for the method call use more than 20us,
        // so it maybe failed to detect the bit0.
        bool ok = false;
        int tick = 0;
        for (int i = 0; i < 8; i++, tick++) {
            if (digitalRead(pin) != HIGH) {
                ok = true;
                break;
            }
            delayMicroseconds(10);
        }
        if (!ok) {
            return DHTLIB_ERROR_TIMEOUT;
        }
        data[j] = (tick > 3? 1:0);
    }

    // DHT11 EOF:
    //    1. PULL LOW 50us.
    if (confirm(pin, 50, LOW)) {
        return DHTLIB_ERROR_TIMEOUT;
    }

    return 0;
}

int dht::parse(byte data[40], byte* ptemperature, byte* phumidity) {
    byte humidity = bits2byte(data);
    byte humidity2 = bits2byte(data + 8);
    byte temperature = bits2byte(data + 16);
    byte temperature2 = bits2byte(data + 24);
    byte check = bits2byte(data + 32);
    byte expect = humidity + humidity2 + temperature + temperature2;
    if (check != expect) {
        return DHTLIB_ERROR_CHECKSUM;
    }
    *ptemperature = temperature;
    *phumidity = humidity;
    return 0;
}

int dht::read(int pin, byte* ptemperature, byte* phumidity, byte pdata[40]) {
    int ret = 0;

    byte data[40] = {0};
    if ((ret = sample(pin, data)) != 0) {
        return ret;
    }

    byte temperature = 0;
    byte humidity = 0;
    if ((ret = parse(data, &temperature, &humidity)) != 0) {
        return ret;
    }

    if (pdata) {
        memcpy(pdata, data, 40);
    }
    if (ptemperature) {
        *ptemperature = temperature;
    }
    if (phumidity) {
        *phumidity = humidity;
    }

    return ret;
}
int dht::read11(uint8_t pin)
{
    // READ VALUES
    temperature = 0;
  	humidity = 0;
    int rv = read(pin, &temperature, &humidity, NULL);
    return rv;
}

