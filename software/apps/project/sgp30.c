#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <sgp30.h>
#include "nrf_delay.h"
#include <inttypes.h>

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;
uint32_t sgp30_buffer = 0;

// ASSUME: WE WRITE A COMMAND TO THE DEVICE, it will be able to provide data
static uint64_t sgp30_read(uint8_t i2c_addr, uint16_t count) {
  uint64_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
						   NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, count, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
  return rx_buf;
}

static void sgp30_write(uint8_t i2c_addr, uint16_t command) {
  uint8_t MSB = (command >> 8) & 0xff;
  uint8_t LSB = command & 0xff;
  uint8_t concat[] = {MSB, LSB};
  nrf_twi_mngr_transfer_t const write_transfer[] = {
				                    NRF_TWI_MNGR_WRITE(i2c_addr, concat, 2, NRF_TWI_MNGR_NO_STOP)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
}

void sgp30_init(const nrf_twi_mngr_t* i2c){
  i2c_manager = i2c;
  sgp30_test();
  sgp30_iaq();
}



void sgp30_iaq(){
  sgp30_write(SGP30_ADDRESS, INIT_AIR_QUALITY);
  nrf_delay_ms(10);
}

void sgp30_measure(){
  sgp30_write(SGP30_ADDRESS, MEASURE_AIR_QUALITY);
  nrf_delay_ms(12);
  uint64_t raw = sgp30_read(SGP30_ADDRESS, 6);
  uint32_t co2 = raw & 0xffffff;
  uint32_t tvoc = (raw >> 24) & 0xffffff;


  uint16_t co2_MSB = co2 & 0x0000ff;
  uint16_t co2_LSB = co2 & 0x00ff00;
  uint16_t co2_act = (co2_MSB << 8) | (co2_LSB >> 8);

  uint16_t tvoc_MSB = tvoc & 0x0000ff;
  uint16_t tvoc_LSB = tvoc & 0x00ff00;
  uint16_t tvoc_act = (tvoc_MSB << 8) | (tvoc_LSB >> 8);

  //printf("raw data: 0x%" PRIx64 "\n", raw);
  //printf("CO2: %d ppm, TVOC: %d ppb\n", co2_act, tvoc_act);

  sgp30_buffer = (co2_act << 16) | tvoc_act;
  //printf("CO2: %d ppm, TVOC: %d ppb\n", ((sgp30_buffer & 0xffff0000) >> 16), (sgp30_buffer & 0x0000ffff));
}

void sgp30_test(){
  sgp30_write(SGP30_ADDRESS, MEASURE_TEST);
  nrf_delay_ms(220);
  uint32_t measureTest = sgp30_read(SGP30_ADDRESS, 3);
  if (measureTest != 0xc600d4){
    printf("Measure Test Failed.\n");
  }else{
    printf("Measure Test Succeeded.\n");
  }
}
