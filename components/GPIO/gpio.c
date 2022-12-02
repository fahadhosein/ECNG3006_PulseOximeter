#include "gpio.h"

esp_err_t gpio_init()
{
    gpio_config_t io_conf;

    // GPIO OUTPUT Configuration
    io_conf.intr_type = GPIO_INTR_DISABLE;      // disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;            // set as output mode
    io_conf.pin_bit_mask = (1ULL << GPIO_OUTPUT_IO); // bit mask of the pins that you want to set,e.g.GPIO15/16
    io_conf.pull_down_en = 0;                   // disable pull-down mode
    io_conf.pull_up_en = 0;                     // disable pull-up mode
    gpio_config(&io_conf);                      // configure GPIO with the given settings
    return ESP_OK;
}

void gpio_on(int *delay)
{
    gpio_set_level(GPIO_OUTPUT_IO, 0);
    vTaskDelay(*delay / portTICK_PERIOD_MS);
}

void gpio_off(int *delay)
{
    gpio_set_level(GPIO_OUTPUT_IO, 1);
    vTaskDelay(*delay / portTICK_PERIOD_MS);
}