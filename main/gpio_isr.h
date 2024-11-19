#ifndef GPIO_ISR_H
#define GPIO_ISR_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

extern QueueHandle_t gpio_evt_queue;

void gpio_isr_handler(void *arg);
void gpio_task(void *arg);
void gpio_init(void);

#endif // GPIO_ISR_H
