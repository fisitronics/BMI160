#include <stdio.h>
#include "BMI160.h"
#include "esp_log.h"
#include "freertos/task.h"  // Incluir para usar vTaskDelay


extern "C" void app_main(void) {
    BMI160 bmi160(SPI3_HOST, 5);  // SPI3_HOST y pin CS en GPIO5

    // Lectura del registro 0x00 después de la sincronización SPI
    uint8_t reg_value = bmi160.readRegister(0x00);
    printf("Valor del registro 0x00: 0x%02X\n", reg_value);

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}