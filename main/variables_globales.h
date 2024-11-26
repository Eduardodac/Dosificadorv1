#ifndef SHARED_MUTEX_H
#define SHARED_MUTEX_H

#include "freertos/semphr.h"

extern SemaphoreHandle_t xMutexEstadoActivacion;
extern int estadoActivacion;

extern float ultraMeasure;
extern SemaphoreHandle_t xMutexUltraMeasure;

extern float loadCellMeasure;
extern SemaphoreHandle_t xMutexLoadCellMeasure;

void create_mutex_estadoActivacion();
void create_mutex_ultraMeasure();
void create_mutex_loadCellMeasure();

#endif 
