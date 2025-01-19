#include "BMI160.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <cstring>  // Incluir para usar memset

BMI160::BMI160(spi_host_device_t spi_host, int pin_cs) : cs_pin(pin_cs), spi_host(spi_host) {
    initSPI();
    // Realizar una lectura dummy del registro 0x7F para sincronizar la comunicación SPI
    readRegister(0x7F);
}

BMI160::~BMI160() {
    spi_bus_remove_device(spi);
}

void BMI160::initSPI() {
    spi_bus_config_t buscfg = {};  // Inicializar todos los campos a cero
    buscfg.mosi_io_num = 23;       // Pin MOSI
    buscfg.miso_io_num = 19;       // Pin MISO
    buscfg.sclk_io_num = 18;       // Pin SCLK
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.data4_io_num = -1;
    buscfg.data5_io_num = -1;
    buscfg.data6_io_num = -1;
    buscfg.data7_io_num = -1;
    buscfg.max_transfer_sz = 0;
    buscfg.flags = 0;
    buscfg.intr_flags = 0;
    buscfg.isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO;  // Usar asignación automática de núcleo

    spi_device_interface_config_t devcfg = {};  // Inicializar todos los campos a cero
    devcfg.command_bits = 8;
    devcfg.address_bits = 8;
    devcfg.dummy_bits = 0;
    devcfg.mode = 0;
    devcfg.duty_cycle_pos = 128;
    devcfg.cs_ena_pretrans = 0;
    devcfg.cs_ena_posttrans = 0;
    devcfg.clock_speed_hz = 1 * 1000 * 1000;  // 1 MHz
    devcfg.input_delay_ns = 0;
    devcfg.spics_io_num = cs_pin;
    devcfg.flags = 0;
    devcfg.queue_size = 1;
    devcfg.pre_cb = nullptr;
    devcfg.post_cb = nullptr;
    devcfg.clock_source = SPI_CLK_SRC_DEFAULT;  // Asegurar que esté en el orden correcto

    esp_err_t ret = spi_bus_initialize(spi_host, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(spi_host, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
}

uint8_t BMI160::readRegister(uint8_t reg) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));  // Inicializar la estructura a cero
    t.length = 8;              // 8 bits de comando
    t.tx_buffer = &reg;
    t.rxlength = 8;            // 8 bits de datos
    uint8_t rx_data;
    t.rx_buffer = &rx_data;

    esp_err_t ret = spi_device_transmit(spi, &t);
    ESP_ERROR_CHECK(ret);

    return rx_data;
}