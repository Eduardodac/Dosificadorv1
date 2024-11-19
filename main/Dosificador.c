#include "ultra.h"
#include "gpio_isr.h"
#include "servo.h"
#include <esp_err.h>
#include <esp_log.h>
#include "variables_globales.h"
#include "motor_dc.h"
static const char *TAGMAIN = "MAIN";

void app_main(void)
{
    create_mutex_estadoActivacion();
    xTaskCreate(ultrasonic_task, "ultrasonic_task", 2048, NULL, 5, NULL);
    gpio_init();
    servo_init();
    motor_dc_init();

    while(1){
        if (estadoActivacion == 1) {
            servo_update_angle(-55);
            motor_dc_forward();
            motor_dc_set_speed(500);
            vTaskDelay(pdMS_TO_TICKS(1000));
            servo_update_angle(20);
            if (xSemaphoreTake(xMutexEstadoActivacion, portMAX_DELAY) == pdTRUE) {
                estadoActivacion = 0;
                xSemaphoreGive(xMutexEstadoActivacion);
            }

            vTaskDelay(pdMS_TO_TICKS(10000));
            motor_dc_reverse();
            motor_dc_set_speed(350);
            vTaskDelay(pdMS_TO_TICKS(500));
            motor_dc_set_speed(0);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}