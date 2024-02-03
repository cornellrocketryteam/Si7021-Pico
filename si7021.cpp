#include "si7021.hpp"
#include <cstdio>

Si7021::Si7021(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool Si7021::begin() {
    if (!reset()) {
        return false;
    }

    sleep_ms(100);

    uint8_t buf[8];
    if (!get_id(buf)) {
        return false;
    }

    if (buf[4] != SI7021_DEVICE_ID) {
#ifdef DEBUG
        fprintf(stderr, "Incorrect device ID: %d\n", buf[4]);
#endif
        return false;
    }
    return true;
}

bool Si7021::read_temperature(float *temperature) {
    int ret;
    uint8_t temp_cmd[1] = {SI7021_CMD_TEMP_NO_HOLD};
    uint8_t temp_buf[2];

    ret = i2c_write_blocking(i2c, SI7021_ADDR, temp_cmd, 1, true);
    if (ret < 1) {
        return false;
    }

    sleep_ms(10);
    ret = i2c_read_blocking(i2c, SI7021_ADDR, temp_buf, 2, false);
    if (ret < 1) {
        return false;
    }

    uint16_t temp_int = (temp_buf[0] << 8) | temp_buf[1];

    *temperature = (175.72 * (float)temp_int / 65536) - 46.85;
    return true;
}

bool Si7021::read_humidity(float *humidity) {
    int ret;
    uint8_t hum_cmd[1] = {SI7021_CMD_HUM_NO_HOLD};
    uint8_t hum_buf[2];

    ret = i2c_write_blocking(i2c, SI7021_ADDR, hum_cmd, 1, true);
    if (ret < 1) {
        return false;
    }

    sleep_ms(20);
    ret = i2c_read_blocking(i2c, SI7021_ADDR, hum_buf, 2, false);
    if (ret < 1) {
        return false;
    }

    uint16_t hum_int = (hum_buf[0] << 8) | hum_buf[1];

    *humidity = (125 * (float)hum_int / 65536) - 6;
    return true;
}

bool Si7021::get_id(uint8_t *buf) {
    int ret;
    uint8_t buf_id_0[2] = {SI7021_CMD_ID_0_0, SI7021_CMD_ID_0_1};
    uint8_t buf_id_1[2] = {SI7021_CMD_ID_1_0, SI7021_CMD_ID_1_1};

    uint8_t read_buf[4];

    ret = i2c_write_blocking(i2c, SI7021_ADDR, buf_id_0, 2, true);
    if (ret < 1) {
        return false;
    }

    ret = i2c_read_blocking(i2c, SI7021_ADDR, read_buf, 4, false);
    if (ret < 1) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        buf[i] = read_buf[i];
    }

    ret = i2c_write_blocking(i2c, SI7021_ADDR, buf_id_1, 2, true);
    if (ret < 1) {
        return false;
    }

    ret = i2c_read_blocking(i2c, SI7021_ADDR, read_buf, 4, false);
    if (ret < 1) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        buf[i + 4] = read_buf[i];
    }

    return true;
}

bool Si7021::reset() {
    uint8_t buf[1] = {SI7021_CMD_RESET};

    int ret = i2c_write_blocking(i2c, SI7021_ADDR, buf, 1, false);
    if (ret < 1) {
#ifdef DEBUG
        fprintf(stderr, "Chip reset failed\n");
#endif
        return false;
    }

    return true;
}