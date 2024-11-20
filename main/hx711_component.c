#include "hx711_component.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hx711.h>

static const char *TAGHX = "hx711";

void hx711_task(void *pvParameters)
{
    hx711_t dev = {
        .dout = HX711_DOUT_PIN,
        .pd_sck = HX711_SCK_PIN,
        .gain = HX711_GAIN_A_64
    };

    // Inicializar el dispositivo
    ESP_ERROR_CHECK(hx711_init(&dev));
    int32_t DataSet = 0;
    int calibrar = 1;
    ESP_LOGI(TAGHX, "CALIBRANDO...");
    while (calibrar == 1)
    {
        esp_err_t readCal = hx711_wait(&dev, 100);
        if (readCal != ESP_OK)
        {
            ESP_LOGE(TAGHX, "Dispositivo no encontrado: %d (%s)", readCal, esp_err_to_name(readCal));
            continue;
        }

        int32_t dataCalibration;
        readCal = hx711_read_average(&dev, 1000, &dataCalibration);
        if (readCal != ESP_OK)
        {
            ESP_LOGE(TAGHX, "No se pudo leer el dato: %d (%s)", readCal, esp_err_to_name(readCal));
            continue;
        }

        DataSet = dataCalibration;
        calibrar = 0;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    ESP_LOGI(TAGHX, "CALIBRADÍSIMO");
    // Leer datos del dispositivo
    while (1)
    {
        esp_err_t r = hx711_wait(&dev, 100);
        if (r != ESP_OK)
        {
            ESP_LOGE(TAGHX, "Dispositivo no encontrado: %d (%s)", r, esp_err_to_name(r));
            continue;
        }

        int32_t data;
        r = hx711_read_average(&dev, 50, &data);
        if (r != ESP_OK)
        {
            ESP_LOGE(TAGHX, "No se pudo leer el dato: %d (%s)", r, esp_err_to_name(r));
            continue;
        }

        int32_t newData = data - DataSet;
        float measure = (double)newData / 114.3;
        ESP_LOGI(TAGHX, "Datos: %" PRIi32, newData);
        ESP_LOGI(TAGHX, "Medida: %.2f grms", measure);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
