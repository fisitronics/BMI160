#ifndef BMI160_H
#define BMI160_H

#include "driver/spi_master.h"

class BMI160 {
public:
    BMI160(spi_host_device_t spi_host, int pin_cs);
    ~BMI160();
    uint8_t readRegister(uint8_t reg);

private:
    spi_device_handle_t spi;
    int cs_pin;
    spi_host_device_t spi_host;  // Almacenar el host SPI
    void initSPI();
};

#endif // BMI160_H