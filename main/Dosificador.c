#include "ultra.h"
#include "gpio_isr.h"
#include "servo.h"
#include <esp_err.h>
#include <esp_log.h>
#include "variables_globales.h"
#include "motor_dc.h"
#include "hx711_component.h"

static const char *TAGMAIN = "MAIN";
#define DOUT_PIN GPIO_NUM_19    // Pin de datos (DOUT)
#define PD_SCK_PIN GPIO_NUM_18  // Pin de reloj (SCK)

void app_main(void)
{
    create_mutex_estadoActivacion();
    create_mutex_ultraMeasure();
    //xTaskCreate(ultrasonic_task, "ultrasonic_task", 2048, NULL, 5, NULL);
    //xTaskCreate(hx711_task, "hx711_task", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
    //xTaskCreate(estado_task, "estado_task", 2048, NULL, 5, NULL);
    gpio_init();
    servo_init();
    motor_dc_init();
    int status = 1;

    while(1){
        if (status == 1) {
            servo_update_angle(-55);
            motor_dc_forward();
            motor_dc_set_speed(500);
            vTaskDelay(pdMS_TO_TICKS(1000));
            servo_update_angle(20);
            if (xSemaphoreTake(xMutexEstadoActivacion, portMAX_DELAY) == pdTRUE) {
                status = 0;
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

    // // Inicializar el sensor HX711
    // HX711_init(DOUT_PIN, PD_SCK_PIN, eGAIN_128);

    // // Realizar una tara (ajustar el valor de referencia)
    // HX711_set_scale(15.09);

    // // Establecer la escala (por ejemplo, 1000 para obtener gramos)
    // HX711_set_scale(1000);

    // // Leer y mostrar el valor promedio de las lecturas
    // while (1) {
    //     float weight = HX711_get_units(10); // Lee el peso promedio de 10 lecturas
    //     ESP_LOGI("Peso", "El peso es: %.2f gramos", weight);
    //     vTaskDelay(500 / portTICK_PERIOD_MS); // Espera 1 segundo antes de la siguiente lectura
    // }
}