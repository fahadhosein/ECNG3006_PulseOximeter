#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "ads1115.h"

static const char *TAG = "main";

static void i2c_task(void *arg)
{
    uint16_t data;
    double voltage;
    esp_err_t ret;

    ret  = i2c_master_ads1115_init(I2C_MASTER_NUM);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "ADS1115 Initialised!\n");
    }
    else
    {
        i2c_driver_delete(I2C_MASTER_NUM);
    }
    for(;;)
    {
        ret = data_read(I2C_MASTER_NUM, ADS1115_CONV, &data);
        if(ret == ESP_OK)
        {
            printf("%d\n", (uint16_t)data);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        else
        {
            ESP_LOGI(TAG, "Unable To Read ADS1115\n");
        }
    }
}

static void gpio_task(void *arg)
{
    for(;;)
    {

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    //start I2C task
    xTaskCreate(i2c_task, "i2c_task", 2048, NULL, 1, NULL);
}