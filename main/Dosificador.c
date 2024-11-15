#include "ultra.h"
#include "gpio_isr.h"
#include "servo.h"

int estadoServo = 1;

void app_main(void)
{
    xTaskCreate(ultrasonic_task, "ultrasonic_task", 2048, NULL, 5, NULL);
    gpio_init();
    xTaskCreate(servo_control_task, "servo_control_task", 4096, NULL, 5, NULL);
}
