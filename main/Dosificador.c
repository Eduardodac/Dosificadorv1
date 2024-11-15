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
    servo_init();

    int angle = 25;
    int step = 5;

    while(1){
        ESP_LOGI(TAGMAIN, "Ángulo de rotación: %d", angle);
        servo_update_angle(angle);
        vTaskDelay(pdMS_TO_TICKS(100));

        if (estadoServo == 0) {
            angle = -50;
        }

        if (estadoServo == 1) {
            if (angle < 25) {
                angle += step;
            }
        }
    }
}