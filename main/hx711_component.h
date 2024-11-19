#ifndef HX711_COMPONENT_H
#define HX711_COMPONENT_H

#include <inttypes.h>
#include <esp_err.h>

#define HX711_DOUT_PIN 21
#define HX711_SCK_PIN  22

void hx711_task(void *pvParameters);

#endif
