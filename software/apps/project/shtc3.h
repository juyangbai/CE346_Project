#pragma once
#include "nrf_twi_mngr.h"

static const uint8_t SHTC3_ADDRESS = 0x70;
static const uint8_t SHTC3_OK = 0x01;
extern float SHTC3_BUFFER_TEMPERATRUE;
extern float SHTC3_BUFFER_HUMIDITY;

typedef enum {
	      SLEEP = 0xB098,
	      WAKEUP = 0x3517,
	      NM_T_RH_CSE = 0x7CA2,
	      NM_RH_T_CSE = 0x5c24,
	      NM_T_RH_CSD = 0x7866,
	      NM_RH_T_CSD = 0x58E0,
	      LP_T_RH_CSE = 0x6458,
	      LP_RH_T_CSE = 0x44DE,
	      LP_T_RH_CSD = 0x609C,
	      LP_RH_T_CSD = 0x401A,
	      SOFT_RESET = 0x805D,
	      READ_ID_REG = 0xEFC8
} shtc3_cmd_t;

void shtc3_init(const nrf_twi_mngr_t* i2c);

void shtc3_measure();

void shtc3_test();
