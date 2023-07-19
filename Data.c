#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

#define MAX_DATA_COUNT 10 // 处理的最大数据数量

struct Data
{
    float humidity;
    float temperature;
    int gas_concentration;
    float bat_v;
};

struct Data data_buffer[MAX_DATA_COUNT];
int data_count = 0;

void process_data(const char *data)
{
    char *token;
    char data_copy[BUFFER_SIZE];

    // 将data拷贝到data_copy，因为strtok会修改原始数据
    strncpy(data_copy, data, sizeof(data_copy));

    // 使用strtok解析逗号分隔的数据项
    token = strtok(data_copy, ",");
    if (token != NULL)
    {
        // 第一个数据项是humidity
        float humidity = atof(token);
        printf("Humidity: %.2f %%\n", humidity);
        data_buffer[data_count].humidity = humidity;
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        // 第二个数据项是temperature
        float temperature = atof(token);
        printf("Temperature: %.2f °C\n", temperature);
        data_buffer[data_count].temperature = temperature;
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        // 第三个数据项是gas_concentration
        int gas_concentration = atoi(token);
        printf("Gas Concentration: %d ppm\n", gas_concentration);
        data_buffer[data_count].gas_concentration = gas_concentration;
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        // 第四个数据项是bat_v
        float bat_v = atof(token);
        printf("Battery Voltage: %.2f V\n", bat_v);
        data_buffer[data_count].bat_v = bat_v;
    }

    data_count++;
    if (data_count == MAX_DATA_COUNT)
    {
        // 达到指定数据数量后，进行数据处理
        float humidity_sum = 0;
        float temperature_sum = 0;
        int gas_concentration_sum = 0;
        float bat_v_sum = 0;
        float humidity_max = data_buffer[0].humidity;
        float humidity_min = data_buffer[0].humidity;
        float temperature_max = data_buffer[0].temperature;
        float temperature_min = data_buffer[0].temperature;
        int gas_concentration_max = data_buffer[0].gas_concentration;
        int gas_concentration_min = data_buffer[0].gas_concentration;
        float bat_v_max = data_buffer[0].bat_v;
        float bat_v_min = data_buffer[0].bat_v;

        for (int i = 0; i < MAX_DATA_COUNT; i++)
        {
            humidity_sum += data_buffer[i].humidity;
            temperature_sum += data_buffer[i].temperature;
            gas_concentration_sum += data_buffer[i].gas_concentration;
            bat_v_sum += data_buffer[i].bat_v;

            if (data_buffer[i].humidity > humidity_max)
            {
                humidity_max = data_buffer[i].humidity;
            }

            if (data_buffer[i].humidity < humidity_min)
            {
                humidity_min = data_buffer[i].humidity;
            }

            if (data_buffer[i].temperature > temperature_max)
            {
                temperature_max = data_buffer[i].temperature;
            }

            if (data_buffer[i].temperature < temperature_min)
            {
                temperature_min = data_buffer[i].temperature;
            }

            if (data_buffer[i].gas_concentration > gas_concentration_max)
            {
                gas_concentration_max = data_buffer[i].gas_concentration;
            }

            if (data_buffer[i].gas_concentration < gas_concentration_min)
            {
                gas_concentration_min = data_buffer[i].gas_concentration;
            }

            if (data_buffer[i].bat_v > bat_v_max)
            {
                bat_v_max = data_buffer[i].bat_v;
            }

            if (data_buffer[i].bat_v < bat_v_min)
            {
                bat_v_min = data_buffer[i].bat_v;
            }
        }

        float humidity_avg = humidity_sum / MAX_DATA_COUNT;
        float temperature_avg = temperature_sum / MAX_DATA_COUNT;
        int gas_concentration_avg = gas_concentration_sum / MAX_DATA_COUNT;
        float bat_v_avg = bat_v_sum / MAX_DATA_COUNT;

        printf("\n------ Data Processing Results ------\n");
        printf("Humidity Max: %.2f %%\n", humidity_max);
        printf("Humidity Min: %.2f %%\n", humidity_min);
        printf("Humidity Avg: %.2f %%\n", humidity_avg);
        printf("Temperature Max: %.2f °C\n", temperature_max);
        printf("Temperature Min: %.2f °C\n", temperature_min);
        printf("Temperature Avg: %.2f °C\n", temperature_avg);
        printf("Gas Concentration Max: %d ppm\n", gas_concentration_max);
        printf("Gas Concentration Min: %d ppm\n", gas_concentration_min);
        printf("Gas Concentration Avg: %d ppm\n", gas_concentration_avg);
        printf("Battery Voltage Max: %.2f V\n", bat_v_max);
        printf("Battery Voltage Min: %.2f V\n", bat_v_min);
        printf("Battery Voltage Avg: %.2f V\n", bat_v_avg);
        printf("-------------------------------------\n\n");

        data_count = 0;
    }
}