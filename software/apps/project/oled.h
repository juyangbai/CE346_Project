#pragma once

#include "nrf_twi_mngr.h"

// Chip address for OLED
static const uint8_t OLED_ADDRESS_SA0 = 0x3C;
static const uint8_t OLED_ADDRESS_SA1 = 0x3D;

typedef enum {
  CTRL_CMD = 0x00,
  CTRL_DATA = 0x40,
} oled_control_cmd_t;

// Fundamental Command Table
typedef enum{
  // 0x7F = 127
  SET_CONTRAST_CONTROL = 0x81,
  ENTIRE_DISPLAY_ON_FOLLOW_CONTENT = 0xA4,
  ENTIRE_DISPLAY_ON_IGNORE_CONTENT = 0xA5,
  SET_NORMAL_DISPLAY = 0xA6,
  SET_INVERSE_DISPLAY = 0xA7,
  SET_DISPLAY_ON = 0xAF,
  SET_DISPLAY_OFF = 0xAE
} oled_fdmtl_cmd_t;

// Addressing Seting Command Table
typedef enum{
  SET_MEMORY_ADDRESSING_MODE = 0x20,
  SET_COLUMN_ADDRESS_HAM_VAM = 0x21,
  SET_PAGE_ADDRESS_HAM_VAM = 0x22
} oled_address_cmd_t;


void oled_init(const nrf_twi_mngr_t* i2c);
void oled_draw_data(int data_gas, int data_tvoc, double data_temp, double data_humid);
void oled_adv_draw(void);
