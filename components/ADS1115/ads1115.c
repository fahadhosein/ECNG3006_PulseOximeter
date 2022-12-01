#include "ads1115.h"

esp_err_t i2c_master_init()
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = 1;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = 1;
    conf.clk_stretch_tick = 300; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
    ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
    ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
    return ESP_OK;
}

esp_err_t i2c_master_ads1115_write(i2c_port_t i2c_num, uint8_t reg_address, uint8_t *data, size_t data_len)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ADS1115_GND << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_address, ACK_CHECK_EN);
    i2c_master_write(cmd, data, data_len, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

esp_err_t i2c_master_ads1115_read(i2c_port_t i2c_num, uint8_t reg_address, uint8_t *data, size_t data_len)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ADS1115_GND << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_address, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) 
    {
        ESP_LOGI(TAG_ADS, "Unable to read data from ADS1115!\n");
        return ret;
    }

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ADS1115_GND << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read(cmd, data, data_len, LAST_NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

esp_err_t data_write(i2c_port_t i2c_num, uint8_t reg_address, uint16_t data)
{
    int ret;
    uint8_t buf[2];
    buf[0] = (data >> 8) & 0xFF;
    buf[1] = (data >> 0) & 0xFF;

    ret = i2c_master_ads1115_write(i2c_num, reg_address, buf, 2);
    return ret;
}

esp_err_t data_read(i2c_port_t i2c_num, uint8_t reg_address, uint16_t *data)
{
    int ret;
    uint8_t buf[2];

    ret = i2c_master_ads1115_read(i2c_num, reg_address, buf, 2);
    *data = (buf[0] << 8) | buf[1];
    return ret;
}

esp_err_t i2c_master_ads1115_init(i2c_port_t i2c_num)
{
    vTaskDelay(100 / portTICK_RATE_MS);
    i2c_master_init();

    ADS1115_CONF conf;
    conf.OS = 0x00;        // NULL
    conf.MUX = 0x04;       // AINp = AIN0 and AINn = GND
    conf.PGA = 0x01;       // FS = 4.096 V
    conf.MODE = 0x00;      // Continuous-Conversion Mode
    conf.DR = 0x04;        // 128SPS
    conf.COMP_MODE = 0x00; // Traditional Comparator
    conf.COMP_POL = 0x00;  // Active Low
    conf.COMP_LAT = 0x00;  // Non-latching Comparator
    conf.COMP_QUE = 0x02;  // Assert After Four Conversions

    uint16_t data;
    data = (conf.OS << 3) | conf.MUX;
    data = (data << 3) | conf.PGA;
    data = (data << 1) | conf.MODE;
    data = (data << 3) | conf.DR;
    data = (data << 1) | conf.COMP_MODE;
    data = (data << 1) | conf.COMP_POL;
    data = (data << 1) | conf.COMP_LAT;
    data = (data << 2) | conf.COMP_QUE;
    conf.CONFIG = data;

    // Output Configuration Bits
    ESP_LOGI(TAG_ADS, "Configuration Bits: %d\n", (int)conf.CONFIG);

    // Writing to CONFIG Register
    ESP_ERROR_CHECK(data_write(i2c_num, ADS1115_CONFIG, conf.CONFIG));

    return ESP_OK;
}