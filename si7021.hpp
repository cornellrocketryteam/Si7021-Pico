#ifndef SI7021_HPP
#define SI7021_HPP

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define SI7021_ADDR (0x40)

#define SI7021_DEVICE_ID (0x15)

#define SI7021_CMD_HUM_NO_HOLD (0xF5)
#define SI7021_CMD_TEMP_NO_HOLD (0xF3)

#define SI7021_CMD_RESET (0xFE)

#define SI7021_CMD_ID_0_0 (0xFA)
#define SI7021_CMD_ID_0_1 (0x0F)

#define SI7021_CMD_ID_1_0 (0xFC)
#define SI7021_CMD_ID_1_1 (0xC9)

class Si7021 {
public:
    /**
     * Initializes an Si7021 object on an I2C bus.
     * @param i2c_type The I2C bus that this sensor is on
     */
    Si7021(i2c_inst_t *i2c_type);

    /**
     * Attempts to establish a connection with the sensor and initiates a software reset.
     * @return True on successful connection, false otherwise
     */
    bool begin();

    /**
     * Reads a temperature value in degrees celsius.
     * @param temperature The resulting temperature
     * @return True on successful read, false otherwise
     */
    bool read_temperature(float *temperature);

    /**
     * Reads a relative humidity value.
     * @param humidity The resulting humidity
     * @return True on successful read, false otherwise
     */
    bool read_humidity(float *humidity);

    /**
     * Triggers a software reset of the Si7021.
     * @return True on successful reset, false otherwise
     */
    bool reset();

private:
    /**
     * Reads the Si7021's electronic ID.
     * @param buf The resulting electronic ID
     * @return True on successful read, false otherwise
     */
    bool get_id(uint8_t *buf);

    /**
     * Return value for I2C reads and writes.
     */
    int ret;

    /**
     * The I2C bus.
     */
    i2c_inst_t *i2c;
};

#endif // SI7021_HPP