#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <shtc3.h>
#include "nrf_delay.h"
#include <inttypes.h>

static const nrf_twi_mngr_t* i2c_manager = NULL;
float SHTC3_BUFFER_TEMPERATRUE = 0;
float SHTC3_BUFFER_HUMIDITY = 0;

static uint64_t shtc3_read(uint8_t i2c_addr, uint16_t count) {
  uint64_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
						   NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, count, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
  return rx_buf;
}

static void shtc3_write(uint8_t i2c_addr, uint16_t command) {
  uint8_t MSB = (command >> 8) & 0xff;
  uint8_t LSB = command & 0xff;
  uint8_t concat[] = {MSB, LSB};
  nrf_twi_mngr_transfer_t const write_transfer[] = {
				                    NRF_TWI_MNGR_WRITE(i2c_addr, concat, 2, NRF_TWI_MNGR_NO_STOP)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
}

void shtc3_sleep(){
  shtc3_write(SHTC3_ADDRESS, SLEEP);
  nrf_delay_ms(13);
}

void shtc3_wakeup(){
  shtc3_write(SHTC3_ADDRESS, WAKEUP);
  nrf_delay_ms(13);
}

void shtc3_init(const nrf_twi_mngr_t* i2c){
  i2c_manager = i2c;
  shtc3_sleep();
  shtc3_test();
}

void shtc3_test(){
  shtc3_wakeup();
  shtc3_write(SHTC3_ADDRESS, READ_ID_REG);
  nrf_delay_ms(13);
  uint32_t ID = shtc3_read(SHTC3_ADDRESS, 3);
  printf("SHTC3 ID is %lx\n", ID);
  // 0101'1011'1000'0111  00001000

  shtc3_sleep();
}

// Try CSE
void shtc3_measure(){
  shtc3_wakeup();
  shtc3_write(SHTC3_ADDRESS, NM_T_RH_CSE);
  nrf_delay_ms(13);
  uint64_t raw = shtc3_read(SHTC3_ADDRESS,6);
  // printf("SHTC3 RAW DATA: %" PRIx64 "\n", raw);

  uint32_t temperature = raw & 0xffffff;
  uint32_t humidity = (raw >> 24) & 0xffffff;

  uint16_t temperature_MSB = temperature & 0x0000ff;
  uint16_t temperature_LSB = temperature & 0x00ff00;
  uint16_t temperature_val = (temperature_MSB << 8) | (temperature_LSB >> 8);
  float temperature_cov = ((int)temperature_val * 175.0) / 65526 - 45;
  SHTC3_BUFFER_TEMPERATRUE = temperature_cov;

  uint16_t humidity_MSB = humidity & 0x0000ff;
  uint16_t humidity_LSB = humidity & 0x00ff00;
  uint16_t humidity_val = (humidity_MSB << 8) | (humidity_LSB >> 8);
  float humidity_cov = (int)humidity_val * 100.0 / 65536;
  SHTC3_BUFFER_HUMIDITY = humidity_cov;

  // printf("temperature: %.2f, humidity: %.2f%%\n", temperature_cov, humidity_cov);
}
