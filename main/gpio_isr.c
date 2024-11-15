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

void gpio_task(void *arg)
{
    int pin_number;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &pin_number, portMAX_DELAY))
        {
            ESP_LOGI(TAGISR, "Interrupt in GPIO, estadoServo: %d", estadoServo);
            estadoServo = !estadoServo;
        }
    }
}

void gpio_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << GPIO_INPUT_PIN),
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&io_conf);

    gpio_evt_queue = xQueueCreate(10, sizeof(int));
    if (!gpio_evt_queue)
    {
        ESP_LOGE(TAGISR, "Failed to create queue");
        return;
    }

    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_INPUT_PIN, gpio_isr_handler, (void *)GPIO_INPUT_PIN);
}
