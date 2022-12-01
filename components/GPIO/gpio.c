#include "gpio.h"

void gpio_on(void *arg)
{
    gpio_set_level(GPIO_OUTPUT_IO, 0);
}

void gpio_off(void *arg)
{
    gpio_set_level(GPIO_OUTPUT_IO, 1);
}