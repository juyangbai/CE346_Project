#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <app_timer.h>
#include <sgp30.h>
#include <shtc3.h>
#include <oled.h>

#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

// Define data
int sgp30_data;

int main(void) {
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;

  // Change 1: to the EDGE 19 and 20
  i2c_config.scl = EDGE_P19;
  i2c_config.sda = EDGE_P20;

  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Initialize the LSM303AGR accelerometer/magnetometer sensor
  sgp30_init(&twi_mngr_instance);
  shtc3_init(&twi_mngr_instance);
  oled_init(&twi_mngr_instance);

  // //TODO: implement me!
  APP_TIMER_DEF(my_timer_1);
  app_timer_init();
  app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, oled_adv_draw);
  app_timer_start(my_timer_1, 32768/2, NULL);


  // Loop forever
  while (1) {
    // // Don't put any code in here. Instead put periodic code in a callback using a timer.
    //nrf_delay_ms(1000);
    // printf("FROM MAIN - CO2: %ld ppm, TVOC: %ld ppb\n", ((sgp30_buffer & 0xffff0000) >> 16), (sgp30_buffer & 0x0000ffff));
    // printf("temperature: %.2f, humidity: %.2f%%\n", SHTC3_BUFFER_TEMPERATRUE, SHTC3_BUFFER_HUMIDITY);

    //oled_draw_data((sgp30_buffer & 0xffff0000) >> 16, sgp30_buffer & 0x0000ffff, SHTC3_BUFFER_TEMPERATRUE, SHTC3_BUFFER_HUMIDITY);
  }
}
