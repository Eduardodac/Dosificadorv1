#include "ultra.h"
#include "gpio_isr.h"
#include "servo.h"
#include <esp_err.h>
#include <esp_log.h>
static const char *TAGMAIN = "MAIN";

void app_main(void)
{
    xTaskCreate(ultrasonic_task, "ultrasonic_task", 2048, NULL, 5, NULL);
    gpio_init();

    while(1){
        ESP_LOGI(TAGMAIN, "Estado del servo %d!", estadoServo);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
