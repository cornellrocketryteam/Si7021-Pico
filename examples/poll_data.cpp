#include "../si7021.hpp"
#include "tusb.h"
#include <cstdio>

#define I2C_PORT i2c0
#define I2C_SDA 12
#define I2C_SCL 13

Si7021 therm(I2C_PORT);

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    if (therm.begin()) {
        printf("Init successful\n");
    } else {
        printf("Init failed\n");
        return 1;
    }

    float temp, humidity;
    bool ret;

    while (true) {
        ret = therm.read_temperature(&temp);
        if (!ret) {
            printf("Temperature reading failed\n");
        }

        ret = therm.read_humidity(&humidity);
        if (!ret) {
            printf("Humidity reading failed\n");
        }

        printf("Temperature: %.3f C\n", temp);
        printf("Humidity: %.3f %%\n", humidity);
        sleep_ms(200);
    }

    return 0;
}