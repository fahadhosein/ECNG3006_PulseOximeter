#ifndef REPORT_H
#define REPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "driver/uart.h"

#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"

// UART
#define UART_NUM UART_NUM_0
#define TX_BUF (1024)
#define RX_BUF (1024)

// SPIFFS
#define NUM_FILES 5 
#define STORAGE_PATH "/spiffs"
#define FILE_NUM_MEASUREMENTS "/num_measurements.txt"
#define FILE_MEASUREMENTS "/measurements.txt"
#define FILE_INTERVALS "/intervals.txt"
#define FILE_FORMAT "/format.txt"

// Function Prototypes
void serial_init(void* pvParam);

void storage_init(void *pvParam);

void create_file(const char *filename);

void read_file(const char *filename, int* read_data);

void write_file(const char* filename,int* value);

void write_measurements(const char *filename, const char *mode, char *name, int *bpm, int *spo2);

#endif