#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "variables_globales.h"

SemaphoreHandle_t xMutexEstadoActivacion = NULL;

int estadoActivacion = 0;

void create_mutex_estadoActivacion() {
    if (xMutexEstadoActivacion == NULL) {
        xMutexEstadoActivacion = xSemaphoreCreateMutex();
        if (xMutexEstadoActivacion == NULL) {
            printf("Error: No se pudo crear el mutex compartido\n");
        }
    }
}
