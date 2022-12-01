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

static const *TAG_GPIO = "gpio";

#define GPIO_OUTPUT_IO  2

gpio_config_t io_conf;

// GPIO OUTPUT Configuration
io_conf.intr_type = GPIO_INTR_DISABLE;      // disable interrupt
io_conf.mode = GPIO_MODE_OUTPUT;            // set as output mode
io_conf.pin_bit_mask = (1ULL << GPIO_OUTPUT_IO); // bit mask of the pins that you want to set,e.g.GPIO15/16
io_conf.pull_down_en = 0;                   // disable pull-down mode
io_conf.pull_up_en = 0;                     // disable pull-up mode
gpio_config(&io_conf);                      // configure GPIO with the given settings

// GPIO INPUT Configuration
io_conf.intr_type = GPIO_INTR_NEGEDGE;      // interrupt of rising edge
io_conf.pin_bit_mask = (1ULL << GPIO_INPUT_IO);  // bit mask of the pins, use GPIO4/5 here
io_conf.mode = GPIO_MODE_INPUT;             // set as input mode
io_conf.pull_up_en = 0;                     // disable pull-up mode
gpio_config(&io_conf);

// Change gpio intrrupt type for one pin
gpio_set_intr_type(GPIO_INPUT_IO, GPIO_INTR_NEGEDGE);

#endif