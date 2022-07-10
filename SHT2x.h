#pragma once
//
//    FILE: SHT2x.h
//  AUTHOR: Rob Tillaart, Viktor Balint
// VERSION: 0.1.4
//    DATE: 2021-09-25
// PURPOSE: Arduino library for the SHT2x temperature and humidity sensor
//     URL: https://github.com/RobTillaart/SHT2x
//


#include "Arduino.h"
#include "Wire.h"


#define SHT2x_LIB_VERSION             (F("0.1.4"))


//  fields getStatus
#define SHT2x_STATUS_OPEN_CIRCUIT     0x00
#define SHT2x_STATUS_TEMPERATURE      0x01
#define SHT2x_STATUS_HUMIDITY         0x02
#define SHT2x_STATUS_CLOSED_CIRCUIT   0x03


//  error codes
//  kept in sync with SHT31 library
#define SHT2x_OK                      0x00
#define SHT2x_ERR_WRITECMD            0x81
#define SHT2x_ERR_READBYTES           0x82
#define SHT2x_ERR_HEATER_OFF          0x83
#define SHT2x_ERR_NOT_CONNECT         0x84
#define SHT2x_ERR_CRC_TEMP            0x85
#define SHT2x_ERR_CRC_HUM             0x86
#define SHT2x_ERR_CRC_STATUS          0x87     // not used
#define SHT2x_ERR_HEATER_COOLDOWN     0x88
#define SHT2x_ERR_HEATER_ON           0x89


class SHT2x
{
public:
  SHT2x();

#if defined(ESP8266) || defined(ESP32)
  bool begin(const int dataPin, const int clockPin);
#endif
  bool begin(TwoWire *wire = &Wire);

  //  check sensor is reachable over I2C
  bool isConnected();

  //  read must be called get getTemperature / getHumidity
  bool read();

  float    getTemperature();
  float    getHumidity();
  uint16_t getRawTemperature() { return _rawTemperature; };
  uint16_t getRawHumidity()    { return _rawHumidity; };

  //  might take up to 15 milliseconds.
  bool reset();

  //  from datasheet HTU20
  //
  //  |  bits  | value  | meaning             |
  //  |:------:|:------:|:--------------------|
  //  |  00    |   0    | open circuit        |
  //  |  01    |   1    | temperature reading |
  //  |  10    |   2    | humidity reading    |
  //  |  11    |   3    | closed circuit      |
  //
  uint8_t getStatus();

  // lastRead is in milliSeconds since start
  uint32_t lastRead() { return _lastRead; };


  //  HEATER
  //  do not use heater for long periods,
  //  use it for max 3 minutes to heat up
  //  and let it cool down at least 3 minutes.
  void    setHeatTimeout(uint8_t seconds);
  uint8_t getHeatTimeout() { return _heatTimeout; };

  bool heatOn();
  bool heatOff();
  bool isHeaterOn();  // is the sensor still heating up?

  bool setHeaterLevel(uint8_t level);    // level = 0..15
  bool getHeaterLevel(uint8_t & level);  // 0..15


  int getError(); // clears error flag

  //  Electronic ID bytes
  uint32_t  getEIDA();
  uint32_t  getEIDB();
  uint8_t   getFirmwareVersion();

private:
  uint8_t   crc8(const uint8_t *data, uint8_t len);

  bool      writeCmd(uint8_t cmd);
  bool      writeCmd(uint8_t cmd, uint8_t value);
  bool      readBytes(uint8_t n, uint8_t *val, uint8_t maxDuration);
  TwoWire* _wire;

  uint8_t   _heatTimeout;   // seconds
  uint32_t  _lastRead;
  uint32_t  _lastRequest;   // for async interface
  uint32_t  _heaterStart;
  uint32_t  _heaterStop;
  bool      _heaterOn;

  uint16_t  _rawHumidity;
  uint16_t  _rawTemperature;

  uint8_t   _status;

  uint8_t   _error;
};



////////////////////////////////////////////////////////
//
//  DERIVED SHT
//
class SHT20 : public SHT2x
{
public:
  SHT20();
};


class SHT21 : public SHT2x
{
public:
  SHT21();
};


class SHT25 : public SHT2x
{
public:
  SHT25();
};


////////////////////////////////////////////////////////
//
//  DERIVED HTU
//
class HTU20 : public SHT2x
{
public:
  HTU20();
};


class HTU21 : public SHT2x
{
public:
  HTU21();
};


////////////////////////////////////////////////////////
//
//  DERIVED Si70xx
//
class Si7013 : public SHT2x
{
public:
  Si7013();
};


class Si7020 : public SHT2x
{
public:
  Si7020();
};


class Si7021 : public SHT2x
{
public:
  Si7021();
};


// -- END OF FILE --
