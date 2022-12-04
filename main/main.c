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
#include "gpio.h"

#define MAX_VALS 1000
#define SAMPLES_MS 10 

static const char *TAG = "main";

uint16_t red_data[MAX_VALS];
uint16_t ir_data[MAX_VALS];
int i = 0;
int j = 0;
int n = 0;

static bool is_peak(uint16_t arr[], int n, uint16_t num, int i, int j)
{
    if(i>=0 && arr[i] > num)
        return false;
    if(j<n && arr[j] > num)
        return false;
    return true;
}

static bool is_trough(uint16_t arr[], int n, uint16_t num, int i, int j)
{
    if(i>=0 && arr[i] < num)
        return false;
    if(j<n && arr[j] < num)
        return false;
    return true;
}


static double calc_hr(uint16_t arr[], int n)
{
    double pk = 0.0;
    double tr = 0.0;
    double bpm = 0.0;
    double spo2 = 0.0;
    double base = 0.0;
    double peak = 0.0;
    double base_avg = 0.0;
    double peak_avg = 0.0;
    
    double min = arr[0];
    double max = arr[0];

    for(int i = 0; i<n; i++)
    {
        if(max>arr[i])
        {
            max = arr[i];
        }
        if(min<arr[i])
        {
            min = arr[i];
        }
        if(is_peak(arr, n, arr[i], i-1, i+1))
        {
            pk++;
            peak_avg = peak_avg + arr[i];
            
        }
        if(is_trough(arr, n, arr[i], i-1, i+1))
        {
            tr++;
            base_avg = base_avg + arr[i];
        }
    }
    bpm = (pk/100) * 6;
    peak = peak_avg/pk;
    base = base_avg/tr;
    spo2 = peak/base;
    printf("BPM = %d\n", (uint16_t)bpm);
    return spo2;
}

static void gpio_task(void *arg)
{
    int delay = SAMPLES_MS;
    for(;;)
    {
        gpio_on(&delay);
        gpio_off(&delay);
    }
}

static void i2c_task(void *arg)
{
    uint16_t data;
    esp_err_t ret;
    double red_spo2 = 0.0;
    double ir_spo2 = 0.0;
    double spo2 = 0.0;
    double r = 0.0;

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
            if(n%2)
            {
                ir_data[i]=(uint16_t)data;
                // printf("%d\n", (uint16_t)data);
                i++;
                n++;
            }
            else
            {
                red_data[j]=(uint16_t)data;
                // printf("%d\n", (uint16_t)data);
                j++;
                n++;
            }
            vTaskDelay(SAMPLES_MS / portTICK_PERIOD_MS);
            if(i == MAX_VALS)
            {
                printf("\n------------------------------------------------------\n");
                printf("Red LED Data: \n");
                red_spo2 = calc_hr(red_data, sizeof(red_data));
                printf("IR LED Data: \n");
                ir_spo2 = calc_hr(ir_data, sizeof(ir_data));
                r = red_spo2/ir_spo2;
                spo2 = (4.23 - r)/0.038;
                printf("SPO2 = %d\n", (uint16_t)spo2);
                vTaskDelete(NULL);
            }
        }
        else
        {
            ESP_LOGI(TAG, "Unable To Read ADS1115\n");
        }
    }
}

void app_main(void)
{
    gpio_init();
    //start I2C task
    xTaskCreate(i2c_task, "i2c_task", 2048, NULL, 1, NULL);
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 2, NULL);
}