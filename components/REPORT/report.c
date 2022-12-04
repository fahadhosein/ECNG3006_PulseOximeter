#include "report.h"

static const char *TAG_SERIAL = "SERIAL";
static const char *TAG_STORAGE = "STORAGE";

static QueueHandle_t uart_queue;

void serial_init(void *pvParam)
{
    uart_config_t uart_config = {
        .baud_rate = 74880,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, RX_BUF * 2, TX_BUF * 2, 150, &uart_queue, 0);
}

void storage_init(void *pvParam)
{
    esp_err_t ret;
    size_t bytes = 0, bytes_written = 0;

    esp_vfs_spiffs_conf_t spiff_config = {
        .base_path = STORAGE_PATH,
        .partition_label = NULL,
        .max_files = NUM_FILES,
        .format_if_mount_failed = true};

    ret = esp_vfs_spiffs_register(&spiff_config);

    if (ret != ESP_OK)
    {
        ESP_LOGI(TAG_STORAGE, "Failed To Configure SPIFFS!");
    }
    else
    {
        ESP_LOGI(TAG_STORAGE, "Successfully Initialized SPIFFs\n");
        ESP_LOGI(TAG_STORAGE, "Storage: %d/%d Bytes\n", bytes, bytes_written);
    }
}

void create_file(const char *filename)
{
    struct stat s;
    if (stat(STORAGE_PATH + *filename, &s) == 1)
    {
        FILE *file;
        file = fopen(STORAGE_PATH + *filename, "w+");
        ESP_LOGI(TAG_STORAGE, "Created File: %s\n", filename);
        fclose(file);
    }
    else
    {
        ESP_LOGI(TAG_STORAGE, "File Exists: %s\n", filename);
    }
}

void read_file(const char *filename, int* read_data)
{
    FILE *file;
    struct stat s;
    if (stat(STORAGE_PATH + *filename, &s) == 0)
    {
        file = fopen(STORAGE_PATH + *filename, "r+");
        
        if (sizeof(*filename) == sizeof(FILE_MEASUREMENTS))
        {
            char name[20];
            int bpm, spo2;
            while (fscanf(file, "%s %d %d", name, &bpm, &spo2) != EOF)
            {
                printf("%s\t\t%d\t\t%d\n", name, bpm, spo2);
            }
            fclose(file);
        }
        else if (sizeof(*filename) == sizeof(FILE_NUM_MEASUREMENTS))
        {
            int num_measurements;
            fscanf(file, "%d", &num_measurements);
            ESP_LOGI(TAG_STORAGE, "Maximum Number of measurements is: %d\n", num_measurements);
            *read_data = num_measurements;
            fclose(file);
        }
        else if (sizeof(*filename) == sizeof(FILE_INTERVALS))
        {
            int interval;
            fscanf(file, "%d", &interval);
            if (interval)
                printf("The interval is: %d\n", interval);
            *read_data = interval;
            fclose(file);
        }
    }
    else
    {
        ESP_LOGE(TAG_STORAGE, "The file does not exists. Use create file function to create the file.\n");
        
    }
}

void write_file(const char *filename, int *value)
{
    FILE *file;
    struct stat st;
    if (stat(STORAGE_PATH + *filename, &st) == 0)
    {
        file = fopen(STORAGE_PATH + *filename, "w+");
        fprintf(file, (const char*) *value);
        fclose(file);
    }
}

void write_measurements_file(const char *filename, const char *mode, char *name, int *bpm, int *spo2)
{
    FILE *file = fopen(filename, mode);
    fprintf(file, "%s  %d  %d", name, *bpm, *spo2);
    fclose(file);
}