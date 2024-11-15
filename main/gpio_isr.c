#include "gpio_isr.h"
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define GPIO_INPUT_PIN 2
static const char *TAGISR = "GPIO_ISR";

QueueHandle_t gpio_evt_queue = NULL;
int estadoServo = 1;

void IRAM_ATTR gpio_isr_handler(void *arg)
{
    int pin_number = (int)arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(gpio_evt_queue, &pin_number, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }
}

void gpio_task(void* arg) {
    int pin_number;
    for (;;) {
        // Espera hasta recibir una notificación desde la ISR
        if (xQueueReceive(gpio_evt_queue, &pin_number, portMAX_DELAY)) {
            // Procesa el evento en contexto de tarea (seguro)
            if(estadoServo == 0)
            {
                estadoServo = 1;
            }else if (estadoServo == 1)
            {
                estadoServo = 0;
            }
            ESP_LOGI(TAGISR, "Interrupción en GPIO estadoservo: %d!", estadoServo);
        }
    }
}

void gpio_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << GPIO_INPUT_PIN),
        .pull_up_en = GPIO_PULLDOWN_ENABLE,

    };
    gpio_config(&io_conf);

    gpio_evt_queue = xQueueCreate(10, sizeof(int));
    if (gpio_evt_queue == NULL) {
        ESP_LOGE(TAGISR, "Error creando la cola");
        return;
    }

    BaseType_t task_created = xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);
    if (task_created != pdPASS) {
        ESP_LOGE(TAGISR, "Error creando la tarea");
        return;
    }

    esp_err_t err = gpio_install_isr_service(0);
    if (err != ESP_OK) {
        ESP_LOGE(TAGISR, "Error instalando el servicio ISR: %s", esp_err_to_name(err));
        return;
    }

    gpio_isr_handler_add(GPIO_INPUT_PIN, gpio_isr_handler, (void*) GPIO_INPUT_PIN);

    ESP_LOGI(TAGISR, "Configuración ISR completada");
}
