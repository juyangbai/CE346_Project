#pragma once

#include "nrf_twi_mngr.h"

// Chip addresses for SGP30 Air Quality
static const uint8_t SGP30_ADDRESS = 0x58;
static const uint8_t SGP30_OK = 0x01;
extern uint32_t sgp30_buffer;

// Register definitions for magnetometer
typedef enum {
  INIT_AIR_QUALITY = 0x2003,
  MEASURE_AIR_QUALITY = 0x2008,
  GET_BASELINE = 0x2005,
  SET_BASELINE = 0x201e,
  SET_HUMIDITY = 0x2061,
  MEASURE_TEST = 0x2032,
  GET_FEATURE_SET_VERSION = 0x202f,
  MEASURE_RAW_SIGNALS = 0x2050,
  RESET = 0x0006,
  GET_SERIAL_ID =  0x3682
} sgp30_cmd_t;

// Function prototypes

// Initialize and configure SGP30
//
// i2c - pointer to already initialized and enabled twim instance
// exp:- void lsm303agr_init(const nrf_twi_mngr_t* i2c);

void sgp30_init(const nrf_twi_mngr_t* i2c);

void sgp30_measure();

// Read the external quality sensor
//
// Return measurement as floating point value in UNIT NEEDED
void sgp30_test(void);
