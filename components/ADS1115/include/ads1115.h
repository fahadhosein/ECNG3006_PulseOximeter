#ifndef ADS1115_H
#define ADS1115_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "driver/i2c.h"


static const char *TAG_ADS = "ADS";

#define I2C_MASTER_SCL_IO   2   // GPIO number for I2C master clock
#define I2C_MASTER_SDA_IO   0   // GPIO number for I2C master data
#define I2C_MASTER_NUM I2C_NUM_0        // I2C port number for master dev
#define I2C_MASTER_TX_BUF_DISABLE   0   // I2C master do not need buffer
#define I2C_MASTER_RX_BUF_DISABLE   0   // I2C master do not need buffer

// ADS1115 Pin Addresses
#define ADS1115_GND 0x48
#define ADS1115_VDD 0x49
#define ADS1115_SDA 0x4A
#define ADS1115_SCL 0x4B

#define WRITE_BIT       I2C_MASTER_WRITE // I2C master write
#define READ_BIT        I2C_MASTER_READ  // I2C master read
#define ACK_CHECK_EN    0x1              // I2C master will check ack from slave
#define ACK_CHECK_DIS   0x0              // I2C master will not check ack from slave
#define ACK_VAL         0x0              // I2C ack value
#define NACK_VAL        0x1              // I2C nack value
#define LAST_NACK_VAL   0x2              // I2C last_nack value

// ADS1115 Register Addresses
#define ADS1115_CONV 0x00
#define ADS1115_CONFIG 0x01
#define ADS1115_LOTHRESH 0x02
#define ADS1115_HITHRESH 0x03

typedef struct conf
{
    uint8_t OS;             // Operational Status: 1-Bit
    uint8_t MUX;            // Input MUX: 3-Bits
    uint8_t PGA;            // Programmable Gain Amplifier: 3-Bits
    uint8_t MODE;           // Mode: 1-Bit
    uint8_t DR;             // Data Rate: 3-Bits
    uint8_t COMP_MODE;      // Comparator Mode: 1-Bit
    uint8_t COMP_POL;       // Comparator Polarity: 1-Bit
    uint8_t COMP_LAT;       // Latching Comparator: 1-Bit
    uint8_t COMP_QUE;       // Comparator Queue and Disable: 2-Bits
    uint16_t CONFIG;        // 16-bit Configuration
} ADS1115_CONF;

esp_err_t i2c_master_init();

esp_err_t i2c_master_ads1115_write(i2c_port_t i2c_num, uint8_t reg_address, uint8_t *data, size_t data_len);

esp_err_t i2c_master_ads1115_read(i2c_port_t i2c_num, uint8_t reg_address, uint8_t *data, size_t data_len);

esp_err_t data_write(i2c_port_t i2c_num, uint8_t reg_address, uint16_t data);

esp_err_t data_read(i2c_port_t i2c_num, uint8_t reg_address, uint16_t *data);

esp_err_t i2c_master_ads1115_init(i2c_port_t i2c_num);

#endif