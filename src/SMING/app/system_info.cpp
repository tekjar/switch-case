#include <app.h>

/**
 * @file
 * @brief All your system information APIs
 */


/**
 * @brief      Gets your current system information
 */
void system_showInfo() {
    PRINTF_INFO("\r\nSDK: v%s\r\n", system_get_sdk_version());
    PRINTF_INFO("Free Heap: %d\r\n", system_get_free_heap_size());
    PRINTF_INFO("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
    PRINTF_INFO("System Chip ID: %x\r\n", system_get_chip_id());
    PRINTF_INFO("SPI Flash ID: %x\r\n", spi_flash_get_id());
    //PRINTF_INFO("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
}