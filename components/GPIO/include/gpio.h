#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char *TAG_GPIO = "gpio";

#define GPIO_OUTPUT_IO  3

esp_err_t gpio_init();

void gpio_on(int *delay);

void gpio_off(int *delay);

#endif