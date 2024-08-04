#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include "bh1750.h"

const uint8_t POWER_DOWN = 0x00;
const uint8_t POWER_ON = 0x01;
const uint8_t RESET = 0x07;
const uint8_t CONTINUOUS_HIGH = 0x10;
const uint8_t CONTINOUS_LOW = 0x13;
const uint8_t ONE_TIME_HIGH = 0x20;
const uint8_t ONE_TIME_LOW = 0x23;

const uint16_t BH1750_addr = 0x23;
uint8_t buffer[2];

void bh1750_reset() {
    i2c_write_blocking(i2c0, BH1750_addr, &RESET,1, false);
}

uint16_t bh1750_read_lux() {

    uint16_t lux = 0;

    i2c_write_blocking(i2c0, BH1750_addr, &ONE_TIME_HIGH, 1, true);
    sleep_ms(200);

    i2c_read_blocking(i2c0, BH1750_addr, buffer, 2, false);
    
    lux = buffer[0] << 8 | buffer[1];
    printf("lux: %d\n", lux);

    return lux;
}