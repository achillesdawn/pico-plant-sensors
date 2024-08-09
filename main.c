#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include "bh1750/bh1750.h"
#include "dht/dhtlib.h"
#include "soil_sensor/soil_sensor.h"

#include "rust/bindings.h"

const uint8_t PIN_LED_BLUE = 14;
const uint8_t DHT_PIN = 2;

const uint8_t PIN_SDA = 0;
const uint8_t PIN_SCL = 1;

const uint32_t PIN_MASK = (1u << PIN_LED_BLUE) | (1u << DHT_PIN);



volatile bool led_state = false;

bool repeating_toogle_led() {
    led_state = !led_state;
    gpio_put(PIN_LED_BLUE, led_state);
    return true;
}

struct repeating_timer *setup_led() {
    printf("initializing timer\n");
    gpio_set_dir(PIN_LED_BLUE, true);
    gpio_put(PIN_LED_BLUE, true);

    struct repeating_timer *led_timer =
        calloc(1, sizeof(struct repeating_timer));

    add_repeating_timer_ms(500, repeating_toogle_led, NULL, led_timer);
    printf("timer initialized\n");
    return led_timer;
}



void setup_i2c() {
    printf("initializing i2c0 on pin SDA %d and SCL %d\n", PIN_SDA, PIN_SCL);
    i2c_init(i2c0, 400 * 1000);

    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(PIN_SDA);
    gpio_pull_up(PIN_SCL);

    printf("i2c initialized\n");
}

int main() {

    stdio_init_all();

    sleep_ms(2000);

    printf("initializing\n");

    gpio_init_mask(PIN_MASK);
    gpio_set_dir_out_masked(PIN_MASK);
    gpio_put_masked(PIN_MASK, PIN_MASK);

    struct repeating_timer *led_timer = setup_led();

    soil_sensor_init();
    setup_i2c();
    bh1750_reset();

    while (true) {
        float soil_humidity = soil_sensor_read();
        printf("soil humidity: %f\n", soil_humidity);
        uint16_t lux = bh1750_read_lux();
        printf("lux: %d\n", lux);
        sleep_ms(500);
        DhtData *data = dht_init_sequence(DHT_PIN);
    }
}
