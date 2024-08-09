#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/adc.h"
#include "pico/stdlib.h"

#include "soil_sensor.h"

const uint8_t PIN_ADC = 29;
const uint16_t RANGE_MAX = 1400;
const uint16_t RANGE_MIN = 3200;

void soil_sensor_init() {
    adc_init();
    adc_gpio_init(PIN_ADC);
    adc_select_input(3);
}

float map_within(uint16_t min_value, uint16_t max_value, uint16_t value) {
    float range = max_value - min_value;
    float result = (float)(value - min_value) / range;
    return result * 100.0f;
}

float soil_sensor_read() {
    uint16_t value = adc_read();
    printf("raw value %d", value);
    float mapped = map_within(RANGE_MIN, RANGE_MAX, value);
    return mapped;
}